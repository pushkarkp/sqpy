/**
 * Copyright 2018 Pushkar Piggott
 *
 * Display.c
 */

#include "Display.h"

#include <stdlib.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
#define CUBE_SIZE (spHeight + 2)
#define CUBE_ROWS_PER_LEVEL (CUBE_SIZE + 1)
#define MAX_STORE 16
#define MARGIN_GLYPH ' '

///////////////////////////////////////////////////////////////////////////////
const char* eol[eEolTypes] = {"\n", "\r", "\r\n"};
int rows[eDisplayShapes] = {0};
int columns[eDisplayShapes] = {0};

///////////////////////////////////////////////////////////////////////////////
void initDisplay() {
   rows[ePyramid] = 0;
   int i = 0;
   for (i = 0; i < spHeight + 2; ++i) {
      rows[ePyramid] += i;
   }
   columns[ePyramid] = spHeight;
   rows[eCube] = CUBE_SIZE * CUBE_ROWS_PER_LEVEL + 1;
   columns[eCube] = CUBE_SIZE;
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
         z = row / CUBE_ROWS_PER_LEVEL;
         y = row % CUBE_ROWS_PER_LEVEL - 1;
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
         pos = spWholeRowToString(pos, spX - 1 - y, z, sp);
      }
   }
   pos = fill(columns[shape] + 1 + str - pos, MARGIN_GLYPH, pos);
   *pos = 0;
   return str;
}

///////////////////////////////////////////////////////////////////////////////
void display1(EDisplayShape shape, const TPlace* sp) {
   display1RowRange(shape, 0, rows[shape], sp);
}

///////////////////////////////////////////////////////////////////////////////
void display1RowRange(EDisplayShape shape, int row0, int row1, const TPlace* sp) {
   char* buf = malloc(columns[shape] + 1);
   const int r0 = (row0 > 0) ? row0 : 0;
   const int r1 = (row1 < rows[shape]) ? row1 : rows[shape];
   int r;
   for (r = r0; r < r1; ++r) {
      printf("%s%s", display1Line(buf, r, shape, sp), EOL);
   }
   free(buf);
}

///////////////////////////////////////////////////////////////////////////////
int displayPage(EDisplayShape shape, int w, int h, int n, const TPlace* sps) {
   return displayPageRowRange(shape, w, h, 0, rows[shape], n, sps);
}

///////////////////////////////////////////////////////////////////////////////
int displayPageRowRange(EDisplayShape shape, int w, int h, int row0, int row1, int n, const TPlace* sps) {
   char* buf = malloc(columns[shape] + 1);
   const int across = w / columns[shape];
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
void displayWide(EDisplayShape shape, int w, const TPlace* sp) {
   displayWideRowRange(shape, w, 0, rows[shape], sp);
}

///////////////////////////////////////////////////////////////////////////////
void displayWideRowRange(EDisplayShape shape, int w, int r0, int r1, const TPlace* sp) {
   static TPlace* store = 0;
   static int stored = 0;
   if (sp) {
      if (!store) {
         store = SP_NEW(MAX_STORE);
      }
      spCopy(&store[SPS(stored++)], sp);
   }
   if (!sp || stored + 1 > w / (columns[shape] + 1)) {
      displayPageRowRange(shape, w, 1000, r0, r1, stored, store);
      stored = 0;
   }
}
