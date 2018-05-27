/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solutions.h
 */
#ifndef SolutionsH
#define SolutionsH

#include "SquarePyramid.h"
#include "Topics.h"
#include "Display.h"
#include "SetOf.h"

///////////////////////////////////////////////////////////////////////////////
void solInit(TSet);
int solGetCount(int key);
const TPlace* solGet(int key, int);
int solIsUnique(int key, const const TPlace*);
int solIsUniqueSymmetric(int key, const char*, const TPlace*);
int solAddUniqueSymmetric(int key, const char*, const TPlace*);
void solDisplay(int key, EDisplayShape);
int solCountForCount(int count);
void solDisplayByCount(int count, EDisplayShape);
void solShowSetKeys(int k, FSetElementToString);

#endif
