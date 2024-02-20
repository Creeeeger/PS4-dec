#include "kernel_utils.h"

#define X86_CR0_WP (1 << 16)

int kpayload_get_fw_version(struct thread *td, struct kpayload_get_fw_version_args *args) {
  void *kernel_base = 0;
  int (*copyout)(const void *kaddr, void *uaddr, size_t len) = 0;

  uint64_t fw_version = 0x666;

if (!memcmp((char *)(&((uint8_t *)__readmsr(0xC0000082))[-K1100_XFAST_SYSCALL]), (char[4]){0x7F, 0x45, 0x4C, 0x46}, 4)) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1100_XFAST_SYSCALL];
    if (!memcmp((char *)(kernel_base + K1100_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x1100; // 11.00
        copyout = (void *)(kernel_base + K1100_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K1070_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x1070; // 10.70
        copyout = (void *)(kernel_base + K1070_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K1050_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x1050; // 10.50
        copyout = (void *)(kernel_base + K1050_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K1000_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x1000; // 10.00
        copyout = (void *)(kernel_base + K1000_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K960_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x960; // 9.60
        copyout = (void *)(kernel_base + K960_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K950_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x950; // 9.50
        copyout = (void *)(kernel_base + K950_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K900_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x900; // 9.00
        copyout = (void *)(kernel_base + K900_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K850_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x850; // 8.50
        copyout = (void *)(kernel_base + K850_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K800_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x800; // 8.00
        copyout = (void *)(kernel_base + K800_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K750_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x750; // 7.50
        copyout = (void *)(kernel_base + K750_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K700_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
        fw_version = 0x700; // 7.00, 7.01, 7.02
        copyout = (void *)(kernel_base + K700_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K670_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x670; // 6.70, 6.71, and 6.72
      copyout = (void *)(kernel_base + K670_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K650_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x650; // 6.50 and 6.51
      copyout = (void *)(kernel_base + K650_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K620_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x620; // 6.20
      copyout = (void *)(kernel_base + K620_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K600_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x600; // 6.00 and 6.02
      copyout = (void *)(kernel_base + K600_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K555_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x555; // 5.55 and 5.56
      copyout = (void *)(kernel_base + K555_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K553_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x553; // 5.53
      copyout = (void *)(kernel_base + K553_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K550_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x550; // 5.50
      copyout = (void *)(kernel_base + K550_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K505_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x505; // 5.05 and 5.07
      copyout = (void *)(kernel_base + K505_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K503_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x503; // 5.03
      copyout = (void *)(kernel_base + K503_COPYOUT);
    } else if (!memcmp((char *)(kernel_base + K500_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x500; // 5.00 and 5.01
      copyout = (void *)(kernel_base + K500_COPYOUT);
    }
  } else if (!memcmp((char *)(&((uint8_t *)__readmsr(0xC0000082))[-K470_XFAST_SYSCALL]), (char[4]){0x7F, 0x45, 0x4C, 0x46}, 4)) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K470_XFAST_SYSCALL];
    if (!memcmp((char *)(kernel_base + K470_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x470; // 4.70
      copyout = (void *)(kernel_base + K470_COPYOUT);
    }
  } else if (!memcmp((char *)(&((uint8_t *)__readmsr(0xC0000082))[-K471_XFAST_SYSCALL]), (char[4]){0x7F, 0x45, 0x4C, 0x46}, 4)) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K471_XFAST_SYSCALL];
    if (!memcmp((char *)(kernel_base + K471_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x471; // 4.71, 4.72, 4.73, and 4.74
      copyout = (void *)(kernel_base + K471_COPYOUT);
    }
  } else if (!memcmp((char *)(&((uint8_t *)__readmsr(0xC0000082))[-K450_XFAST_SYSCALL]), (char[4]){0x7F, 0x45, 0x4C, 0x46}, 4)) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K450_XFAST_SYSCALL];
    if (!memcmp((char *)(kernel_base + K450_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x450; // 4.50 and 4.55
      copyout = (void *)(kernel_base + K450_COPYOUT);
    }
  } else if (!memcmp((char *)(&((uint8_t *)__readmsr(0xC0000082))[-K406_XFAST_SYSCALL]), (char[4]){0x7F, 0x45, 0x4C, 0x46}, 4)) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K406_XFAST_SYSCALL];
    if (!memcmp((char *)(kernel_base + K406_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      // TODO: 4.06 and 4.07 overlap here even though other offsets to not
      fw_version = 0x406; // 4.06 and 4.07
      copyout = (void *)(kernel_base + K406_COPYOUT);
    }
  } else if (!memcmp((char *)(&((uint8_t *)__readmsr(0xC0000082))[-K405_XFAST_SYSCALL]), (char[4]){0x7F, 0x45, 0x4C, 0x46}, 4)) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K405_XFAST_SYSCALL];
    if (!memcmp((char *)(kernel_base + K405_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x405; // 4.05
      copyout = (void *)(kernel_base + K405_COPYOUT);
    }
  } else if (!memcmp((char *)(&((uint8_t *)__readmsr(0xC0000082))[-K400_XFAST_SYSCALL]), (char[4]){0x7F, 0x45, 0x4C, 0x46}, 4)) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K400_XFAST_SYSCALL];
    if (!memcmp((char *)(kernel_base + K400_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x400; // 4.00 and 4.01
      copyout = (void *)(kernel_base + K400_COPYOUT);
    }
  } else if (!memcmp((char *)(&((uint8_t *)__readmsr(0xC0000082))[-K370_XFAST_SYSCALL]), (char[4]){0x7F, 0x45, 0x4C, 0x46}, 4)) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K370_XFAST_SYSCALL];
    if (!memcmp((char *)(kernel_base + K370_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x370; // 3.70
      copyout = (void *)(kernel_base + K370_COPYOUT);
    }
  } else if (!memcmp((char *)(&((uint8_t *)__readmsr(0xC0000082))[-K355_XFAST_SYSCALL]), (char[4]){0x7F, 0x45, 0x4C, 0x46}, 4)) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K355_XFAST_SYSCALL];
    if (!memcmp((char *)(kernel_base + K355_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x355; // 3.55
      copyout = (void *)(kernel_base + K355_COPYOUT);
    }
  } else if (!memcmp((char *)(&((uint8_t *)__readmsr(0xC0000082))[-K350_XFAST_SYSCALL]), (char[4]){0x7F, 0x45, 0x4C, 0x46}, 4)) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K350_XFAST_SYSCALL];
    if (!memcmp((char *)(kernel_base + K350_PRINTF), (char[12]){0x55, 0x48, 0x89, 0xE5, 0x53, 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8D, 0x1D}, 12)) {
      fw_version = 0x350; // 3.50
      copyout = (void *)(kernel_base + K350_COPYOUT);
    }
  } else {
    return -1;
  }

  uint64_t uaddr = args->kpayload_get_fw_version_info->uaddr;
  copyout(&fw_version, (uint64_t *)uaddr, 8);

  return 0;
}

int kpayload_jailbreak(struct thread *td, struct kpayload_jailbreak_args *args) {
  struct filedesc *fd;
  struct ucred *cred;
  fd = td->td_proc->p_fd;
  cred = td->td_proc->p_ucred;

  void *kernel_base;
  uint8_t *kernel_ptr;
  void **got_prison0;
  void **got_rootvnode;

  uint64_t fw_version = args->kpayload_jailbreak_info->fw_version;


  if (fw_version == 0x350) {
    // 3.50
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K350_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K350_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K350_ROOTVNODE];
  } else if (fw_version == 0x355) {
    // 3.55
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K355_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K355_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K355_ROOTVNODE];
  } else if (fw_version == 0x370) {
    // 3.70
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K370_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K370_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K370_ROOTVNODE];
  } else if (fw_version == 0x400) {
    // 4.00 and 4.01
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K400_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K400_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K400_ROOTVNODE];
  } else if (fw_version == 0x405) {
    // 4.05
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K405_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K405_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K405_ROOTVNODE];
  } else if (fw_version == 0x406) {
    // 4.06
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K406_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K406_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K406_ROOTVNODE];
  } else if (fw_version == 0x407) {
    // 4.07
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K407_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K407_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K407_ROOTVNODE];
  } else if (fw_version == 0x450) {
    // 4.50 and 4.55
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K450_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K450_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K450_ROOTVNODE];
  } else if (fw_version == 0x470) {
    // 4.70
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K470_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K470_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K470_ROOTVNODE];
  } else if (fw_version == 0x471) {
    // 4.71, 4.72, 4.73, and 4.74
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K471_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K471_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K471_ROOTVNODE];
  } else if (fw_version == 0x500) {
    // 5.00 and 5.01
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K501_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K500_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K500_ROOTVNODE];
  } else if (fw_version == 0x503) {
    // 5.03
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K503_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K503_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K503_ROOTVNODE];
  } else if (fw_version == 0x505) {
    // 5.05 and 5.07
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K505_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K505_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K505_ROOTVNODE];
  } else if (fw_version == 0x550) {
    // 5.50
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K550_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K550_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K550_ROOTVNODE];
  } else if (fw_version == 0x553) {
    // 5.53
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K553_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K553_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K553_ROOTVNODE];
  } else if (fw_version == 0x555) {
    // 5.55 and 5.56
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K555_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K555_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K555_ROOTVNODE];
  } else if (fw_version == 0x600) {
    // 6.00 and 6.02
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K600_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K600_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K600_ROOTVNODE];
  } else if (fw_version == 0x620) {
    // 6.20
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K620_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K620_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K620_ROOTVNODE];
  } else if (fw_version == 0x650) {
    // 6.50 and 6.51
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K650_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K650_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K650_ROOTVNODE];
  } else if (fw_version == 0x670) {
    // 6.70, 6.71, and 6.72
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K670_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K670_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K670_ROOTVNODE];
  } else if (fw_version == 0x700) {
    // 7.00, 7.01, 7.02
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K700_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K700_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K700_ROOTVNODE];
  } else if (fw_version == 0x750) {
    // 7.50
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K750_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K750_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K750_ROOTVNODE];
  } else if (fw_version == 0x800) {
    // 8.00
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K800_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K800_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K800_ROOTVNODE];
  } else if (fw_version == 0x850) {
    // 8.50
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K850_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K850_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K850_ROOTVNODE];
  } else if (fw_version == 0x900) {
    // 9.00
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K900_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K900_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K900_ROOTVNODE];
  } else if (fw_version == 0x950) {
    // 9.50
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K950_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K950_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K950_ROOTVNODE];
  } else if (fw_version == 0x960) {
    // 9.60
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K960_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K960_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K960_ROOTVNODE];
  } else if (fw_version == 0x1000) {
    // 10.00
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1000_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K1000_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K1000_ROOTVNODE];
  } else if (fw_version == 0x1050) {
    // 10.50
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1050_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K1050_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K1050_ROOTVNODE];
  } else if (fw_version == 0x1070) {
    // 10.70
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1070_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K1070_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K1070_ROOTVNODE];
  } else if (fw_version == 0x1100) {
    // 11.00
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1100_XFAST_SYSCALL];
    kernel_ptr = (uint8_t *)kernel_base;
    got_prison0 = (void **)&kernel_ptr[K1100_PRISON_0];
    got_rootvnode = (void **)&kernel_ptr[K1100_ROOTVNODE];
  } else {
    return -1;
  }

  cred->cr_uid = 0;
  cred->cr_ruid = 0;
  cred->cr_rgid = 0;
  cred->cr_groups[0] = 0;

  cred->cr_prison = *got_prison0;
  fd->fd_rdir = fd->fd_jdir = *got_rootvnode;

  void *td_ucred = *(void **)(((char *)td) + 304);

  uint64_t *sonyCred = (uint64_t *)(((char *)td_ucred) + 96);
  *sonyCred = 0xffffffffffffffff;

  uint64_t *sceProcessAuthorityId = (uint64_t *)(((char *)td_ucred) + 88);
  *sceProcessAuthorityId = 0x3801000000000013;

  uint64_t *sceProcCap = (uint64_t *)(((char *)td_ucred) + 104);
  *sceProcCap = 0xffffffffffffffff;

  return 0;
}

