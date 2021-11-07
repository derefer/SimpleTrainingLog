%{
#include <string.h>
#include <unistd.h>
/* QMake renames this.  */
#include "dataelements.h"
#include "exercisedataparser_yacc.h"  /* Tokens from the parser.  */
extern YYSTYPE yylval;  /* Defined by us.  */
extern YYLTYPE yylloc;  /* Location tracking.  */
extern void set_location(const char *str, int reset = 0);
static void warning(const char *str);  /* Lexer warnings.  */
%}
%option debug yylineno noyywrap nounput
digit [0-9]
%%
\"([^\"\n]|\\\")*[\"\n] {
  /* An example of using quoted string is found in Tom Niemann's "A Compact
     Guide To Lex & Yacc" book.  This string can be terminated by a '"' or a
     '\n'.  Only single line strings are supported.  */
  set_location(yytext);
  exercisedataparserlval.stringval = yytext + 1;
  int length = exercisedataparserlval.stringval.length();
  if (exercisedataparserlval.stringval[length - 1] != '"') {
    warning("Improperly terminated string!");
  } else {
    exercisedataparserlval.stringval.chop(1);
  }
  return STRING;  /* Damn.  */
}
#[^\n]*    { set_location(yytext); }  /* Don't return anything and don't confuse with "comment".  */
"="        { set_location(yytext); return EQ; }
"("        { set_location(yytext); return LBRACE; }
")"        { set_location(yytext); return RBRACE; }
"{"        { set_location(yytext); return LCURLY; }
"}"        { set_location(yytext); return RCURLY; }
-?{digit}+ { set_location(yytext); exercisedataparserlval.intval = atoi(yytext); return INT; }
shoes      { set_location(yytext); return SHOES; }  /* Sections.  */
exercises  { set_location(yytext); return EXERCISES; }
sports     { set_location(yytext); return SPORTS; }
places     { set_location(yytext); return PLACES; }
weathers   { set_location(yytext); return WEATHERS; }
id         { set_location(yytext); return ID; }  /* Attributes.  */
name       { set_location(yytext); return NAME; }
color      { set_location(yytext); return COLOR; }
buy        { set_location(yytext); return BUY; }
comment    { set_location(yytext); return COMMENT; }
date       { set_location(yytext); return DATE; }
time       { set_location(yytext); return TIME; }
distance   { set_location(yytext); return DISTANCE; }
duration   { set_location(yytext); return DURATION; }
sport      { set_location(yytext); return SPORT; }
shoe       { set_location(yytext); return SHOE; }
place      { set_location(yytext); return PLACE; }
weather    { set_location(yytext); return WEATHER; }
pulse      { set_location(yytext); return PULSE; }
calories   { set_location(yytext); return CALORIES; }

[ \t]+  { set_location(yytext); } /* Blank characters.  Eat up whitespace.  */
[\r\n]+ { set_location(yytext, 1); }
. { set_location(yytext); return yytext[0]; }  /* Any character (e.g. semicolons, colons, relational
operators) except newline.  Literals?  */
%%
/* Functions would come here.  */

void warning(const char *str)
{
  fprintf(stderr, "%s\n", str);
}
