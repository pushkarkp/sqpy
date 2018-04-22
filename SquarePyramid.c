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

#define ZOFFSET(z) (spXY * (MARGIN + z))
#define YOFFSET(y) (spX * (MARGIN + y))

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
void spClear(TPlace* sp) {
   memset(sp, 0, spXYZ);
}

///////////////////////////////////////////////////////////////////////////////
void spInit(TPlace* sp) {
   memset(sp, -1, spXYZ);
   int z;
   for (z = 0; z < spHeight; ++z) {
      int zoffset = ZOFFSET(z);
      int y;
      for (y = 0; y <= z; ++y) {
         int yoffset = YOFFSET(y);
         int x;
         for (x = 0; x <= z; ++x) {
            sp[zoffset + yoffset + MARGIN + x] = 0;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void spEnumerate(TPlace* sp) {
   int c = -25;
   int z;
   for (z = 0; z < spHeight; ++z) {
      int zoffset = ZOFFSET(z);
      int y;
      for (y = 0; y <= z; ++y) {
         int yoffset = YOFFSET(y);
         int x;
         for (x = 0; x <= z; ++x) {
            sp[zoffset + yoffset + MARGIN + x] = c++;
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
//printf("z %d zoffset %d y %d yoffset %d%s", z, zoffset, y, yoffset, EOL);
   int x;
   for (x = 0; x <= z; ++x) {
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
