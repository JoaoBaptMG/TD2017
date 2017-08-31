
%name-prefix "logc_"
%define api.pure full
%output "LogicalParser.tab.cpp"
%defines "LogicalParser.tab.hpp"

%lex-param {yyscan_t scanner}
%parse-param {yyscan_t scanner} {ExpPtr& result}

%{
typedef void* yyscan_t;
#include <string>
#include <memory>
#include "LogicalParser.hpp"
#include "ExpTree.hpp"
extern int logc_lex(YYSTYPE*, yyscan_t);
void logc_error(yyscan_t, ExpPtr, std::string error);
%}

%define api.value.type {ExpPtr}

%token IMPLIES STRING ERROR
%start top_expr

%left IMPLIES
%left '|'
%left '&'
%precedence '!'

%%

top_expr: expression { result = std::make_shared<ExpTree>(ExpTree::Type::Root, $1); }

expression: %empty {} |
    expression '&' expression { $$ = std::make_shared<ExpTree>(ExpTree::Type::And, $1, $3); } |
    expression '|' expression { $$ = std::make_shared<ExpTree>(ExpTree::Type::Or, $1, $3); } |
    expression IMPLIES expression { $$ = std::make_shared<ExpTree>(ExpTree::Type::Implies, $1, $3); } |
    '(' expression ')' { $$ = std::move($2); } |
    '!' expression { $$ = std::make_shared<ExpTree>(ExpTree::Type::Negation, $2); } |
    STRING { $$ = std::move($1); } ;

%%

void logc_error(yyscan_t, ExpPtr, std::string error)
{
    std::cout << "Error parsing expression! " << error << std::endl;
}

