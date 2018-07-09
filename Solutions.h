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
void solInit();
int solGetCount(int key);
const TPlace* solGet(int key, int);
int solIsUnique(int key, const TPlace*);
int solIsUniqueSymmetric(int key, const char*, const TPlace*);
int solAddUniqueSymmetric(int key, const char*, const TPlace*);
int solMaxPieceCount();
int solCountForPieceCount(int);
void solDisplay(int key, EDisplayShape);
void solDisplayByPieceCount(int, EDisplayShape);
void solShowSetKeys(int k, FSetElementToString);

#endif
