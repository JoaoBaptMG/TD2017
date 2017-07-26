#include <cstdio>
#include "LogicalParser.hpp"

extern int logc_lex_init(yyscan_t* scanner);
extern int logc_lex_destroy(yyscan_t yyscanner);
extern void logc_set_in(FILE * _in_str, yyscan_t yyscanner);

void traverse(ExpTree* tree, long depth = 0)
{    
    switch (tree->getType())
    {
        case ExpTree::Type::Terminal:
            std::cout << tree->getName();
            break;
        case ExpTree::Type::Negation:
            std::cout << "!(";
            traverse(tree->getLeftNode(), depth+1);
            std::cout << ')';
            break;
        case ExpTree::Type::And:
            std::cout << '(';
            traverse(tree->getLeftNode(), depth+1);
            std::cout << '&';
            traverse(tree->getRightNode(), depth+1);
            std::cout << ')';
            break;
        case ExpTree::Type::Or:
            std::cout << '(';
            traverse(tree->getLeftNode(), depth+1);
            std::cout << '|';
            traverse(tree->getRightNode(), depth+1);
            std::cout << ')';
            break;
        case ExpTree::Type::Implies:
            std::cout << '(';
            traverse(tree->getLeftNode(), depth+1);
            std::cout << "->";
            traverse(tree->getRightNode(), depth+1);
            std::cout << ')';
            break;
    }
}

int main()
{
    FILE* f = fopen("expr.log", "r");
    
    yyscan_t scanner;
    ExpTree* result = nullptr;
    
    logc_lex_init(&scanner);
    logc_set_in(f, scanner);
    logc_parse(scanner, result);
    
    traverse(result);
    std::cout << std::endl;
    
    logc_lex_destroy(scanner);
    fclose(f);
    
    delete result;
}
