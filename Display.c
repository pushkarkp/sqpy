/**
 * Copyright 2018 Pushkar Piggott
 *
 * Display.c
 */

#include "Display.h"

#include <stdlib.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
#define MARGIN_GLYPH ' '

///////////////////////////////////////////////////////////////////////////////
const char* eol[eEolTypes] = {"\n", "\r", "\r\n"};
static int plane = 0;
static int pageWidth = 0;
static int spCount = 0;
static int rows[eDisplayShapes] = {0};

///////////////////////////////////////////////////////////////////////////////
void initDisplay(int pw) {
   plane = 0;
   rows[ePyramid] = 0;
   int i = 0;
   for (i = 0; i < spHeight + 2; ++i) {
      rows[ePyramid] += i;
   }
   rows[eCube] = spHeight * (spHeight + 1) + 1;
   if (pw >= 0) {
      pageWidth = pw;
   }
//printf("rows[ePyramid] %d rows[eCube] %d\r\n", rows[ePyramid], rows[eCube]);
}

///////////////////////////////////////////////////////////////////////////////
char* fill(int n, char c, char* pos) {
   int i;
   for (i = 0; i < n; ++i) {
      *pos++ = c;
   }
   return pos;
}

///////////////////////////////////////////////////////////////////////////////
char* display1Line(char* str, int row, EDisplayShape shape, const TPlace* sp) {
   int z = 0;
   int y = 0;
   switch (shape) {
      case ePyramid: {
         int r = 0;
         for (z = 0; r + z + 1 < row; ++z) {
            r += z + 2;
         }
         y = row - r - 1;
         break;
      }
      case eCube: {
         z = row / (spHeight + 1) + 1;
         y = row % (spHeight + 1) - 1;
         break;
      }
   }

//printf("z %d y %d%s", z, y, EOL);
   char* pos = str;
   *pos++ = MARGIN_GLYPH;
   if (y != -1) {
      if (shape == ePyramid) {
         // z - y puts y == 0 last (at the bottom)
         pos = spRowToString(pos, z - y, z, sp);
      } else {
         // spX - 1 - y puts y == 0 last (at the bottom)
         pos = spCubeRowToString(pos, spX - 1 - y, z, sp);
      }
   }
   pos = fill(spHeight + 1 + str - pos, MARGIN_GLYPH, pos);
   *pos = 0;
   return str;
}

///////////////////////////////////////////////////////////////////////////////
typedef void (*FDisplayRowRange)(EDisplayShape, int, int, const TPlace*);
void displayPlane(int z, const TPlace* sp, FDisplayRowRange f) {
   plane = 1;
   int start = (spHeight + 1) * z;
   f(eCube, start, start + spHeight + 2, sp);
}

///////////////////////////////////////////////////////////////////////////////
void display1(EDisplayShape shape, const TPlace* sp) {
   display1RowRange(shape, 0, rows[shape], sp);
}

///////////////////////////////////////////////////////////////////////////////
void display1Plane(int z, const TPlace* sp) {
   displayPlane(z, sp, display1RowRange);
}

///////////////////////////////////////////////////////////////////////////////
void display1RowRange(EDisplayShape shape, int row0, int row1, const TPlace* sp) {
   char* buf = malloc(spHeight + 1);
   const int r0 = (row0 > 0) ? row0 : 0;
   const int r1 = (row1 < rows[shape]) ? row1 : rows[shape];
   int r;
   for (r = r0; r < r1; ++r) {
      printf("%s%s", display1Line(buf, r, shape, sp), EOL);
   }
   free(buf);
}

///////////////////////////////////////////////////////////////////////////////
int displayPage(EDisplayShape shape, int h, int n, const TPlace* sps) {
   return displayPageRowRange(shape, h, 0, rows[shape], n, sps);
}

///////////////////////////////////////////////////////////////////////////////
int displayPageRowRange(EDisplayShape shape, int h, int row0, int row1, int n, const TPlace* sps) {
   char* buf = malloc(spHeight + 1);
   const int across = pageWidth / (spHeight + 1);
   const int down = h / rows[shape];
   const int r0 = (row0 > 0) ? row0 : 0;
   const int r1 = (row1 < rows[shape]) ? row1 : rows[shape];
   int p = 0;
   int d;
   for (d = 0; d < down && p < n; ++d) {
      int limit = (p + across > n) ? n - p : across;
      int r;
      for (r = r0; r < r1; ++r) {
         int a;
         for (a = 0; a < limit; ++a) {
            printf(display1Line(buf, r, shape, &sps[SPS(p + a)]));
         }
         printf(EOL);
      }
      p += limit;
   }
   return p;
}

///////////////////////////////////////////////////////////////////////////////
void displayWideRowRange(EDisplayShape shape, int r0, int r1, const TPlace* sp) {
   static TPlace* store = 0;
   static int stored = 0;
   if (sp) {
      if (pageWidth / (spHeight + 1) > spCount) {
         spCount = pageWidth / (spHeight + 1);
         store = SP_EXTEND(store, spCount);
      }
      spCopy(&store[SPS(stored++)], sp);
   }
   if (!sp || stored + 1 > spCount) {
      displayPageRowRange(shape, 1000, r0, r1, stored, store);
      stored = 0;
      plane = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
void displayWide(EDisplayShape shape, const TPlace* sp) {
   if (sp && plane) {
      printf("display conflict%s", EOL);
      displayWideRowRange(shape, 0, rows[shape], 0);
   }      
   displayWideRowRange(shape, 0, rows[shape], sp);
}

///////////////////////////////////////////////////////////////////////////////
void displayWidePlane(int z, const TPlace* sp) {
   displayPlane(z, sp, displayWideRowRange);
}

///////////////////////////////////////////////////////////////////////////////
int displayIsPlane() {
   return plane;
}
