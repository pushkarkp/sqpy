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

#define LO_LIMIT(z) ((z + 1) / 2)
#define HI_LIMIT(z) ((z + 2) / 2)

///////////////////////////////////////////////////////////////////////////////
void spClear(TSquarePyramid sp) {
   memset(sp, 0, SP_SIZE * SP_SIZE * SP_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
void spInit(TSquarePyramid sp) {
   memset(sp, -1, SP_SIZE * SP_SIZE * SP_SIZE);
   int z;
   for (z = MARGIN; z < MARGIN + HEIGHT; ++z) {
      int y;
      for (y = MARGIN; y <= z; ++y) {
         int x;
         for (x = MARGIN; x <= z; ++x) {
            sp[z][y][x] = 0;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void spEnumerate(TSquarePyramid sp) {
   int c = -25;
   int z;
   for (z = MARGIN; z < MARGIN + HEIGHT; ++z) {
      int y;
      for (y = MARGIN; y <= z; ++y) {
         int x;
         for (x = MARGIN; x <= z; ++x) {
            sp[z][y][x] = c++;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
PSquarePyramid spCopy(TSquarePyramid dst, const TSquarePyramid src) {
   return memcpy(dst, src, SP_SIZE * SP_SIZE * SP_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
int spEqual(const TSquarePyramid sp1, const TSquarePyramid sp2) {
   return 0 == memcmp(sp1, sp2, SP_SIZE * SP_SIZE * SP_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
char* spRowToString(char* str, int y, int z, const TSquarePyramid sp) {
   y += MARGIN;
   z += MARGIN;
   int x;
   for (x = MARGIN; x <= z; ++x) {
      *str++ = GLYPH(sp[z][y][x]);
   }
   return str;
}

///////////////////////////////////////////////////////////////////////////////
char* spWholeRowToString(char* str, int y, int z, const TSquarePyramid sp) {
   int x;
   for (x = 0; x < SP_SIZE; ++x) {
      *str++ = GLYPH(sp[z][y][x]);
   }
   return str;
}

///////////////////////////////////////////////////////////////////////////////
void spFind(TPosition* pos, char c, const TSquarePyramid sp) {
   // find the potentially symmetrical central spots first
   int z;
   for (z = 0; z < HEIGHT; z += 2) {
      int z2 = z / 2;
      if (sp[MARGIN + z][MARGIN + z2][MARGIN + z2] == c) {
         pos->d[eX] = z2;
         pos->d[eY] = z2;
         pos->d[eZ] = z;
         return;
      }
   }
   int r;
   for (r = 0; r < (HEIGHT + 1) / 2; ++r) {
      for (z = r + 1; z < HEIGHT; ++z) {
         int o = (z - 1) / 2 - r;
         int l = z / 2 + r + 1;
         if (o < 0) {
            continue;
         }
         int x;
         int y;
         y = o;
         for (x = o; x < l; ++x) {
            if (sp[MARGIN + z][MARGIN + y][MARGIN + x] == c) {
               pos->d[eX] = x;
               pos->d[eY] = y;
               pos->d[eZ] = z;
               return;
            }
         }
         y = l;
         for (x = l; x > o; --x) {
            if (sp[MARGIN + z][MARGIN + y][MARGIN + x] == c) {
               pos->d[eX] = x;
               pos->d[eY] = y;
               pos->d[eZ] = z;
               return;
            }
         }
         x = l;
         for (y = o; y < l; ++y) {
            if (sp[MARGIN + z][MARGIN + y][MARGIN + x] == c) {
               pos->d[eX] = x;
               pos->d[eY] = y;
               pos->d[eZ] = z;
               return;
            }
         }
         x = o;
         for (y = l; y > o; --y) {
            if (sp[MARGIN + z][MARGIN + y][MARGIN + x] == c) {
               pos->d[eX] = x;
               pos->d[eY] = y;
               pos->d[eZ] = z;
               return;
            }
         }
      }
   }
   pos->d[eX] = -1;
   pos->d[eY] = -1;
   pos->d[eZ] = -1;
   return;
}

///////////////////////////////////////////////////////////////////////////////
ERotation spEqualRotate(const TSquarePyramid sp1, const TSquarePyramid sp2) {
   if (sp1[MARGIN][MARGIN][MARGIN]
    != sp2[MARGIN][MARGIN][MARGIN]) {
      return e0;
   }
   if (sp1[MARGIN][MARGIN][MARGIN] == eAbsent
    && sp2[MARGIN][MARGIN][MARGIN] == eAbsent) {
//printf("spEqualRotate() empty\r\n"); 
      return e90;
   }
   int z;
   for (z = 1; z < HEIGHT; ++z) {
      int y;
      for (y = 0; y <= z; ++y) {
         int x;
         for (x = 0; x <= z; ++x) {
            if (sp1[MARGIN + z][MARGIN + y][MARGIN + x]
             != sp2[MARGIN + z][MARGIN + (z - y)][MARGIN + (z - x)]) {
//printf("180(%d, %d, %d) %c != (%d, %d) %c\r\n", 
//   x, y, z,           GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   (z - x), (z - y), GLYPH(sp2[MARGIN + z][MARGIN + (z - y)][MARGIN + (z - x)]));
               return e0;
            }
         }
      }
   }
   for (z = 1; z < HEIGHT; ++z) {
      int y;
      for (y = 0; y <= z; ++y) {
         int x;
         for (x = 0; x <= z; ++x) {
            if (sp1[MARGIN + z][MARGIN + y][MARGIN + x]
             != sp2[MARGIN + z][MARGIN + (z - x)][MARGIN + y]
             || sp1[MARGIN + z][MARGIN + y][MARGIN + x]
             != sp2[MARGIN + z][MARGIN + x][MARGIN + (z - y)]) {
//printf("90(%d, %d, %d) %c ?= (%d, %d) %c (%d, %d) %c\r\n", 
//   x, y, z,    GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   y, (z - x), GLYPH(sp2[MARGIN + z][MARGIN + (z - x)][MARGIN + y]),
//   (z - y), x, GLYPH(sp2[MARGIN + z][MARGIN + x][MARGIN + (z - y)]));
               return e180;
            }
         }
      }
   }
   return e90;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfReflectionPlanes spEqualReflect(const TSquarePyramid sp1, const TSquarePyramid sp2) {
   if (sp1[MARGIN][MARGIN][MARGIN]
    != sp2[MARGIN][MARGIN][MARGIN]) {
      return 0;
   }
   TSetOfReflectionPlanes sorp = SET_ALL(eReflectionPlanes);
   if (sp1[MARGIN][MARGIN][MARGIN] == eAbsent) {
      return sorp;
   }
   int z;
   for (z = 1; z < HEIGHT; ++z) {
      int y;
      for (y = 0; y <= z; ++y) {
         int x;
         for (x = 0; x <= z; ++x) {
            if (SET_HAS(sorp, e100Reflection)
             && (sp1[MARGIN + z][MARGIN + y][MARGIN + x]
              != sp2[MARGIN + z][MARGIN + y][MARGIN + z - x])) {
//printf("100(%d, %d, %d) %c != (%d, %d) %c\r\n", 
//   x, y, z,  GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   z - x, y, GLYPH(sp2[MARGIN + z][MARGIN + y][MARGIN + z - x]));
               sorp = SET_WITHOUT(sorp, e100Reflection);
               if (!sorp) {
                  return 0;
               }
            }
            if (SET_HAS(sorp, e010Reflection)
             && (sp1[MARGIN + z][MARGIN + y][MARGIN + x]
              != sp2[MARGIN + z][MARGIN + z - y][MARGIN + x])) {
//printf("010(%d, %d, %d) %c != (%d, %d) %c\r\n", 
//   x, y, z,  GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   x, z - y, GLYPH(sp2[MARGIN + z][MARGIN + z - y][MARGIN + x]));
               sorp = SET_WITHOUT(sorp, e010Reflection);
               if (!sorp) {
                  return 0;
               }
            }
            if (SET_HAS(sorp, e110Reflection)
             && (sp1[MARGIN + z][MARGIN + y][MARGIN + x]
              != sp2[MARGIN + z][MARGIN + x][MARGIN + y])) {
//printf("110(%d, %d, %d) %c != (%d, %d) %c\r\n", 
//   x, y, z, GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   y, x,    GLYPH(sp2[MARGIN + z][MARGIN + x][MARGIN + y]));
               sorp = SET_WITHOUT(sorp, e110Reflection);
               if (!sorp) {
                  return 0;
               }
            }
            if (SET_HAS(sorp, e1T0Reflection)
             && (sp1[MARGIN + z][MARGIN + y][MARGIN + x]
              != sp2[MARGIN + z][MARGIN + (z - x)][MARGIN + (z - y)])) {
//printf("1T0(%d, %d, %d) %c != (%d, %d) %c\r\n", 
//   x, y, z,          GLYPH(sp1[MARGIN + z][MARGIN + y][MARGIN + x]),
//   (z - y), (z - x), GLYPH(sp2[MARGIN + z][MARGIN + (z - x)][MARGIN + (z - y)]));
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
