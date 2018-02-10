/**
 * Copyright 2018 Pushkar Piggott
 *
 * SquarePyramid.c
 */

#include "SquarePyramid.h"
#include "Presence.h"   // glyph

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LO_LIMIT(z) ((z + 1) / 2)
#define HI_LIMIT(z) ((z + 2) / 2)

///////////////////////////////////////////////////////////////////////////////
const char* symToString(ESymmetry eSym) {
   switch (eSym) {
      case eAsymmetric: return "asymmetric";
      case e180: return "180";
      case e90: return "90";
      default: return "Unknown symmetry";
   }
}
   
///////////////////////////////////////////////////////////////////////////////
void spClear(TSquarePyramid sp) {
   memset(sp, 0, SIZE * SIZE * SIZE);
}

///////////////////////////////////////////////////////////////////////////////
void spInit(TSquarePyramid sp) {
   memset(sp, -1, SIZE * SIZE * SIZE);
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
   return memcpy(dst, src, SIZE * SIZE * SIZE);
}

///////////////////////////////////////////////////////////////////////////////
int spEqual(TSquarePyramid sp1, TSquarePyramid sp2) {
   return 0 == memcmp(sp1, sp2, SIZE * SIZE * SIZE);
}

///////////////////////////////////////////////////////////////////////////////
ESymmetry spSymmetry(TSquarePyramid sp) {
   ESymmetry eSymmetry = e90;
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
               return eAsymmetric;
            }
            if (c00 != c10 || c01 != c11) {
               eSymmetry = e180;
            }
            if (c00 != eAbsent || c01 != eAbsent 
             || c10 != eAbsent || c11 != eAbsent) {
               eLevelPresence = eMargin;
            }
//printf("p00 %s: %c p01 %s: %c p11 %s: %c p10 %s: %c %s\r\n", posToString(buf00, &p00), glyph[spGet(&p00, sp)], posToString(buf01, &p01), glyph[spGet(&p01, sp)], posToString(buf11, &p11), glyph[spGet(&p11, sp)], posToString(buf10, &p10), glyph[spGet(&p10, sp)], symToString(eSymmetry));
         }
      }
      if (eLevelPresence == eAbsent) {
         if (size & 1) {
            return eSymmetry;
         }
         TPosition pm = {ylimit, ylimit, size};
//char bufm[16];
//printf("Middle size %d ylimit %d pm %s: %c\r\n", size, ylimit, posToString(bufm, &pm), glyph[spGet(&pm, sp)]);
         if (spGet(&pm, sp) == eAbsent) {
            return eSymmetry;
         }
      }
   }
   return eSymmetry;
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
   for (x = 0; x < SIZE; ++x) {
      *str++ = glyph[sp[z][y][x]];
   }
   return str;
}

///////////////////////////////////////////////////////////////////////////////
void sqFind(TPosition* pos, char c, ESymmetry sym, TSquarePyramid sp) {
   int z;
   for (z = 0; z < HEIGHT; ++z) {
      int xlimit = z + 1;
      int ylimit = z + 1;
      if (z > 0 && sym) {
         // symmetrical, scan a quarter
         xlimit = HI_LIMIT(z);
         ylimit = LO_LIMIT(z);
printf("(%d, %d, %d) %c %s\r\n", xlimit, ylimit, z, glyph[sp[MARGIN + z][MARGIN + xlimit - 1][MARGIN + xlimit - 1]], symToString(sym));
         // odd size, check the centre first
         if (xlimit != ylimit
          && sp[MARGIN + z][MARGIN + xlimit - 1][MARGIN + xlimit - 1] != c) {
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
printf("(%d, %d, %d) %c %s\r\n", x, y, z, glyph[sp[MARGIN + z][MARGIN + y][MARGIN + x]], symToString(sym));
            if (sp[MARGIN + z][MARGIN + y][MARGIN + x] != c) {
               pos->d[eX] = x;
               pos->d[eY] = y;
               pos->d[eZ] = z;
               return;
            }
         }
      }
      if (z > 0 && sym == e180) {
printf("sym == e180\r\n");
         // 180 symmetrical, scan another quarter
         int y;
         for (y = ylimit; y <= z; ++y) {
             int x;
            for (x = 0; x < ylimit; ++x) {
printf("(%d, %d, %d) %c %s\r\n", x, y, z, glyph[sp[MARGIN + z][MARGIN + y][MARGIN + x]], symToString(sym));
               if (sp[MARGIN + z][MARGIN + y][MARGIN + x] != c) {
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
char spGet(TPosition* pat, TSquarePyramid sp) {
//printf("spGet x %d y %d z %d: %s\r\n", pat->d[eX], pat->d[eY], pat->d[eZ], presenceToString(sp[MARGIN + pat->d[eZ]][MARGIN + pat->d[eY]][MARGIN + pat->d[eX]]));
   return sp[MARGIN + pat->d[eZ]][MARGIN + pat->d[eY]][MARGIN + pat->d[eX]];
}

///////////////////////////////////////////////////////////////////////////////
int spSet(TSquarePyramid sp, char c, TPosition* pat) {
//   if (spGet(pat, sp)) {
//      return 0;
//   }
   sp[MARGIN + pat->d[eZ]][MARGIN + pat->d[eY]][MARGIN + pat->d[eX]] = c;
   return 1;
}
