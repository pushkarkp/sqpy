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
   eTopicPaths, eTopicRepeat, eTopicSteps, eTopicSymmetries, 
   eTopicSearch, eTopicAdd, eTopicProgress,
   eTopics
} ETopic;
#define IS_TOPIC(t) SET_HAS(displayTopics(), t)

///////////////////////////////////////////////////////////////////////////////
const char* displayTopicToString(int);
const char* displayTopicDescription(int);
void displayTopicsDescribe();
int displayTopicsAdd(const char*);
int displayTopics();
void displayTopicsNone();
void displayTopicsAll();
void displayTopicsRevert();

#endif
