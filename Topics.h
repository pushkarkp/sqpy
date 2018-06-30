/**
 * Copyright 2018 Pushkar Piggott
 *
 * Topics.h
 */
#ifndef TopicsH
#define TopicsH

#include "SetOf.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum {
   eTopicSettings, eTopicPyramid, eTopicOrder, eTopicOrientations, 
   eTopicPaths, eTopicRepeat, eTopicSteps, eTopicAdd, eTopicSymmetries, 
   eTopics
} ETopic;
#define ALL_TOPICS 0xff

///////////////////////////////////////////////////////////////////////////////
const char* displayTopicToString(int);
const char* displayTopicDescription(int);
void describeDisplayTopics();
TSet matchDisplayTopics(const char*);

#endif
