/**
 * Copyright 2018 Pushkar Piggott
 *
 * Path.c
 */

#include "Path.h"
#include "Display.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ONE_OF_ADVANCE(c) (PATH_STEP_IS_FORWARD(c) ? 1 : -1)

///////////////////////////////////////////////////////////////////////////////
int makeStep(int sideways, int advance) {
   return advance
        + (advance > 0 ? PATH_STEP_MIN - 1 : PATH_STEP_MAX + 1) 
        & (sideways ? ~0x20 : ~0);
}

#define ONE_BACKWARDS_OF(c) makeStep(PATH_STEP_IS_SIDEWAYS(c), -ONE_OF_ADVANCE(c))
#define ONE_LESS_THAN(c) makeStep(PATH_STEP_IS_SIDEWAYS(c), PATH_STEP_ADVANCE(c) - ONE_OF_ADVANCE(c))
#define ONE_MORE_THAN(c) makeStep(PATH_STEP_IS_SIDEWAYS(c), PATH_STEP_ADVANCE(c) + ONE_OF_ADVANCE(c))

///////////////////////////////////////////////////////////////////////////////
int removeDotPair(char* path, int dot) {
   int len = strlen(path);
   if (len < 3
    || path[dot] != '.'
    || path[0] != '.') {
      return 0;
   }
   int i;
   for (i = 0; i < dot - 1; ++i) {
      path[i] = path[i + 1];
   }
   path[i] = 0;
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int pathDotCount(TPath path) {
   int len = strlen(path);
   int dots = 0;
   int i;
   for (i = 0; i < len; ++i) {
      if (path[i] == '.') {
         ++dots;
      }
   }
   return dots;
}

///////////////////////////////////////////////////////////////////////////////
char* pathStartReversal(TPath path) {
   char* newpath = (char*)malloc((strlen(path) + 4) * sizeof(char));
   newpath[0] = '.';
   newpath[1] = ONE_BACKWARDS_OF(path[0]);
   newpath[2] = '.';
   int pre = 3;
   if (abs(PATH_STEP_ADVANCE(path[0])) > 1) {
      newpath[3] = ONE_LESS_THAN(path[0]);
      ++pre;
   }
   strcpy(newpath + pre, path + 1);
   return newpath;
}

///////////////////////////////////////////////////////////////////////////////
char* pathNextReversal(TPath path) {
//printf("pathNextReversal(%s)\r\n", path);
   int len = strlen(path);
   if (len < 4) {
      printf("path '%s' is too short%s", path, EOL);
      return 0;
   }
   char* newpath = strdup(path);
   int dot;
   for (dot = 2; dot < len; ++dot) {
      if (!newpath[dot]) {
         free(newpath);
         return 0;
      }
      if (newpath[dot] == '.') {
         break;
      }
   }
   if (dot == len - 1) {
      printf("path '%s' is dot delimited%s", path, EOL);
      return 0;
   }
   if (path[dot + 1] == '.') {
      printf("path '%s' has consecutive dots%s", path, EOL);
      return 0;
   }
   if (PATH_STEP_IS_FORWARD(path[1]) == PATH_STEP_IS_FORWARD(path[dot + 1])
    || PATH_STEP_IS_SIDEWAYS(path[1]) != PATH_STEP_IS_SIDEWAYS(path[dot + 1])) {
      newpath = (char*)realloc(newpath, (strlen(newpath) + 1) * sizeof(char));
      int i;
      for (i = len + 1; i > 1; --i) {
         newpath[i] = newpath[i - 1];
      }
      len = strlen(newpath);
      ++dot;
      newpath[1] = ONE_BACKWARDS_OF(newpath[dot + 1]) + ONE_OF_ADVANCE(newpath[dot + 1]);
   }
   if (abs(PATH_STEP_ADVANCE(newpath[dot + 1])) > 1) {
      newpath[dot + 1] = ONE_LESS_THAN(newpath[dot + 1]);
   } else {
      int i;
      for (i = dot + 1; i < len; ++i) {
         newpath[i] = newpath[i + 1];
      }
   }
   newpath[1] = ONE_MORE_THAN(newpath[1]);
   return newpath;
}

///////////////////////////////////////////////////////////////////////////////
int pathRemoveBothEndDots(char* path) {
   int len = strlen(path);
   if (len < 1
    || path[len - 1] != '.'
    || path[0] != '.') {
      return 0;
   }
   int i;
   for (i = 0; i < len - 2; ++i) {
      path[i] = path[i + 1];
   }
   path[i] = 0;
   return 1;
}
