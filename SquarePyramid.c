/**
 * Copyright 2018 Pushkar Piggott
 *
 * SquarePyramid.c
 */

#include "SquarePyramid.h"
#include "Presence.h"   // glyph
#include "Display.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LO_LIMIT(z) ((z + 1) / 2)
#define HI_LIMIT(z) ((z + 2) / 2)

///////////////////////////////////////////////////////////////////////////////
const char* rotationToString(ERotation rot) {
   switch (rot) {
      case e0: return "0";
      case e180: return "180";
      case e90: return "90";
      default: return "Unknown rotation";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* reflectionToString(char* buf, int ref) {
   *buf = 0;
   const char* sep = "";
   int i;
   for (i = 1; i <= e1T0Reflection; i <<= 1) {
      if (ref & i) {
         strcat(buf, sep);
         sep = " ";
         switch (i) {
            case e100Reflection: strcat(buf, "100"); break;
            case e010Reflection: strcat(buf, "010"); break;
            case e110Reflection: strcat(buf, "110"); break;
            case e1T0Reflection: strcat(buf, "1T0"); break;
         }
      }
   }
   return buf;
}

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
   int z;
   for (z = MARGIN; z < MARGIN + HEIGHT; ++z) {
      int y;
      for (y = MARGIN; y <= z; ++y) {
         int x;
         for (x = MARGIN; x <= z; ++x) {
            sp[z][y][x] = x;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
PSquarePyramid spCopy(TSquarePyramid dst, TSquarePyramid src) {
   return memcpy(dst, src, SP_SIZE * SP_SIZE * SP_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
int spEqual(TSquarePyramid sp1, TSquarePyramid sp2) {
   return 0 == memcmp(sp1, sp2, SP_SIZE * SP_SIZE * SP_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
char spGet(TPosition* pat, TSquarePyramid sp) {
//printf("spGet x %d y %d z %d: %s\r\n", pat->d[eX], pat->d[eY], pat->d[eZ], presenceToString(sp[MARGIN + pat->d[eZ]][MARGIN + pat->d[eY]][MARGIN + pat->d[eX]]));
   return sp[MARGIN + pat->d[eZ]][MARGIN + pat->d[eY]][MARGIN + pat->d[eX]];
}

///////////////////////////////////////////////////////////////////////////////
void spSet(TSquarePyramid sp, char c, TPosition* pat) {
   sp[MARGIN + pat->d[eZ]][MARGIN + pat->d[eY]][MARGIN + pat->d[eX]] = c;
}

///////////////////////////////////////////////////////////////////////////////
int spFindRepeat(int n, TSquarePyramid* sps) {
   int eq;
   for (eq = n - 1; eq >= 0; --eq) {
      if (spEqual(sps[n], sps[eq])) {
         return n - 1;
      }
   }
   return n;
}

///////////////////////////////////////////////////////////////////////////////
ERotation spSymmetry(TSquarePyramid sp) {
   ERotation rotmetry = e90;
   TPosition p00 = {0, 0, 0};
   // start on the 2x2 level (z==1)
   for (p00.d[eZ] = 1; p00.d[eZ] < HEIGHT; ++p00.d[eZ]) {
      EPresence eLevelPresence = eAbsent;
      TPosition p01 = {0, 0, p00.d[eZ]};
      TPosition p11 = {0, 0, p00.d[eZ]};
      TPosition p10 = {0, 0, p00.d[eZ]};
      int size = p00.d[eZ];
      int ylimit = LO_LIMIT(p00.d[eZ]);
      int xlimit = HI_LIMIT(p00.d[eZ]);
//printf("Z size %d ylimit %d \r\n", size, ylimit);
      for (p00.d[eY] = 0; p00.d[eY] < ylimit; ++p00.d[eY]) {
         p01.d[eX] = p00.d[eY];
         p11.d[eY] = size - p00.d[eY];
         p10.d[eX] = size - p00.d[eY];
         for (p00.d[eX] = 0; p00.d[eX] < xlimit; ++p00.d[eX]) {
            p01.d[eY] = size - p00.d[eX];
            p11.d[eX] = size - p00.d[eX];
            p10.d[eY] = p00.d[eX];
            int c00 = spGet(&p00, sp);
            int c01 = spGet(&p01, sp);
            int c11 = spGet(&p11, sp);
            int c10 = spGet(&p10, sp);
//char buf00[16];
//char buf01[16];
//char buf11[16];
//char buf10[16];
            if (c00 != c11 || c01 != c10) {
//printf("p00 %s: %c p01 %s: %c p11 %s: %c p10 %s: %c Asymmetric\r\n", posToString(buf00, &p00), glyph[spGet(&p00, sp)], posToString(buf01, &p01), glyph[spGet(&p01, sp)], posToString(buf11, &p11), glyph[spGet(&p11, sp)], posToString(buf10, &p10), glyph[spGet(&p10, sp)]);
               return e0;
            }
            if (c00 != c10 || c01 != c11) {
               rotmetry = e180;
            }
            if (c00 != eAbsent || c01 != eAbsent 
             || c10 != eAbsent || c11 != eAbsent) {
               eLevelPresence = eMargin;
            }
//printf("p00 %s: %c p01 %s: %c p11 %s: %c p10 %s: %c %s\r\n", posToString(buf00, &p00), glyph[spGet(&p00, sp)], posToString(buf01, &p01), glyph[spGet(&p01, sp)], posToString(buf11, &p11), glyph[spGet(&p11, sp)], posToString(buf10, &p10), glyph[spGet(&p10, sp)], rotationToString(rotmetry));
         }
      }
      if (eLevelPresence == eAbsent) {
         if (size & 1) {
            return rotmetry;
         }
         TPosition pm = {ylimit, ylimit, size};
//char bufm[16];
//printf("Middle size %d ylimit %d pm %s: %c\r\n", size, ylimit, posToString(bufm, &pm), glyph[spGet(&pm, sp)]);
         if (spGet(&pm, sp) == eAbsent) {
            return rotmetry;
         }
      }
   }
   return rotmetry;
}

///////////////////////////////////////////////////////////////////////////////
char* spRowToString(char* str, int y, int z, TSquarePyramid sp, const char* glyph) {
   y += MARGIN;
   z += MARGIN;
   int x;
   for (x = MARGIN; x <= z; ++x) {
      *str++ = glyph[sp[z][y][x]];
   }
   return str;
}

///////////////////////////////////////////////////////////////////////////////
char* spWholeRowToString(char* str, int y, int z, TSquarePyramid sp, const char* glyph) {
   int x;
   for (x = 0; x < SP_SIZE; ++x) {
      *str++ = glyph[sp[z][y][x]];
   }
   return str;
}

///////////////////////////////////////////////////////////////////////////////
void spFind1(TPosition* pos, char c, ERotation sym, TSquarePyramid sp) {
   if (sp[MARGIN][MARGIN][MARGIN] == c) {
      pos->d[eX] = 0;
      pos->d[eY] = 0;
      pos->d[eZ] = 0;
      return;
   }
   int z;
   for (z = 1; z < HEIGHT; ++z) {
      int xlimit = z + 1;
      int ylimit = z + 1;
      if (sym) {
         // symmetrical, scan a quarter
         xlimit = HI_LIMIT(z);
         ylimit = LO_LIMIT(z);
//printf("xlimit %d ylimit %d z %d %c %s\r\n", xlimit, ylimit, z, glyph[sp[MARGIN + z][MARGIN + xlimit - 1][MARGIN + xlimit - 1]], rotationToString(sym));
         // odd size, check the centre first
         if (xlimit != ylimit
          && sp[MARGIN + z][MARGIN + xlimit - 1][MARGIN + xlimit - 1] == c) {
            pos->d[eX] = xlimit - 1;
            pos->d[eY] = xlimit - 1;
            pos->d[eZ] = z;
            return;
         }
      }
      int y;
      for (y = 0; y < ylimit; ++y) {
          int x;
         for (x = 0; x < xlimit; ++x) {
//printf("(%d, %d, %d) %c %s\r\n", x, y, z, glyph[sp[MARGIN + z][MARGIN + y][MARGIN + x]], rotationToString(sym));
            if (sp[MARGIN + z][MARGIN + y][MARGIN + x] == c) {
               pos->d[eX] = x;
               pos->d[eY] = y;
               pos->d[eZ] = z;
               return;
            }
         }
      }
      if (z > 0 && sym == e180) {
         // 180 symmetrical, scan another quarter
         int y;
         for (y = ylimit; y <= z; ++y) {
            int x;
            for (x = 0; x < ylimit; ++x) {
//printf("(%d, %d, %d) %c %s\r\n", x, y, z, glyph[sp[MARGIN + z][MARGIN + y][MARGIN + x]], rotationToString(sym));
               if (sp[MARGIN + z][MARGIN + y][MARGIN + x] == c) {
                  pos->d[eX] = x;
                  pos->d[eY] = y;
                  pos->d[eZ] = z;
                  return;
               }
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
void spFind(TPosition* pos, char c, TSquarePyramid sp) {
   if (sp[MARGIN][MARGIN][MARGIN] == c) {
      pos->d[eX] = 0;
      pos->d[eY] = 0;
      pos->d[eZ] = 0;
      return;
   }
   int z;
   for (z = 1; z < HEIGHT; ++z) {
      int y;
      for (y = 0; y <= z; ++y) {
         int x;
         for (x = 0; x <= z; ++x) {
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
ERotation spEqualRotation(TSquarePyramid sp1, TSquarePyramid sp2) {
   if (sp1[MARGIN][MARGIN][MARGIN]
    != sp2[MARGIN][MARGIN][MARGIN]) {
      return e0;
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
//   x, y, z,           glyph[sp1[MARGIN + z][MARGIN + y][MARGIN + x]], 
//   (z - x), (z - y), glyph[sp2[MARGIN + z][MARGIN + (z - y)][MARGIN + (z - x)]]);
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
//printf("90(%d, %d, %d) %c != (%d, %d) %c (%d, %d) %c\r\n", 
//   x, y, z,    glyph[sp1[MARGIN + z][MARGIN + y][MARGIN + x]], 
//   y, (z - x), glyph[sp2[MARGIN + z][MARGIN + (z - x)][MARGIN + y]],
//   (z - y), x, glyph[sp2[MARGIN + z][MARGIN + x][MARGIN + (z - y)]]);
               return e180;
            }
         }
      }
   }
   return e90;
}

///////////////////////////////////////////////////////////////////////////////
int spEqualReflection(TSquarePyramid sp1, TSquarePyramid sp2) {
   int ref = e100Reflection | e010Reflection | e110Reflection | e1T0Reflection;
   if (sp1[MARGIN][MARGIN][MARGIN]
    != sp2[MARGIN][MARGIN][MARGIN]) {
      return eNoReflection;
   }
   int z;
   for (z = 1; z < HEIGHT; ++z) {
      int y;
      for (y = 0; y <= z; ++y) {
         int x;
         for (x = 0; x <= z; ++x) {
            if ((ref & e100Reflection)
             && (sp1[MARGIN + z][MARGIN + y][MARGIN + x]
              != sp2[MARGIN + z][MARGIN + y][MARGIN + z - x])) {
//printf("110(%d, %d, %d) %c != (%d, %d) %c\r\n", 
//   x, y, z,  glyph[sp1[MARGIN + z][MARGIN + y][MARGIN + x]], 
//   z - x, y, glyph[sp2[MARGIN + z][MARGIN + y][MARGIN + z - x]]);
               ref &= ~e100Reflection;
               if (ref == eNoReflection) {
                  return eNoReflection;
               }
            }
            if ((ref & e010Reflection)
             && (sp1[MARGIN + z][MARGIN + y][MARGIN + x]
              != sp2[MARGIN + z][MARGIN + z - y][MARGIN + x])) {
//printf("110(%d, %d, %d) %c != (%d, %d) %c\r\n", 
//   x, y, z,  glyph[sp1[MARGIN + z][MARGIN + y][MARGIN + x]], 
//   x, z - y, glyph[sp2[MARGIN + z][MARGIN + z - y][MARGIN + x]]);
               ref &= ~e010Reflection;
               if (ref == eNoReflection) {
                  return eNoReflection;
               }
            }
            if ((ref & e110Reflection)
             && (sp1[MARGIN + z][MARGIN + y][MARGIN + x]
              != sp2[MARGIN + z][MARGIN + x][MARGIN + y])) {
//printf("110(%d, %d, %d) %c != (%d, %d) %c\r\n", 
//   x, y, z, glyph[sp1[MARGIN + z][MARGIN + y][MARGIN + x]], 
//   y, x,    glyph[sp2[MARGIN + z][MARGIN + x][MARGIN + y]]);
               ref &= ~e110Reflection;
               if (ref == eNoReflection) {
                  return eNoReflection;
               }
            }
            if ((ref & e1T0Reflection)
             && (sp1[MARGIN + z][MARGIN + y][MARGIN + x]
              != sp2[MARGIN + z][MARGIN + (z - x)][MARGIN + (z - y)])) {
//printf("1T0(%d, %d, %d) %c != (%d, %d) %c\r\n", 
//   x, y, z,          glyph[sp1[MARGIN + z][MARGIN + y][MARGIN + x]], 
//   (z - y), (z - x), glyph[sp2[MARGIN + z][MARGIN + (z - x)][MARGIN + (z - y)]]);
               ref &= ~e1T0Reflection;
               if (ref == eNoReflection) {
                  return eNoReflection;
               }
            }
         }
      }
   }
   return ref;
}
