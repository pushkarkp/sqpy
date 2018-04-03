/**
 * Copyright 2018 Pushkar Piggott
 *
 * Display.c
 */

#include "Display.h"

#include <stdlib.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
#define UPRIGHT_ROWS 21
#define UPRIGHT_COLUMNS 7
#define FOLDED_ROWS 12
#define FOLDED_COLUMNS 10
#define CUBE_ROWS 57
#define CUBE_COLUMNS 9
#define MAX_LAYOUT 7
#define MAX_STORE 16
#define MARGIN_GLYPH ' '

///////////////////////////////////////////////////////////////////////////////
const char* eol[eEolTypes] = {"\n", "\r", "\r\n"};

///////////////////////////////////////////////////////////////////////////////
typedef enum {eLeftMargin, eGap, eRightMargin, eSpaceFields} ESpaceField;
const int rows[eDisplayShapes] = {UPRIGHT_ROWS, FOLDED_ROWS, CUBE_ROWS};
const int columns[eDisplayShapes] = {UPRIGHT_COLUMNS, FOLDED_COLUMNS, CUBE_COLUMNS};
const int layout[eDisplayShapes][CUBE_ROWS][MAX_LAYOUT] = {
   {
      {7, 0, 0},
      {1, 0, 5, 0, 0},
      {7, 0, 0},
      {1, 0, 4, 1, 1},
      {1, 0, 4, 0, 1},
      {7, 0, 0},
      {1, 0, 3, 2, 2},
      {1, 0, 3, 1, 2},
      {1, 0, 3, 0, 2},
      {7, 0, 0},
      {1, 0, 2, 3, 3},
      {1, 0, 2, 2, 3},
      {1, 0, 2, 1, 3},
      {1, 0, 2, 0, 3},
      {7, 0, 0},
      {1, 0, 1, 4, 4},
      {1, 0, 1, 3, 4},
      {1, 0, 1, 2, 4},
      {1, 0, 1, 1, 4},
      {1, 0, 1, 0, 4},
      {7, 0, 0}
   }, {
      {10, 0, 0},
      {1, 1, 1, 3, 3, 2, 2},
      {1, 1, 1, 2, 3, 1, 2},
      {1, 1, 1, 1, 3, 0, 2},
      {1, 0, 5, 0, 3},
      {7, 0, 1, 1, 1},
      {1, 1, 1, 4, 4, 0, 1},
      {1, 0, 4, 3, 4},
      {1, 2, 1, 2, 4, 0, 0},
      {1, 0, 4, 1, 4},
      {1, 0, 4, 0, 4},
      {10, 0, 0}
   }, {
      {9, 0, 0},
      {1, 0, 1, 6, 0},
      {1, 0, 1, 5, 0},
      {1, 0, 1, 4, 0},
      {1, 0, 1, 3, 0},
      {1, 0, 1, 2, 0},
      {1, 0, 1, 1, 0},
      {1, 0, 1, 0, 0},
      {9, 0, 0},
      {1, 0, 1, 6, 1},
      {1, 0, 1, 5, 1},
      {1, 0, 1, 4, 1},
      {1, 0, 1, 3, 1},
      {1, 0, 1, 2, 1},
      {1, 0, 1, 1, 1},
      {1, 0, 1, 0, 1},
      {9, 0, 0},
      {1, 0, 1, 6, 2},
      {1, 0, 1, 5, 2},
      {1, 0, 1, 4, 2},
      {1, 0, 1, 3, 2},
      {1, 0, 1, 2, 2},
      {1, 0, 1, 1, 2},
      {1, 0, 1, 0, 2},
      {9, 0, 0},
      {1, 0, 1, 6, 3},
      {1, 0, 1, 5, 3},
      {1, 0, 1, 4, 3},
      {1, 0, 1, 3, 3},
      {1, 0, 1, 2, 3},
      {1, 0, 1, 1, 3},
      {1, 0, 1, 0, 3},
      {9, 0, 0},
      {1, 0, 1, 6, 4},
      {1, 0, 1, 5, 4},
      {1, 0, 1, 4, 4},
      {1, 0, 1, 3, 4},
      {1, 0, 1, 2, 4},
      {1, 0, 1, 1, 4},
      {1, 0, 1, 0, 4},
      {9, 0, 0},
      {1, 0, 1, 6, 5},
      {1, 0, 1, 5, 5},
      {1, 0, 1, 4, 5},
      {1, 0, 1, 3, 5},
      {1, 0, 1, 2, 5},
      {1, 0, 1, 1, 5},
      {1, 0, 1, 0, 5},
      {9, 0, 0},
      {1, 0, 1, 6, 6},
      {1, 0, 1, 5, 6},
      {1, 0, 1, 4, 6},
      {1, 0, 1, 3, 6},
      {1, 0, 1, 2, 6},
      {1, 0, 1, 1, 6},
      {1, 0, 1, 0, 6},
      {9, 0, 0}
   }
};

