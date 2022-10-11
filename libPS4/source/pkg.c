// Copyright (C) 2013       Hykem <hykem@hotmail.com>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/gpl-2.0.txt

#include "file.h"
#include "libc.h"
#include "strings.h"

#include "pkg.h"

#define EOF '\00'

// Helper functions.
static inline uint16_t bswap_16(uint16_t val) {
  return ((val & (uint16_t)0x00ffU) << 8) | ((val & (uint16_t)0xff00U) >> 8);
}

static inline uint32_t bswap_32(uint32_t val) {
  return ((val & (uint32_t)0x000000ffUL) << 24) | ((val & (uint32_t)0x0000ff00UL) << 8) | ((val & (uint32_t)0x00ff0000UL) >> 8) | ((val & (uint32_t)0xff000000UL) >> 24);
}

int isfpkg(char *pkgfn) {
  int result = 0;
  FILE *in = NULL;
  struct cnt_pkg_main_header m_header;
  struct cnt_pkg_content_header c_header;
  memset_s(&m_header, sizeof(struct cnt_pkg_main_header), 0, sizeof(struct cnt_pkg_main_header));
  memset_s(&c_header, sizeof(struct cnt_pkg_main_header), 0, sizeof(struct cnt_pkg_content_header));

  if ((in = fopen(pkgfn, "rb")) == NULL) {
    result = 1;
  } else {
    char buffer[5];
    fseek(in, 1, SEEK_SET);
    fread(buffer, 1, 4, in);
    if (strcmp(buffer, "CNT@") == 0) {
      result = 0;
    } else {
      fseek(in, 0, SEEK_SET);
      fread(&m_header, 1, 0x180, in);

      if (m_header.magic != PS4_PKG_MAGIC) {
        result = 2;
      } else if (bswap_32(m_header.type) != 1) {
        result = 3;
      }
    }
  }

  fclose(in);
  return result;
}

static void _mkdir(const char *dir) {
  char tmp[256];
  char *p = NULL;

  snprintf(tmp, sizeof(tmp), "%s", dir);
  for (p = tmp + 1; *p; p++) {
    if (*p == '/') {
      *p = 0;
      mkdir(tmp, 0777);
      *p = '/';
    }
  }
}

static char *get_entry_name_by_type(uint32_t type) {
  char *entry_name = malloc(32);

  if ((type >= 0x1201) && (type <= 0x121F)) {
    sprintf(entry_name, "icon0_%02u.png", type - 0x1201);
  } else if ((type >= 0x1241) && (type <= 0x125F)) {
    sprintf(entry_name, "pic1_%02u.png", type - 0x1241);
  } else if ((type >= 0x1261) && (type <= 0x127F)) {
    sprintf(entry_name, "changeinfo/changeinfo_%02u.xml", type - 0x1261);
  } else if ((type >= 0x1281) && (type <= 0x129F)) {
    sprintf(entry_name, "icon0_%02u.dds", type - 0x1281);
  } else if ((type >= 0x12C1) && (type <= 0x12DF)) {
    sprintf(entry_name, "pic1_%02u.dds", type - 0x12C1);
  } else if ((type >= 0x1400) && (type <= 0x147F)) {
    sprintf(entry_name, "trophy/trophy%02u.trp", type - 0x1400);
  } else if ((type >= 0x1600) && (type <= 0x1609)) {
    sprintf(entry_name, "keymap_rp/%03u.png", type - 0x15FF);
  } else if ((type >= 0x1610) && (type <= 0x16F5)) {
    sprintf(entry_name, "keymap_rp/%02u/%03u.png", (type - 0x1610) / 10, (((type - 0x160F) % 10) ? (type - 0x160F) % 10 : 10));
  } else {
    free(entry_name);
    entry_name = NULL;
    switch (type) {
    case 0x0400:
      entry_name = "license.dat";
      break;
    case 0x0401:
      entry_name = "license.info";
      break;
    case 0x0402:
      entry_name = "nptitle.dat";
      break;
    case 0x0403:
      entry_name = "npbind.dat";
      break;
    case 0x0404:
      entry_name = "selfinfo.dat";
      break;
    case 0x0406:
      entry_name = "imageinfo.dat";
      break;
    case 0x0407:
      entry_name = "target-deltainfo.dat";
      break;
    case 0x0408:
      entry_name = "origin-deltainfo.dat";
      break;
    case 0x0409:
      entry_name = "psreserved.dat";
      break;
    case 0x1000:
      entry_name = "param.sfo";
      break;
    case 0x1001:
      entry_name = "playgo-chunk.dat";
      break;
    case 0x1002:
      entry_name = "playgo-chunk.sha";
      break;
    case 0x1003:
      entry_name = "playgo-manifest.xml";
      break;
    case 0x1004:
      entry_name = "pronunciation.xml";
      break;
    case 0x1005:
      entry_name = "pronunciation.sig";
      break;
    case 0x1006:
      entry_name = "pic1.png";
      break;
    case 0x1007:
      entry_name = "pubtoolinfo.dat";
      break;
    case 0x1008:
      entry_name = "app/playgo-chunk.dat";
      break;
    case 0x1009:
      entry_name = "app/playgo-chunk.sha";
      break;
    case 0x100A:
      entry_name = "app/playgo-manifest.xml";
      break;
    case 0x100B:
      entry_name = "shareparam.json";
      break;
    case 0x100C:
      entry_name = "shareoverlayimage.png";
      break;
    case 0x100D:
      entry_name = "save_data.png";
      break;
    case 0x100E:
      entry_name = "shareprivacyguardimage.png";
      break;
    case 0x1200:
      entry_name = "icon0.png";
      break;
    case 0x1220:
      entry_name = "pic0.png";
      break;
    case 0x1240:
      entry_name = "snd0.at9";
      break;
    case 0x1260:
      entry_name = "changeinfo/changeinfo.xml";
      break;
    case 0x1280:
      entry_name = "icon0.dds";
      break;
    case 0x12A0:
      entry_name = "pic0.dds";
      break;
    case 0x12C0:
      entry_name = "pic1.dds";
      break;
    }
  }

  return entry_name;
}

