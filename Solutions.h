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
PSquarePyramid solGet(int key, int);
int solIsUnique(int, TSquarePyramid);
int solIsUniqueSymmetric(int, const char*, TSquarePyramid);
int solAddUniqueSymmetric(int, const char*, TSquarePyramid);
void solDisplay(int key, EDisplayShape, int pageWidth);
void solDisplayByCount(int count, EDisplayShape, int pageWidth);

#endif
