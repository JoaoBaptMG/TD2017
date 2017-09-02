#include <cstdio>
#include <cstring>
#include <string>
#include <memory>
#include "LogicalParser.hpp"

struct yy_buffer_state;
typedef yy_buffer_state *YY_BUFFER_STATE;
extern int logc_lex_init(yyscan_t* scanner);
extern int logc_lex_destroy(yyscan_t yyscanner);
extern void logc_set_in(FILE * _in_str, yyscan_t yyscanner);
extern YY_BUFFER_STATE logc__scan_string (const char *yy_str ,yyscan_t yyscanner);
extern void logc__delete_buffer (YY_BUFFER_STATE b ,yyscan_t yyscanner);

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
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << "<expression> OR" << std::endl
                  << "       " << argv[0] << "-f <file>" << std::endl;
        return -1;
    }

    yyscan_t scanner;
    YY_BUFFER_STATE buffer;
    ExpPtr result = nullptr;
    
    logc_lex_init(&scanner);

    if (argc > 2 && argv[1][0] == '-' && (argv[1][1] == 'f' || argv[1][1] == 'F'))
    {
        FILE *f = fopen(argv[2], "r");
        fseek(f, 0, SEEK_END);
        auto size = ftell(f);
        char *str = new char[size];
        fseek(f, 0, SEEK_SET);
        fread(str, 1, size, f);
        buffer = logc__scan_string(str, scanner);
        delete[] str;
        fclose(f);
    }
    else buffer = logc__scan_string(argv[1], scanner);
    
    int r = -(int)logc_parse(scanner, result);
    if (r != 0) goto end;
    
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
    
    std::cout << "Conversion to implicational logic: ";
    result = convertImplicational(result);
    traverse(result);
    std::cout << std::endl;
    
end:
    logc__delete_buffer(buffer, scanner);
    logc_lex_destroy(scanner);
    return r;
}
