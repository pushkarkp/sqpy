/**
 * Copyright 2018 Pushkar Piggott
 *
 * SqPy.c
 */

#include "SquarePyramid.h"
#include "Display.h"
#include "Repeat.h"
#include "Solve.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

///////////////////////////////////////////////////////////////////////////////
void usage() {
   printf("usage:%s", EOL);
   printf("$ sqpy [-a] [-[-h <height>] [-p | -f | -s [<orientation>] | -o [<piece>] [<path>]]%s", EOL);
   printf(" -u                     stop when pieces all used, even if space remains%s", EOL);
   printf(" -h <height>            specify the pyramid height%s", EOL);
   printf(" -p                     show the pyramid%s", EOL);
   printf(" -l                     show the order of path start locations%s", EOL);
   printf(" -s [<orientation>]     show symmetries%s", EOL);
   printf(" -o [<piece>] [<path>]  show %s", EOL);
   printf("                        show the solutions%s", EOL);
   exit(0);
}

///////////////////////////////////////////////////////////////////////////////
const char* getOptionExtraValue(int* pi, char** argv) {
   if (argv[*pi + 1] && argv[*pi + 1][0] != '-') {
      ++*pi;
      return argv[*pi];
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
const char* getOptionValue(int* pi, char** argv) {
   if (argv[*pi][2]) {
      return &argv[*pi][2];
   }
   return getOptionExtraValue(pi, argv);
}

///////////////////////////////////////////////////////////////////////////////
const char* getMandatory(const char* v, char c) {
   if (!v) {
      printf("missing value for option '-%c'%s", c, EOL);
      usage();
   }
   return v;
}

///////////////////////////////////////////////////////////////////////////////
const char* getNumber(const char* v, char c) {
   if (v && !isdigit(v[0])) {
      printf("value for option '-%c' is not a number%s", c, EOL);
      usage();
   }
   return v;
}

///////////////////////////////////////////////////////////////////////////////
void testPyramid() {
   TPlace* sps = SP_NEW(2);
   spInit(&sps[0]);
   displayWide(ePyramid, 65, &sps[0]);
   spEnumerate(&sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, 0);
   free(sps);
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
   int useOnce = 0;
   spSetHeight(5);
   initDisplay();
   int i;
   for (i = 1; argv[i] != NULL && argv[i][0] == '-'; ++i) {
      switch (argv[i][1]) {
      case 'u': {
         useOnce = 1;
         break;
      }
      case 'h': {
         const char* n = getNumber(getMandatory(getOptionValue(&i, argv), argv[i][1]), argv[i][1]);
         spSetHeight(strtol(n, 0, 10));
         initDisplay();
         break;
      }
      case 'p': {
         testPyramid();
         return 0;
      }
      case 'l': {
         testSpFind();
         return 0;
      }
      case 's': {
         EOrientation or = e001XPlusPlus;
         const char* n = getNumber(getOptionValue(&i, argv), argv[i][1]);
         if (n) {
            or = strtol(n, 0, 10);
         }
         testPathSymmetry(or);
         return 0;
      }
      case 'o': {
         EPresence pc = 0;
         const char* n = getMandatory(getOptionValue(&i, argv), argv[i][1]);
         if (isdigit(n[0])) {
            pc = strtol(n, 0, 10);
         } else {
            pc = PIECE(n[0]);
         }
         const char* path = getOptionExtraValue(&i, argv);
         if (!path) {
            path = "0";
         }
         if (isdigit(path[0])) {
            int p = strtol(path, 0, 10);
            int i;
            for (i = 0; pieces[pc][i]; ++i) {}
            if (p >= i) {
               printf("path index (%d) out of range [0..%d] for %c (%s)%s", p, i - 1, GLYPH(pc), presToString(pc), EOL);
               exit(0);
            }
            path = pieces[pc][p];
         }
         testPieceOrientations(pc, path);
         return 0;
      }
      default:
         printf("unrecognized option '-%c'%s", argv[i][1], EOL);
         usage();
      }
   }
   setPathMaxLength();
   findRepeat();
   printf("solutions %d%s", solve(useOnce), EOL);
   return 0;
}