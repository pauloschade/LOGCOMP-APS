%{
#include <string>
// #include "node.h"
// #include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
%}

%%

[ \t\n]                 ;
[0-9]+                        SAVE_TOKEN; return TINTEGER;
"="                           return TOKEN(TEQUAL);
"=="                          return TOKEN(TCEQ);
"!="                          return TOKEN(TCNE);
"<"                           return TOKEN(TCLT);
"<="                          return TOKEN(TCLE);
">"                           return TOKEN(TCGT);
">="                          return TOKEN(TCGE);
"("                           return TOKEN(TLPAREN);
")"                           return TOKEN(TRPAREN);
"{"                           return TOKEN(TLBRACE);
"}"                           return TOKEN(TRBRACE);
"+"                           return TOKEN(TPLUS);
"-"                           return TOKEN(TMINUS);
"*"                           return TOKEN(TMUL);
"/"                           return TOKEN(TDIV);
","                           return TOKEN(TCOMMA);
"loop"                        return TOKEN(TLOOP);
"if"                          return TOKEN(TIF);
"else"                        return TOKEN(TELSE);
"function"                    return TOKEN(TFUNC);
"create"                      return TOKEN(TCREATE);
"delete"                      return TOKEN(TDELETE);
"withdraw"                    return TOKEN(TWITHDRAR);
"transfer"                    return TOKEN(TTRANSFER);
"balance"                     return TOKEN(TBALANCE);
[_][a-zA-Z0-9_]+              SAVE_TOKEN; return TPERSONIDENTIFIER;
[a-zA-Z][a-zA-Z0-9_]*         SAVE_TOKEN; return TIDENTIFIER;
.                             printf("Unknown token!n"); yyterminate();

%%