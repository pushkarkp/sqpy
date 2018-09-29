/**
 * Copyright 2018 Pushkar Piggott
 *
 * SquarePyramid.c
 */

#include "SquarePyramid.h"
#include "Presence.h"
#include "Piece.h"
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
int spXYZ2 = 0;


///////////////////////////////////////////////////////////////////////////////
void spSetHeight(int height) {
   spHeight = height;
   spX = spHeight + MARGIN * 2;
   spXY = spX * spX;
   spXYZ = spX * spXY;
   spXYZ2 = spXYZ * 2;
//printf("spHeight %d spXYZ %d\r\n", spHeight, spXYZ);
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
   memset(sp, eAbsent, spXYZ2);
}

///////////////////////////////////////////////////////////////////////////////
// Initialize all to eMargin with the pyramid hollowed out as eAbsent. 
void spInit(TPlace* sp) {
   memset(sp, eMargin, spXYZ);
   memset(sp + spXYZ, 0, spXYZ);
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
   memset(sp, eMargin, spXYZ2);
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
   memset(sp, eMargin, spXYZ2);
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
   return memcpy(dst, src, spXYZ2);
}

///////////////////////////////////////////////////////////////////////////////
// ignore instance counts
int spEqual(const TPlace* sp1, const TPlace* sp2) {
   return 0 == memcmp(sp1, sp2, spXYZ);
}

///////////////////////////////////////////////////////////////////////////////
char* spRowToString(char* str, int y, int z, const TPlace* sp) {
   int offset = ZOFFSET(z) + YOFFSET(y) + MARGIN;
   int x;
   for (x = 0; x <= z; ++x) {
      int c = sp[offset + x];
      int n = (sp + spXYZ)[offset + x];
      *str++ = GLYPH(c + n * pieceCount);
   }
   return str;
}

