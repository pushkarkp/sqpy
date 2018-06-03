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

///////////////////////////////////////////////////////////////////////////////
typedef TPath* TPiece;
extern TPath testPath;
extern const TPiece* pieces;
extern int pieceCount;
extern int maxPathLength;
extern int* pieceMaxInstances;

///////////////////////////////////////////////////////////////////////////////
char* pcRead(int, const char**);
TPiece pcCreate(TPath path);

int pcWalk(EPresence, TPath, EOrientation, const TPosition*, TPlace*);
int* pcDupInstanceCounts(int*);
int pcSumInstanceCounts(int*);

void pcSetHeight();
void pcTestOrientations(EPresence, int path, TSetOfOrientations);
#endif