int kpayload_get_kbase(struct thread *td, struct kpayload_get_kbase_args *args) {
  void *kernel_base;
  int (*copyout)(const void *kaddr, void *uaddr, size_t len);

  uint64_t fw_version = args->kpayload_get_kbase_info->fw_version;

  if (fw_version == 0x350) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K350_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K350_COPYOUT);
  } else if (fw_version == 0x355) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K355_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K355_COPYOUT);
  } else if (fw_version == 0x370) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K370_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K370_COPYOUT);
  } else if (fw_version == 0x400 || 0x401) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K400_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K400_COPYOUT);
  } else if (fw_version == 0x405) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K405_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K405_COPYOUT);
  } else if (fw_version == 0x406) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K406_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K406_COPYOUT);
  } else if (fw_version == 0x407) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K407_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K407_COPYOUT);
  } else if (fw_version == 0x450 || fw_version == 0x455) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K450_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K450_COPYOUT);
  } else if (fw_version == 0x470) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K470_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K470_COPYOUT);
  } else if (fw_version == 0x471 || fw_version == 0x472 || fw_version == 0x473 || fw_version == 0x474) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K471_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K471_COPYOUT);
  } else if (fw_version == 0x500 || fw_version == 0x501) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K500_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K500_COPYOUT);
  } else if (fw_version == 0x503) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K503_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K503_COPYOUT);
  } else if (fw_version == 0x505 || fw_version == 0x507) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K505_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K505_COPYOUT);
  } else if (fw_version == 0x550) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K550_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K550_COPYOUT);
  } else if (fw_version == 0x553) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K553_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K553_COPYOUT);
  } else if (fw_version == 0x555 || fw_version == 0x556) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K555_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K555_COPYOUT);
  } else if (fw_version == 0x600 || fw_version == 0x602) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K600_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K600_COPYOUT);
  } else if (fw_version == 0x620) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K620_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K620_COPYOUT);
  } else if (fw_version == 0x650 || fw_version == 0x651) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K650_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K650_COPYOUT);
  } else if (fw_version == 0x670 || fw_version == 0x671 || fw_version == 0x672) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K670_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K670_COPYOUT);
  } else if (fw_version == 0x700 || fw_version == 0x701 || fw_version == 0x702) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K700_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K700_COPYOUT);
  } else if (fw_version == 0x750 || fw_version == 0x751 || fw_version == 0x755) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K750_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K750_COPYOUT);
  } else if (fw_version == 0x800 || fw_version == 0x801 || fw_version == 0x803) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K800_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K800_COPYOUT);
  } else if (fw_version == 0x850 || fw_version == 0x852) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K850_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K850_COPYOUT);
  } else if (fw_version == 0x900 || fw_version == 0x903 || fw_version == 0x904) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K900_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K900_COPYOUT);
  } else if (fw_version == 0x950 || fw_version == 0x951) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K950_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K950_COPYOUT);
  } else if (fw_version == 0x960) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K960_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K960_COPYOUT);
  } else if (fw_version == 0x1000 || fw_version == 0x1001) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1000_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K1000_COPYOUT);
  } else if (fw_version == 0x1050) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1050_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K1050_COPYOUT);
  } else if (fw_version == 0x1070 || fw_version == 0x1071) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1070_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K1070_COPYOUT);
  } else if (fw_version == 0x1100 || fw_version == 0x1102) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1100_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K1100_COPYOUT);
  } else {
    return -1;
  }

  uint64_t uaddr = args->kpayload_get_kbase_info->uaddr;
  copyout(&kernel_base, (uint64_t *)uaddr, 8);

  return 0;
}

