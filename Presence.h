/**
 * Copyright 2018 Pushkar Piggott
 *
 * Presence.h
 */
#ifndef PresenceH
#define PresenceH

///////////////////////////////////////////////////////////////////////////////
typedef enum {
   eMargin = -1,
   eAbsent, 
   eGrey, ePinkPurple, eLightBlue, eDarkGreen, eYellow, eDarkBlue, 
   eRed, ePink, eBluePurple, eLightGreen, eOrange, eWhite,
   ePresences} EPresence;
const char* presenceToString(EPresence);
extern const char glyph[ePresences];
typedef struct {
   char presence[ePresences];
} TSetOfPresences;
const char* setOfPresencesToString(char* buf, TSetOfPresences*);
int setOfPresencesAll(int, const TSetOfPresences*);

#endif
