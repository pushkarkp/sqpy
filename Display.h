/**
 * Copyright 2018 Pushkar Piggott
 *
 * Display.h
 */
#ifndef DisplayH
#define DisplayH

#include "SquarePyramid.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum {eUnix, eMac, eWindows, eEolTypes} EEol;
extern const char* eol[eEolTypes];
#define EOL (eol[eWindows])

///////////////////////////////////////////////////////////////////////////////
typedef enum {ePyramid, eCube, eDisplayShapes} EDisplayShape;
void initDisplay(int pageWidth);
void display1(EDisplayShape, const TPlace* sp);
void display1RowRange(EDisplayShape, int r0, int r1, const TPlace* sp);
int displayPage(EDisplayShape, int h, int n, const TPlace* sps);
int displayPageRowRange(EDisplayShape, int h, int r0, int r1, int n, const TPlace* sps);
void displayWide(EDisplayShape, const TPlace* sp);
void displayWideRowRange(EDisplayShape, int r0, int r1, const TPlace* sp);

#endif
