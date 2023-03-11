#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include "fw_defines.h"
#include "ps4.h"


struct kpayload_get_fw_version_info {
  uint64_t uaddr;
};

struct kpayload_get_fw_version_args {
  void *syscall_handler;
  struct kpayload_get_fw_version_info *kpayload_get_fw_version_info;
};

struct kpayload_jailbreak_info {
  uint64_t fw_version;
};

struct kpayload_jailbreak_args {
  void *syscall_handler;
  struct kpayload_jailbreak_info *kpayload_jailbreak_info;
};

struct kpayload_get_kbase_info {
  uint64_t fw_version;
  uint64_t uaddr;
};

struct kpayload_get_kbase_args {
  void *syscall_handler;
  struct kpayload_get_kbase_info *kpayload_get_kbase_info;
};

struct kpayload_kernel_dumper_info {
  uint64_t fw_version;
  uint64_t uaddr;
  uint64_t kaddr;
  size_t size;
};

struct kpayload_kernel_dumper_args {
  void *syscall_handler;
  struct kpayload_kernel_dumper_info *kpayload_kernel_dumper_info;
};

uint64_t get_fw_version(void);
int jailbreak(uint64_t fw_version);
uint64_t get_kernel_base(uint64_t fw_version);
int dump_kernel(uint64_t fw_version, uint64_t kaddr, uint64_t *dump, size_t size);

#endif
