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
int pathOk(TPath path) {
   const int len = strlen(path);
   if (len < 1) {
      printf("empty path%s", EOL);
      return 0;
   }
   int i;
   for (i = 0; i < len; ++i) {
      if (path[i] == '.' && path[i + 1] == '.') {
         printf("path '%s' contains adjacent dots%s", path, EOL);
         return 0;
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void cut(int first, int count, char* path) {
printf("cut(%d %d %s)%s", first, count, path, EOL);
   int len = strlen(path);
   int i;
   for (i = first; i < len - count + 1; ++i) {
      path[i] = path[i + count];
   }
}

///////////////////////////////////////////////////////////////////////////////
int pathDotCount(TPath path) {
   int dots = 0;
   int i;
   for (i = 0; path[i]; ++i) {
      if (path[i] == '.') {
         ++dots;
      }
   }
   return dots;
}

///////////////////////////////////////////////////////////////////////////////
int nextDot(TPath path) {
   int dot;
   for (dot = 0; path[dot] && path[dot] != '.'; ++dot) {}
   return dot;
}

///////////////////////////////////////////////////////////////////////////////
// preserve dimension and length, reverse direction
int reverseChar(int c) {
   return makeStep(PATH_STEP_IS_SIDEWAYS(c), -PATH_STEP_ADVANCE(c));
}

///////////////////////////////////////////////////////////////////////////////
char* reversePrefix(char* path) {
   int dot = nextDot(path);
//printf("reversePrefix %s dot %d\r\n", path, dot);
   int i;
   for (i = 0; i < dot / 2; ++i) {
      int right = dot - 1 - i;
      char c = reverseChar(path[right]);
      path[right] = reverseChar(path[i]);
      path[i] = c;
   }
   if (dot & 1) {
      int mid = dot / 2;
      path[mid] = reverseChar(path[mid]);
   }
//printf("reversePrefix newpath %s\r\n", path);
   return path;
}

///////////////////////////////////////////////////////////////////////////////
char* flip(int dot, TPath path) {
   int len = strlen(path) - 1;
//printf("flip(%d %s) len %d\r\n", dot, path, len);
   char* newpath = (char*)malloc((len + 1) * sizeof(char));
   int i;
   for (i = 0; i < len - dot; ++i) {
      newpath[i] = path[dot + 1 + i];
   }
   newpath[i] = 0;
//printf("flip path1 '%s'\r\n", newpath);
   int j;
   for (j = 0; j < dot; ++j) {
      newpath[i + j] = path[j];
   }
   newpath[len] = 0;
//printf("flip path2 '%s'\r\n", newpath);
   return newpath;
}

///////////////////////////////////////////////////////////////////////////////
char* removeLast(int dot, char* path) {
   int cut = (path[dot + 2] == '.') ? 2 : 1;
//printf("removeLast(%d, %s) cut %d\r\n", dot, path, cut);
   int i;
   for (i = dot + 1; path[i]; ++i) {
      path[i] = path[i + cut];
      if (!path[i]) {
         return path;
      }
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
char* addOne(int len, TPath path) {
   char* newpath = (char*)malloc((len + 2) * sizeof(char));
   int i;
   for (i = len + 1; i > 1; --i) {
      newpath[i] = path[i - 1];
   }
   newpath[0] = '.';
   return newpath;
}

///////////////////////////////////////////////////////////////////////////////
int removeDotPair(int first, int last, char* path) {
   int len = strlen(path);
   if (last == -1) {
      last = len - 1;
   }
   if (len < 3
    || path[first] != '.'
    || path[last] != '.') {
      return 0;
   }
   int i;
   for (i = first; i < last - 1; ++i) {
      path[i] = path[i + 1];
   }
   for (; i < len - 1; ++i) {
      path[i] = path[i + 2];
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int pathRemoveBothEndDots(char* path) {
   return removeDotPair(0, -1, path);
}

///////////////////////////////////////////////////////////////////////////////
char* startReversal(TPath path) {
//printf("startReversal(%s)\r\n", path);
   char* newpath = (char*)malloc((strlen(path) + 4) * sizeof(char));
   newpath[0] = '.';
   newpath[1] = ONE_BACKWARDS_OF(path[0]);
   newpath[2] = '.';
   if (abs(PATH_STEP_ADVANCE(path[0])) == 1) {
   newpath[3] = 0;
printf("startReversal %s %s\r\n", newpath, path + 1);
      strcpy(newpath + 2, path + 1);
      return newpath;
   }
   newpath[3] = ONE_LESS_THAN(path[0]);
   strcpy(newpath + 4, path + 1);
   return newpath;
}

///////////////////////////////////////////////////////////////////////////////
char* pathNext(TPath path) {
printf("pathNext(%s) %d dots\r\n", path, pathDotCount(path));
   if (path[0] != '.') {
      return startReversal(path);
   }
   int len = strlen(path);
   if (len < 4) {
      printf("path '%s' is too short%s", path, EOL);
      return 0;
   }
   int dots = pathDotCount(path);
   int dot = nextDot(path + 2) + 2;
   if (!path[dot]) {
      return 0;
   }
   if (!path[dot + 1]) {
      printf("path '%s' is dot delimited%s", path, EOL);
      return 0;
   }
   if (path[dot + 1] == '.') {
      printf("path '%s' has consecutive dots%s", path, EOL);
      return 0;
   }
   if (dots & 1) {
      return reversePrefix(flip(dot, path));
   }
   char* newpath;
   if (PATH_STEP_IS_FORWARD(path[1]) != PATH_STEP_IS_FORWARD(path[dot + 1])
    && PATH_STEP_IS_SIDEWAYS(path[1]) == PATH_STEP_IS_SIDEWAYS(path[dot + 1])) {
      newpath = strdup(path);
   } else {
      newpath = addOne(len, path);
      ++len;
      ++dot;
      newpath[1] = ONE_BACKWARDS_OF(newpath[dot + 1]) + ONE_OF_ADVANCE(newpath[dot + 1]);
   }
   newpath[1] = ONE_MORE_THAN(newpath[1]);
   if (abs(PATH_STEP_ADVANCE(newpath[dot + 1])) > 1) {
      newpath[dot + 1] = ONE_LESS_THAN(newpath[dot + 1]);
      return newpath;
   }
   return removeLast(dot, newpath);
}
