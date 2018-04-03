/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solutions.c
 */

#include "Solutions.h"
#include "Steps.h"
#include "Presence.h"
#include "SetOf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
#define PAGE_WIDTH 72
#define INITIAL_KEYS 16
#define INITIAL_SPS 4
#define GROWTH_FACTOR 2

static int max_keys = 0;
static int* keys = 0;
static int* max_sps = 0;
static int* sps_count = 0;
static char*** steps = 0;
static TSquarePyramid** sps = 0;

///////////////////////////////////////////////////////////////////////////////
void zeroAllFrom(int i) {
   if (!keys) {
      return;
   }
   for (; i < max_keys; ++i) {
      keys[i] = 0;
      max_sps[i] = 0;
      sps_count[i] = 0;
      steps[i] = 0;
      sps[i] = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
int findKey(int key) {
   int k;
   for (k = 0; keys[k]; ++k) {
       if (keys[k] == key) {
          return k;
       }
   }
   return k;
}

///////////////////////////////////////////////////////////////////////////////
void addKey(int key, int k) {
   if (k == max_keys - 1) {
      max_keys *= GROWTH_FACTOR;
//printf("addKey(key %d k %d) max_keys %d%s", key, k, max_keys, EOL);
      keys = (int*)realloc(keys, max_keys * sizeof(int));
      max_sps = (int*)realloc(max_sps, max_keys * sizeof(int));
      sps_count = (int*)realloc(sps_count, max_keys * sizeof(int));
      steps = (char***)realloc(steps, max_keys * sizeof(char**));
      sps = (TSquarePyramid**)realloc(sps, max_keys * sizeof(TSquarePyramid*));
      zeroAllFrom(k);
   }
   if (!keys[k]) {
//printf("addKey(key %d k %d) keys[k] %d%s", key, k, keys[k], EOL);
      keys[k] = key;
      max_sps[k] = INITIAL_SPS;
      sps_count[k] = 0;
      steps[k] = (char**)malloc(max_sps[k] * sizeof(char*));
      sps[k] = (TSquarePyramid*)malloc(max_sps[k] * SP_SIZE * SP_SIZE * SP_SIZE * sizeof(char));
   }
}

///////////////////////////////////////////////////////////////////////////////
int findSp(int k, TSquarePyramid sp) {
   int i;
   for (i = sps_count[k] - 1; i >= 0; --i) {
      if (spEqual(sp, sps[k][i])) {
         return i;
      }
   }
   return sps_count[k];
}

///////////////////////////////////////////////////////////////////////////////
int findSymmetricSp(ERotation* prot, TSetOfReflectionPlanes* psorp, int k, TSquarePyramid sp) {
   int i;
   for (i = sps_count[k] - 1; i >= 0; --i) {
      ERotation rot = spEqualRotate(sp, sps[k][i]);
      TSetOfReflectionPlanes sorp = spEqualReflect(sp, sps[k][i]);
      if (rot || sorp) {
         if (prot) {
            *prot = rot;
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
void solInit() {
   max_keys = INITIAL_KEYS;
   if (keys) {
      free(keys);
      free(max_sps);
      free(sps_count);
      int i;
      for (i = 0; sps[i]; ++i) {
         free(steps[i]);
         free(sps[i]);
      }
      free(steps);
      free(sps);
   }
   keys = (int*)malloc(max_keys * sizeof(int));
   max_sps = (int*)malloc(max_keys * sizeof(int));
   sps_count = (int*)malloc(max_keys * sizeof(int));
   steps = (char***)malloc(max_keys * sizeof(char**));
   sps = (TSquarePyramid**)malloc(max_keys * sizeof(TSquarePyramid*));
   zeroAllFrom(0);
}

///////////////////////////////////////////////////////////////////////////////
void solShowKeys(int k) {
   char buf[SOP_BUF_SIZE];
   if (k == -1) {
      for (k = 0; keys[k]; ++k) {
         printf("%s %d%s", sopToString(buf, keys[k]), sps_count[k], EOL);
      }
   } else {
      printf("%s %d%s", sopToString(buf, keys[k]), sps_count[k], EOL);
   }
}

///////////////////////////////////////////////////////////////////////////////
int solGetCount(int key) {
   return sps_count[findKey(key)];
}

///////////////////////////////////////////////////////////////////////////////
PSquarePyramid solGet(int key, int i) {
   int k = findKey(key);
   if (!keys[k] || i < 0 || i >= sps_count[k]) {
      return 0;
   }
   return sps[k][i];
}

///////////////////////////////////////////////////////////////////////////////
int solIsUnique(int key, TSquarePyramid sp) {
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
int findUniqueSymmetric(int key, const char* spsteps, TSquarePyramid sp) {
   int k = findKey(key);
   if (!keys[k]) {
      return k;
   }
   int s = findSp(k, sp);
   if (s < sps_count[k]) {
printf("findUniqueSymmetric() duplicate: k %d s %d %s; %s%s", k, s, steps[k][s], spsteps, EOL);
      return -1;
   }
   ERotation rot = 0;
   TSetOfReflectionPlanes sorp = 0;
   s = findSymmetricSp(&rot, &sorp, k, sp);
   if (s < sps_count[k]) {
      catSteps(&steps[k][s], rot, sorp, spsteps);
printf("findUniqueSymmetric() symmetric: k %d s %d %s%s", k, s, steps[k][s], EOL);
      return -1;
   }
   return k;
}

///////////////////////////////////////////////////////////////////////////////
int solIsUniqueSymmetric(int key, const char* spsteps, TSquarePyramid sp) {
   return findUniqueSymmetric(key, spsteps, sp) != -1;
}

///////////////////////////////////////////////////////////////////////////////
int solAddUniqueSymmetric(int key, const char* spsteps, TSquarePyramid sp) {
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
      sps[k] = (TSquarePyramid*)realloc(sps[k], max_sps[k] * SP_SIZE * SP_SIZE * SP_SIZE * sizeof(char));
   }
printf("solAddUniqueSymmetric add: k %d max_keys %d s %d max_sps[k] %d %s%s", 
       k, max_keys, s, max_sps[k], spsteps, EOL);
   steps[k][s] = stepsCopy(spsteps);
   spCopy(sps[k][s], sp);
   ++sps_count[k];
//displayWide(eUpright, PAGE_WIDTH, sp);
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void displayKey(int key, EDisplayShape eShape, int pageWidth) {
   int k = findKey(key);
   int i;
   for (i = 0; i < sps_count[k]; ++i) {
      printf("%s%s", steps[k][i], EOL);
      displayWide(eShape, pageWidth, sps[k][i]);
   }
}

///////////////////////////////////////////////////////////////////////////////
void solDisplay(int key, EDisplayShape eShape, int pageWidth) {
   displayKey(key, eShape, pageWidth);
   displayWide(eShape, pageWidth, 0);
}

///////////////////////////////////////////////////////////////////////////////
void solDisplayByCount(int count, EDisplayShape eShape, int pageWidth) {
   int k;
   for (k = 0; keys[k]; ++k) {
      if (setCount(keys[k]) == count) {
         displayKey(keys[k], eShape, pageWidth);
      }
   }
   displayWide(eShape, pageWidth, 0);
}
