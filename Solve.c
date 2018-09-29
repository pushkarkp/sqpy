/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solve.c
 */

#include "Solve.h"
#include "Solutions.h"
#include "Repeat.h"
#include "Piece.h"
#include "Position.h"
#include "Steps.h"
#include "Label.h"
#include "SetOf.h"
#include "Topics.h"
#include "Display.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int fill = 0;

///////////////////////////////////////////////////////////////////////////////
int countUse(const int* use) {
   int count = 0;
   int i;
   for (i = 0; i < pieceCount; ++i) {
      count += use[i];
   }
   return count;
}

///////////////////////////////////////////////////////////////////////////////
void showSymmetries(const TPosition* pos, const TPlace* sp, 
                    TSetOfRotations sorn, TSetOfReflectionPlanes sorp) {
   if (IS_TOPIC(eTopicProgress)) {
      // eTopicProgress already displayed sp
      displayWide(ePyramid, 0);
   } else {
      display1(ePyramid, sp);
   }
   char posBuf[POS_BUF_SIZE];
   printf("Symmetries for last above pyramid, from %s (%s axis, ",
          posToString(posBuf, pos), ON_AXIS(pos) ? "on" : "off");
   int orp = onReflectionPlanes(pos);
   if (orp == 0) {
      printf("off planes)%s", EOL);
   } else {
      char* strPlanes = setToString(orp, reflectionPlaneToString, "");
      printf("on %s plane)%s", strPlanes, EOL);
      free(strPlanes);
   }
   char* strsorn = setToString(sorn, rotationToString, "none");
   printf(" rotations: %s%s", strsorn, EOL);
   free(strsorn);
   char* strsorp = setToChars(sorp, reflectionPlaneToChar, "none");
   printf(" reflections: %s%s", strsorp, EOL); 
   free(strsorp);
}

///////////////////////////////////////////////////////////////////////////////
void showSkip(EPresence pc, int path, TSetOfOrientations skip) {
   char* chsk = 0; 
   char* strsoor = 0;
   if (setCount(skip) > eOrientations / 2) {
      chsk = "Check";
      strsoor = setToString(SET_NOT(skip, eOrientations), orToString, "");
   } else {
      chsk = "Skip";
      strsoor = setToString(skip, orToString, "");
   }
   printf("%s orientations for path %c[%d] %s: %s%s",
          chsk, GLYPH(pc), path, pieces[pc][path], strsoor, EOL);
   free(strsoor);
}

