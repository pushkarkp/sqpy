/**
 * Copyright 2018 Pushkar Piggott
 *
 * SqPy.c
 */

#include "Piece.h"
#include "Repeat.h"
#include "Solve.h"
#include "Solutions.h"
#include "Symmetry.h"
#include "Steps.h"
#include "Display.h"
#include "Topics.h"
#include "SetOf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 128

///////////////////////////////////////////////////////////////////////////////
static const char const* ERR_OPEN = "failed to open file \"%s\"";
static const char const* ERR_FILE = "file error";
static const char const* ERR_NO_ORIENT = "no orientation matched \"%s\"";
static const char const* ERR_NO_TOPIC = "no display topic matched \"%s\"";
static const char const* ERR_BAD_TOPIC = "more than one display topic matched \"%s\"";
static const char const* ERR_BAD_PLAY = "bad play ";
static const char const* ERR_UNPLAYABLE = "play collides \"%s\"";
static const char const* ERR_EXTRA_PLAY = "too many plays";
static const char const* ERR_BAD_SYM = "failed to parse symmetry";
static int search = 0;
static int fill = 0;
static TSetOfRotations sorn = SET_ALL_OF(eRotations);
static TSetOfReflectionPlanes sorp = SET_ALL_OF(eReflectionPlanes);
static int pc = eAbsent;
static int pathIndex = 0;
static TPath path = "aA";
static char* play[2] = {0};
static TSet soor = 0;
static int pageWidth = 76;

int getOptions(const char**, const char*);

///////////////////////////////////////////////////////////////////////////////
void usage() {
   printf("usage:%s", EOL);
   printf("sqpy [-g <page width>] [-h <height>] [-v] [-q [<planes>]] [-x [<planes>]] [-r [<rotations>]] [-a <path index>] [-f <file>] [-s [f] [x] [r]] [-y <play> [<play>]...] [-p <piece> [<count>]] [-i <piece>] [-d <topics>]%s", EOL);
   printf(" -g <page width> (%d)      the width of the display page (default)%s", pageWidth, EOL);
   printf(" -h <height> (5)           the pyramid height%s", EOL);
   printf(" -v                        show the pyramid volume%s", EOL);
   printf(" -q [b] [d] [x] [y]        show the sequence of placement (with symmetry planes)%s", EOL);
   printf(" -r [0] [90] [180] [270]   show orientations skipped for the given%s", EOL);
   printf(" -x [b] [d] [x] [y]           rotations or reflection planes%s", EOL);
   printf(" -a <path or index> (%s)   the path to orient%s", path, EOL);
   printf(" -o <orientations match>   show paths in orientations (default all)%s", EOL);
   printf(" -f <file>                 file contains arguments and one piece per line%s", EOL);
   printf(" -s [f] [r] [x]            search for and display complete pyramids%s", EOL);
   printf("    f                         ignore instance counts and fill the pyramid%s", EOL);
   printf("    r                         no rotational symmetry optimization%s", EOL);
   printf("    x                         no reflective symmetry optimization%s", EOL);
   printf(" -y <play> [<play>]...     plays (? for help), two sets of plays are compared%s", EOL);
   printf(" -p <piece> [<count>]      a piece (? for help)%s", EOL);
   printf(" -i <piece> (all)          the initial piece, by letter or index%s", EOL);
   printf(" -d <topics>               display one or more topics (see below, ? for help)%s", EOL);
   printf("%sDisplay topics (any unique prefix matches):%s", EOL, EOL);
   char* str = setToString(SET_ALL_OF(eTopics), displayTopicToString, "none");
   printf("%s%s", str, EOL);
   free(str);
   exit(0);
}