int kpayload_kernel_dumper(struct thread *td, struct kpayload_kernel_dumper_args *args) {
  void *kernel_base;
  int (*copyout)(const void *kaddr, void *uaddr, size_t len);

  uint64_t fw_version = args->kpayload_kernel_dumper_info->fw_version;

  if (fw_version == 0x350) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K350_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K350_COPYOUT);
  } else if (fw_version == 0x355) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K355_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K355_COPYOUT);
  } else if (fw_version == 0x370) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K370_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K370_COPYOUT);
  } else if (fw_version == 0x400 || 0x401) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K400_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K400_COPYOUT);
  } else if (fw_version == 0x405) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K405_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K405_COPYOUT);
  } else if (fw_version == 0x406) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K406_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K406_COPYOUT);
  } else if (fw_version == 0x407) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K407_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K407_COPYOUT);
  } else if (fw_version == 0x450 || fw_version == 0x455) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K450_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K450_COPYOUT);
  } else if (fw_version == 0x470) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K470_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K470_COPYOUT);
  } else if (fw_version == 0x471 || fw_version == 0x472 || fw_version == 0x473 || fw_version == 0x474) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K471_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K471_COPYOUT);
  } else if (fw_version == 0x500 || fw_version == 0x501) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K500_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K500_COPYOUT);
  } else if (fw_version == 0x503) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K503_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K503_COPYOUT);
  } else if (fw_version == 0x505 || fw_version == 0x507) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K505_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K505_COPYOUT);
  } else if (fw_version == 0x550) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K550_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K550_COPYOUT);
  } else if (fw_version == 0x553) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K553_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K553_COPYOUT);
  } else if (fw_version == 0x555 || fw_version == 0x556) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K555_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K555_COPYOUT);
  } else if (fw_version == 0x600 || fw_version == 0x602) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K600_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K600_COPYOUT);
  } else if (fw_version == 0x620) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K620_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K620_COPYOUT);
  } else if (fw_version == 0x650 || fw_version == 0x651) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K650_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K650_COPYOUT);
  } else if (fw_version == 0x670 || fw_version == 0x671 || fw_version == 0x672) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K670_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K670_COPYOUT);
  } else if (fw_version == 0x700 || fw_version == 0x701 || fw_version == 0x702) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K700_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K700_COPYOUT);
  } else if (fw_version == 0x750 || fw_version == 0x751 || fw_version == 0x755) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K750_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K750_COPYOUT);
  } else if (fw_version == 0x800 || fw_version == 0x801 || fw_version == 0x803) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K800_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K800_COPYOUT);
  } else if (fw_version == 0x850 || fw_version == 0x852) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K850_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K850_COPYOUT);
  } else if (fw_version == 0x900 || fw_version == 0x903 || fw_version == 0x904) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K900_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K900_COPYOUT);
  } else if (fw_version == 0x950 || fw_version == 0x951) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K950_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K950_COPYOUT);
  } else if (fw_version == 0x960) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K960_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K960_COPYOUT);
  } else if (fw_version == 0x1000 || fw_version == 0x1001) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1000_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K1000_COPYOUT);
  } else if (fw_version == 0x1050) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1050_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K1050_COPYOUT);
  } else if (fw_version == 0x1070 || fw_version == 0x1071) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1070_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K1070_COPYOUT);
  } else if (fw_version == 0x1100 || fw_version == 0x1102) {
    kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K1100_XFAST_SYSCALL];
    copyout = (void *)(kernel_base + K1100_COPYOUT);
  } else {
    return -1;
  }

  uint64_t kaddr = args->kpayload_kernel_dumper_info->kaddr;
  uint64_t uaddr = args->kpayload_kernel_dumper_info->uaddr;
  size_t size = args->kpayload_kernel_dumper_info->size;

  int cpRet = copyout((uint64_t *)kaddr, (uint64_t *)uaddr, size);

  if (cpRet == -1) {
    memset((uint64_t *)uaddr, 0, size);
  }

  return cpRet;
}

