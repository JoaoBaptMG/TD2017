

%{
#include "LogicalParser.hpp"
%}

%option prefix="logc_"
%option outfile="LogicalParser.lex.cpp"
%option never-interactive
%option reentrant
%option nostdinit
%option noyywrap
%option bison-bridge
%option noinput
%option nounput

%%

[ \t]   ;
"&" { *yylval = nullptr; return '&'; }
"|" { *yylval = nullptr; return '|'; }
"(" { *yylval = nullptr; return '('; }
")" { *yylval = nullptr; return ')'; }
"!" { *yylval = nullptr; return '!'; }
"->"    { *yylval = nullptr; return IMPLIES; }
[a-zA-Z][a-zA-Z0-9]*    { *yylval = new ExpTree(std::string(yytext)); return STRING; }
.   { *yylval = new ExpTree("Lex error!"); return ERROR; }

%%

