#include "kernel.h"
#include "libc.h"
#include "module.h"
#include "syscall.h"

#include "sysutil.h"

int sysUtilHandle;
int libSceSystemService;
int libSceUserService;

int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char *message);
int (*sceSystemServiceLaunchWebBrowser)(const char *uri, void *);
int (*sceUserServiceInitialize)(void *);
int (*sceUserServiceGetLoginUserIdList)(SceUserServiceLoginUserIdList *);
int (*sceUserServiceGetUserName)(int32_t userId, char *userName, const size_t size);
int (*sceUserServiceGetInitialUser)(int32_t *);
int (*sceUserServiceTerminate)();
int (*sceKernelReboot)();

void initSysUtil(void) {
  if (!sysUtilHandle) {
    sysUtilHandle = sceKernelLoadStartModule("/system/common/lib/libSceSysUtil.sprx", 0, 0, 0, NULL, NULL);

    RESOLVE(sysUtilHandle, sceSysUtilSendSystemNotificationWithText);
  }

  if (!libSceSystemService) {
    libSceSystemService = sceKernelLoadStartModule("/system/common/lib/libSceSystemService.sprx", 0, 0, 0, NULL, NULL);

    RESOLVE(libSceSystemService, sceSystemServiceLaunchWebBrowser);
  }
}

void initUserService(void) {
  if (libSceUserService) {
    return;
  }

  libSceUserService = sceKernelLoadStartModule("/system/common/lib/libSceUserService.sprx", 0, 0, 0, NULL, NULL);

  RESOLVE(libSceUserService, sceUserServiceInitialize);
  RESOLVE(libSceUserService, sceUserServiceGetInitialUser);
  RESOLVE(libSceUserService, sceUserServiceGetLoginUserIdList);
  RESOLVE(libSceUserService, sceUserServiceGetUserName);
  RESOLVE(libSceUserService, sceUserServiceTerminate);
}

void openBrowser(char *uri) {
  sceSystemServiceLaunchWebBrowser(uri, NULL);
}

int getUserIDList(SceUserServiceLoginUserIdList *userIdList) {
  initUserService();
  if (sceUserServiceInitialize(NULL) == 0) {
    if (sceUserServiceGetLoginUserIdList(userIdList) == 0) {
      sceUserServiceTerminate();
      return 0;
    }
  }
  return -1;
}

int32_t getUserID() {
  SceUserServiceLoginUserIdList userIdList;
  getUserIDList(&userIdList);
  for (int i = 0; i < 1; i++) {
    if (userIdList.userId[i] != -1) {
      return userIdList.userId[i];
    }
  }
  return -1;
}

char *getUserName(int32_t userId) {
  char *retval = malloc(SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1);
  if (retval == NULL) {
    return NULL;
  }
  initUserService();
  if (sceUserServiceInitialize(NULL) == 0) {
    char username[SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1];
    if (sceUserServiceGetUserName(userId, username, sizeof(username)) == 0) {
      strcpy(retval, username);
      sceUserServiceTerminate();
      return retval;
    }
  }
  free(retval);
  return NULL;
}

int32_t getInitialUser() {
  int32_t userId;
  initUserService();
  if (sceUserServiceInitialize(NULL) == 0) {
    if (sceUserServiceGetInitialUser(&userId) == 0) {
      sceUserServiceTerminate();
      return userId;
    }
  }
  return -1;
}

void shutdown() {
  int evf = syscall(540, "SceSysCoreReboot");
  syscall(546, evf, 0x4000, 0);
  syscall(541, evf);
  syscall(37, 1, 30);
}

void reboot() {
  int libkernel = sceKernelLoadStartModule("/system/common/lib/libkernel.sprx", 0, 0, 0, NULL, NULL);
  RESOLVE(libkernel, sceKernelReboot);
  sceKernelReboot();
}
