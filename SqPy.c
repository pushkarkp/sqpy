/**
 * Copyright 2018 Pushkar Piggott
 *
 * LonSolve.c
 */

#include "SquarePyramid.h"
#include "Display.h"
#include "Repeat.h"
#include "Solve.h"
#include <stdio.h>

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
   printf("\r\nleft %d\r\n", left);
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
   //testPyramid();
   findRepeat();
   solve();
   return 0;
}