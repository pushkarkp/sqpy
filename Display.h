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
extern const char* eol[eEolTypes];
#define EOL (eol[eWindows])

///////////////////////////////////////////////////////////////////////////////
typedef enum {eUpright, eFolded, eCube, eDisplayShapes} EDisplayShape;
void display1(EDisplayShape, const TSquarePyramid);
void display1RowRange(EDisplayShape, int r0, int r1, const TSquarePyramid);
int displayPage(EDisplayShape, int w, int h, int n, const TSquarePyramid*);
int displayPageRowRange(EDisplayShape, int w, int h, int r0, int r1, int n, const TSquarePyramid*);
void displayWide(EDisplayShape, int w, const TSquarePyramid);
void displayWideRowRange(EDisplayShape, int w, int r0, int r1, const TSquarePyramid);

#endif
