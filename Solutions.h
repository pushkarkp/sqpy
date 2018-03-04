/**
 * Copyright 2018 Pushkar Piggott
 *
 * Solutions.h
 */

#include "SquarePyramid.h"
#include "Display.h"

///////////////////////////////////////////////////////////////////////////////
void solInit();
int solGetCount(int);
int solIsUniqueSymmetric(int, TSquarePyramid);
int solAddUniqueSymmetric(int, TSquarePyramid);
void solDisplay(int used, EDisplayShape, int pageWidth);
void solDisplayBySize(int size, EDisplayShape, int pageWidth);
