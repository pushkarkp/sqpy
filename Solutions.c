/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solutions.c
 */

#include "Solutions.h"
#include "SetOf.h"
#include "Presence.h" // sopToString

#include <stdlib.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
#define PAGE_WIDTH 72
#define INITIAL_KEYS 16
#define INITIAL_SPS 4
#define GROWTH_FACTOR 2
static int max_keys = 0;
static int* keys = 0;
static int* max_sps = 0;
static int* sps_count = 0;
static TSquarePyramid** sps = 0;

///////////////////////////////////////////////////////////////////////////////
void zeroAllFrom(int i) {
   for (; i < max_keys; ++i) {
      keys[i] = 0;
      max_sps[i] = 0;
      sps_count[i] = 0;
      sps[i] = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
int findKey(int key) {
   int i;
   for (i = 0; keys[i]; ++i) {
       if (keys[i] == key) {
          return i;
       }
   }
   return i;
}

///////////////////////////////////////////////////////////////////////////////
int findOrAddKey(int key) {
  int i = findKey(key);
   if (i == max_keys - 1) {
      max_keys *= GROWTH_FACTOR;
      keys = (int*)realloc(keys, max_keys * sizeof(int));
      max_sps = (int*)realloc(max_sps, max_keys * sizeof(int));
      sps_count = (int*)realloc(sps_count, max_keys * sizeof(int));
      sps = (TSquarePyramid**)realloc(sps, max_keys * sizeof(TSquarePyramid*));
printf("max_keys %d\r\n", max_keys);
      zeroAllFrom(i);
   }
   if (!keys[i]) {
      keys[i] = key;
      max_sps[i] = INITIAL_SPS;
      sps_count[i] = 0;
      sps[i] = (TSquarePyramid*)malloc(max_sps[i] * SP_SIZE * SP_SIZE * SP_SIZE * sizeof(char));
   }
   return i;
}

///////////////////////////////////////////////////////////////////////////////
int findSp(int i, TSquarePyramid sp) {
   int eq;
   for (eq = sps_count[i] - 1; eq >= 0; --eq) {
      ERotation rot = spEqualRotate(sp, sps[i][eq]);
      TSetOfReflectionPlanes sorp = spEqualReflect(sp, sps[i][eq]);
      if (rot || sorp) {
         if (rot) {
            char bufSop[ePresences + 1];
            printf("%s[%d] %s rotation%s", sopToString(bufSop, keys[i]), eq, rotationToString(rot), EOL);
         } 
         if (sorp) {
            char bufSop[ePresences + 1];
            char bufSorp[16];
            printf("%s[%d] %s reflection%s", sopToString(bufSop, keys[i]), eq, sorpToString(bufSorp, sorp), EOL);
         }
         return eq;
      }
   }
   return sps_count[i];
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
         free(sps[i]);
      }
      free(sps);
   }
   keys = (int*)malloc(max_keys * sizeof(int));
   max_sps = (int*)malloc(max_keys * sizeof(int));
   sps_count = (int*)malloc(max_keys * sizeof(int));
   sps = (TSquarePyramid**)malloc(max_keys * sizeof(TSquarePyramid*));
   zeroAllFrom(0);
}

///////////////////////////////////////////////////////////////////////////////
int solGetCount(int key) {
   int k = findKey(key);
   char bufSop[ePresences + 1];
/*
   printf("%d %s\r\n", k, sopToString(bufSop, key));
   int i;
   for (i = 0; keys[i]; ++i) {
      printf("%d %s\r\n", i, sopToString(bufSop, keys[i]));
   }
*/
   return sps_count[k];
}

///////////////////////////////////////////////////////////////////////////////
int solIsUniqueSymmetric(int key, TSquarePyramid sp) {
   int i = findKey(key);
   if (!keys[i]) {
      return 1;
   }
   int s = findSp(i, sp);
   if (s == sps_count[i]) {
      return 1;
   }
displayWide(eUpright, PAGE_WIDTH, NULL);
printf("skip\r\n");
displayWide(eUpright, PAGE_WIDTH, sps[i][s]);
displayWide(eUpright, PAGE_WIDTH, sp);
displayWide(eUpright, PAGE_WIDTH, NULL);
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
int solAddUniqueSymmetric(int key, TSquarePyramid sp) {
   int i = findOrAddKey(key);
   int s = findSp(i, sp);
   if (s < sps_count[i]) {
displayWide(eUpright, PAGE_WIDTH, NULL);
printf("skip\r\n");
displayWide(eUpright, PAGE_WIDTH, sps[i][s]);
displayWide(eUpright, PAGE_WIDTH, sp);
displayWide(eUpright, PAGE_WIDTH, NULL);
      return 0;
   }
   if (sps_count[i] == max_sps[i]) {
      max_sps[i] *= GROWTH_FACTOR;
printf("max_sps[i] %d ", max_sps[i]);
      sps[i] = (TSquarePyramid*)realloc(sps[i], max_sps[i] * SP_SIZE * SP_SIZE * SP_SIZE * sizeof(char));
   }
   spCopy(sps[i][sps_count[i]++], sp);
displayWide(eUpright, PAGE_WIDTH, sp);
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void displayKey(int key, EDisplayShape eShape, int pageWidth) {
   int k = findKey(key);
   int i;
   for (i = 0; i < sps_count[k]; ++i) {
      displayWide(eShape, pageWidth, sps[k][i]);
   }
}

///////////////////////////////////////////////////////////////////////////////
void solDisplay(int key, EDisplayShape eShape, int pageWidth) {
   displayKey(key, eShape, pageWidth);
   displayWide(eShape, pageWidth, 0);
}

///////////////////////////////////////////////////////////////////////////////
void solDisplayBySize(int size, EDisplayShape eShape, int pageWidth) {
   int i;
   for (i = 0; keys[i]; ++i) {
      if (setCount(keys[i]) == size) {
         displayKey(keys[i], eShape, pageWidth);
      }
   }
   displayWide(eShape, pageWidth, 0);
}
