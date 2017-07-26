
%name-prefix "logc_"
%define api.pure full
%output "LogicalParser.tab.cpp"

%lex-param {yyscan_t scanner}
%parse-param {yyscan_t scanner} {ExpTree*& result}

%{
typedef void* yyscan_t;
#include <string>
#include <memory>
#include "LogicalParser.hpp"
#include "ExpTree.hpp"
extern int logc_lex(YYSTYPE*, yyscan_t);
void logc_error(yyscan_t, ExpTree*, std::string error);
%}

%define api.value.type {ExpTree*}

%token IMPLIES STRING ERROR
%start top_expr

%left IMPLIES
%left '|'
%left '&'
%precedence '!'

%destructor { delete $$; } STRING ERROR

%%

top_expr: expression { result = $1; }

expression: %empty {} |
    expression '&' expression { $$ = new ExpTree(ExpTree::Type::And, $1, $3); } |
    expression '|' expression { $$ = new ExpTree(ExpTree::Type::Or, $1, $3); } |
    expression IMPLIES expression { $$ = new ExpTree(ExpTree::Type::Implies, $1, $3); } |
    '(' expression ')' { $$ = $2; } |
    '!' expression { $$ = new ExpTree(ExpTree::Type::Negation, $2); } |
    STRING { $$ = $1; } ;

%%

void logc_error(yyscan_t, ExpTree*, std::string error)
{
    std::cout << "Error parsing expression! " << error << std::endl;
}

