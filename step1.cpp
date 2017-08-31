#include "ExpTree.hpp"

ExpPtr convertIntuitionistic(ExpPtr tree)
{
    tree = std::make_shared<ExpTree>(ExpTree::Type::Negation, std::make_shared<ExpTree>(ExpTree::Type::Negation, tree));
    return tree;
}
