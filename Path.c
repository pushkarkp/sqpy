/**
 * Copyright 2018 Pushkar Piggott
 *
 * Path.c
 */

#include "Path.h"
#include "Display.h"
#include "SetOf.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MIN_MARKER '*'
#define MAX_MARKER '/'
#define MARKER MAX_MARKER
#define MARKER_AS_EL(c) (c - MIN_MARKER + 1)
#define IS_MARKER(c) (c >= MIN_MARKER && c <= MAX_MARKER)

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
char* pathMarkers() {
   return "*+,-.";
}

///////////////////////////////////////////////////////////////////////////////
int pathMarkerCount(TPath path) {
   int markers = 0;
   int i;
   for (i = 0; path[i]; ++i) {
      if (IS_MARKER(path[i])) {
         ++markers;
      }
   }
   return markers;
}

///////////////////////////////////////////////////////////////////////////////
int pathMarkCount(TPath path) {
   int markers = 0;
   int i;
   for (i = 0; path[i]; ++i) {
      if (IS_MARKER(path[i]) && !SET_HAS(markers, MARKER_AS_EL(path[i]))) {
         markers = SET_WITH(markers, MARKER_AS_EL(path[i]));
      }
   }
   return setCount(markers);
}

///////////////////////////////////////////////////////////////////////////////
int pathOk(TPath path, const char* prefix) {
   const int len = strlen(path);
   if (len < 1) {
      printf("%sempty path%s", prefix?prefix:"", EOL);
      return 0;
   }
   if (pathMarkerCount(path) == 1) {
      printf("%spath '%s' contains only one of marker '%c'%s", 
             prefix?prefix:"", path, MARKER, EOL);
      return 0;
   }
   int one = 0;;
   int more = 0;;
   int i;
   for (i = 0; i < len; ++i) {
      if ((path[i] < MIN_MARKER || path[i] >= MAX_MARKER)
       && (path[i] < 'A' || path[i] > 'Z')
       && (path[i] < 'a' || path[i] > 'z')) {
         printf("%spath '%s' contains illegal character '%c' (%d)%s", 
                prefix?prefix:"", path, path[i], (int)path[i], EOL);
         return 0;
      }
      if (IS_MARKER(path[i])) {
         if (path[i] == path[i + 1]) {
            printf("%spath '%s' contains adjacent '%c' markers%s", 
                   prefix?prefix:"", path, path[i], EOL);
            return 0;
         }
         if (IS_MARKER(path[i + 1])) {
            printf("%spath '%s' contains adjacent markers '%c' and '%c'%s", 
                   prefix?prefix:"", path, path[i], path[i + 1], EOL);
            return 0;
         }
         if (!SET_HAS(one, MARKER_AS_EL(path[i]))) {
            one = SET_WITH(one, MARKER_AS_EL(path[i]));
         } else 
         if (!SET_HAS(more, MARKER_AS_EL(path[i]))) {
            more = SET_WITH(more, MARKER_AS_EL(path[i]));
         }
      } else if (path[i + 1] 
              && !IS_MARKER(path[i + 1])
              && PATH_STEP_IS_SIDEWAYS(path[i]) == PATH_STEP_IS_SIDEWAYS(path[i + 1])) {
         printf("%spath '%s' contains consecutive steps '%c%c' in the same dimension%s", 
                prefix?prefix:"", path, path[i], path[i + 1], EOL);
         return 0;
      }
   }
   for (i = 0; i < MAX_MARKER - MIN_MARKER; ++i) {
      if (SET_HAS(one, 1 << i) && !SET_HAS(more, 1 << i)) {
         printf("%spath '%s' contains only one of '%c' marker%s", 
                prefix?prefix:"", path, i + MIN_MARKER, EOL);
         return 0;
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int nextMarker(TPath path) {
   int marker;
   for (marker = 0; path[marker] && !IS_MARKER(path[marker]); ++marker) {}
   return marker;
}

///////////////////////////////////////////////////////////////////////////////
// preserve dimension and length, reverse direction
int reverseChar(int c) {
   return makeStep(PATH_STEP_IS_SIDEWAYS(c), -PATH_STEP_ADVANCE(c));
}

///////////////////////////////////////////////////////////////////////////////
char* reversePrefix(char* path) {
   int marker = nextMarker(path);
//printf("reversePrefix(%s) marker %d\r\n", path, marker);
   int i;
   for (i = 0; i < marker / 2; ++i) {
      int right = marker - 1 - i;
      char c = reverseChar(path[right]);
      path[right] = reverseChar(path[i]);
      path[i] = c;
   }
   if (marker & 1) {
      int mid = marker / 2;
      path[mid] = reverseChar(path[mid]);
   }
//printf("reversePrefix newpath %s\r\n", path);
   return path;
}

///////////////////////////////////////////////////////////////////////////////
void cut(int first, int count, char* path) {
//printf("cut(%d %d %s)%s", first, count, path, EOL);
   int len = strlen(path);
   int i;
   for (i = first; i < len - count + 1; ++i) {
      path[i] = path[i + count];
   }
}

///////////////////////////////////////////////////////////////////////////////
char* flip(int marker, TPath path) {
   int len = strlen(path) - 1;
//printf("flip(%d %s) len %d\r\n", marker, path, len);
   char* newpath = (char*)malloc((len + 1) * sizeof(char));
   int i;
   for (i = 0; i < len - marker; ++i) {
      newpath[i] = path[marker + 1 + i];
   }
//newpath[i] = 0;
//printf("flip path1 '%s'\r\n", newpath);
   newpath[i] = path[nextMarker(path + marker + 1) + marker + 1];
   int j;
   for (j = 1; j < marker; ++j) {
      newpath[i + j] = path[j];
   }
   newpath[len] = 0;
//printf("flip path2 '%s'\r\n", newpath);
   return newpath;
}

///////////////////////////////////////////////////////////////////////////////
char* removeLast(int marker, char* path) {
//printf("0 removeLast(%d, %s)\r\n", marker, path);
   int cut = 1;
   if (IS_MARKER(path[marker + 2])) {
      path[0] = path[marker] = path[marker + 2];
      cut = 2;
   }
//printf("1 removeLast() %s cut %d\r\n", path, cut);
   int i;
   for (i = marker + 1; path[i]; ++i) {
      path[i] = path[i + cut];
      if (!path[i]) {
//printf("2 removeLast() %s\r\n", path);
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
   newpath[0] = MARKER;
   return newpath;
}

///////////////////////////////////////////////////////////////////////////////
int removeMarkerPair(int first, int last, char* path) {
   int len = strlen(path);
   if (last == -1) {
      last = len - 1;
   }
   if (len < 3
    || !IS_MARKER(path[first])
    || path[last] != path[first]) {
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
int pathRemoveBothEndMarkers(char* path) {
   return removeMarkerPair(0, -1, path);
}

///////////////////////////////////////////////////////////////////////////////
char* startReversal(TPath path) {
//printf("0 startReversal(%s)\r\n", path);
   char* newpath = (char*)malloc((strlen(path) + 4) * sizeof(char));
   newpath[0] = MARKER;
   newpath[1] = ONE_BACKWARDS_OF(path[0]);
   newpath[2] = MARKER;
   if (abs(PATH_STEP_ADVANCE(path[0])) == 1) {
      if (path[1] == 0) {
         newpath[0] = newpath[1];
         newpath[1] = 0;
//printf("1 startReversal %s\r\n", newpath);
         return newpath;
      }
      newpath[3] = 0;
//printf("2 startReversal %s %s -> ", newpath, path + 1);
      int start = 3;
      if (IS_MARKER(path[1])) {
         start = 2;
         newpath[0] = path[1];
      }
      strcpy(newpath + start, path + 1);
//printf("%s\r\n", newpath);
      return newpath;
   }
   newpath[3] = ONE_LESS_THAN(path[0]);
   strcpy(newpath + 4, path + 1);
   return newpath;
}

///////////////////////////////////////////////////////////////////////////////
char* pathNext(TPath path) {
//printf("0 pathNext(%s)\r\n", path);
   if (!IS_MARKER(path[0])) {
      return startReversal(path);
   }
   int len = strlen(path);
   if (len < 4) {
      printf("path '%s' is too short%s", path, EOL);
      return 0;
   }
   int markers = pathMarkerCount(path);
   int marker = nextMarker(path + 2) + 2;
   if (!path[marker]) {
      return 0;
   }
   if (!path[marker + 1]) {
      printf("path '%s' is marker delimited%s", path, EOL);
      return 0;
   }
   if (IS_MARKER(path[marker + 1])) {
      printf("path '%s' has consecutive markers '%c%c'%s", 
             path, path[marker], path[marker + 1], EOL);
      return 0;
   }
//printf("1 pathNext(%s) markers %d marker %d%s", path, markers, marker, EOL);
   int m = nextMarker(path);
   if (m < len && path[m] != MARKER) {
//printf("reversePrefix(%s) path[m] %c%s", path, path[m], EOL);
      // reached fork
      return reversePrefix(flip(marker, path));
   }
   char* newpath;
   if (PATH_STEP_IS_FORWARD(path[1]) != PATH_STEP_IS_FORWARD(path[marker + 1])
    && PATH_STEP_IS_SIDEWAYS(path[1]) == PATH_STEP_IS_SIDEWAYS(path[marker + 1])) {
//printf("2 pathNext(%s) dup%s", path, EOL);
      newpath = strdup(path);
   } else {
//printf("2 pathNext(%s) add one%s", path, EOL);
      newpath = addOne(len, path);
      ++len;
      ++marker;
      newpath[1] = ONE_BACKWARDS_OF(newpath[marker + 1]) + ONE_OF_ADVANCE(newpath[marker + 1]);
   }
   newpath[1] = ONE_MORE_THAN(newpath[1]);
   if (abs(PATH_STEP_ADVANCE(newpath[marker + 1])) > 1) {
//printf("3 pathNext(%s) ONE_LESS_THAN%s", path, EOL);
      newpath[marker + 1] = ONE_LESS_THAN(newpath[marker + 1]);
      return newpath;
   }
//printf("3 pathNext(%s) removeLast%s", path, EOL);
   return removeLast(marker, newpath);
}
