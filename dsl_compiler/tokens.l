%{
#include <string>
#include "node.h"
#include "parser.hpp"

#define SAVE_TOKEN  yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t)    (yylval.token = t)
%}

%option noyywrap

%%

[ \t\n]					        ;
"extern"                return TOKEN(TEXTERN);
"return"				        return TOKEN(TRETURN);
"print"				          return TOKEN(TPRINT);
"create"                return TOKEN(TCREATE);
"deposit"               return TOKEN(TDEPOSIT);
"transfer"              return TOKEN(TTRANSFER);
"withdraw"              return TOKEN(TWITHDRAW);
"balance"               return TOKEN(TBALANCE);
"if"                    return TOKEN(TIF);
"loop"                  return TOKEN(TLOOP);
"to"                    return TOKEN(TTO);
"then"                  return TOKEN(TTHEN);
"end"                  return TOKEN(TEND);
"function"                  return TOKEN(TFUNC);
[a-zA-Z][a-zA-Z0-9_]*   SAVE_TOKEN; return TIDENTIFIER;
[0-9]+					        SAVE_TOKEN; return TINTEGER;

"="						          return TOKEN(TEQUAL);
"=="				          	return TOKEN(TCEQ);
"!="			          		return TOKEN(TCNE);
"<"				          		return TOKEN(TCLT);
"<="	          				return TOKEN(TCLE);
">"				          		return TOKEN(TCGT);
">="					          return TOKEN(TCGE);

"("	          					return TOKEN(TLPAREN);
")"					          	return TOKEN(TRPAREN);
"{"         				    return TOKEN(TLBRACE);
"}"					          	return TOKEN(TRBRACE);

"."         				    return TOKEN(TDOT);
","				          		return TOKEN(TCOMMA);

"+"				          		return TOKEN(TPLUS);
"-"		          				return TOKEN(TMINUS);
"*"		          				return TOKEN(TMUL);
"/"				          		return TOKEN(TDIV);
"_"				          		return TOKEN(TUNDERSCORE);

.                       printf("Unknown token!\n"); yyterminate();

%%
