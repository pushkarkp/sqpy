/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solutions.h
 */
#ifndef SolutionsH
#define SolutionsH

#include "SquarePyramid.h"
#include "Display.h"

///////////////////////////////////////////////////////////////////////////////
void solInit();
int solGetCount(int);
const TPlace* solGet(int key, int);
int solIsUnique(int, const const TPlace*);
int solIsUniqueSymmetric(int, const char*, const TPlace*);
int solAddUniqueSymmetric(int, const char*, const TPlace*);
void solDisplay(int key, EDisplayShape, int pageWidth);
int solCountForCount(int count);
void solDisplayByCount(int count, EDisplayShape, int pageWidth);

#endif
