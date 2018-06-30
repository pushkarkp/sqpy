/**
 * Copyright 2018 Pushkar Piggott
 *
 * Piece.c
 */

#include "Piece.h"
#include "PathState.h"
#include "Move.h"
#include "Steps.h"
#include "Display.h"
#include "Topics.h"
#include "SetOf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char const* ERROR_NO_PATH = "no path";
static const char const* ERROR_BAD_CHAR = "bad character '%c' in path \"%s\"";

static int maxPieces = 0;
int maxPathCount = 0;

int maxPathLength = 0;
const TPiece* pieces = 0;
int pieceCount = 1;
int* pieceMaxInstances = 0;

///////////////////////////////////////////////////////////////////////////////
int* pcDupInstanceCounts(int* instances) {
   int size = pieceCount * sizeof(int);
   return memcpy((int*)malloc(size), instances, size);
}

///////////////////////////////////////////////////////////////////////////////
int pcSumInstanceCounts(int* instances) {
   int sum = 0;
   int pc;
   for (pc = eFirstPiece; pc < pieceCount; ++pc) {
      sum += instances[pc];
   }
   return sum;
}

///////////////////////////////////////////////////////////////////////////////
int pcCountPaths(TPiece pc) {
   int i;
   int pathCount = 0;
   for (i = 0; pc[i]; ++i) {
      ++pathCount;
   }
   return pathCount;
}

///////////////////////////////////////////////////////////////////////////////
void extendPieces(int pathCount) {
   if (pathCount > maxPathCount) {
      maxPathCount = pathCount;
   }
   if (pieceCount >= maxPieces) {
      maxPieces = maxPieces ? maxPieces * 2 : 4;
      pieces = (const TPiece*)realloc((TPiece*)pieces, maxPieces * sizeof(TPiece));
      pieceMaxInstances = (int*)realloc(pieceMaxInstances, maxPieces * sizeof(int));
      if (pieceCount == 1) {
         // eAbsent
         ((TPiece*)pieces)[0] = (const TPiece)malloc(sizeof(TPiece));
         ((TPiece*)pieces)[0][0] = 0;
         pieceMaxInstances[0] = 0;
      }
   }
   ++pieceCount;
}

