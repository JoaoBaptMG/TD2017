#include "ExpTree.hpp"
#include <new>

constexpr bool GlobalSimplificationEnabled = true;

ExpPtr simplify(ExpPtr tree, bool intuitionistic)
{
    if (!GlobalSimplificationEnabled) return tree;

    if (!tree) return nullptr;
    if (tree->getType() == ExpTree::Type::Terminal)
        return tree;
        
    for (size_t i : { 0, 1 })
    {
        if (!tree->getNode(i)) continue;
    
        tree->setNode(i, simplify(tree->getNode(i), intuitionistic));

        if (tree->getNode(i)->getType() == ExpTree::Type::Implies)
        {
            auto node = tree->getNode(i);
            
            if (*node->getLeftNode() == *node->getRightNode())
                tree->setNode(i, ExpTree::True);
            else if (node->getLeftNode()->getType() == ExpTree::Type::Negation &&
                *node->getRightNode() == *node->getLeftNode()->getLeftNode())
                tree->setNode(i, node->getLeftNode());
            else if (node->getRightNode()->getType() == ExpTree::Type::Negation &&
                *node->getLeftNode() == *node->getRightNode()->getLeftNode())
                tree->setNode(i, node->getRightNode());
            else if (node->getLeftNode()->isTruehood() || node->getRightNode()->isTruehood())
                tree->setNode(i, node->getRightNode());
            else if (node->getRightNode()->isFalsehood())
                tree->setNode(i, std::make_shared<ExpTree>(ExpTree::Type::Negation, node->getLeftNode()));
            else if (node->getLeftNode()->isFalsehood())
                tree->setNode(i, ExpTree::True);
        }
    
        if (tree->getNode(i)->getType() == ExpTree::Type::Negation)
        {
            auto node = tree->getNode(i);
            
            if (node->getLeftNode()->getType() == ExpTree::Type::Negation &&
                (!intuitionistic || node->getLeftNode()->getLeftNode()->getType() == ExpTree::Type::Negation))
                    tree->setNode(i, node->getLeftNode()->getLeftNode());
            else if (node->getLeftNode()->isFalsehood())
                tree->setNode(i, ExpTree::True);
            else if (node->getLeftNode()->isTruehood())
                tree->setNode(i, ExpTree::False);
        }

        if (tree->getNode(i)->getType() == ExpTree::Type::And)
        {
            auto node = tree->getNode(i);
        
            if (node->getLeftNode()->isFalsehood() || node->getRightNode()->isFalsehood())
                tree->setNode(i, ExpTree::False);
            else if (node->getLeftNode()->getType() == ExpTree::Type::Negation &&
                *node->getRightNode() == *node->getLeftNode()->getLeftNode())
                tree->setNode(i, ExpTree::False);
            else if (node->getRightNode()->getType() == ExpTree::Type::Negation &&
                *node->getLeftNode() == *node->getRightNode()->getLeftNode())
                tree->setNode(i, ExpTree::False);
            else if (*node->getLeftNode() == *node->getRightNode())
                tree->setNode(i, node->getLeftNode());
            else if (node->getLeftNode()->isTruehood())
                tree->setNode(i, node->getRightNode());
            else if (node->getRightNode()->isTruehood())
                tree->setNode(i, node->getLeftNode());
        }
        
        if (tree->getNode(i)->getType() == ExpTree::Type::Or)
        {
            auto node = tree->getNode(i);
        
            if (node->getLeftNode()->isTruehood() || node->getRightNode()->isTruehood())
                tree->setNode(i, ExpTree::True);
            else if (!intuitionistic && node->getLeftNode()->getType() == ExpTree::Type::Negation &&
                *node->getRightNode() == *node->getLeftNode()->getLeftNode())
                tree->setNode(i, ExpTree::True);
            else if (!intuitionistic && node->getRightNode()->getType() == ExpTree::Type::Negation &&
                *node->getLeftNode() == *node->getRightNode()->getLeftNode())
                tree->setNode(i, ExpTree::True);
            else if (*node->getLeftNode() == *node->getRightNode())
                tree->setNode(i, node->getLeftNode());
            else if (node->getLeftNode()->isFalsehood())
                tree->setNode(i, node->getRightNode());
            else if (node->getRightNode()->isFalsehood())
                tree->setNode(i, node->getLeftNode());
        }
        
    }

    return tree;
}
