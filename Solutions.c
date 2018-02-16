/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solutions.c
 */

#include "Solutions.h"
#include "Display.h"

//#include <stdlib.h>
//#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
#define MAX_SOLUTIONS 1000
static TSquarePyramid solutions[MAX_SOLUTIONS];
static int count = 0;

///////////////////////////////////////////////////////////////////////////////
void solReset() {
   count = 0;   
}

///////////////////////////////////////////////////////////////////////////////
int solGetCount() {
   return count;   
}

///////////////////////////////////////////////////////////////////////////////
int solAddUniqueSymmetric(TSquarePyramid sp) {
   int eq;
   for (eq = count - 1; eq >= 0; --eq) {
      ERotation rot = spEqualRotation(sp, solutions[eq]);
      int ref = spEqualReflection(sp, solutions[eq]);
      if (rot != e0 || ref != eNoReflection) {
displayWide(eUpright, 80, NULL);
         if (rot != e0) {
printf("rotation: %s %d==%d%s", rotationToString(rot), count, eq, eol[EOL_TYPE]);
         } else if (ref != e001) {
char buf[16];
printf("reflection: %s %d==%d%s", reflectionToString(buf, ref), count, eq, eol[EOL_TYPE]);
         }
display1(eUpright, sp);
         return;
      }
   }
printf("count %d\r\n", count);
displayWide(eUpright, 80, sp);
   spCopy(solutions[count++], sp);
}