///////////////////////////////////////////////////////////////////////////////
char* pcCheckPath(TPath path) {
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
void pcAdd(TPiece pc, int times) {
   extendPieces(pcCountPaths(pc));
   ((TPiece*)pieces)[pieceCount - 1] = pc;
   pieceMaxInstances[pieceCount - 1] = times;
}

///////////////////////////////////////////////////////////////////////////////
TPiece pcCreate(TPath path) {
   if (!pathOk(path, 0)) {
      return 0;
   }
   const int pathMarkers = pathMarkerCount(path);
   int repeatmarkers = 0;
   int repeatmarkers1 = 0;
//printf("pcCreate(%s) %d markers\r\n", path, pathMarkers);
   int maxpaths = 8;
   int skip = 0;
   char** piece = (char**)malloc(maxpaths * sizeof(char*));
   int newpaths = 0;
   piece[newpaths++] = strdup(path);
   while (!pathRemoveBothEndMarkers(piece[newpaths - 1])) {
      piece[newpaths] = pathNext(piece[newpaths - 1]);
      if (skip) {
         skip = 0;
         --newpaths;
//printf("skip %s\r\n", piece[newpaths]);
         free(piece[newpaths]);
         piece[newpaths] = piece[newpaths + 1];
      }
      if (!piece[newpaths]) {
         return 0;
      }
      int markers = pathMarkerCount(piece[newpaths]);
      if (markers == pathMarkers) {
         if (repeatmarkers == markers) {
//printf("%s -> %s markers (%d) == pathMarkers == repeatmarkers, finished\r\n", piece[newpaths - 1], piece[newpaths], markers);
            if (!markers) {
               // accept a/A/z/Z -> z/Z/a/A
               ++newpaths;
            } else {
               if (strcmp(path, piece[newpaths])) {
                  return 0;
               }
               free(piece[newpaths]);
            }
            break;
         }
         ++repeatmarkers;
      } else if (markers == pathMarkers + 1) {
//printf("%s markers (%d) == pathMarkers + 1 repeatmarkers1 %d\r\n", piece[newpaths], markers, repeatmarkers1);
         if (repeatmarkers1 == markers) {
//printf("%s -> %s markers (%d) == pathMarkers + 1 == repeatmarkers1, finished\r\n", piece[newpaths - 1], piece[newpaths], markers);
            if (strcmp(path, piece[newpaths])) {
//printf("%s fail\r\n", piece[newpaths]);
               return 0;
            }
            free(piece[newpaths]);
            break;
         }
         skip = repeatmarkers1 != 0;
         ++repeatmarkers1;
      }
      ++newpaths;
      if (newpaths > maxpaths) {
         maxpaths *= 2;
         piece = (char**)realloc(piece, maxpaths * sizeof(char*));
      }
   }
   piece[newpaths] = 0;
   return (TPiece)piece;
}

///////////////////////////////////////////////////////////////////////////////
TPiece addPiece(int pathCount, int times) {
   extendPieces(pathCount);
   ((TPiece*)pieces)[pieceCount - 1] = (const TPiece)malloc((pathCount + 1) * sizeof(TPiece));
   pieceMaxInstances[pieceCount - 1] = times;
   return ((TPiece*)pieces)[pieceCount - 1];
}

///////////////////////////////////////////////////////////////////////////////
char* pcRead(int pathCount, const char** argv) {
   int times = strtol(argv[pathCount - 1], 0, 10);
   if (times > 0 || isdigit(argv[pathCount - 1][0])) {
      --pathCount;
   } else {
      times = 1;
   }
   if (pathCount < 1) {
       return strdup(ERROR_NO_PATH);
   }
   TPiece pc = addPiece(pathCount, times);
   int i;
   for (i = 0; i < pathCount; ++i) {
      int len = strlen(argv[i]);
      if (len > maxPathLength) {
         maxPathLength = len;
      }
      pc[i] = (const char*)malloc((len + 1) * sizeof(char));
      strcpy(((char*)pc[i]), argv[i]);
   }
   pc[pathCount] = 0;
   for (i = 0; i < pathCount; ++i) {
      int j;
      for (j = 0; pc[i][j]; ++j) {
         if (pc[i][j] != '.' && pc[i][j] != ','
          && (pc[i][j] < 'a' || pc[i][j] > 'z')
          && (pc[i][j] < 'A' || pc[i][j] > 'Z')) {
            char* err = (char*)malloc((strlen(ERROR_BAD_CHAR) + strlen(pc[i])) * sizeof(char));
            sprintf(err, ERROR_BAD_CHAR, pc[i][j], pc[i]);
            return err;
         }
      }
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
EPresence pcWalk(EPresence pc, TPath path, EOrientation or, const TPosition* p, TPlace* sp) {
   TPosition ps[ePositionStores][eDimensions] = {{-1, -1, -1}, {-1, -1, -1}};
   TPosition pos[eDimensions];
   POS_COPY(pos, p);
   for (; *path; ++path) {
      path = psRead(pos, path, ps);
      const TMove* pmove;
      int count = moveFromChar(&pmove, *path, or);
      int i;
      for (i = 0; i < count; ++i) {
         moveStep(pos, pmove);
         EPresence ePresence = SP_GET(pos, sp);
         if (ePresence != eAbsent) {
            //printf("%s %d: %c%s", path, i, GLYPH(ePresence), EOL);
            return ePresence;
         }
         if (pc != eAbsent) {
            SP_SET(sp, pc, pos);
         }
      }
   }
   return eAbsent;
}

///////////////////////////////////////////////////////////////////////////////
int pcPathOrientation(TSet topics, EPresence pc, TPath path, TSetOfOrientations soor) {
   if (pc == eAbsent) {
      pc = eFirstPiece;
   }
   TPlace* sp = SP_NEW(1);
   TPosition pos[eDimensions] = {spHeight / 2, spHeight / 2, spHeight / 2};
   EOrientation or;
   for (or = 0; or < eOrientations; ++or) {
      if (soor != 0 && !SET_HAS(soor, or)) {
         continue;
      }
      spInitCube(sp); 
      SP_SET(sp, pc, pos);
      EPresence p = pcWalk(pc, path, or, pos, sp);
      if (p != eAbsent) {
         printf("%s: %c%s", path, GLYPH(p), EOL);
         display1Plane(spHeight / 2, sp);
         free(sp);
         return 0;
      }
      if (SET_HAS(topics, eTopicOrientations)
       || SET_HAS(topics, eTopicSteps)) {
         if (SET_HAS(topics, eTopicSteps)) {
            printf("%s ", stepToString(pc, pos, path, or)); 
         }
         if (SET_HAS(topics, eTopicOrientations)) {
            printf("%s", orToString(or)); 
          }
         printf("%s", EOL); 
      }
      if (SET_HAS(topics, eTopicPaths)) {
         display1Plane(spHeight / 2, sp);
         printf("%s", EOL);
      }
   }
   free(sp);
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void pcSetHeightForPath(TPath path, TSetOfOrientations soor) {
   while (!pcPathOrientation(0, eAbsent, path, soor)) {
      spSetHeight(spHeight + 1);
   }
}

///////////////////////////////////////////////////////////////////////////////
void pcSetHeight() {
  spSetHeight(1);
  int pc;
  for (pc = eFirstPiece; pc < pieceCount; ++pc) {
      int p;
      for (p = 0; pieces[pc][p]; ++p) {
         pcSetHeightForPath(pieces[pc][p], 0);
      }
   }
   initDisplay(-1);
}

///////////////////////////////////////////////////////////////////////////////
void pcDisplay(TPiece p) {
   int i;
   for (i = 0; p[i]; ++i) {
      printf("%s ", p[i]);
   }
}

///////////////////////////////////////////////////////////////////////////////
void pcDisplayAll() {
   int i;
   // skip 0, the empty path
   for (i = 1; i < pieceCount; ++i) {
      pcDisplay(pieces[i]);
      printf("%s", EOL);
   }
}

///////////////////////////////////////////////////////////////////////////////
void testOrientations(EPresence pcStart, EPresence pcEnd, TSetOfOrientations soor) {
   EPresence pc;
   for (pc = pcStart; pc < pcEnd; ++pc) {
      int p;
      for (p = 0; pieces[pc][p]; ++p) {
         pcSetHeightForPath(pieces[pc][p], soor);
      }
   }
   initDisplay(-1);
   for (pc = pcStart; pc < pcEnd; ++pc) {
      int p;
      for (p = 0; pieces[pc][p]; ++p) {
         pcPathOrientation(1, pc, pieces[pc][p], soor);
      }
   }
   displayWide(eCube, 0);
}

///////////////////////////////////////////////////////////////////////////////
void pcTestOrientations(EPresence pc, int path, TSetOfOrientations soor) {
   printf("piece: %s path: %s%s", pc ? presToString(pc) : "all", path > -1 ? pieces[pc][path] : "all", EOL);
   pcSetHeight(1);
   if (path == -1) {
      EPresence pcStart = pc ? pc : eFirstPiece;
      EPresence pcEnd = pc ? pc + 1: pieceCount;
      testOrientations(pcStart, pcEnd, soor);
   }  else {
      if (pc == eAbsent) {
         pc = eFirstPiece;
      }
      pcSetHeightForPath(pieces[pc][path], soor);
      initDisplay(-1);
      pcPathOrientation(1, pc, pieces[pc][path], soor);
   }
   displayWide(eCube, 0);
}