///////////////////////////////////////////////////////////////////////////////
int search(EPresence pc, const  int* used, const TPosition* pos, const char* steps, const TPlace* sp, 
           const TSetOfRotations sorn, const TSetOfReflectionPlanes sorp) {
   int solutions = 0;
   TPlace* newsp = SP_NEW(1);
   int path;
   for (path = 0; pieces[pc][path]; ++path) {
      TSetOfOrientations skip = 0;
      if (sorn) {
         skip |= pathRotateSkip(pc, path, sorn) | spaceRotateSkip(sorn);
      }
      if (sorp) {
         skip |= pathReflectSkip(pc, path, sorp) | spaceReflectSkip(sorp);
      }
      if (skip && IS_TOPIC(eTopicSymmetries)) {
         showSkip(pc, path, skip);
      }
      EOrientation or;
      for (or = 0; or < eOrientations; ++or) {
         if (skip && SET_HAS(skip, or)) {
            if (IS_TOPIC(eTopicProgress)) {
               char* newsteps = catStep(steps, stepToString(pc, pos, pieces[pc][path], or));
               printf("Skip symmetry duplicate: %s%s", newsteps, EOL);
               free(newsteps);
            }
            continue;
         }
         if (SET_HAS(repeat[pc][path], or)) {
            if (IS_TOPIC(eTopicRepeat) && IS_TOPIC(eTopicProgress)) {
               char* newsteps = catStep(steps, stepToString(pc, pos, pieces[pc][path], or));
               printf("Skip path repeat: %s%s", newsteps, EOL);
               free(newsteps);
            }
            continue;
         }
         EPresence hit = pcWalk(eAbsent, 0, pieces[pc][path], or, pos, (TPlace*)sp);
         if (hit != eAbsent) {
            if (IS_TOPIC(eTopicProgress)) {
               char* newsteps = catStep(steps, stepToString(pc, pos, pieces[pc][path], or));
               printf("Hit '%c' %s%s", GLYPH(hit), newsteps, EOL);
               free(newsteps);
            }
         } else {
            spCopy(newsp, sp);
            SP_SET_PC_N(newsp, pc, used[pc], pos);
            pcWalk(pc, used[pc], pieces[pc][path], or, pos, newsp);
            TLabel label = LABEL_MAKE(0, 
                                      pcSumInstanceCounts(used) + 1, 
                                      SET_WITH(pcInstanceCountSet(used), pc));
            char* newsteps = catStep(steps, stepToString(pc, pos, pieces[pc][path], or));
            if (!solIsUniqueSymmetric(label, newsteps, newsp)) {
               if (IS_TOPIC(eTopicProgress)) {
                  printf("Skip duplicate solution: %s%s", newsteps, EOL);
               }
               free(newsteps);
               continue;
            }
            if (IS_TOPIC(eTopicProgress)) {
               if (IS_TOPIC(eTopicSteps)) {
                  printf("%s%s", newsteps, EOL);
               }
               displayWide(ePyramid, newsp);
            }
            TSetOfReflectionPlanes newsorp = 0;
            if (sorp) {
               newsorp = spEqualReflect(newsp, newsp, 0);
            }
            TPosition newpos[eDimensions];
            spFind(newpos, eAbsent, newsp, newsorp);
            if (newpos[eX] == -1) {
               if (solAddUniqueSymmetric(label, newsteps, newsp)) {
                  solSetComplete(label);
                  solutions = 1;
               }
            } else {
               if (newsorp) {
                  TSetOfReflectionPlanes posPlanes = onReflectionPlanes(newpos);
/*char* strsp = setToString(sorp, reflectionPlaneToString, "");
char* strpos = setToString(posPlanes, reflectionPlaneToString, "");
char* strsorp = setToString(sorp & posPlanes, reflectionPlaneToString, "");
printf("sp %s pos %s sorp %s\r\n", strsp, strpos, strsorp);
free(strsp);
free(strpos);
free(strsorp);*/
                  newsorp &= posPlanes;
               }
               TSetOfRotations newsorn = 0;
               if (sorn && ON_AXIS(newpos)) {
                  newsorn = spEqualRotate(newsp, newsp, 0);
/*
char* strsorn = setToString(newsorn, rotationToString, "");
printf("sorn %s\r\n", strsorn);
free(strsorn);
*/
               }
               if (IS_TOPIC(eTopicSymmetries)
                && (newsorn || newsorp || sorn || sorp)) {
                  showSymmetries(newpos, newsp, newsorn, newsorp);
               }
               int* newused = pcDupInstanceCounts(used);
               ++newused[pc];
               int next_solutions = 0;
               int togo = 0;
               int fork = 0;
               EPresence next;
               for (next = eFirstPiece; next < pieceCount; ++next) {
                  if (fill || newused[next] < pieceMaxInstances[next]) {
                     ++togo;
                     int s = search(next, newused, newpos, newsteps, newsp, newsorn, newsorp);
                     if (s && next_solutions) {
                        fork = 1;
                     }
                     next_solutions += s;
                  }
               }
               if (fork || togo == 0) {
                  if (solAddUniqueSymmetric(label, newsteps, newsp)
                   && !fork) {
                     solSetComplete(label);
                     next_solutions = 1;
                  }
               }
               solutions += next_solutions;
               free(newused);
            }
            free(newsteps);
         }
      }
   }
   free(newsp);
   return solutions;
}

///////////////////////////////////////////////////////////////////////////////
int solve(EPresence pc, int f, int byz,
          TSetOfRotations sorn, TSetOfReflectionPlanes sorp) {
   fill = f;
   int solutions = 0;
   TPlace* sp = SP_NEW(1);
   TPosition pos[eDimensions] = {0, 0, 0};
   EPresence end = pc + 1;
   if (pc == eAbsent) {
      pc = eFirstPiece;
      end = pieceCount;
   }
   for (; pc < end; ++pc) {
      solInit(byz);
      TSet used = 0;
      spInit(sp);
//showSymmetries(pos, sp, sorn, sorp);
      int sol = search(pc, pieceZeroInstances, pos, "", sp, sorn, sorp);
      solutions += sol;
      displayWide(ePyramid, 0);
      int max = solMaxPlayCount();
      printf("%c: %d solutions%s", GLYPH(pc), sol, EOL);
      int n;
      for (n = 1; n <= max; ++n) {
         int count = solCountForPlayCount(n, !IS_TOPIC(eTopicForks));
         if (count > 0) {
            printf("%d with %d plays%s", count, n, EOL);
            solDisplayByPlayCount(n, !IS_TOPIC(eTopicForks), ePyramid);
         }
      }
   }
   free(sp);
   return solutions;
}
