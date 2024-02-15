#pragma once

#ifndef SYSUTIL_H
#define SYSUTIL_H

#include "debug.h"
#include "kernel.h"
#include "libc.h"
#include "network.h"
#include "types.h"

#define SCE_USER_SERVICE_MAX_LOGIN_USERS 4
#define SCE_USER_SERVICE_MAX_USER_NAME_LENGTH 16

extern int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char *message);

typedef struct SceUserServiceLoginUserIdList {
  int32_t userId[SCE_USER_SERVICE_MAX_LOGIN_USERS];
} SceUserServiceLoginUserIdList;

void initSysUtil(void);
void openBrowser(char *uri);
int getUserIDList(SceUserServiceLoginUserIdList *userIdList);
int32_t getUserID();
char *getUserName(int32_t userId);
int32_t getInitialUser();
void reboot();
void shutdown();

// HUGE shoutout to OSM-Made for removing the need to use the football/soccer icon in the notifications
// https://github.com/OSM-Made/PS4-Notify
#define printf_notification(...)                                                                         \
  do {                                                                                                   \
    SceNotificationRequest noti_buffer;                                                                  \
    char icon_uri[38] = "cxml://psnotification/tex_icon_system";                                         \
    noti_buffer.type = 0;                                                                                \
    noti_buffer.unk3 = 0;                                                                                \
    noti_buffer.use_icon_image_uri = 1;                                                                  \
    noti_buffer.target_id = -1;                                                                          \
    snprintf_s(noti_buffer.uri, sizeof(noti_buffer.uri), icon_uri);                                      \
    snprintf_s(noti_buffer.message, sizeof(noti_buffer.message), ##__VA_ARGS__);                         \
    printf_debug("[NOTIFICATION]: %s\n", noti_buffer.message);                                           \
    sceKernelSendNotificationRequest(0, (SceNotificationRequest *)&noti_buffer, sizeof(noti_buffer), 0); \
  } while (0)

#endif
