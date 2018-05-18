/**
 * Copyright 2018 Pushkar Piggott
 *
 * SquarePyramid.c
 */

#include "SquarePyramid.h"
#include "Presence.h"   // GLYPH
#include "SetOf.h"
#include "Display.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
#define ZOFFSET(z) (spXY * (MARGIN + z))
#define YOFFSET(y) (spX * (MARGIN + y))

///////////////////////////////////////////////////////////////////////////////
int spHeight = 0;
int spX = 0;
int spXY = 0;
int spXYZ = 0;

///////////////////////////////////////////////////////////////////////////////
void spSetHeight(int height) {
   spHeight = height;
   spX = spHeight + MARGIN * 2;
   spXY = spX * spX;
   spXYZ = spX * spXY;
}

///////////////////////////////////////////////////////////////////////////////
int getPyramidVolume() {
   int size = 0;
   int z;
   for (z = 0; z < spHeight; ++z) {
      size += (z + 1) * (z + 1);
   }
   return size;
}

///////////////////////////////////////////////////////////////////////////////
// Clear the whole cube to absent. 
void spClear(TPlace* sp) {
   memset(sp, eAbsent, spXYZ);
}

///////////////////////////////////////////////////////////////////////////////
// Initialize all to eMargin with the pyramid hollowed out as eAbsent. 
void spInit(TPlace* sp) {
   memset(sp, eMargin, spXYZ);
   int z;
   for (z = 0; z < spHeight; ++z) {
      int zoffset = ZOFFSET(z);
      int y;
      for (y = 0; y <= z; ++y) {
         int yoffset = YOFFSET(y);
         int x;
         for (x = 0; x <= z; ++x) {
            sp[zoffset + yoffset + MARGIN + x] = eAbsent;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
// Initialize all to eAbsent with an all round margin. 
void spInitCube(TPlace* sp) {
   memset(sp, eMargin, spXYZ);
   int z;
   for (z = 0; z < spHeight; ++z) {
      int zoffset = ZOFFSET(z);
      int y;
      for (y = 0; y < spHeight; ++y) {
         int yoffset = YOFFSET(y);
         int x;
         for (x = 0; x < spHeight; ++x) {
            sp[zoffset + yoffset + MARGIN + x] = eAbsent;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void spEnumerate(TPlace* sp) {
   int c = 0;
   int z;
   for (z = 0; z < spHeight; ++z) {
      int y;
      for (y = 0; y <= z; ++y) {
         int x;
         for (x = 0; x <= z; ++x) {
            sp[SP_XYZ(x, y, z)] =  c++ % 26 + 1;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
TPlace* spCopy(TPlace* dst, const TPlace* src) {
   return memcpy(dst, src, spXYZ);
}

///////////////////////////////////////////////////////////////////////////////
int spEqual(const TPlace* sp1, const TPlace* sp2) {
   return 0 == memcmp(sp1, sp2, spXYZ);
}

///////////////////////////////////////////////////////////////////////////////
char* spRowToString(char* str, int y, int z, const TPlace* sp) {
   int zoffset = ZOFFSET(z);
   int yoffset = YOFFSET(y);
   int x;
   for (x = 0; x <= z; ++x) {
      *str++ = GLYPH(sp[zoffset + yoffset + MARGIN + x]);
   }
   return str;
}

///////////////////////////////////////////////////////////////////////////////
char* spCubeRowToString(char* str, int y, int z, const TPlace* sp) {
   int zoffset = ZOFFSET(z - MARGIN);
   int yoffset = YOFFSET(y - (2 * MARGIN));
   int x;
   for (x = 0; x < spX - 2; ++x) {
      *str++ = GLYPH(sp[zoffset + yoffset + MARGIN + x]);
   }
   return str;
}

///////////////////////////////////////////////////////////////////////////////
char* spWholeRowToString(char* str, int y, int z, const TPlace* sp) {
   int zoffset = ZOFFSET(z - MARGIN);
   int yoffset = YOFFSET(y - MARGIN);
   int x;
   for (x = 0; x < spX; ++x) {
      *str++ = GLYPH(sp[zoffset + yoffset + x]);
   }
   return str;
}

///////////////////////////////////////////////////////////////////////////////
void spTestPyramid() {
   TPlace* sps = SP_NEW(3);
   spClear(&sps[SPS(0)]);
   spInit(&sps[SPS(1)]);
   spEnumerate(&sps[SPS(2)]);
   displayWide(eCube, &sps[SPS(0)]);
   displayWide(eCube, &sps[SPS(1)]);
   displayWide(eCube, &sps[SPS(2)]);
   displayWide(eCube, 0);
   displayWide(ePyramid, &sps[SPS(0)]);
   displayWide(ePyramid, &sps[SPS(1)]);
   displayWide(ePyramid, &sps[SPS(2)]);
   displayWide(ePyramid, 0);
   free(sps);
}

///////////////////////////////////////////////////////////////////////////////
void spFind(TPosition* pos, TPlace c, const TPlace* sp) {
   // find the potentially symmetrical central spots first
   int z;
   for (z = 0; z < spHeight; z += 2) {
      int z2 = z / 2;
      if (sp[SP_XYZ(z2, z2, z)] == c) {
         pos[eX] = z2;
         pos[eY] = z2;
         pos[eZ] = z;
         return;
      }
   }
   int r;
   for (r = 0; r < (spHeight + 1) / 2; ++r) {
      for (z = r + 1; z < spHeight; ++z) {
         int o = (z - 1) / 2 - r;
         int l = z / 2 + r + 1;
         if (o < 0) {
            continue;
         }
         int zoffset = ZOFFSET(z);
         int x;
         int y;
         int yoffset;
         y = o;
         yoffset = YOFFSET(y);
         for (x = o; x < l; ++x) {
            if (sp[zoffset + yoffset + MARGIN + x] == c) {
               pos[eX] = x;
               pos[eY] = y;
               pos[eZ] = z;
               return;
            }
         }
         y = l;
         yoffset = YOFFSET(y);
         for (x = l; x > o; --x) {
            if (sp[zoffset + yoffset + MARGIN + x] == c) {
               pos[eX] = x;
               pos[eY] = y;
               pos[eZ] = z;
               return;
            }
         }
         x = l;
         for (y = o; y < l; ++y) {
            if (sp[zoffset + YOFFSET(y) + MARGIN + x] == c) {
               pos[eX] = x;
               pos[eY] = y;
               pos[eZ] = z;
               return;
            }
         }
         x = o;
         for (y = l; y > o; --y) {
            if (sp[zoffset + YOFFSET(y) + MARGIN + x] == c) {
               pos[eX] = x;
               pos[eY] = y;
               pos[eZ] = z;
               return;
            }
         }
      }
   }
   pos[eX] = -1;
   pos[eY] = -1;
   pos[eZ] = -1;
   return;
}

///////////////////////////////////////////////////////////////////////////////
void spTestOrder() {
   int volume = getPyramidVolume();
   TPlace* sp = SP_NEW(1);
   spInit(sp);
   char buf[POS_BUF_SIZE];
   TPosition pos[eDimensions];
   int i;
   for (i = 0; i < volume; ++i) {
      spFind(pos, eAbsent, sp);
      printf("%d %s %d %c%s", i, posToString(buf, pos), SP_POS(pos), GLYPH(i % 26 + 1), EOL);
      SP_SET(sp, i % 26 + 1, pos);
      displayWide(ePyramid, sp);
   }
   displayWide(ePyramid, 0);
   free(sp);
}

///////////////////////////////////////////////////////////////////////////////
ERotation spEqualRotate(const TPlace* sp1, const TPlace* sp2) {
   int origin = SP_XYZ(MARGIN, MARGIN, MARGIN);
   if (sp1[origin] != sp2[origin]) {
      return e0;
   }
   if (sp1[origin] == eAbsent && sp2[origin] == eAbsent) {
      return e90;
   }
   int z;
   for (z = 1; z < spHeight; ++z) {
      int zoffset = ZOFFSET(z);
      int y;
      for (y = 0; y <= z; ++y) {
         int yoffset = YOFFSET(y);
         int zyoffset = YOFFSET(z - y);
         int x;
         for (x = 0; x <= z; ++x) {
            if (sp1[zoffset + yoffset + MARGIN + x]
             != sp2[zoffset + zyoffset + MARGIN + (z - x)]) {
//printf("180(%d, %d, %d) %c != (%d, %d) %c%s", 
//   x, y, z,           GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   (z - x), (z - y), GLYPH(sp2[MARGIN + z][MARGIN + (z - y)][MARGIN + (z - x)]), EOL);
               return e0;
            }
         }
      }
   }
   for (z = 1; z < spHeight; ++z) {
      int zoffset = ZOFFSET(z);
      int y;
      for (y = 0; y <= z; ++y) {
         int yoffset = YOFFSET(y);
         int zyoffset = YOFFSET(z - y);
         int x;
         for (x = 0; x <= z; ++x) {
            if (sp1[zoffset + yoffset + MARGIN + x]
             != sp2[zoffset + YOFFSET(z - x) + MARGIN + y]
             || sp1[zoffset + yoffset + MARGIN + x]
             != sp2[zoffset + YOFFSET(x) + MARGIN + (z - y)]) {
//printf("90(%d, %d, %d) %c ?= (%d, %d) %c (%d, %d) %c%s", 
//   x, y, z,    GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   y, (z - x), GLYPH(sp2[MARGIN + z][MARGIN + (z - x)][MARGIN + y]),
//   (z - y), x, GLYPH(sp2[MARGIN + z][MARGIN + x][MARGIN + (z - y)]), EOL);
               return e180;
            }
         }
      }
   }
   return e90;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfReflectionPlanes spEqualReflect(const TPlace* sp1, const TPlace* sp2) {
   int origin = SP_XYZ(MARGIN, MARGIN, MARGIN);
    if (sp1[origin] != sp2[origin]) {
      return 0;
   }
   TSetOfReflectionPlanes sorp = SET_ALL(eReflectionPlanes);
   if (sp1[origin] == eAbsent && sp2[origin] == eAbsent) {
      return sorp;
   }
   int z;
   for (z = 1; z < spHeight; ++z) {
      int zoffset = ZOFFSET(z);
      int y;
      for (y = 0; y <= z; ++y) {
         int yoffset = YOFFSET(y);
         int zyoffset = YOFFSET(z - y);
         int x;
         for (x = 0; x <= z; ++x) {
            if (SET_HAS(sorp, e100Reflection)
             && (sp1[zoffset + yoffset + MARGIN + x]
              != sp2[zoffset + yoffset + MARGIN + z - x])) {
//printf("100(%d, %d, %d) %c != (%d, %d) %c%s", 
//   x, y, z,  GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   z - x, y, GLYPH(sp2[MARGIN + z][MARGIN + y][MARGIN + z - x]), EOL);
               sorp = SET_WITHOUT(sorp, e100Reflection);
               if (!sorp) {
                  return 0;
               }
            }
            if (SET_HAS(sorp, e010Reflection)
             && (sp1[zoffset + yoffset + MARGIN + x]
              != sp2[zoffset + zyoffset + MARGIN + x])) {
//printf("010(%d, %d, %d) %c != (%d, %d) %c%s", 
//   x, y, z,  GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   x, z - y, GLYPH(sp2[MARGIN + z][MARGIN + z - y][MARGIN + x]), EOL);
               sorp = SET_WITHOUT(sorp, e010Reflection);
               if (!sorp) {
                  return 0;
               }
            }
            if (SET_HAS(sorp, e110Reflection)
             && (sp1[zoffset + yoffset + MARGIN + x]
              != sp2[zoffset + YOFFSET(x) + MARGIN + y])) {
//printf("110(%d, %d, %d) %c != (%d, %d) %c%s", 
//   x, y, z, GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   y, x,    GLYPH(sp2[MARGIN + z][MARGIN + x][MARGIN + y]), EOL);
               sorp = SET_WITHOUT(sorp, e110Reflection);
               if (!sorp) {
                  return 0;
               }
            }
            if (SET_HAS(sorp, e1T0Reflection)
             && (sp1[zoffset + yoffset + MARGIN + x]
              != sp2[zoffset + YOFFSET(z - x) + MARGIN + (z - y)])) {
//printf("1T0(%d, %d, %d) %c != (%d, %d) %c%s", 
//   x, y, z,          GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   (z - y), (z - x), GLYPH(sp2[MARGIN + z][MARGIN + (z - x)][MARGIN + (z - y)]), EOL);
               sorp = SET_WITHOUT(sorp, e1T0Reflection);
               if (!sorp) {
                  return 0;
               }
            }
         }
      }
   }
   return sorp;
}
