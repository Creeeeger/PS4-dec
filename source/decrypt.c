#include "ps4.h"
#include "pup.h"
#include "bls.h"
#include "decryptio.h"
#include "encryptsrv.h"
#include "defines.h"
#include "debug.h"
#include "kernel_utils.h"


#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef DEBUG_SOCKET
  #define printfsocket(format, ...)\
    do {\
      char __printfsocket_buffer[512];\
      int __printfsocket_size = sprintf(__printfsocket_buffer, format, ##__VA_ARGS__);\
      sceNetSend(sock, __printfsocket_buffer, __printfsocket_size, 0);\
    } while(0)
#endif

void notify(char* message);
uint8_t GetElapsed(uint64_t ResetInterval);

extern int sock;
extern time_t prevtime;

#define SSIZET_FMT "%zd"

#endif


int verify_segment(const decrypt_state* state, int index, pup_segment* segment, int additional)
{
  int result;
  uint8_t* buffer = NULL;

  buffer = memalign(0x4000, segment->compressed_size);
  ssize_t bytesread = readbytes(state, segment->offset, segment->compressed_size, buffer, segment->compressed_size);
  if (bytesread != segment->compressed_size)
  {
     printfsocket("Failed to read segment #%d for verification!\n", index);
     result = -1;
     goto end;
  }

  result = encsrv_verify_segment(state->device_fd, index, buffer, segment->compressed_size, additional);
  if (result != 0)
  {
    printfsocket("Failed to verify segment #%d! %d\n", index, errno);
    goto end;
  }

end:
  if (buffer != NULL)
  {
    free(buffer);
  }

  return result;
}

int verify_segments(const decrypt_state* state, pup_segment* segments, int segment_count)
{
  int result = 0;

  for (int i = 0; i < segment_count; i++)
  {
    pup_segment* segment = &segments[i];
    if ((segment->flags & 0xF0000000) == 0xE0000000)
    {
      printfsocket("Verifying segment #%d (%d)... [1]\n", i, segment->flags >> 20);
      result = verify_segment(state, i, segment, 1);
      if (result < 0)
      {
        goto end;
      }
    }
  }

  for (int i = 0; i < segment_count; i++)
  {
    pup_segment* segment = &segments[i];
    if ((segment->flags & 0xF0000000) == 0xF0000000)
    {
      printfsocket("Verifying segment #%d (%d)... [0]\n", i, segment->flags >> 20);
      result = verify_segment(state, i, segment, 0);
      if (result < 0)
      {
        goto end;
      }
    }
  }

end:
  return result;
}

int decrypt_segment(const decrypt_state* state, uint16_t index, pup_segment* segment)
{
  int result = -1;

  uint8_t* buffer = buffer = memalign(0x4000, segment->compressed_size);

  int is_compressed = (segment->flags & 8) != 0 ? 1 : 0;

  size_t remaining_size = segment->compressed_size;
  if (is_compressed == 1)
  {
    remaining_size &= ~0xFull;
  }

  if (remaining_size > 0)
  {
    size_t padding_size = segment->compressed_size & 0xF;
    size_t encrypted_size = remaining_size;

    if (segment->compressed_size < remaining_size)
    {
      encrypted_size = segment->compressed_size;
    }

    ssize_t bytesread = readbytes(state, segment->offset, encrypted_size, buffer, segment->compressed_size);
    if (bytesread != encrypted_size)
    {
      printfsocket("Failed to read segment #%d!\n", index);
      result = -1;
      goto end;
    }

    result = encsrv_decrypt_segment(state->device_fd, index, buffer, encrypted_size);
    if (result != 0)
    {
      int errcode = errno;
      printfsocket("Failed to decrypt segment #%d! - Error: %d (%s)\n", index, errcode, strerror(errcode));
      goto end;
    }

    int unencrypted_size = remaining_size - padding_size;
    if (is_compressed == 0 || encrypted_size != remaining_size)
    {
      unencrypted_size = encrypted_size;
    }

    ssize_t byteswritten = writebytes(state, segment->offset, unencrypted_size, buffer, segment->compressed_size);
    if (byteswritten != unencrypted_size) {
      printfsocket("Failed to write segment #%d!\n", index);
      result = -1;
      goto end;
    }
  }

end:
  if (buffer != NULL)
  {
    free(buffer);
  }

  return result;
}

int decrypt_segment_blocks(const decrypt_state * state, uint16_t index, pup_segment* segment,
                           uint16_t table_index, pup_segment* table_segment)
{
  int result = -1;
  uint8_t* table_buffer = NULL;
  uint8_t* block_buffer = NULL;

  size_t table_length = table_segment->compressed_size;
  table_buffer = memalign(0x4000, table_length);

  ssize_t bytesread = readbytes(state, table_segment->offset, table_length, table_buffer, table_length);
  if (bytesread != table_length)
  {
    printfsocket("  Failed to read table for segment #%d!\n", index);
    result = -1;
    goto end;
  }

  printfsocket("  Decrypting table #%d for segment #%d\n", table_index, index);
  result = encsrv_decrypt_segment(state->device_fd,
                                 table_index, table_buffer, table_length);
  if (result != 0)
  {
    int errcode = errno;
    printfsocket("  Failed to decrypt table for segment #%d! Error: %d (%s)\n", index, errcode, strerror(errcode));
    goto end;
  }

  int is_compressed = (segment->flags & 8) != 0 ? 1 : 0;

  size_t block_size = 1 << (((segment->flags & 0xF000) >> 12) + 12);
  size_t block_count = (block_size + segment->uncompressed_size - 1) / block_size;

  size_t tail_size = segment->uncompressed_size % block_size;
  if (tail_size == 0)
  {
    tail_size = block_size;
  }

  pup_block_info* block_info = NULL;
  if (is_compressed == 1)
  {
    size_t valid_table_length = block_count * (32 + sizeof(pup_block_info));
    if (valid_table_length != table_length)
    {
      printfsocket("  Strange segment #%d table: %llu vs %llu\n",
                   index, valid_table_length, table_length);
    }
    block_info = (pup_block_info*)&table_buffer[32 * block_count];
  }

  block_buffer = memalign(0x4000, block_size);

  printfsocket("  Decrypting %d blocks...\n   ", block_count);

  int Seeked = 0;
  GetElapsed(0);

  size_t remaining_size = segment->compressed_size;
  int last_index = block_count - 1;
  for (int i = 0; i < block_count; i++)
  {
    printfsocket("  Decrypting block %d/%d...\n", i, block_count);

    if ((block_count > 50) && (i % 5 == 0) && (GetElapsed(15) == 1)) {
       uint32_t percentage = (uint32_t)(((float)i / (float)block_count) * 100.0f);
       sprintf(state->notifystr, "Approximately %d percent complete processing entry %s (%d/%d) from %s", percentage, state->entryname, state->entryid, state->totalentries, state->input_path);
       printf_notification(state->notifystr);
    }

    size_t read_size;
    ssize_t block_offset = 0;

    if (is_compressed == 1)
    {
      pup_block_info* tblock_info = &block_info[i];
      uint32_t unpadded_size = (tblock_info->size & ~0xFu) - (tblock_info->size & 0xFu);

      read_size = block_size;
      if (unpadded_size != block_size)
      {
        read_size = tblock_info->size;
        if (i != last_index || tail_size != tblock_info->size)
        {
          read_size &= ~0xFu;
        }
      }

      if (block_info->offset != 0)
      {
          block_offset = tblock_info->offset;
      }

    }
    else
    {
      read_size = remaining_size;
      if (block_size < read_size)
      {
        read_size = block_size;
      }
    }

    size_t SeekTo = DIO_NOSEEK;
    if (Seeked == 0) {
        SeekTo = (block_offset != 0) ? (segment->offset + block_offset) : segment->offset;
    } else {
        SeekTo = (Seeked != 0) ? DIO_NOSEEK : segment->offset;
    }

    ssize_t bytesread = readbytes(state, SeekTo, read_size, block_buffer, block_size);
    if (bytesread != read_size)
    {
      printfsocket("  Failed to read block %d for segment #%d! %d\n", i, index, bytesread);
      goto end;
    }

    result = encsrv_decrypt_segment_block(state->device_fd, index, i, block_buffer,
					 read_size, table_buffer, table_length);
    if (result < 0)
    {
      int errcode = errno;
      printfsocket("  Failed to decrypt block for segment #%d! Error: %d (%s)\n", index, errcode, strerror(errcode));
      goto end;
    }

    ssize_t byteswritten = writebytes(state, SeekTo, read_size, block_buffer, block_size);
    if (byteswritten != read_size)
    {
      printfsocket("  Failed to write block %d for segment #%d!\n", i, index);
      goto end;
    }

    Seeked = 1;
    remaining_size -= read_size;
  }

end:
  if (block_buffer != NULL)
  {
    free(block_buffer);
  }

  if (table_buffer != NULL)
  {
    free(table_buffer);
  }

  return result;
}

