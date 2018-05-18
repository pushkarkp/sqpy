/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.c
 */

#include "Solve.h"
#include "Solutions.h"
#include "Steps.h"
#include "Repeat.h"
#include "Piece.h"
#include "Position.h"
#include "SetOf.h"
#include "Display.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int useOnce = 0;
TSetOfTopics topics = 0;

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations getSkip(int pc, const TPosition* p, TPlace* sp) {
   TSetOfOrientations skip = 0;
   ERotation rot = 0;
   if (ON_AXIS(p)) {
      rot = spEqualRotate(sp, sp);
      skip |= rotateSkip(rot);
   }
   TSetOfReflectionPlanes sorp = onPlanes(p);
   if (sorp) {
      sorp &= spEqualReflect(sp, sp);
      skip |= reflectSkip(sorp);
   }
   if (skip && SET_HAS(topics, eDisplaySymmetries)) {
      char posBuf[POS_BUF_SIZE];
      char sorpBuf1[SORP_BUF_SIZE];
      char sorpBuf2[SORP_BUF_SIZE];
      char soorBuf[SOOR_BUF_SIZE];
      printf("Symmetries for %c at %s (%s axis)%s", 
             GLYPH(pc), posToString(posBuf, p), ON_AXIS(p) ? "on" : "off", EOL);
      printf("Rotations %s%s",
             rotationToString(rot), EOL);
      printf("Reflections %s: %s%s",
             sorpToString(sorpBuf1, onPlanes(p)),
             sorpToString(sorpBuf2, sorp),
             EOL);
      printf("Skip %s%s", soorToString(soorBuf, skip), EOL);
   }
   return skip;
}

