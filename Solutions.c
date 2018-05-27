/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solutions.c
 */

#include "Solutions.h"
#include "Piece.h"
#include "Steps.h"
#include "SetOf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
#define INITIAL_KEYS 16
#define INITIAL_SPS 4
#define GROWTH_FACTOR 2

///////////////////////////////////////////////////////////////////////////////
static TSet topics = 0;
static int max_keys = 0;
static int* keys = 0;
static int* max_sps = 0;
static int* sps_count = 0;
static char*** steps = 0;
static TPlace*** sps = 0;

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
      sps = (TPlace***)realloc(sps, max_keys * sizeof(TPlace**));
      zeroAllFrom(k);
   }
   if (!keys[k]) {
//printf("addKey(key %d (0x%x) k %d) keys[k] %d%s", key, key, k, keys[k], EOL);
      keys[k] = key;
      max_sps[k] = INITIAL_SPS;
      sps_count[k] = 0;
      steps[k] = (char**)malloc(max_sps[k] * sizeof(char*));
      sps[k] = (TPlace**)malloc(max_sps[k] * spXYZ * sizeof(TPlace));
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
int findSymmetricSp(ERotation* prot, TSetOfReflectionPlanes* psorp, int k, const TPlace* sp) {
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
void solInit(TSet sot) {
   topics = sot;
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
   }
   keys = (int*)malloc(max_keys * sizeof(int));
   max_sps = (int*)malloc(max_keys * sizeof(int));
   sps_count = (int*)malloc(max_keys * sizeof(int));
   steps = (char***)malloc(max_keys * sizeof(char**));
   sps = (TPlace***)malloc(max_keys * sizeof(TPlace**));
   zeroAllFrom(0);
}

///////////////////////////////////////////////////////////////////////////////
void solShowSetKeys(int k, FSetElementToString toString) {
   if (k == -1) {
      for (k = 0; keys[k]; ++k) {
         char* strsop = setToString(keys[k], toString);
         printf("%s %d%s", strsop, sps_count[k], EOL);
         free(strsop);
      }
   } else {
      char* strsop = setToString(keys[k], toString);
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
int findUniqueSymmetric(int key, const char* spsteps, const const TPlace* sp) {
   int k = findKey(key);
   if (!keys[k]) {
      return k;
   }
   int s = findSp(k, sp);
   if (s < sps_count[k]) {
      displayWide(ePyramid, 0);
      printf("findUniqueSymmetric() found duplicate: k %d s %d %s; %s%s", 
             k, s, steps[k][s], spsteps, EOL);
      display1(ePyramid, sp);
      return -1;
   }
   ERotation rot = 0;
   TSetOfReflectionPlanes sorp = 0;
   s = findSymmetricSp(&rot, &sorp, k, sp);
   if (s < sps_count[k]) {
      catSteps(&steps[k][s], rot, sorp, spsteps);
      if (SET_HAS(topics, eDisplaySymmetries)) {
         displayWide(ePyramid, 0);
         const char* strsteps =
            SET_HAS(topics, eDisplaySteps)
            ? steps[k][s] : "";
         printf("symmetric with existing (on left): k %d s %d %s%s", k, s, strsteps, EOL);
         displayWide(ePyramid, sps[k][s]);
         displayWide(ePyramid, sp);
         displayWide(ePyramid, 0);
      }
      return -1;
   }
   return k;
}

///////////////////////////////////////////////////////////////////////////////
int solIsUniqueSymmetric(int key, const char* spsteps, const const TPlace* sp) {
   return findUniqueSymmetric(key, spsteps, sp) != -1;
}

///////////////////////////////////////////////////////////////////////////////
int solAddUniqueSymmetric(int key, const char* spsteps, const const TPlace* sp) {
   int k = findUniqueSymmetric(key, spsteps, sp);
   printf("solAddUniqueSymmetric add: k %d %s%s", k, spsteps, EOL);
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
      sps[k] = (TPlace**)realloc(sps[k], max_sps[k] * spXYZ * sizeof(TPlace));
   }
   //printf("solAddUniqueSymmetric add: k %d i %d %s%s", k, s, spsteps, EOL);
   if (SET_HAS(topics, eDisplayAdd)) {
      printf("Add: %s%s", spsteps, EOL);
   }
   steps[k][s] = stepsCopy(spsteps);
   sps[k][s] = SP_NEW(1);
   spCopy(sps[k][s], sp);
   ++sps_count[k];
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void displayKey(int key, EDisplayShape eShape) {
   int k = findKey(key);
   int i;
   for (i = 0; i < sps_count[k]; ++i) {
      printf("%s%s", steps[k][i], EOL);
      displayWide(eShape, sps[k][i]);
   }
}

///////////////////////////////////////////////////////////////////////////////
void solDisplay(int key, EDisplayShape eShape) {
   displayKey(key, eShape);
   displayWide(eShape, 0);
}

///////////////////////////////////////////////////////////////////////////////
int solCountForCount(int count) {
   int found = 0;
   int k;
   for (k = 0; keys[k]; ++k) {
printf("k %d has %d pieces (0x%x)%s", k,  setCount(keys[k]), keys[k], EOL);
      if (setCount(keys[k]) == count) {
         found += sps_count[k];
      }
   }
   return found;
}

///////////////////////////////////////////////////////////////////////////////
void solDisplayByCount(int count, EDisplayShape eShape) {
   int k;
   for (k = 0; keys[k]; ++k) {
      if (setCount(keys[k]) == count) {
         displayKey(keys[k], eShape);
      }
   }
   displayWide(eShape, 0);
}
