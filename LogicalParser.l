

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

[ \t\n]   ;
"&" { *yylval = nullptr; return '&'; }
"|" { *yylval = nullptr; return '|'; }
"(" { *yylval = nullptr; return '('; }
")" { *yylval = nullptr; return ')'; }
"!" { *yylval = nullptr; return '!'; }
"->"    { *yylval = nullptr; return IMPLIES; }
"#" { *yylval = ExpTree::False; return STRING; }
"@" { *yylval = ExpTree::True; return STRING; }
[a-zA-Z0-9]+    { *yylval = std::make_shared<ExpTree>(std::string(yytext)); return STRING; }
.   { *yylval = std::make_shared<ExpTree>("Lex error!"); return ERROR; }

%%

