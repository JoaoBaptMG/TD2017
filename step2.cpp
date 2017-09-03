#include "ExpTree.hpp"
#include <new>
#include <unordered_map>

static ExpPtr preprocess(ExpPtr tree)
{
    if (!tree) return nullptr;
        
    tree->setLeftNode(preprocess(tree->getLeftNode()));
    tree->setRightNode(preprocess(tree->getRightNode()));
        
    // Turn !A into A -> #
    if (tree->getType() == ExpTree::Type::Negation)
        return std::make_shared<ExpTree>(ExpTree::Type::Implies, tree->getLeftNode(), ExpTree::False);
    // Turn @ into # -> #
    else if (tree->isTruehood())
        return std::make_shared<ExpTree>(ExpTree::Type::Implies, ExpTree::False, ExpTree::False);
    
    return tree;
}

ExpPtr convertImplicational(ExpPtr tree)
{
    // Get rid with @'s and negations
    tree = preprocess(tree);

    

    return tree;
}
