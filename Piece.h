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
extern int* pieceZeroInstances;
extern int* pieceMaxInstances;

///////////////////////////////////////////////////////////////////////////////
TPiece pcCreate(TPath path);
void pcAdd(TPiece pc, int times);

EPresence pcWalk(EPresence, TPath, EOrientation, const TPosition*, TPlace*);
int* pcDupInstanceCounts(const int*);
int pcSumInstanceCounts(const int*);
TSet pcInstanceCountSet(const int*);

void pcDisplay(TPiece);
void pcDisplayAll();

void pcSetHeight();
void pcSetHeightForPath(TPath, TSetOfOrientations);
int pcPathOrientation(EPresence, TPath, TSetOfOrientations);
void pcTestOrientations(EPresence, int path, TSetOfOrientations);
void pcRemovePath(int pc, int path);
#endif
