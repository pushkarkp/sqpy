/**
 * Copyright 2018 Pushkar Piggott
 *
 * SetOf.c
 */

#include "SetOf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INT_BITS (sizeof(int) * 8)

///////////////////////////////////////////////////////////////////////////////
int setGetMax(TSet set) {
   int i;
   for (i = SET_MAX_SIZE - 1; i >= 0; --i) {
      if (SET_HAS(set, i)) {
         return i;
      }
   }
   return -1;
}

///////////////////////////////////////////////////////////////////////////////
int setCount(TSet set) {
   int count = 0;
   int e;
   for (e = 1; e; e <<= 1) {
      if (set & e) {
         ++count;
      }
   }
   return count;
}

//////////////////////////////////////////////////////////////////////////////
int setGetSingle(TSet set) {
   int found = -1;
   int i;
   for (i = 0; i < SET_MAX_SIZE; ++i) {
      if (SET_HAS(set, i)) {
         if (found != -1) {
            return -1;
         }
         found = i;
      }
   }
   return found;
}

//////////////////////////////////////////////////////////////////////////////
const char* withUnknown(int i, FSetElementToString elToString) {
   const char* str = elToString(i);
   if (str[0] != 0) {
      return str;
   }
   return "unknown";
}

//////////////////////////////////////////////////////////////////////////////
char* setToString(TSet set, FSetElementToString elToString, const char* empty) {
   if (set == 0) {
      return strdup(empty);
   }
   int len = 0;
   int i;
   for (i = 0; i < SET_MAX_SIZE; ++i) {
      if (SET_HAS(set, i)) {
         len += strlen(withUnknown(i, elToString)) + 1;
      }
   }
   char* str = (char*)malloc(len);
   char* p = str;
   for (i = 0; i < SET_MAX_SIZE; ++i) {
      if (SET_HAS(set, i)) {
         const char* se = withUnknown(i, elToString);
         int c;
         for (c = 0; se[c]; ++c) {
            p[c] = se[c];
         }
         p[c] = ' ';
         p += c + 1;
      }
   }
   *(p - 1) = 0; 
   return str;
}

//////////////////////////////////////////////////////////////////////////////
char* setToChars(TSet set, FSetElementToChar elToChar, const char* empty) {
   if (set == 0) {
      return strdup(empty);
   }
   char* str = (char*)malloc(setCount(set) + 1);
   char* p = str;
   int i;
   for (i = 0; i < SET_MAX_SIZE; ++i) {
      if (SET_HAS(set, i)) {
         *p++ = elToChar(i);
      }
   }
   *p = 0; 
   return str;
}

///////////////////////////////////////////////////////////////////////////////
int setParseElement(const char* str, int n, FSetElementToString elToString) {
   int i;
   for (i = 0; i < n; ++i) {
//printf("%s =?= %s\r\n", str, elToString(i));
      if (!strcmp(elToString(i), str)) {
         return i;
      }
   }
   return n;
}