///////////////////////////////////////////////////////////////////////////////
char* margin(char* pos, int n) {
   int i;
   for (i = 0; i < n; ++i) {
      *pos++ = MARGIN_GLYPH;
   }
   return pos;
}

///////////////////////////////////////////////////////////////////////////////
char* display1Line(char* str, int r, EDisplayShape shape, const TSquarePyramid sp) {
   char* pos = str;
   pos = margin(pos, layout[shape][r][eLeftMargin]);
   int i;
   for (i = eSpaceFields; pos + layout[shape][r][eRightMargin] < str + columns[shape]; i += 2) {
      if (i > eSpaceFields) {
         pos = margin(pos, layout[shape][r][eGap]);
      }
      if (shape == eCube) {
         pos = spWholeRowToString(pos, layout[shape][r][i], layout[shape][r][i + 1], sp);
      } else {
         pos = spRowToString(pos, layout[shape][r][i], layout[shape][r][i + 1], sp);
      }
   }
   pos = margin(pos, layout[shape][r][eRightMargin]);
   *pos = 0;
   return str;
}

///////////////////////////////////////////////////////////////////////////////
void display1(EDisplayShape shape, const TSquarePyramid sp) {
   display1RowRange(shape, 0, rows[shape], sp);
}

///////////////////////////////////////////////////////////////////////////////
void display1RowRange(EDisplayShape shape, int row0, int row1, const TSquarePyramid sp) {
   char* buf = malloc(columns[shape] + 1);
//printf("display1 buf size %d%s", columns[shape] + 1, EOL);
   const int r0 = (row0 > 0) ? row0 : 0;
   const int r1 = (row1 < rows[shape]) ? row1 : rows[shape];
   int r;
   for (r = r0; r < r1; ++r) {
      printf(display1Line(buf, r, shape, sp));
      printf(EOL);
   }
}

///////////////////////////////////////////////////////////////////////////////
int displayPage(EDisplayShape shape, int w, int h, int n, const TSquarePyramid* sp) {
   return displayPageRowRange(shape, w, h, 0, rows[shape], n, sp);
}

///////////////////////////////////////////////////////////////////////////////
int displayPageRowRange(EDisplayShape shape, int w, int h, int row0, int row1, int n, const TSquarePyramid* sp) {
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
            printf(display1Line(buf, r, shape, sp[p + a]));
         }
         printf(EOL);
      }
      p += limit;
   }
   return p;
}

///////////////////////////////////////////////////////////////////////////////
void displayWide(EDisplayShape shape, int w, const TSquarePyramid sp) {
   displayWideRowRange(shape, w, 0, rows[shape], sp);
}

///////////////////////////////////////////////////////////////////////////////
void displayWideRowRange(EDisplayShape shape, int w, int r0, int r1, const TSquarePyramid sp) {
   static TSquarePyramid store[MAX_STORE];
   static int stored = 0;
   if (sp) {
      spCopy(store[stored++], sp);
   }
   if (!sp || stored + 1 > w / columns[shape]) {
      displayPageRowRange(shape, w, 1000, r0, r1, stored, store);
      stored = 0;
   }
}
