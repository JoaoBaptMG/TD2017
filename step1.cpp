#include "ExpTree.hpp"

ExpPtr convertIntuitionistic(ExpPtr tree)
{
    if (tree->getType() == ExpTree::Type::Root)
        tree->setLeftNode(convertIntuitionistic(tree->getLeftNode()));
    else tree = std::make_shared<ExpTree>(ExpTree::Type::Negation, std::make_shared<ExpTree>(ExpTree::Type::Negation, tree));
    
    return tree;
}
