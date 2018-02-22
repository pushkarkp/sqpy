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

///////////////////////////////////////////////////////////////////////////////
typedef int TSetOfPresences;
const char* sopToString(char* buf, TSetOfPresences);
int sopAll();
int sopCount(TSetOfPresences);
#define SOP_WITH(sop, pres) ((sop) | (1 << (pres)))
#define SOP_WITHOUT(sop, pres) ((sop) & ~(1 << (pres)))
#define SOP_HAS(sop, pres) ((sop) & ((1 << pres)))

#endif
