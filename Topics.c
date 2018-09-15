/**
 * Copyright 2018 Pushkar Piggott
 *
 * Topics.c
 */

#include "Topics.h"
#include "Display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

///////////////////////////////////////////////////////////////////////////////
static TSet topics = 0;
static TSet setTopics = 0;

///////////////////////////////////////////////////////////////////////////////
const char* displayTopicToString(int t) {
   switch (t) {
      case eTopicSettings: return "settings";
      case eTopicPaths: return "paths";
      case eTopicRepeat: return "repeats";
      case eTopicSteps: return "steps";
      case eTopicPlays: return "plays";
      case eTopicSymmetries: return "symmetries";
      case eTopicAdd: return "adding";
      case eTopicProgress: return "progress";
      case eTopicCollisions: return "collisions";
      default: return "";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* displayTopicDescription(int t) {
   switch (t) {
      case eTopicSettings: return "Display the setting values.";
      case eTopicPaths: return "Display the paths for each piece.";
      case eTopicRepeat: return "Display each piece path orientation and whether it is a repeat.";
      case eTopicSteps: return "Display the steps for paths.";
      case eTopicPlays: return "Display the plays for each partial solution.";
      case eTopicSymmetries: return "Display the symmetries found for each symmetrical partial solution.";
      case eTopicAdd: return "Display each unique solution as it is added.";
      case eTopicProgress: return "Display search progress.";
      case eTopicCollisions: return "Display all collisions.";
      default: return "unknown";
   }
}

///////////////////////////////////////////////////////////////////////////////
void displayTopicsDescribe() {
   int maxlen = 0;
   ETopic t;
   for (t = 0; t < eTopics; ++t) {
      int len = strlen(displayTopicToString(t));
      if (len > maxlen) {
         maxlen = len;
      }
   }
   for (t = 0; t < eTopics; ++t) {
      printf(" %-*s %s%s", maxlen + 1, displayTopicToString(t), displayTopicDescription(t), EOL);
   }
}

///////////////////////////////////////////////////////////////////////////////
int displayTopicsAdd(const char* s) {
   int len = strlen(s);
   int m = 0;
   ETopic t;
   for (t = 0; t < eTopics; ++t) {
      if (0 == strncasecmp(displayTopicToString(t), s, len)) {
         topics = SET_WITH(topics, t);
         ++m;
      }
   }
//printf("%s %d%s", setToString(topics, displayTopicToString, "none"), setCount(topics), EOL, "none");
   return m;
}

///////////////////////////////////////////////////////////////////////////////
int displayTopics() {
   return topics;
}

///////////////////////////////////////////////////////////////////////////////
void displayTopicsAll() {
   if (setTopics != 0) {
      printf("displayTopicsAll() repeat call%s", EOL);
   }
   setTopics = topics;
   topics = -1;
}

///////////////////////////////////////////////////////////////////////////////
void displayTopicsNone() {
   if (setTopics != 0) {
      printf("displayTopicsNone() repeat call%s", EOL);
   }
   setTopics = topics;
   topics = 0;
}

///////////////////////////////////////////////////////////////////////////////
void displayTopicsRevert() {
//printf("displayTopicsRevert()%s", EOL);
   topics = setTopics;
   setTopics = 0;
}

