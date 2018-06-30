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
      case eTopicSettings: return "settings";
      case eTopicPyramid: return "pyramid";
      case eTopicOrder: return "order";
      case eTopicOrientations: return "orientations";
      case eTopicPaths: return "paths";
      case eTopicRepeat: return "repeats";
      case eTopicSteps: return "steps";
      case eTopicAdd: return "adding";
      case eTopicSymmetries: return "symmetries";
      default: return "unknown";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* displayTopicDescription(int t) {
   switch (t) {
      case eTopicSettings: return "Display the setting values.";
      case eTopicPyramid: return "Display the pyramid.";
      case eTopicOrder: return "Display the order of the positions at which paths are started.";
      case eTopicOrientations: return "Display the specified path in all orientations.";
      case eTopicPaths: return "Display the paths for each piece.";
      case eTopicRepeat: return "Display each piece path orientation and whether it is a repeat.";
      case eTopicSteps: return "Display the steps ";
      case eTopicAdd: return "Display each unique solution as it is added.";
      case eTopicSymmetries: return "Display the symmetries found for each symmetrical partial solution.";
      default: return "unknown";
   }
}

///////////////////////////////////////////////////////////////////////////////
void describeDisplayTopics() {
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
TSet matchDisplayTopics(const char* s) {
   int len = strlen(s);
   TSet sot = 0;
   ETopic t;
   for (t = 0; t < eTopics; ++t) {
      if (0 == strncasecmp(displayTopicToString(t), s, len)) {
         sot = SET_WITH(sot, t);
      }
   }
//printf("%s %d%s", setToString(sot, displayTopicToString), setCount(sot), EOL);
   return sot;
}
