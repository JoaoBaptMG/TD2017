#include <cstdio>
#include <cstring>
#include <unordered_set>
#include <memory>
#include "LogicalParser.hpp"

extern int logc_lex_init(yyscan_t* scanner);
extern int logc_lex_destroy(yyscan_t yyscanner);
extern void logc_set_in(FILE * _in_str, yyscan_t yyscanner);

std::unordered_set<ExpPtr> trees;

ExpPtr simplify(ExpPtr, bool intuitionistic = false);
ExpPtr convertIntuitionistic(ExpPtr);
ExpPtr convertImplicational(ExpPtr);

void traverse(ExpPtr tree, long depth = 0)
{    
    switch (tree->getType())
    {
        case ExpTree::Type::Terminal:
            std::cout << tree->getName();
            break;
        case ExpTree::Type::Negation:
            std::cout << '!';
         case ExpTree::Type::Root:
            traverse(tree->getLeftNode(), depth+1);
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
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    FILE* f = fopen("expr.log", "r");
    
    yyscan_t scanner;
    ExpPtr result = nullptr;
    
    logc_lex_init(&scanner);
    logc_set_in(f, scanner);
    logc_parse(scanner, result);
    
    std::cout << "Input received: ";
    traverse(result);
    std::cout << std::endl;
    
    std::cout << "After simplification: ";
    result = simplify(result);
    traverse(result);
    std::cout << std::endl;
    
    std::cout << "Conversion to intuitionistic logic: ";
    result = convertIntuitionistic(result);
    traverse(result);
    std::cout << std::endl;
    
    logc_lex_destroy(scanner);
    fclose(f);
}
