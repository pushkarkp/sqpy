/**
 * Copyright 2018 Pushkar Piggott
 *
 * Topics.c
 */

#ifndef TopicsH
#define TopicsH

///////////////////////////////////////////////////////////////////////////////
typedef enum {
   eDisplaySettings, eDisplayPyramid, eDisplayOrder, eDisplayOrientations, 
   eDisplayRepeat, eDisplayAdd, eDisplaySymmetries, eDisplayTopics
} EDisplayTopic;
typedef int TSetOfTopics;

///////////////////////////////////////////////////////////////////////////////
const char* displayTopicToString(EDisplayTopic);
const char* displayTopicDescription(EDisplayTopic);
void listDisplayTopics();
void describeDisplayTopics();
TSetOfTopics matchDisplayTopics(const char*);

#endif
