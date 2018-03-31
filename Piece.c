/**
 * Copyright 2018 Pushkar Piggott
 *
 * Piece.c
 */

#include "Piece.h"

#include <string.h>

int pathMaxLength = 0;

///////////////////////////////////////////////////////////////////////////////
// a -> (0, +1), b -> (0, +2) ... z -> (0, -1), y -> (0, -2) ...
// A -> (+1, 0), B -> (+2, 0) ... Z -> (-1, 0), Y -> (-2, 0) ...
// . or , saves and then restores the current location.
// For the pyramid symmetry optimization to work, paths must be 
// predominantly positive (prefer aA over zZ).
const TPiece pieces[ePresences] = {
   {0}, // Absent
   {"a.a.A.Z", ".z.a.A.Z", 0}, // Grey
   {"aAaA", ".z.AaA", ".Zz.aA", 0}, // Pink Purple
   {"bB", ".z.aB", ".b.B", 0}, // Light Blue
   {"aAb", ".z.Ab", ".Zz.b", ".z.aAa", "bAa", 0}, // Dark Green
   {"AbZ", ".A.bA", ".zA.aA", 0}, // Yellow
   {"Ac", ".A.c", ".zA.b", ".z.bA", "cA", 0}, // Dark Blue
   {"bAz", ".z.aAz", ".b.Aa", ".Ab.a", "zAb", 0}, // Red
   {"a.b.A", ".z.b.A", "A.z.b", ".z.a,A,a", "b.a.A", 0}, // Pink
   {"c", ".z.b", 0}, // Blue Purple
   {"aAz", 0}, // Light Green
   {"bA", ".b.A", ".zZ.a", "Ab", 0}, // Orange
   {"aA", ".a.A", 0} // White
};

///////////////////////////////////////////////////////////////////////////////
void setPathMaxLength() {
   int pc;
   for (pc = 0; pc < ePresences; ++pc) {
      int p;
      for (p = 0; pieces[pc][p]; ++p) {
         int len = strlen(pieces[pc][p]);
         if (len > pathMaxLength) {
            pathMaxLength = len;
         }
      }
   }
}