///////////////////////////////////////////////////////////////////////////////
char* spCubeRowToString(char* str, int y, int z, const TPlace* sp) {
   int zoffset = ZOFFSET(z - MARGIN);
   int yoffset = YOFFSET(y - (2 * MARGIN));
   int x;
   for (x = 0; x < spX - 2; ++x) {
      int i = sp[zoffset + yoffset + MARGIN + x];
      int n = (sp + spXYZ)[zoffset + yoffset + MARGIN + x];
      *str++ = GLYPH(i + n * pieceCount);
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
void spTestVolume() {
   TPlace* sps = SP_NEW(3);
   spClear(&sps[SPS(0)]);
   spInit(&sps[SPS(1)]);
   spEnumerate(&sps[SPS(2)]);
   EDisplayShape s;
   for (s = 0; s < eDisplayShapes; ++s) {
      displayWide(s, &sps[SPS(0)]);
      displayWide(s, &sps[SPS(1)]);
      displayWide(s, &sps[SPS(2)]);
      displayWide(s, 0);
   }
   free(sps);
}

///////////////////////////////////////////////////////////////////////////////
int test(TPosition* pos, int x, int y, int z, TPlace c, const TPlace* sp, TSetOfReflectionPlanes sorp) {
   if (sp[SP_XYZ(x, y, z)] != c) {
      return 0;
   }
   pos[eX] = x;
   pos[eY] = y;
   pos[eZ] = z;
   return sorp == 0 || (sorp & onReflectionPlanes(pos));
}

///////////////////////////////////////////////////////////////////////////////
int spFind(TPosition* pos, TPlace c, const TPlace* sp, TSetOfReflectionPlanes sorp) {
   int z;
   // find the potentially symmetrical rotational axis spots 
   for (z = 0; z < spHeight; z += 2) {
      int z2 = z / 2;
      if (test(pos, z2, z2, z, c, sp, 0)) {
         return 1;
      }
   }
   // then the reflection planes
   for (z = 1; z < spHeight; ++z) {
      int r;
      for (r = 0; r < spHeight / 2; ++r) {
         if (test(pos, r, r, z, c, sp, sorp)
          || test(pos, z - r, r, z, c, sp, sorp)
          || test(pos, r, z - r, z, c, sp, sorp)
          || test(pos, z - r, z - r, z, c, sp, sorp)) {
            return 1;
         }
         if (!(z & 1)) {
            if (test(pos, r, z / 2, z, c, sp, sorp)
             || test(pos, z / 2, r, z, c, sp, sorp)
             || test(pos, z - r, z / 2, z, c, sp, sorp)
             || test(pos, z / 2, z - r, z, c, sp, sorp)) {
               return 1;
            }
         }
      }
   }
   if (sorp != 0) {
      return 0;
   }
   // fill in the remainder
   for (z = 3; z < spHeight; ++z) {
      int y;
      for (y = 0; y <= z; ++y) {
         int x;
         for (x = 0; x <= z; ++x) {
            if (x == y
             || z - x == y
             || x == z - y
             || z - x == z - y) {
               continue;
            }
            if (!(z & 1)
             && (x == z / 2
              || y == z / 2)) {
               continue;
            }
            if (test(pos, x, y, z, c, sp, 0)) {
               return 1;
            }
         }
      }
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
void spTestSequence(TSetOfReflectionPlanes sorp) {
   int volume = getPyramidVolume();
   TPlace* sp = SP_NEW(1);
   spInit(sp);
   char buf[POS_BUF_SIZE];
   TPosition pos[eDimensions];
   int i;
   for (i = 0; ; ++i) {
      if (spFind(pos, eAbsent, sp, sorp)) {
         int c = i % 26 + 1;
         printf("%d %c %s %d%s", i, GLYPH(c), posToString(buf, pos), SP_POS(pos), EOL);
         SP_SET(sp, c, pos);
         displayWide(ePyramid, sp);
      } else if (sorp != 0) {
printf("sorp = 0\r\n");
         sorp = 0;
         --i;
      } else {
         break;
      }
   }
   displayWide(ePyramid, 0);
   free(sp);
}

///////////////////////////////////////////////////////////////////////////////
int equalZ(int z, const TPlace* sp1, const TPlace* sp2) {
   int zoffset = ZOFFSET(z);
   int y;
   for (y = 0; y <= z; ++y) {
      int yoffset = YOFFSET(y);
      int x;
      for (x = 0; x <= z; ++x) {
         int offset = zoffset + yoffset + MARGIN + x;
         if (sp1[offset] != sp2[offset]) {
            return 0;
         }
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfRotations spEqualRotate(const TPlace* sp1, const TPlace* sp2, int fixz) {
   int origin = SP_XYZ(0, 0, 0);
   if (sp1[origin] != sp2[origin]) {
      return 0;
   }
   TSet sall = SET_WITHOUT(SET_ALL_OF(eRotations), e0);
   TSet sorn = sall;
//   if (sp1[origin] == eAbsent && sp2[origin] == eAbsent) {
//      return sorn;
//   }
   int z;
   for (z = 1; z < spHeight; ++z) {
      if (!fixz) {
         if (equalZ(z, sp1, sp2)) {
            continue;
         }
//         if (sorn != sall) {
//printf("%s%s", setToString(sorn, rotationToString, "none"), EOL);
//         }
         sorn = sall;
      }
      int zoffset = ZOFFSET(z);
      int y;
      for (y = 0; y <= z; ++y) {
         int yoffset = YOFFSET(y);
         int zyoffset = YOFFSET(z - y);
         int x;
         for (x = 0; x <= z; ++x) {
            if (SET_HAS(sorn, e90)
             && sp1[zoffset + yoffset + MARGIN + x]
             != sp2[zoffset + YOFFSET(z - x) + MARGIN + y]) {
               /*
               printf("90: %d%d%d %c != %d%d%d %c%s",
                  x, y, z,       GLYPH(sp1[zoffset + yoffset + MARGIN + x]),
                  y, (z - x), z, GLYPH(sp2[zoffset + YOFFSET(z - x) + MARGIN + y]), EOL);
               */
               sorn = SET_WITHOUT(sorn, e90);
               if (!sorn) {
                  return 0;
               }
            }
            if (SET_HAS(sorn, e180)
             && sp1[zoffset + yoffset + MARGIN + x]
             != sp2[zoffset + zyoffset + MARGIN + (z - x)]) {
               /*
               printf("180: %d%d%d %c != %d%d%d %c%s",
                  x, y, z,             GLYPH(sp1[zoffset + yoffset + MARGIN + x]),
                  (z - x), (z - y), z, GLYPH(sp2[zoffset + zyoffset + MARGIN + (z - x)]), EOL);
               */
               sorn = SET_WITHOUT(sorn, e180);
               if (!sorn) {
                  return 0;
               }
            }
            if (SET_HAS(sorn, e270)
             && sp1[zoffset + yoffset + MARGIN + x]
             != sp2[zoffset + YOFFSET(x) + MARGIN + (z - y)]) {
               /*
               printf("270: %d%d%d %c != %d%d%d %c%s",
                  x, y, z,       GLYPH(sp1[zoffset + yoffset + MARGIN + x]),
                  (z - y), x, z, GLYPH(sp2[zoffset + YOFFSET(x) + MARGIN + (z - y)]), EOL);
               */
               sorn = SET_WITHOUT(sorn, e270);
               if (!sorn) {
                  return 0;
               }
            }
         }
      }
   }
   return sorn;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfReflectionPlanes spEqualReflect(const TPlace* sp1, const TPlace* sp2, int fixz) {
   int origin = SP_XYZ(0, 0, 0);
   if (sp1[origin] != sp2[origin]) {
      return 0;
   }
   TSetOfReflectionPlanes sall = SET_ALL_OF(eReflectionPlanes);
   TSetOfReflectionPlanes sorp = sall;
//   if (sp1[origin] == eAbsent && sp2[origin] == eAbsent) {
//      return sorp;
//   }
   int z;
   for (z = 1; z < spHeight; ++z) {
      if (!fixz) {
         if (equalZ(z, sp1, sp2)) {
            continue;
         }
//         if (sorp != sall) {
//printf("%s%s", setToString(sorp, reflectionPlaneToString, "none"), EOL);
//         }
         sorp = sall;
      }
      int zoffset = ZOFFSET(z);
      int y;
      for (y = 0; y <= z; ++y) {
         int yoffset = YOFFSET(y);
         int zyoffset = YOFFSET(z - y);
         int x;
         for (x = 0; x <= z; ++x) {
            if (SET_HAS(sorp, eXReflection)
             && (sp1[zoffset + yoffset + MARGIN + x]
              != sp2[zoffset + zyoffset + MARGIN + x])) {
               /*
               printf("x: %d%d%d %c != %d%d%d %c%s",
                  x, y, z,     GLYPH(sp1[zoffset + yoffset + MARGIN + x]),
                  x, z - y, z, GLYPH(sp2[zoffset + zyoffset + MARGIN + x]), EOL);
               */
               sorp = SET_WITHOUT(sorp, eXReflection);
               if (!sorp) {
                  return 0;
               }
            }
            if (SET_HAS(sorp, eYReflection)
             && (sp1[zoffset + yoffset + MARGIN + x]
              != sp2[zoffset + yoffset + MARGIN + z - x])) {
               /*
               printf("y: %d%d%d %c != %d%d%d %c%s",
                  x, y, z,     GLYPH(sp1[zoffset + yoffset + MARGIN + x]),
                  z - x, y, z, GLYPH(sp2[zoffset + yoffset + MARGIN + z - x]), EOL);
               */
               sorp = SET_WITHOUT(sorp, eYReflection);
               if (!sorp) {
                  return 0;
               }
            }
            if (SET_HAS(sorp, eBReflection)
             && (sp1[zoffset + yoffset + MARGIN + x]
              != sp2[zoffset + YOFFSET(x) + MARGIN + y])) {
               /*
               printf("b: %d%d%d %c != %d%d%d %c%s",
                  x, y, z, GLYPH(sp1[zoffset + yoffset + MARGIN + x]),
                  y, x, z, GLYPH(sp2[zoffset + YOFFSET(x) + MARGIN + y]), EOL);
               */
               sorp = SET_WITHOUT(sorp, eBReflection);
               if (!sorp) {
                  return 0;
               }
            }
            if (SET_HAS(sorp, eDReflection)
             && (sp1[zoffset + yoffset + MARGIN + x]
              != sp2[zoffset + YOFFSET(z - x) + MARGIN + (z - y)])) {
               /*
               printf("d: %d%d%d %c != %d%d%d %c%s",
                  x, y, z,             GLYPH(sp1[zoffset + yoffset + MARGIN + x]),
                  (z - y), (z - x), z, GLYPH(sp2[zoffset + YOFFSET(z - x) + MARGIN + (z - y)]), EOL);
               */
               sorp = SET_WITHOUT(sorp, eDReflection);
               if (!sorp) {
                  return 0;
               }
            }
         }
      }
   }
   return sorp;
}

///////////////////////////////////////////////////////////////////////////////
void spTestOrientations(TSetOfOrientations soor) {
   TPlace* sp = SP_NEW(1);
   EOrientation or;
   for (or = 0; or < eOrientations; ++or) {
      if (soor != 0 && !SET_HAS(soor, or)) {
         continue;
      }
      spInit(sp);
      int z;
      for (z = 0; z < spHeight; ++z) {
         sp[SP_XYZ(0, 0, z)] = 'x';
         printf("%s%s", orToString(or), EOL); 
         displayWide(ePyramid, sp);
         sp[SP_XYZ(0, 0, z)] = '`';
      }
   }
   displayWide(ePyramid, 0);
   free(sp);
}
