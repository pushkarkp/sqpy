/**
 * Copyright 2018 Pushkar Piggott
 *
 * Piece.c
 */

#include "Piece.h"
#include "PathState.h"
#include "Move.h"
#include "Steps.h"
#include "SquarePyramid.h"
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
int* pieceZeroInstances = 0;
int* pieceMaxInstances = 0;

///////////////////////////////////////////////////////////////////////////////
int* pcDupInstanceCounts(const int* instances) {
   int size = pieceCount * sizeof(int);
   return memcpy((int*)malloc(size), instances, size);
}

///////////////////////////////////////////////////////////////////////////////
int pcSumInstanceCounts(const int* instances) {
   int sum = 0;
   int pc;
   for (pc = eFirstPiece; pc < pieceCount; ++pc) {
      sum += instances[pc];
   }
   return sum;
}

///////////////////////////////////////////////////////////////////////////////
TSet pcInstanceCountSet(const int* instances) {
   TSet set = 0;
   int pc;
   for (pc = eFirstPiece; pc < pieceCount; ++pc) {
      if (instances[pc] > 0) {
         set = SET_WITH(set, pc);
      }
   }
   return set;
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
      pieceZeroInstances = (int*)realloc(pieceZeroInstances, maxPieces * sizeof(int));
      pieceMaxInstances = (int*)realloc(pieceMaxInstances, maxPieces * sizeof(int));
      if (pieceCount == 1) {
         // eAbsent
         ((TPiece*)pieces)[0] = (const TPiece)malloc(sizeof(TPiece));
         ((TPiece*)pieces)[0][0] = 0;
         pieceZeroInstances[0] = 0;
         pieceMaxInstances[0] = 0;
      }
   }
   ++pieceCount;
}

///////////////////////////////////////////////////////////////////////////////
void pcAdd(TPiece pc, int times) {
   int i;
   for (i = 0; pc[i]; ++i) {
      int len = strlen(pc[i]);
      if (len > maxPathLength) {
         maxPathLength = len;
      }
   }
   extendPieces(pcCountPaths(pc));
   ((TPiece*)pieces)[pieceCount - 1] = pc;
   pieceZeroInstances[pieceCount - 1] = 0;
   pieceMaxInstances[pieceCount - 1] = times;
}

///////////////////////////////////////////////////////////////////////////////
TPiece pcCreate(TPath path) {
   if (!pathOkForPiece(path, 0)) {
      return 0;
   }
   const int pathMarkers = pathMarkersCount(path);
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
      int markers = pathMarkersCount(piece[newpaths]);
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
EPresence pcWalk(EPresence pc, int n, TPath path, EOrientation or, const TPosition* p, TPlace* sp) {
   TPosition ps[ePathMarkers][eDimensions] = PATH_MARKER_STORES;
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
            if (IS_TOPIC(eTopicCollisions)) {
               char buf[POS_BUF_SIZE];
               printf("%s %d: %s %c%s", path, i, posToString(buf, pos), GLYPH(ePresence), EOL);
            }
            return ePresence;
         }
         if (pc != eAbsent) {
            SP_SET_PC_N(sp, pc, n, pos);
         }
      }
   }
   return eAbsent;
}

