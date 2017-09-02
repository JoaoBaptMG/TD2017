#include "ExpTree.hpp"
#include <new>

static ExpPtr preprocess(ExpPtr tree)
{
    if (!tree) return nullptr;
    if (tree->getType() == ExpTree::Type::Terminal)
        return tree;
        
    for (size_t i : { 0, 1 })
    {
        if (!tree->getNode(i)) continue;
    
        tree->setNode(i, preprocess(tree->getNode(i)));
        auto node = tree->getNode(i);
        
        if (node->getType() == ExpTree::Type::Negation)
            tree->setNode(i, std::make_shared<ExpTree>(ExpTree::Type::Implies, node->getLeftNode(), ExpTree::False));
        else if (node->isTruehood())
            tree->setNode(i, std::make_shared<ExpTree>(ExpTree::Type::Implies, ExpTree::False, ExpTree::False));
    }
    
    return tree;
}

ExpPtr convertImplicational(ExpPtr tree)
{
    tree = preprocess(tree);

    return tree;
}