uint64_t get_fw_version(void) {
  uint64_t fw_version = 0x666;
  uint64_t *fw_version_ptr = mmap(NULL, 8, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  struct kpayload_get_fw_version_info kpayload_get_fw_version_info;
  kpayload_get_fw_version_info.uaddr = (uint64_t)fw_version_ptr;
  kexec(&kpayload_get_fw_version, &kpayload_get_fw_version_info);
  memcpy(&fw_version, fw_version_ptr, 8);
  munmap(fw_version_ptr, 8);

  return fw_version;
}

int jailbreak(uint64_t fw_version) {
  struct kpayload_jailbreak_info kpayload_jailbreak_info;
  kpayload_jailbreak_info.fw_version = fw_version;
  kexec(&kpayload_jailbreak, &kpayload_jailbreak_info);

  return 0;
}

uint64_t get_kernel_base(uint64_t fw_version) {
  uint64_t kernel_base = -1;
  uint64_t *kernel_base_ptr = mmap(NULL, 8, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  struct kpayload_get_kbase_info kpayload_get_kbase_info;
  kpayload_get_kbase_info.fw_version = fw_version;
  kpayload_get_kbase_info.uaddr = (uint64_t)kernel_base_ptr;
  kexec(&kpayload_get_kbase, &kpayload_get_kbase_info);
  memcpy(&kernel_base, kernel_base_ptr, 8);
  munmap(kernel_base_ptr, 8);

  return kernel_base;
}

int dump_kernel(uint64_t fw_version, uint64_t kaddr, uint64_t *dump, size_t size) {
  struct kpayload_kernel_dumper_info kpayload_kernel_dumper_info;
  kpayload_kernel_dumper_info.fw_version = fw_version;
  kpayload_kernel_dumper_info.kaddr = kaddr;
  kpayload_kernel_dumper_info.uaddr = (uint64_t)dump;
  kpayload_kernel_dumper_info.size = size;
  kexec(&kpayload_kernel_dumper, &kpayload_kernel_dumper_info);

  return 0;
}
