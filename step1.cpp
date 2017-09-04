//=======================================================================
// step1.cpp
//=======================================================================
// Aqui é feito o primeiro passo da tradução das expressões. Se A é uma
// expressão clássica, então !!A pode ser provado na parte intuicionista
//=======================================================================

#include "ExpTree.hpp"

ExpPtr convertIntuitionistic(ExpPtr tree)
{
    return simplify(std::make_shared<ExpTree>(ExpTree::Type::Negation, std::make_shared<ExpTree>(ExpTree::Type::Negation, tree)),
        SimplifyMode::Intuitionistic);
}
