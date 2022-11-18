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

int kpayload_jailbreak(struct thread *td, struct kpayload_firmware_args *args) {
  struct filedesc *fd;
  struct ucred *cred;
  fd = td->td_proc->p_fd;
  cred = td->td_proc->p_ucred;

  void *kernel_base;
  uint8_t *kernel_ptr;
  void **prison0;
  void **rootvnode;

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, jailbreak_macro);

  cred->cr_uid = 0;
  cred->cr_ruid = 0;
  cred->cr_rgid = 0;
  cred->cr_groups[0] = 0;

  cred->cr_prison = *prison0;
  fd->fd_rdir = fd->fd_jdir = *rootvnode;

  void *td_ucred = *(void **)(((char *)td) + 304);

  uint64_t *sonyCred = (uint64_t *)(((char *)td_ucred) + 96);
  *sonyCred = 0xffffffffffffffff;

  uint64_t *sceProcessAuthorityId = (uint64_t *)(((char *)td_ucred) + 88);
  *sceProcessAuthorityId = 0x3801000000000013;

  uint64_t *sceProcCap = (uint64_t *)(((char *)td_ucred) + 104);
  *sceProcCap = 0xffffffffffffffff;

  return 0;
}

int kpayload_mmap(struct thread *td, struct kpayload_firmware_args *args) {
  UNUSED(td);
  void *kernel_base;
  uint8_t *kernel_ptr;

  uint8_t *kmem;
  uint8_t *mmap_patch_1;
  uint8_t *mmap_patch_2;
  uint8_t *mmap_patch_3;

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, mmap_macro);

  uint64_t cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);

  kmem = (uint8_t *)mmap_patch_1;
  kmem[0] = 0xB8;
  kmem[1] = 0x01;
  kmem[2] = 0x00;
  kmem[3] = 0x00;
  kmem[4] = 0x00;
  kmem[5] = 0xC3;

  kmem = (uint8_t *)mmap_patch_2;
  kmem[0] = 0xB8;
  kmem[1] = 0x01;
  kmem[2] = 0x00;
  kmem[3] = 0x00;
  kmem[4] = 0x00;
  kmem[5] = 0xC3;

  kmem = (uint8_t *)mmap_patch_3;
  kmem[0] = 0x31;
  kmem[1] = 0xC0;
  kmem[2] = 0xEB;
  kmem[3] = 0x01;

  writeCr0(cr0);

  return 0;
}

int kpayload_aslr(struct thread *td, struct kpayload_firmware_args *args) {
  UNUSED(td);
  void *kernel_base;
  uint8_t *kernel_ptr;

  uint8_t *kmem;
  uint8_t *aslr_patch;

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, aslr_macro);

  uint64_t cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);

  // This may change depending on new firmware's function structure
  kmem = (uint8_t *)aslr_patch;
  kmem[0] = 0xEB;
  if (fw_version < 600 || fw_version > 755) {
    // 3.50-5.56 and 8.00-9.60
    kmem[1] = 0x00;
  }

  writeCr0(cr0);

  return 0;
}

int kpayload_kernel_clock(struct thread *td, struct kpayload_kclock_args *args) {
  UNUSED(td);
  void *kernel_base;

  void (*sceSblSrtcSetTime)(uint64_t tm);
  void (*sceSblSrtcClearTimeDifference)(uint64_t value);

  uint16_t fw_version = args->kpayload_kclock_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, kclock_macro);

  uint64_t set_time = args->kpayload_kclock_info->set_time;

  sceSblSrtcSetTime(set_time);

  return 0;
}

int kpayload_enable_browser(struct thread *td, struct kpayload_firmware_args *args) {
  UNUSED(td);
  void *kernel_base;

  uint64_t (*sceRegMgrSetInt)(uint32_t regId, int value);

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, enable_browser_macro);

  sceRegMgrSetInt(0x3C040000, 0);

  return 0;
}

int kpayload_target_id(struct thread *td, struct kpayload_target_id_args *args) {
  UNUSED(td);
  void *kernel_base;
  uint8_t *kernel_ptr;

  uint8_t *kmem;
  uint8_t *tid_patch;

  uint16_t fw_version = args->kpayload_target_id_info->fw_version;
  uint8_t spoof = args->kpayload_target_id_info->spoof;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, tid_macro);

  uint64_t cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);

  kmem = (uint8_t *)tid_patch;
  kmem[0] = spoof;

  writeCr0(cr0);

  return 0;
}

int kpayload_perm_uart(struct thread *td, struct kpayload_firmware_args *args) {
  UNUSED(td);
  void *kernel_base;

  uint64_t (*icc_nvs_write)(uint32_t block, uint32_t offset, uint32_t size, void *value);

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, icc_nvs_write_macro);

  char uart = 1;
  icc_nvs_write(4, 0x31F, 1, &uart);

  return 0;
}

int kpayload_exit_idu(struct thread *td, struct kpayload_firmware_args *args) {
  UNUSED(td);
  void *kernel_base;

  uint64_t (*icc_nvs_write)(uint32_t block, uint32_t offset, uint32_t size, void *value);

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, icc_nvs_write_macro);

  char flag = 0;
  icc_nvs_write(4, 0x1600, 1, &flag);

  return 0;
}

int kpayload_npdrm_patch(struct thread *td, struct kpayload_firmware_args *args) {
  UNUSED(td);
  void *kernel_base;
  uint8_t *kernel_ptr;

  uint8_t *kmem;
  uint8_t *npdrm_close;
  uint8_t *npdrm_open;
  uint8_t *npdrm_ioctl;

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, npdrm_macro);

  uint64_t cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);

  kmem = (uint8_t *)npdrm_open;
  kmem[0] = 0x31;
  kmem[1] = 0xC0;
  kmem[2] = 0xC3;

  kmem = (uint8_t *)npdrm_close;
  kmem[0] = 0x31;
  kmem[1] = 0xC0;
  kmem[2] = 0xC3;

  // This may change depending on new firmware's function structure
  kmem = (uint8_t *)npdrm_ioctl;
  kmem[0] = 0xEB;
  if (fw_version < 500) {
    // 3.50-4.74
    kmem[1] = 0x04;
  } else {
    // 5.00-9.00
    kmem[1] = 0x00;
  }

  writeCr0(cr0);

  return 0;
}

