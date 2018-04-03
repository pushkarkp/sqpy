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
void testPyramid() {
#define SPS 50
   TSquarePyramid sps[SPS];
   int i;
   for (i = 0; i < SPS; ++i) {
      spInit(sps[i]);
   }
   spEnumerate(sps[0]);
   display1(eUpright, sps[0]);
   TSquarePyramid cpy;
   display1(eUpright, spCopy(cpy, sps[0]));
   display1(eFolded, sps[0]);
   int left = SPS - displayPage(eFolded, 45, 50, SPS, sps);
   printf("%sleft %d%s", EOL, left, EOL);
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
   int i;
   for (i = 1; argv[i]; ++i) {
      if (strcmp("-p", argv[i]) == 0) {
         testPyramid();
         return 0;
      }
      if (strcmp("-f", argv[i]) == 0) {
         testSpFind();
         return 0;
      }
      if (strcmp("-s", argv[i]) == 0) {
         EOrientation or = e001XPlusPlus;
         if (argv[i + 1] && isdigit(argv[i + 1][0])) {
            or = strtol(argv[i + 1], 0, 10);
         }
         testPathSymmetry(or);
         return 0;
      }
      if (strcmp("-o", argv[i]) == 0) {
         EPresence pc = 0;
         if (argv[i + 1]) {
            pc = PIECE(argv[i + 1][0]);
         }
         const char* path = 0;
         if (argv[i + 1] && argv[i + 2] && isdigit(argv[i + 2][0])) {
            path = pieces[pc][strtol(argv[i + 2], 0, 10)];
         }
         if (argv[i + 1] && argv[i + 2] && argv[i + 2][0] != '-') {
            path = argv[i + 2];
         }
         testPieceOrientations(pc, path);
         return 0;
      }
   }
   setPathMaxLength();
   findRepeat();
   printf("solutions %d%s", solve(), EOL);
   return 0;
}