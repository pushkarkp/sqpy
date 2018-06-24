/**
 * Copyright 2018 Pushkar Piggott
 *
 * Topics.c
 */
#ifndef TopicsH
#define TopicsH

#include "SetOf.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum {
   eDisplaySettings, eDisplayPyramid, eDisplayOrder, eDisplayOrientations, 
   eDisplayPaths, eDisplayRepeat, eDisplaySteps, eDisplayAdd, eDisplaySymmetries, 
   eDisplayTopics
} EDisplayTopic;
#define ALL_TOPICS 0xff

///////////////////////////////////////////////////////////////////////////////
const char* displayTopicToString(int);
const char* displayTopicDescription(int);
void describeDisplayTopics();
TSet matchDisplayTopics(const char*);

#endif
