/**
 * Copyright 2018 Pushkar Piggott
 *
 * Topics.c
 */

#include "Topics.h"
#include "SetOf.h"
#include "Display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

///////////////////////////////////////////////////////////////////////////////
const char* displayTopicToString(EDisplayTopic t) {
   switch (t) {
      case eDisplaySettings: return "settings";
      case eDisplayPyramid: return "pyramid";
      case eDisplayOrder: return "order";
      case eDisplayOrientations: return "orientations";
      case eDisplayRepeat: return "repeats";
      case eDisplayAdd: return "adding";
      case eDisplaySymmetries: return "symmetries";
      default: return "unknown";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* displayTopicDescription(EDisplayTopic t) {
   switch (t) {
      case eDisplaySettings: return "Display the setting values.";
      case eDisplayPyramid: return "Display the pyramid, then quit.";
      case eDisplayOrder: return "Display the order of positions at which paths are started.";
      case eDisplayOrientations: return "Display the specified path in all z-plane orientations.";
      case eDisplayRepeat: return "Display whether each piece path orientation is a repeat.";
      case eDisplayAdd: return "Display each unique solution as it is added.";
      case eDisplaySymmetries: return "Display the symmetries found for each symmetrical partial solution.";
      default: return "unknown";
   }
}

///////////////////////////////////////////////////////////////////////////////
void listDisplayTopics() {
   EDisplayTopic t;
   for (t = 0; t < eDisplayTopics; ++t) {
      printf(" %s", displayTopicToString(t));
   }
   printf("%s", EOL);
}

///////////////////////////////////////////////////////////////////////////////
void describeDisplayTopics() {
   int maxlen = 0;
   EDisplayTopic t;
   for (t = 0; t < eDisplayTopics; ++t) {
      int len = strlen(displayTopicToString(t));
      if (len > maxlen) {
         maxlen = len;
      }
   }
   for (t = 0; t < eDisplayTopics; ++t) {
      printf(" %-*s %s%s", maxlen + 1, displayTopicToString(t), displayTopicDescription(t), EOL);
   }
}

///////////////////////////////////////////////////////////////////////////////
TSetOfTopics matchDisplayTopics(const char* s) {
   int len = strlen(s);
   TSetOfTopics sot = 0;
   EDisplayTopic t;
   for (t = 0; t < eDisplayTopics; ++t) {
      if (0 == strncasecmp(displayTopicToString(t), s, len)) {
         sot = SET_WITH(sot, t);
      }
   }
   return sot;
}
