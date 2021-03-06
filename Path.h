/**
 * Copyright 2018 Pushkar Piggott
 *
 * Path.h
 */
#ifndef PathH
#define PathH

///////////////////////////////////////////////////////////////////////////////
// lower case -> given axis, upper case -> other axis
// [Aa].. is positive, ..[Zz] is negative
#define PATH_STEP_MIN 'a'
#define PATH_STEP_MAX 'z'
#define PATH_STEP_IS_SIDEWAYS(c) (c < PATH_STEP_MIN - 1)
#define PATH_STEP_SCALAR(c) (c | 0x20)
#define PATH_STEP_IS_FORWARD(c) (PATH_STEP_SCALAR(c) < (PATH_STEP_MIN + PATH_STEP_MAX) / 2)
#define PATH_STEP_ADVANCE(c) (PATH_STEP_SCALAR(c) - (PATH_STEP_IS_FORWARD(c) ? PATH_STEP_MIN - 1 : PATH_STEP_MAX + 1))

///////////////////////////////////////////////////////////////////////////////
typedef const char* TPath;
int pathOk(TPath);
int pathReport(TPath, int start);
int pathOkForPiece(TPath, const char* errorPrefix);
int pathMarkersCount(TPath);
int pathMarkerCount(TPath, int marker);
char* pathNext(TPath);
int pathRemoveBothEndMarkers(char*);
void reportErr(int report, const char* prefix, const char* path, int offset, const char* msg);

#endif