///////////////////////////////////////////////////////////////////////////////
void describePiece() {
   printf("A piece is a series of adjacent locations in (x, y)%s", EOL);
   printf("that form its shape. It also has a maximum instance count.%s", EOL);
   printf("A path uses letters to specify the moves between locations.%s", EOL);
   printf("'a' is 1, 'b' is 2, etc, 'z' is -1, 'y' is -2, etc.%s", EOL);
   printf("Lower case is horizontal (x), upper case is vertical (y).%s", EOL);
   printf("For example, \"AbZ\" specifies the following:%s", EOL);
   printf(" `````%s", EOL);
   printf(" `eee`%s", EOL);
   printf(" `e`e`%s", EOL);
   printf(" `````%s", EOL);
   printf("A marker ('.' ',' or '-') can be used to fork.%s", EOL);
   printf("It saves and then restores the current location.%s", EOL);
   printf("For example, \"a.a.A.Z\" specifies the following:%s", EOL);
   printf(" `````%s", EOL);
   printf(" ``a``%s", EOL);
   printf(" `aaa`%s", EOL);
   printf(" ``a``%s", EOL);
   printf(" `````%s", EOL);
   printf("A user path must start at an end, ie not with a marker.%s", EOL);
   printf("Sqpy uses the extra marker ('/') for intermediate paths.%s", EOL);
   printf("For example, \"/zZ/aZ\" starts 'e' above in the middle.%s", EOL);
}

///////////////////////////////////////////////////////////////////////////////
void describePlay() {
   printf("A play is a short string describing the deployment of a piece.%s", EOL);
   printf("It consists of a piece letter to display,%s", EOL);
   printf("a three digit location to start from,%s", EOL);
   printf("a letter indicating the plane (z, b, or d),%s", EOL);
   printf("a letter indicating the axis (x or y),%s", EOL);
   printf("two signs indicating the alignments,%s", EOL);
   printf("and a path itself.%s%s", EOL, EOL);
   printf("Planes: z is the z surface (001), b is the x==y surface (110),%s", EOL);
   printf("and d is the x==-y surface (1T0).%s", EOL);
   printf("The axis letter specifies the axis of lower case path letters.%s", EOL);
   printf("The signs indicates the directions of lower and upper case.%s", EOL);
   printf("The path may start with a marker, and%s", EOL);
   printf("may contain the extra marker ('/').%s", EOL);
   printf("For example, \"b000bx++/b/A\":%s%s", EOL, EOL);
   printf(" b%s%s", EOL, EOL);
   printf(" `b%s", EOL);
   printf(" b`%s%s", EOL, EOL);
   printf(" ```%s", EOL);
   printf(" ```%s", EOL);
   printf(" b``%s", EOL);
}

///////////////////////////////////////////////////////////////////////////////
int getNextOptionOffset(const char** argv) {
   int i;
   for (i = 1; argv[i] && argv[i][0] != '-'; ++i) {}
   return i;
}

