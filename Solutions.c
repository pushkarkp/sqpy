/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solutions.c
 */

#include "Solutions.h"
#include "Piece.h"
#include "Steps.h"
#include "Label.h"
#include "Symmetry.h"
#include "Topics.h"
#include "SetOf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
#define INITIAL_KEYS 16
#define INITIAL_SPS 4
#define GROWTH_FACTOR 2

///////////////////////////////////////////////////////////////////////////////
static int fixz = 0;
static int max_keys = 0;
static int* keys = 0;
static int* max_sps = 0;
static int* sps_count = 0;
static char*** steps = 0;
static TPlace*** sps = 0;
static int* solutions = 0;

///////////////////////////////////////////////////////////////////////////////
void zeroAllFrom(int k) {
   if (!keys) {
      return;
   }
   for (; k < max_keys; ++k) {
      keys[k] = 0;
      max_sps[k] = 0;
      sps_count[k] = 0;
      steps[k] = 0;
      sps[k] = 0;
      solutions[k] = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
void solSetComplete(int key) {
   int k;
   for (k = 0; keys[k]; ++k) {
       if (LABEL_NO_FULL(keys[k]) == LABEL_NO_FULL(key)) {
          keys[k] = LABEL_SET_FULL(keys[k]);
          return;
       }
   }
}

///////////////////////////////////////////////////////////////////////////////
int findKey(int key) {
   int k;
   for (k = 0; keys[k]; ++k) {
      if (LABEL_NO_FULL(keys[k]) == LABEL_NO_FULL(key)) {
         return k;
      }
   }
   return k;
}

///////////////////////////////////////////////////////////////////////////////
void addKey(int key, int k) {
   if (k == max_keys - 1) {
      max_keys *= GROWTH_FACTOR;
      keys = (int*)realloc(keys, max_keys * sizeof(int));
      max_sps = (int*)realloc(max_sps, max_keys * sizeof(int));
      sps_count = (int*)realloc(sps_count, max_keys * sizeof(int));
      steps = (char***)realloc(steps, max_keys * sizeof(char**));
      sps = (TPlace***)realloc(sps, max_keys * sizeof(TPlace**));
      solutions = (int*)realloc(solutions, max_keys * sizeof(int));
      zeroAllFrom(k);
   }
   if (!keys[k]) {
      keys[k] = key;
      max_sps[k] = INITIAL_SPS;
      sps_count[k] = 0;
      steps[k] = (char**)malloc(max_sps[k] * sizeof(char*));
      sps[k] = (TPlace**)malloc(max_sps[k] * spXYZ2 * sizeof(TPlace));
      solutions[k] = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
int findSp(int k, const TPlace* sp) {
   int i;
   for (i = sps_count[k] - 1; i >= 0; --i) {
      if (spEqual(sp, sps[k][i])) {
         return i;
      }
   }
   return sps_count[k];
}

///////////////////////////////////////////////////////////////////////////////
int findSymmetricSp(TSetOfRotations* psorn, TSetOfReflectionPlanes* psorp, int k, const TPlace* sp) {
   int i;
   for (i = sps_count[k] - 1; i >= 0; --i) {
      TSetOfRotations sorn = spEqualRotate(sps[k][i], sp, fixz);
      TSetOfReflectionPlanes sorp = spEqualReflect(sps[k][i], sp, fixz);
      if (sorn || sorp) {
         if (psorn) {
            *psorn = sorn;
         } 
         if (psorp) {
            *psorp = sorp;
         } 
         return i;
      }
   }
   return sps_count[k];
}

///////////////////////////////////////////////////////////////////////////////
void solInit(int fz) {
   fixz = fz;
   max_keys = INITIAL_KEYS;
   if (keys) {
      int k;
      for (k = 0; sps[k]; ++k) {
         int i;
         for (i = 0; i < sps_count[k]; ++i) {
            free(steps[k][i]);
            free(sps[k][i]);
         }
         free(steps[k]);
         free(sps[k]);
      }
      free(steps);
      free(sps);
      free(keys);
      free(max_sps);
      free(sps_count);
      free(solutions);
   }
   keys = (int*)malloc(max_keys * sizeof(int));
   max_sps = (int*)malloc(max_keys * sizeof(int));
   sps_count = (int*)malloc(max_keys * sizeof(int));
   steps = (char***)malloc(max_keys * sizeof(char**));
   sps = (TPlace***)malloc(max_keys * sizeof(TPlace**));
   solutions = (int*)malloc(max_keys * sizeof(int));
   zeroAllFrom(0);
}

///////////////////////////////////////////////////////////////////////////////
void solShowSetKeys(int k, FSetElementToString toString) {
   if (k == -1) {
      for (k = 0; keys[k]; ++k) {
         char* strsop = setToString(keys[k], toString, "");
         printf("%s %d%s", strsop, sps_count[k], EOL);
         free(strsop);
      }
   } else {
      char* strsop = setToString(keys[k], toString, "");
      printf("%s %d%s", strsop, sps_count[k], EOL);
      free(strsop);
   }
}

///////////////////////////////////////////////////////////////////////////////
int solGetCount(int key) {
   return sps_count[findKey(key)];
}

///////////////////////////////////////////////////////////////////////////////
const TPlace* solGet(int key, int i) {
   int k = findKey(key);
   if (!keys[k] || i < 0 || i >= sps_count[k]) {
      return 0;
   }
   return sps[k][i];
}

///////////////////////////////////////////////////////////////////////////////
int solIsUnique(int key, const const TPlace* sp) {
   int k = findKey(key);
   if (!keys[k]) {
      return 1;
   }
   int i;
   for (i = 0; i < sps_count[k]; ++i) {
      if (spEqual(sp, sps[k][i])) {
         return 0;
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int findUniqueSymmetric(int key, const char* spsteps, const TPlace* sp) {
   int k = findKey(key);
   ++solutions[k];
   if (!keys[k]) {
      return k;
   }
   int s = findSp(k, sp);
   if (s < sps_count[k]) {
      displayWide(ePyramid, 0);
      if (IS_TOPIC(eTopicAdd)) {
         printf("Duplicate: k %d s %d %s; %s%s", 
                k, s, steps[k][s], spsteps, EOL);
         display1(ePyramid, sp);
      }
      return -1;
   }
   TSetOfRotations sorn = 0;
   TSetOfReflectionPlanes sorp = 0;
   s = findSymmetricSp(&sorn, &sorp, k, sp);
   if (s < sps_count[k]) {
      const char* strsornhead = "";
      char* strsorn = strdup("");
      if (sorn) {
         strsornhead = " rotation ";
         strsorn = setToString(sorn, rotationToString, "");
      }
      const char* strsorphead = strdup("");
      char* strsorp = setToChars(sorp, reflectionPlaneToChar, "");
      if (sorp) {
         strsorphead = " reflection ";
      }
      catSteps(&steps[k][s], strsorn, strsorp, spsteps);
      if (IS_TOPIC(eTopicAdd)
       && IS_TOPIC(eTopicSymmetries)) {
         displayWide(ePyramid, 0);
         printf("Symmetric with existing (on right) (key 0x%x k %d s %d): ",
                key, k, s);
         if (IS_TOPIC(eTopicSteps)) {
            printf("%s%s", steps[k][s], EOL);
         } else {
            printf("%s%s%s%s%s", strsornhead, strsorn, strsorphead, strsorp, EOL);
         }
         displayWide(ePyramid, sp);
         displayWide(ePyramid, sps[k][s]);
         displayWide(ePyramid, 0);
      }
      free(strsorn);
      free(strsorp);
      return -1;
   }
   return k;
}

///////////////////////////////////////////////////////////////////////////////
int solIsUniqueSymmetric(int key, const char* spsteps, const TPlace* sp) {
   return findUniqueSymmetric(key, spsteps, sp) != -1;
}

///////////////////////////////////////////////////////////////////////////////
int solAddUniqueSymmetric(int key, const char* spsteps, const const TPlace* sp) {
   int k = findUniqueSymmetric(key, spsteps, sp);
   if (k == -1) {
      return 0;
   }
   if (!keys[k]) {
      addKey(key, k);
   }
   int s = sps_count[k];
   if (s == max_sps[k]) {
      max_sps[k] *= GROWTH_FACTOR;
      steps[k] = (char**)realloc(steps[k], max_sps[k] * sizeof(char*));
      sps[k] = (TPlace**)realloc(sps[k], max_sps[k] * spXYZ2 * sizeof(TPlace));
   }
   steps[k][s] = stepsCopy(spsteps);
   sps[k][s] = SP_NEW(1);
   spCopy(sps[k][s], sp);
   ++sps_count[k];
   if (IS_TOPIC(eTopicAdd)
    && !IS_TOPIC(eTopicProgress)) {
      printf("Add: %s%s", IS_TOPIC(eTopicSteps)?spsteps:"", EOL);
      displayWide(ePyramid, sps[k][s]);
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void displayKey(int key, EDisplayShape eShape) {
   int k = findKey(key);
   int i;
   for (i = 0; i < sps_count[k]; ++i) {
      if (IS_TOPIC(eTopicSteps)) {
         printf("%s%s", steps[k][i], EOL);
      }
      displayWide(eShape, sps[k][i]);
   }
}

///////////////////////////////////////////////////////////////////////////////
int solMaxPlayCount() {
   int max = 0;
   int k;
   for (k = 0; keys[k]; ++k) {
      int n = LABEL_GET_PLAY_COUNT(keys[k]);
      if (n > max) {
         max = n;
      }
   }
   return max;
}

///////////////////////////////////////////////////////////////////////////////
int solMaxShapeCount() {
   int max = 0;
   int k;
   for (k = 0; keys[k]; ++k) {
      int n = setCount(LABEL_GET_SHAPES(keys[k]));
      if (n > max) {
         max = n;
      }
   }
   return max;
}

///////////////////////////////////////////////////////////////////////////////
int solTotalSolutionsForPlayCount(int n, int complete) {
   int found = 0;
   int k;
   for (k = 0; keys[k]; ++k) {
     if (LABEL_GET_PLAY_COUNT(keys[k]) == n
       && (!complete || LABEL_IS_FULL(keys[k]))) {
         found += solutions[k];
      }
   }
   return found;
}

///////////////////////////////////////////////////////////////////////////////
int solCountForPlayCount(int n, int complete) {
   int found = 0;
   int k;
   for (k = 0; keys[k]; ++k) {
     if (LABEL_GET_PLAY_COUNT(keys[k]) == n
       && (!complete || LABEL_IS_FULL(keys[k]))) {
         found += sps_count[k];
      }
   }
   return found;
}

///////////////////////////////////////////////////////////////////////////////
int solTotalSolutionsForShapes(TSet s) {
   int found = 0;
   int k;
   for (k = 0; keys[k]; ++k) {
      if (LABEL_GET_SHAPES(keys[k]) == s) {
         found += solutions[k];
      }
   }
   return found;
}

///////////////////////////////////////////////////////////////////////////////
int solCountForShapes(TSet s) {
   int found = 0;
   int k;
   for (k = 0; keys[k]; ++k) {
      if (LABEL_GET_SHAPES(keys[k]) == s) {
         found += sps_count[k];
      }
   }
   return found;
}

///////////////////////////////////////////////////////////////////////////////
void solDisplay(int key, EDisplayShape eShape) {
   displayKey(key, eShape);
   displayWide(eShape, 0);
}

///////////////////////////////////////////////////////////////////////////////
void solDisplayByPlayCount(int n, int complete, EDisplayShape eShape) {
   int k;
   for (k = 0; keys[k]; ++k) {
      if (LABEL_GET_PLAY_COUNT(keys[k]) == n
       && (!complete || LABEL_IS_FULL(keys[k]))) {
         displayKey(keys[k], eShape);
      }
   }
   displayWide(eShape, 0);
}

///////////////////////////////////////////////////////////////////////////////
void solDisplayByShapes(TSet s, EDisplayShape eShape) {
   int k;
   for (k = 0; keys[k]; ++k) {
      if (LABEL_GET_SHAPES(keys[k]) == s) {
         displayKey(keys[k], eShape);
      }
   }
   displayWide(eShape, 0);
}
