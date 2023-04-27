%{
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
%}

%%
[0-9]+   printf("?");
. ECHO;
%%