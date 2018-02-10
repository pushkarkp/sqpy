/**
 * Copyright 2018 Pushkar Piggott
 *
 * Display.h
 */
#ifndef DisplayH
#define DisplayH

#include "Presence.h"
#include "SquarePyramid.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum {eUnix, eMac, eWindows, eEolTypes} EEol;
#define EOL_TYPE eWindows
extern const char* eol[eEolTypes];

///////////////////////////////////////////////////////////////////////////////
typedef enum {eUpright, eFolded, eCube, eDisplayShapes} EDisplayShape;
void display1(EDisplayShape, TSquarePyramid);
void display1RowRange(EDisplayShape, int r0, int r1, TSquarePyramid);
int displayPage(EDisplayShape, int w, int h, int n, TSquarePyramid*);
int displayPageRowRange(EDisplayShape, int w, int h, int r0, int r1, int n, TSquarePyramid*);
void displayWide(EDisplayShape, int w, TSquarePyramid);
void displayWideRowRange(EDisplayShape, int w, int r0, int r1, TSquarePyramid);

#endif