///////////////////////////////////////////////////////////////////////////////
const char* getOptionExtraValue(int* pi, const char** argv) {
   if (argv[*pi + 1] && argv[*pi + 1][0] != '-') {
      ++*pi;
      return argv[*pi];
   }      
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
const char* getOptionValue(int* pi, const char** argv) {
   if (argv[*pi][2]) {
      return &argv[*pi][2];
   }
   return getOptionExtraValue(pi, argv);
}

///////////////////////////////////////////////////////////////////////////////
const char* getMandatory(const char* v, char c, const char* prefix) {
   if (!v) {
      printf("%smissing value for option '-%c'%s", prefix?prefix:"", c, EOL);
      usage();
   }
   return v;
}

///////////////////////////////////////////////////////////////////////////////
const char* getNumber(const char* v, char c, const char* prefix) {
   if (v && !isdigit(v[0])) {
      printf("%svalue for option '-%c' is not a number%s", prefix?prefix:"", c, EOL);
      usage();
   }
   return v;
}

///////////////////////////////////////////////////////////////////////////////
char* cutComment(char* line) {
   for (; *line && isspace(*line); ++line) {}
   int i;
   for (i = 0; line[i]; ++i) {
      if (line[i] == '/' && line[i + 1] == '/') {
         line[i] = 0;
         break;
      }
   }
   return line;
}
         
///////////////////////////////////////////////////////////////////////////////
int toArgv(const char** argv, char* line, const char* opt) {
   int argc = 0;
   if (opt) {
      argv[argc++] = opt;
   }
   int i;
   for (i = 0; line[i]; ++i) {
      if (isspace(line[i])) {
         line[i] = 0;
      } else if (i == 0 || line[i - 1] == 0) {
         argv[argc++] = &line[i];
      }         
   }
   argv[argc] = 0;
   return argc;
}

///////////////////////////////////////////////////////////////////////////////
int readPathArg(const char* path, const char* n, const char* prefix) {
   if (!pathOkForPiece(path, prefix)) {
      return 0;
   }

   int height = spHeight;
   spSetHeight(1);
   pcSetHeightForPath(path, SET_WITH(0, eZxPlusPlus));
   if (!pcPathOrientation(pieceCount, path, SET_WITH(0, eZxPlusPlus))) {
      printf("%spath '%s' self-conflicts%s", prefix?prefix:"", path, EOL);
      return 0;
   }
   spSetHeight(height);
   initDisplay(-1);

   TPiece pc = pcCreate(path);
   if (!pc) {
      printf("%sfailed to create paths from '%s'%s", prefix?prefix:"", path, EOL);
      return 0;
   }
   int times = (n) ? strtol(n, 0, 10) : 1;
   pcAdd(pc, times);
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
char* read(const char* fname) {
   FILE *f;
   if (!(f = fopen(fname, "r"))) {
      char* buf = (char*)malloc((strlen(ERR_OPEN) + strlen(fname)) * sizeof(char));
      sprintf(buf, ERR_OPEN, fname);
      return buf;
   }
   char lineBuf[MAX_LINE];
   int ln;
   for (ln = 1; fgets(lineBuf, MAX_LINE, f); ++ln) {
      char prefix[16];
      sprintf(prefix, "on line %d: ", ln);
      char* line = lineBuf;
      line = cutComment(line);
      const char* opt = (line[0] && line[0] == '-') ? 0 : "-p";
      const char* argv[16] = {0};
      int argc = toArgv(argv, line, opt);
      if ((argc > 1 || (argc && !opt)) 
       && !getOptions(argv, prefix)) { 
         break;
      }
   }
   if (!feof(f)) {
      return strdup(ERR_FILE);
   }

   fclose(f);
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
void init() {
   if (pieceCount == eFirstPiece) {
      readPathArg(path, "1", 0);
   }
   if (spHeight == 0) {
      pcSetHeight();
      initDisplay(pageWidth);
   }
}

///////////////////////////////////////////////////////////////////////////////
TSet getSet(int n, FSetElementToString elToStr, int* pi, const char** argv) {
   TSet set = 0;
   const char* p;
   for (p = getOptionExtraValue(pi, argv);
        p != 0;
        p = getOptionExtraValue(pi, argv)) {
      int el = setParseElement(p, n, elToStr);
      if (el == n) {
         printf("%s '%s'%s", ERR_BAD_SYM, p, EOL);
         return 0;
      }
      set = SET_WITH(set, el);
   }
   return set;
}

///////////////////////////////////////////////////////////////////////////////
int getOptions(const char** argv, const char* prefix) {
   int i;
   for (i = 0; argv[i] && argv[i][0] == '-'; ++i) {
      switch (argv[i][1]) {
      case 'h': {
         const char* n = getNumber(getMandatory(getOptionValue(&i, argv), argv[i][1], prefix), argv[i][1], prefix);
         spSetHeight(strtol(n, 0, 10));
         initDisplay(pageWidth);
         break;
      }
      case 'v': {
         init();
         spTestVolume();
         return 0;
      }
      case 'q': {
         init();
         spTestSequence(getSet(eReflectionPlanes, reflectionPlaneToString, &i, argv));
         return 0;
      }
      case 'x': {
         TSetOfReflectionPlanes sorp = getSet(eReflectionPlanes, reflectionPlaneToString, &i, argv);
         char* stror = setToString(spaceReflectSkip(sorp), orToString, "none");
         printf("%s%s", stror, EOL);
         free(stror);
         return 0;
      }
      case 'r': {
         TSetOfRotations sorn = getSet(eRotations, rotationToString, &i, argv);
         char* stror = setToString(spaceRotateSkip(sorn), orToString, "none");
         printf("%s%s", stror, EOL);
         free(stror);
         return 0;
      }
      case 'o': {
         TSetOfOrientations s = SET_ALL_OF(eOrientations);
         const char* v = getOptionValue(&i, argv);
         if (v) {
            s = matchOrientation(v);
         }
         if (setCount(s) == 0) {
            printf(ERR_NO_ORIENT, v);
            printf("%s", EOL);
            usage();
         }
         soor |= s;
         init();
         pcTestOrientations(pc, pathIndex, soor);
         return 0;
      }
      case 'f': {
         const char* fname = getMandatory(getOptionValue(&i, argv), argv[i][1], prefix);
         char* err = read(fname);
         if (err) {
            printf("%s%s", err, EOL);
            free(err);
            return 0;
         }
         break;
      }
      case 's': {
         search = 1;
         const char* c = 0;
         while ((c = getOptionExtraValue(&i, argv)) != 0) {
            switch (c[0]) {
               case 'f':
                  fill = 1;
                  break;
               case 'r':
                  sorn = 0;
                  break;
               case 'x':
                  sorp = 0;
                  break;
               default:
                  c = 0;
                  break;
            }
            if (c == 0 || c[1] != 0) {
               printf("expected 'f', 'r' or 'x'.%s", EOL);
               return 0;
            }
         }
         break;
      }
      case 'y': {
         if (play[1]) {
            printf(ERR_EXTRA_PLAY);
            printf(EOL);
            return 0;
         }
         int g = play[0] ? 1 : 0;
         const char* p;
         for (p = getMandatory(getOptionValue(&i, argv), argv[i][1], prefix);
              p;
              p = getOptionExtraValue(&i, argv)) {
            if (!stepOk(p)) {
               if (p[0] != '?') {
                  stepReport(p, ERR_BAD_PLAY);
               }
               describePlay();
               return 0;
            }
            if (!play[g]) {
               play[g] = strdup(p);
            } else {
               int len = strlen(play[g]);
               play[g] = (char*)realloc(play[g], len + strlen(p) + 2);
               play[g][len] = ' ';
               strcpy(play[g] + len + 1, p);
            }
         }
         break;
      }
      case 'p': {
         const char* path = getMandatory(getOptionValue(&i, argv), argv[i][1], prefix);
         if (path && path[0] == '?') {
            init();
            describePiece();
            return 0;
         }
         const char* n = getNumber(getOptionExtraValue(&i, argv), argv[i - 1][1], prefix);
         if (!readPathArg(path, n, prefix)) {
            return 0;
         }
         break;
      }
      case 'i': {
         const char* n = getMandatory(getOptionValue(&i, argv), argv[i][1], prefix);
         pc = strtol(n, 0, 10);
         if (pc == 0 && n[0] != '0') {
            pc = PIECE(n[0]);
         }
         break;
      }
      case 'a': {
         const char* n = getMandatory(getOptionValue(&i, argv), argv[i][1], prefix);
         pathIndex = strtol(n, 0, 10);
         if (pathIndex > 0) {
            pcOnePath(pc, pathIndex);
         } else {
            path = n;
         }
         break;
      }
      case 'g': {
         const char* n = getNumber(getMandatory(getOptionValue(&i, argv), argv[i][1], prefix), argv[i][1], prefix);
         pageWidth = strtol(n, 0, 10);
         initDisplay(pageWidth);
         break;
      }
      case 'd': {
         int j = i;
         const char* h = getMandatory(getOptionValue(&j, argv), argv[i][1], prefix);
         if (!strcmp(h, "?")) {
            displayTopicsDescribe();
            return 0;
         }
         const char* v;
         while ((v = getOptionExtraValue(&i, argv)) != 0) {
            int t = displayTopicsAdd(v);
            if (t != 1) {
               printf("%d 0x%x\r\n", eTopics, SET_ALL_OF(eTopics));
               printf(t == 0 ? ERR_NO_TOPIC : ERR_BAD_TOPIC, v);
               char* str = setToString(SET_ALL_OF(eTopics), displayTopicToString, "none");
               printf("%sTopics available: %s%s", EOL, str, EOL);
               free(str);
               return 0;
            }
         }
         break;
      }
      default:
         printf("unrecognized option '-%c'%s", argv[i][1], EOL);
         usage();
         break;
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void showOptions() {
   printf("page width %d, pyramid height %d, %s%s%d pieces.%s", 
          pageWidth, spHeight, search?"search, ":"", fill?"fill, ":"", 
          pieceCount - 1, EOL);
   if (displayTopics() != 0) {
      char* str = setToString(displayTopics(), displayTopicToString, "none");
      printf("Topics: %s.%s", str, EOL);
      free(str);
   }
   if (play[0]) {
      printf("Play 1: %s.%s", play[0], EOL);
      if (play[1]) {
         printf("Play 2: %s.%s", play[1], EOL);
      }
      return;
   }
   if (pieceCount > 1) {
      printf("Pieces:%s", EOL);
   }
   int i;
   for (i = eFirstPiece; i < pieceCount; ++i) {
       printf(" %c:", GLYPH(i));
      int p;
      for (p = 0; pieces[i][p]; ++p) {
         printf(" %s", pieces[i][p]);
      }
      printf("%s", EOL);
   }
   if (pc != 0) {
      printf("Initial piece: %c.%s", GLYPH(pc), EOL);
   }
   if (pathIndex > 0) {
      printf("path %d (%s).%s", pathIndex, pieces[pc][pathIndex], EOL);
   }
   if (soor != 0) {
      char* str = setToString(soor, orToString, "none");
      printf("orientations: %s.%s", str, EOL);
      free(str);
   }
   if (sorp == 0 || sorn == 0) {
      printf("Ignoring%s%s%s symmetry optimization.%s", 
             sorn?"":" rotational",
             sorn||sorp?"":" and",
             sorp?"":" reflective", EOL);
   }
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, const char** argv) {
   int i = 1;
   if (!getOptions(&argv[i], 0)) {
      return 0;
   }
   init();
   if (IS_TOPIC(eTopicSettings)) {
      showOptions();
   }
   if (IS_TOPIC(eTopicPaths)) {
      pcDisplayAll();
   }
   if (play[0]) {
      solInit();
      int i;
      for (i = 0; play[i]; ++i) {
         TPlace* sp = parseSteps(play[i]);
         if (!sp) {
            printf(ERR_UNPLAYABLE, play[i]); 
            printf(EOL); 
         } else {
            if (!play[1]) {
               display1(ePyramid, sp);
            } else if ((solAddUniqueSymmetric(1, play[i], sp) && i == 1)
                    || (i == 1 && !IS_TOPIC(eTopicSymmetries))) {
               solDisplay(1, ePyramid);
            }
            free(sp);
         }
         free(play[i]);
      }
      displayWide(ePyramid, 0);
      return 0;
   }
   if (search || IS_TOPIC(eTopicRepeat)) {
      findRepeat();
   }
   if (search) {
      printf("Total solutions %d%s", solve(pc, fill, sorn, sorp), EOL);
   }
   return 0;
}