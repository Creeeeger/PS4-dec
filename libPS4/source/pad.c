#include "kernel.h"
#include "module.h"

#include "pad.h"

int libPad;

int (*scePadInit)(void);
int (*scePadOpen)(int userID, int, int, void *);
int (*scePadClose)(int handle);
int (*scePadRead)(int handle, void *data, int count);
int (*scePadReadState)(int handle, void *data);

void initPad(void) {
  if (libPad) {
    return;
  }

  libPad = sceKernelLoadStartModule("libScePad.sprx", 0, 0, 0, NULL, NULL);

  RESOLVE(libPad, scePadInit);
  RESOLVE(libPad, scePadOpen);
  RESOLVE(libPad, scePadClose);
  RESOLVE(libPad, scePadRead);
  RESOLVE(libPad, scePadReadState);

  scePadInit();
}
