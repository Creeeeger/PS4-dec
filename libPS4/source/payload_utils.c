#include "debug.h"
#include "elf.h"
#include "file.h"
#include "fw_defines.h"
#include "kernel.h"
#include "libc.h"
#include "memory.h"

#include "payload_utils.h"

uint16_t g_firmware;

int is_fw_spoofed() {
  SceFwInfo ssv_fw;
  char ssv_fw_trimmed[5];
  uint16_t ssv_fw_final;

  uint16_t libc_fw = get_firmware();
  sceKernelGetSystemSwVersion(&ssv_fw);

  char temp[0x1C];
  snprintf(temp, 0x1C, "%s", ssv_fw.version_string);

  char *first = strtok(temp, ".");
  char *second = strtok(NULL, ".");

  sprintf(ssv_fw_trimmed, "%s%c%c", first, second[0], second[1]);

  ssv_fw_final = atoi(ssv_fw_trimmed);

  if (ssv_fw_final != libc_fw) {
    return 1;
  }

  return 0;
}

int is_jailbroken() {
  int fd = open("/user/.jailbreak", O_WRONLY, 0777);
  if (fd >= 0) {
    close(fd);
    unlink("/user/.jailbreak");
    return 1;
  } else {
    return 0;
  }
}

int kpayload_kbase(struct thread *td, struct kpayload_kbase_args *args) {
  UNUSED(td);
  void *kernel_base;

  int (*copyout)(const void *kaddr, void *uaddr, size_t len);

  uint16_t fw_version = args->kpayload_kbase_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, copyout_macro);

  uint64_t uaddr = args->kpayload_kbase_info->uaddr;
  copyout(&kernel_base, (uint64_t *)uaddr, 8);

  return 0;
}

int kpayload_dump(struct thread *td, struct kpayload_dump_args *args) {
  UNUSED(td);
  void *kernel_base;

  int (*copyout)(const void *kaddr, void *uaddr, size_t len);

  uint16_t fw_version = args->kpayload_dump_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, copyout_macro);

  uint64_t kaddr = args->kpayload_dump_info->kaddr;
  uint64_t uaddr = args->kpayload_dump_info->uaddr;
  size_t size = args->kpayload_dump_info->size;

  int ret = copyout((uint64_t *)kaddr, (uint64_t *)uaddr, size);

  if (ret == -1) {
    memset((uint64_t *)uaddr, 0, size);
  }

  return ret;
}