///////////////////////////////////////////////////////////////////////////////
int search(EPresence pc, int* used, const TPosition* pos, const char* steps, TPlace* sp, TSetOfOrientations skip) {
   int solutions = 0;
   TPlace* newsp = SP_NEW(1);
   int path;
   for (path = 0; pieces[pc][path]; ++path) {
      EOrientation or;
      for (or = 0; or < eOrientations; ++or) {
         if (skip) {
//char* step = stepToString(pc, pos, path, or);
//printf("%s: %s %c %s %s%s", steps, step, GLYPH(pc), (SET_HAS(skip, or) ? "skip" : "chek"), orToString(or), EOL);
//free(step);
            if (SET_HAS(skip, or)) {
               continue;
            }
         }
         const TOrient* por = &orients[or];
         if (!repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]]
          && walk(eAbsent, pieces[pc][path], or, pos, sp)) {
            spCopy(newsp, sp);
            SP_SET(newsp, pc, pos);
            walk(pc, pieces[pc][path], or, pos, newsp);
            int* newused = dupPieceInstanceCounts(used);
            ++newused[pc];
            if (pc == 1 && path == 1) {
               char buf[POS_BUF_SIZE];
               printf("no fit %s %s%s", posToString(buf, pos), orToString(or), EOL);
               displayWide(ePyramid, newsp);
            }
            char* newsteps = catStep(steps, stepToString(pc, pos, path, or));
            if (!solIsUniqueSymmetric(sumPieceInstanceCounts(newused), newsteps, newsp)) {
               displayWide(ePyramid, 0);
               printf("symmetric%s", EOL);
               display1(ePyramid, newsp);
               continue;
            }
            displayWide(ePyramid, 0);
            display1(ePyramid, newsp);
            TPosition newpos[eDimensions];
            spFind(newpos, eAbsent, newsp);
            if (newpos[eX] == -1) {
               if (solAddUniqueSymmetric(sumPieceInstanceCounts(newused), newsteps, newsp)) {
                  solutions = 1;
               }
            } else {
               int next_solutions = 0;
               int togo = 0;
               int fork = 0;
               TSetOfOrientations newskip = 0;
               if (skip) {
                  newskip = getSkip(pc, newpos, newsp);
               }
               EPresence next;
               for (next = eFirstPiece; next < pieceCount; ++next) {
                  if (newused[next] < pieceMaxInstances[next]) {
                     ++togo;
                     int s = search(next, newused, newpos, newsteps, newsp, newskip);
                     if (s && next_solutions) {
                        fork = 1;
                     }
                     next_solutions += s;
                  }
               }
               if (fork || (!togo && useOnce)) {
                  if (solAddUniqueSymmetric(sumPieceInstanceCounts(newused), newsteps, newsp)) {
                     next_solutions = !fork;
                  }
               }
               solutions += next_solutions;
            }
            free(newused);
            free(newsteps);
         }
      }
   }
   free(newsp);
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
int pathOrientation(EPresence givenPc, TPath path, TSetOfOrientations soor) {
   EPresence pc = givenPc;
   if (pc == eAbsent) {
      pc = PIECE('x');
   }
   TPlace* sp = SP_NEW(1);
   TPosition pos[eDimensions] = {spHeight / 2, spHeight / 2, spHeight / 2};
   TSetOfOrientations skip90 = rotateSkip(e90);
   TSetOfOrientations skip180 = rotateSkip(e180);
   TSetOfOrientations skip100 = reflectSkip(SET_WITH(0, e100Reflection));
   TSetOfOrientations skip010 = reflectSkip(SET_WITH(0, e010Reflection));
   TSetOfOrientations skip110 = reflectSkip(SET_WITH(0, e110Reflection));
   TSetOfOrientations skip1T0 = reflectSkip(SET_WITH(0, e1T0Reflection));
   EOrientation or;
   for (or = 0; or < eOrientations; ++or) {
      if (soor != 0 && !SET_HAS(soor, or)) {
         continue;
      }
      const char* isSkip = (SET_HAS(skip90, or) || SET_HAS(skip180, or) 
                         || SET_HAS(skip100, or) || SET_HAS(skip010, or) 
                         || SET_HAS(skip110, or) || SET_HAS(skip1T0, or)) ? "" : "no ";
      printf("%s (%s%s%s%s%s%s%sskip)%s", orToString(or), isSkip, 
               SET_HAS(skip90, or) ? "90 " : "",
               SET_HAS(skip180, or) ? "180 " : "",
               SET_HAS(skip100, or) ? "100 " : "",
               SET_HAS(skip010, or) ? "010 " : "",
               SET_HAS(skip110, or) ? "110 " : "",
               SET_HAS(skip1T0, or) ? "1T0 " : "",
               EOL);
      const TOrient* por = &orients[or];
      spInitCube(sp); 
      SP_SET(sp, pc, pos);
      if (!walk(pc, path, or, pos, sp)) {
         free(sp);
         return 0;
      }
      if (givenPc != eAbsent) {
         displayWide(eCube, sp);
      }
   }
   free(sp);
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void setHeight(TPath path, TSetOfOrientations soor) {
   while (!pathOrientation(eAbsent, path, soor)) {
      spSetHeight(spHeight + 1);
   }
}

///////////////////////////////////////////////////////////////////////////////
// path &&
//   pc == eAbsent: path (pc = 'x')
//   pc != eAbsent: path
// !path && pieceCount > eFirstPiece &&
//   pc == eAbsent: everything
//   pc != eAbsent: piece
// !path && pieceCount == eFirstPiece (no pieces): nothing
void testOrientations(EPresence pc, TPath path, TSetOfOrientations soor) {
   if (pc == eAbsent && pieceCount == eFirstPiece) {
      // pc is only used for glyph
      pc = PIECE('x');
   }
   const char glyph[2] = {GLYPH(pc), 0};
   printf("pieces: %d piece: %s path: %s%s", pieceCount - 1, pc?glyph:"all", path?path:"all", EOL);
   printf("Orientation    Symmetry%s", EOL);
   spSetHeight(1);
   if (pc != eAbsent && path) {
      setHeight(path, soor);
      initDisplay(-1);
      pathOrientation(pc, path, soor);
      return;
   }
   for (pc = eFirstPiece; pc < pieceCount; ++pc) {
      int p;
      for (p = 0; pieces[pc][p]; ++p) {
         setHeight(pieces[pc][p], soor);
      }
   }
   initDisplay(-1);
   printf("height: %d%s", spHeight, EOL);
   for (pc = eFirstPiece; pc < pieceCount; ++pc) {
      int p;
      for (p = 0; pieces[pc][p]; ++p) {
         pathOrientation(pc, pieces[pc][p], soor);
      }
   }
   displayWide(eCube, 0);
}

///////////////////////////////////////////////////////////////////////////////
void testPathSymmetry(EPresence pc, int path, EOrientation or) {
   printf("%s%s", orToString(or), EOL);
   TPlace* sp = SP_NEW(1);
   typedef enum {eMiddle, eCorner, eDispositions} EDisposition;
   const TPosition pos[ePlanes][eDispositions][eDimensions] =
      {{{spHeight / 4, spHeight / 4, spHeight / 2}, {0, 0, 0}},
       {{spHeight / 2, spHeight / 2, spHeight - 1}, {0, 0, spHeight - 1}}};
   const EPlane p = (or < 8) ? e001 : e110; 
   int end = pc + 1;
   if (pc == eAbsent) {
      pc = eFirstPiece;
      end = pieceCount;
   }
   for (; pc < end; ++pc) {
      int endp = path + 1;
      if (path == -1) {
         path = 0;
         endp = 100;
      }
      for (; pieces[pc][path] && path < endp; ++path) {
         spInit(sp);
         EDisposition d;
         for (d = eMiddle; d < eDispositions; ++d) {
            if (walk(eAbsent, pieces[pc][path], or, pos[p][d], sp)) {
               SP_SET(sp, pc, pos[p][d]);
               walk(pc, pieces[pc][path], or, pos[p][d], sp);
               char bufPos[POS_BUF_SIZE];
               char bufSorp[SORP_BUF_SIZE];
               printf("%c %s %s rotation %s reflection %s%s", 
                      GLYPH(pc), pieces[pc][path], posToString(bufPos, pos[p][d]),
                      rotationToString(spEqualRotate(sp, sp)), 
                      sorpToString(bufSorp, spEqualReflect(sp, sp)),
                      EOL);
               displayWide(ePyramid, sp);
               break;
            }
         }
         if (d == eDispositions) {
            printf("%c %s does not fit%s", GLYPH(pc), pieces[pc][path], EOL);
         }
      }
      displayWide(ePyramid, NULL);
   }
   free(sp);
}

///////////////////////////////////////////////////////////////////////////////
int solve(EPresence pc, int once, TSetOfTopics sot) {
   useOnce = once;
   topics = sot;
   int solutions = 0;
   TPlace* sp = SP_NEW(1);
   TPosition pos[eDimensions] = {0, 0, 0};
   EPresence end = pc + 1;
   if (pc == eAbsent) {
      pc = eFirstPiece;
      end = pieceCount;
   }
   for (; pc < end; ++pc) {
      printf("%c%s",  GLYPH(pc), EOL);
      solInit(sot);
      TSetOfPresences used = 0;
      spInit(sp);
      int sol = search(pc, pieceMaxInstances, pos, "", sp, getSkip(pc, pos, sp));
      solutions += sol;
      displayWide(ePyramid, 0);
      printf("%c sol %d%s",  GLYPH(pc), sol, EOL);
      int i;
      for (i = eFirstPiece; i < pieceCount; ++i) {
         int count = solCountForCount(i);
         if (count > 0) {
            printf("%d pieces: %d%s", i, count, EOL);
            solDisplayByCount(i, ePyramid);
         }
      }
   }
   free(sp);
   return solutions;
}
