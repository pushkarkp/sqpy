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
const char* displayTopicToString(int t) {
   switch (t) {
      case eDisplaySettings: return "settings";
      case eDisplayPyramid: return "pyramid";
      case eDisplayOrder: return "order";
      case eDisplayOrientations: return "orientations";
      case eDisplayRepeat: return "repeats";
      case eDisplaySteps: return "steps";
      case eDisplayAdd: return "adding";
      case eDisplaySymmetries: return "symmetries";
      default: return "unknown";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* displayTopicDescription(int t) {
   switch (t) {
      case eDisplaySettings: return "Display the setting values.";
      case eDisplayPyramid: return "Display the pyramid.";
      case eDisplayOrder: return "Display the order of the positions at which paths are started.";
      case eDisplayOrientations: return "Display the specified path in all orientations.";
      case eDisplayRepeat: return "Display each piece path orientation and whether it is a repeat.";
      case eDisplaySteps: return "Display the steps ";
      case eDisplayAdd: return "Display each unique solution as it is added.";
      case eDisplaySymmetries: return "Display the symmetries found for each symmetrical partial solution.";
      default: return "unknown";
   }
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
TSet matchDisplayTopics(const char* s) {
   int len = strlen(s);
   TSet sot = 0;
   EDisplayTopic t;
   for (t = 0; t < eDisplayTopics; ++t) {
      if (0 == strncasecmp(displayTopicToString(t), s, len)) {
         sot = SET_WITH(sot, t);
      }
   }
printf("%s %d%s", setToString(sot, displayTopicToString), setCount(sot), EOL);
   return sot;
}
