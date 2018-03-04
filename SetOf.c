/**
 * Copyright 2018 Pushkar Piggott
 *
 * SetOf.c
 */

#include "SetOf.h"

#define INT_BITS (sizeof(int) * 8)

///////////////////////////////////////////////////////////////////////////////
int setCount(int set) {
   int count = 0;
   int i;
   for (i = 0; i < INT_BITS; ++i) {
      if (SET_HAS(set, i)) {
         ++count;
      }
   }
   return count;
}
