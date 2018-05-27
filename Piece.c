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
TPiece addPiece(int pathCount, int times) {
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
   if (pathCount > maxPathCount) {
      maxPathCount = pathCount;
   }
   ((TPiece*)pieces)[pieceCount] = (const TPiece)malloc((pathCount + 1) * sizeof(TPiece));
   pieceMaxInstances[pieceCount] = times;
   ++pieceCount;
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
int pcWalk(EPresence pc, TPath path, EOrientation or, const TPosition* p, TPlace* sp) {
   TPosition ps[ePositionStores][eDimensions] = {{-1, -1, -1}, {-1, -1, -1}};
   TPosition pos[eDimensions];
   POS_COPY(pos, p);
   for (; *path; ++path) {
      path = psRead(pos, path, ps);
      const TMove* pmove;
      int count = charToMove(&pmove, *path, or);
      int i;
      for (i = 0; i < count; ++i) {
         step(pos, pmove);
         if (SP_GET(pos, sp) != eAbsent) {
            return 0;
         }
         if (pc != eAbsent) {
            SP_SET(sp, pc, pos);
         }
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int pathOrientation(int display, EPresence pc, TPath path, TSetOfOrientations soor) {
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
      if (!pcWalk(pc, path, or, pos, sp)) {
         free(sp);
         return 0;
      }
      if (display) {
         printf("%s %s%s", stepToString(pc, pos, path, or), orToString(or), EOL); 
         displayWide(eCube, sp);
      }
   }
   free(sp);
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void pcSetHeightForPath(TPath path, TSetOfOrientations soor) {
   while (!pathOrientation(0, eAbsent, path, soor)) {
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
         pathOrientation(1, pc, pieces[pc][p], soor);
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
      pathOrientation(1, pc, pieces[pc][path], soor);
   }
   displayWide(eCube, 0);
}

