/**
 * Copyright 2018 Pushkar Piggott
 *
 * Piece.h
 */
#ifndef PieceH
#define PieceH

#include "Presence.h"
#include "Path.h"
#include "SquarePyramid.h"
#include "Position.h"
#include "Orientation.h"
#include "SetOf.h"

///////////////////////////////////////////////////////////////////////////////
typedef TPath* TPiece;
extern TPath testPath;
extern const TPiece* pieces;
extern int pieceCount;
extern int maxPathLength;
extern int* pieceMaxInstances;

///////////////////////////////////////////////////////////////////////////////
char* pcRead(int, const char**);
char* pcCheckPath(TPath path);
TPiece pcCreate(TPath path);
void pcAdd(TPiece pc, int times);

EPresence pcWalk(EPresence, TPath, EOrientation, const TPosition*, TPlace*);
int* pcDupInstanceCounts(int*);
int pcSumInstanceCounts(int*);

void pcDisplay(TPiece);
void pcDisplayAll();

void pcSetHeight();
void pcSetHeightForPath(TPath, TSetOfOrientations);
int pcPathOrientation(TSet display, EPresence, TPath, TSetOfOrientations);
void pcTestOrientations(EPresence, int path, TSetOfOrientations);
#endif
