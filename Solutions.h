/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solutions.h
 */
#ifndef SolutionsH
#define SolutionsH

#include "SquarePyramid.h"
#include "Display.h"
#include "SetOf.h"

///////////////////////////////////////////////////////////////////////////////
void solInit(int fixz);
int solGetCount(int key);
const TPlace* solGet(int key, int);
int solIsUnique(int key, const TPlace*);
int solIsUniqueSymmetric(int key, const char*, const TPlace*);
int solAddUniqueSymmetric(int key, const char*, const TPlace*);
void solSetComplete(int key);
int solMaxPlayCount();
int solCountForPlayCount(int, int complete);
int solCountForShapes(TSet);
void solDisplay(int key, EDisplayShape);
void solDisplayByPlayCount(int, int complete, EDisplayShape);
void solDisplayByShapes(TSet, EDisplayShape);
void solShowSetKeys(int k, FSetElementToString);
#endif