///////////////////////////////////////////////////////////////////////////////
int pcPathOrientation(EPresence pc, TPath path, TSetOfOrientations soor) {
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
      EPresence p = pcWalk(pc, 0, path, or, pos, sp);
      if (IS_TOPIC(eTopicPaths)
       && or >= OR_Z_COUNT
       && displayIsPlane()) {
         displayWidePlane(spHeight / 2, 0);
      }
      if (p != eAbsent) {
         if (IS_TOPIC(eTopicPaths)) {
            if (or < OR_Z_COUNT) {
               displayWidePlane(spHeight / 2, 0);
            } else {
               displayWide(eCube, 0);
            }
            printf("%s: %c%s", path, GLYPH(p), EOL);
            if (or < OR_Z_COUNT) {
               displayWidePlane(spHeight / 2, sp);
            } else {
               displayWide(eCube, sp);
            }
         }
         free(sp);
         return 0;
      }
      if (IS_TOPIC(eTopicPaths)) {
         if (IS_TOPIC(eTopicSteps)) {
            char* str = stepToString(pc, pos, path, or);
            printf("%s%s", str, EOL); 
            free(str);
         }
         if (or < OR_Z_COUNT) {
            displayWidePlane(spHeight / 2, sp);
         } else {
            displayWide(eCube, sp);
         }
      }
   }
   free(sp);
   if (IS_TOPIC(eTopicPaths)) {
      if (displayIsPlane()) {
         displayWidePlane(spHeight / 2, 0);
      } else {
         displayWide(eCube, 0);
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void pcSetHeightForPath(TPath path, TSetOfOrientations soor) {
   displayTopicsNone();
   while (!pcPathOrientation(eAbsent, path, soor)) {
      spSetHeight(spHeight + 1);
   }
   initDisplay(-1);
   displayTopicsRevert();
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
      printf("%c: ", GLYPH(i));
      pcDisplay(pieces[i]);
      printf("%s", EOL);
   }
}

///////////////////////////////////////////////////////////////////////////////
void testOrientations(EPresence pcStart, EPresence pcEnd, TSetOfOrientations soor) {
printf("testOrientations(%c, %c, %s)\r\n", GLYPH(pcStart), GLYPH(pcEnd), setToString(soor, orToString, ""));
   EPresence pc;
   for (pc = pcStart; pc < pcEnd; ++pc) {
      int p;
      for (p = 0; pieces[pc][p]; ++p) {
         pcSetHeightForPath(pieces[pc][p], soor);
      }
   }
   displayTopicsAll();
   for (pc = pcStart; pc < pcEnd; ++pc) {
      int p;
      for (p = 0; pieces[pc][p]; ++p) {
         pcPathOrientation(pc, pieces[pc][p], soor);
      }
   }
   displayTopicsRevert();
}

///////////////////////////////////////////////////////////////////////////////
void pcTestOrientations(EPresence pc, int path, TSetOfOrientations soor) {
   printf("piece: %s path: %s orientations %s%s", 
          pc ? presToString(pc) : "all", 
          path > 0 ? pieces[pc][path] : "all", 
          setToString(soor, orToString, ""), EOL);
   pcSetHeight(1);
   if (path == 0) {
      EPresence pcStart = pc ? pc : eFirstPiece;
      EPresence pcEnd = pc ? pc + 1: pieceCount;
      testOrientations(pcStart, pcEnd, soor);
   }  else {
      if (pc == eAbsent) {
         pc = eFirstPiece;
      }
      pcSetHeightForPath(pieces[pc][path], soor);
      displayTopicsAll();
      pcPathOrientation(pc, pieces[pc][path], soor);
      displayTopicsRevert();
   }
   displayWide(eCube, 0);
}

///////////////////////////////////////////////////////////////////////////////
void pcRemovePath(int pc, int path) {
   free((void*)pieces[pc][path]);
   for (; pieces[pc][path]; ++path) {
      pieces[pc][path] = pieces[pc][path + 1];
   }
}

///////////////////////////////////////////////////////////////////////////////
void pcOnePath(int pc, int path) {
   if (pc >= pieceCount) {
      pc = pieceCount - 1;
   }
   int pathCount = pcCountPaths(pieces[pc]);
   if (path >= pathCount) {
      path = pathCount - 1;
   }
   if (path > 0) {
      free((void*)pieces[pc][0]);
      pieces[pc][0] = pieces[pc][path];
   }
   if (path != 1 && pathCount > 1) {
      free((void*)pieces[pc][1]);
   }
   pieces[pc][1] = 0;
}
