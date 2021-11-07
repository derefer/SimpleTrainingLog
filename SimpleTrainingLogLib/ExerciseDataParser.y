/* Compile this with bison -vd simple.y to get the header, which is needed for
   the scanner to have the token definitions.  Every symbol should have a
   semantic value except the punctuation characters.  Pushing something to the
   stack is called as shifting.  If a group of shifted tokens on the stack
   match a left hand side of a grammar rule, they're reduced.  Semantic values
   of the grouping are calculated during reduction.  This is the bottom-up
   approach.  The union is used for holding semantic values.  */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <QtGlobal>
#include "DataElements.h"  /* Definition of YYSTYPE.  */
#undef YYDEBUG
#define YYERROR_VERBOSE
extern int ExerciseDataParserlineno;
/* Compiling with -d the parser will be in debug mode printing "--accepting
   rule at..." messages to stderr.  To suppress this set yy_flex_debug to
   zero.  By default it's non-zero.  */
extern int ExerciseDataParser_flex_debug;
/* Declare basic functions and globale variables.  Conversions from UTF-8 to
   Latin1 were added.  */
void set_location(const char *str, int reset = 0);
void yyerror(const char *str);
void yyerror_verbose(const YYLTYPE *loc, const char *str);
int yylex();
QList<Shoe*> shoes;
QList<Sport*> sports;
QList<Exercise*> exercises;
QList<Place*> places;
QList<Weather*> weathers;
const char *ExerciseDataParserin_name = NULL;
static int ExerciseDataParsercolumnno = 0;
%}
/* "%debug" is turned off.  */
/* These are terminals with semantic values.  */
%locations
%start database
%token EQ
%token LBRACE RBRACE LCURLY RCURLY
%token SHOES
%token EXERCISES
%token SPORTS
%token PLACES
%token WEATHERS
%token ID
%token NAME
%token COLOR
%token BUY
%token COMMENT
%token DATE
%token TIME
%token DURATION
%token DISTANCE
%token SPORT
%token SHOE
%token PLACE
%token WEATHER
%token PULSE
%token CALORIES
/* Typed terminals.  */
%token <intval> INT
%token <stringval> STRING
/* Types non-terminals.  */
%type <intlist> intlist
%%
database: sections
;

sections: sections section
|  /* Allow empty files.  */
;

section: shoes_section
| exercises_section
| sports_section
| places_section
| weathers_section
;

shoes_section: SHOES LCURLY shoes RCURLY;
exercises_section: EXERCISES LCURLY exercises RCURLY;
sports_section: SPORTS LCURLY sports RCURLY;
places_section: PLACES LCURLY places RCURLY;
weathers_section: WEATHERS LCURLY weathers RCURLY;

shoes: shoes shoe
|  /* No shoes in the section.  */
;

shoe:
LCURLY
ID EQ INT ','
NAME EQ STRING ','
BUY EQ INT ','  /* Year only.  */
COMMENT EQ STRING
RCURLY
{
  if ($4 < 0) yyerror_verbose(&@4, "Invalid shoe identifier");
  shoes.append(new Shoe($4, $8.toUtf8(), $12, $16.toUtf8()));
}
;

exercises: exercises exercise
|  /* No exercises in the section.  */
;

exercise:
LCURLY
ID EQ INT ','
DATE EQ STRING ','
TIME EQ STRING ','
DISTANCE EQ INT ','
DURATION EQ STRING ','
SPORT EQ INT ','
PLACE EQ LBRACE intlist RBRACE ','
SHOE EQ INT ','
COMMENT EQ STRING ','
WEATHER EQ LBRACE intlist RBRACE ','
PULSE EQ INT '/' INT ','
CALORIES EQ INT '/' INT
RCURLY
{
  if ($4 < 0) yyerror_verbose(&@4, "Invalid exercise identifier");
  if ($24 < 0) yyerror_verbose(&@24, "Invalid sport identifier");
  if ($16 < 0) yyerror_verbose(&@16, "Invalid distance data");
  for (int i = 0; i < $29.size(); ++i) if ($29.at(i) < 0) yyerror_verbose(&@29, "Invalid place identifier");
  for (int i = 0; i < $43.size(); ++i) if ($43.at(i) < 0) yyerror_verbose(&@43, "Invalid weather identifier");
  if ($48 < 0 || $48 > 240) yyerror_verbose(&@48, "Invalid pulse data");
  if ($50 < 0 || $50 > 240) yyerror_verbose(&@50, "Invalid pulse data");
  if ($54 < 0) yyerror_verbose(&@54, "Invalid calories data");
  if ($56 < 0 || $56 > 100) yyerror_verbose(&@56, "Invalid fat data");
  Exercise *e = new Exercise($4);
  e->setDate($8.toUtf8());
  e->setTime($12.toUtf8());
  e->setDistance($16);
  e->setDuration($20.toUtf8());
  e->setSport($24);
  e->setPlaces($29);
  e->setShoe($34);
  e->setComment(decodeComment($38.toUtf8()));
  e->setWeathers($43);
  e->setMaxPulse($48);
  e->setAvgPulse($50);
  e->setCal($54);
  e->setFat($56);
  exercises.append(e);
}
;

sports: sports sport
|  /* No sports in the section.  */
;

sport:
LCURLY
ID EQ INT ','
NAME EQ STRING ','
COLOR EQ STRING
RCURLY
{
  if ($4 < 0) yyerror_verbose(&@4, "Invalid sport identifier");
  sports.append(new Sport($4, $8.toUtf8(), QColor($12)));
}
;

places: places place
|  /* No places in the section.  */
;

place:
LCURLY
ID EQ INT ','
NAME EQ STRING
RCURLY
{
  if ($4 < 0) yyerror_verbose(&@4, "Invalid place identifier");
  places.append(new Place($4, $8.toUtf8()));
}
;

weathers: weathers weather
|  /* No weathers in the section.  */
;

weather:
LCURLY
ID EQ INT ','
NAME EQ STRING
RCURLY
{
  if ($4 < 0) yyerror_verbose(&@4, "Invalid weather identifier");
  weathers.append(new Weather($4, $8.toUtf8()));
}
;

/* The order is important.  */
intlist: INT ',' intlist { $3.append($1); $$ += $3; }
| INT { $$.append($1); }
| { }  /* Integer lists can be empty.  Suppress warning with empty action.  */
;

%%
void set_location(const char *str, int reset)
{
  if (reset)
    ExerciseDataParsercolumnno = 0;
  yylloc.first_line = ExerciseDataParserlineno;
  yylloc.first_column = ExerciseDataParsercolumnno;  /* Artificial.  */
  yylloc.last_line = ExerciseDataParserlineno;
  yylloc.last_column = (ExerciseDataParsercolumnno += strlen(str));
}

void yyerror(const char *str)  /* Called by yyparse on error.  The exact
location of the error is reported using yylloc.  */
{
  assert(ExerciseDataParserin_name != NULL);
  qWarning("Bison error in file %s: %s at %d:%d-%d:%d",
    ExerciseDataParserin_name, str, yylloc.first_line, yylloc.first_column,
    yylloc.last_line, yylloc.last_column);
}

/* A more specific error function.  */
void yyerror_verbose(const YYLTYPE *loc, const char *str)
{
  assert(ExerciseDataParserin_name != NULL);
  qWarning("Bison error in file %s: %s at %d:%d-%d:%d",
    ExerciseDataParserin_name, str, loc->first_line, loc->first_column,
    loc->last_line, loc->last_column);
}
