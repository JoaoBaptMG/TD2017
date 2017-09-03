#include "ExpTree.hpp"
#include <new>

// Turn this "false" to disable simplification
constexpr bool GlobalSimplificationEnabled = true;

// Do some simplification techniques
ExpPtr simplify(ExpPtr tree, bool intuitionistic)
{
    if (!GlobalSimplificationEnabled) return tree;
    // Well, we can't simplify a terminal or a nonexistent tree
    if (!tree) return nullptr;

    // recursive simplify the child nodes first
    tree->setLeftNode(simplify(tree->getLeftNode(), intuitionistic));
    tree->setRightNode(simplify(tree->getRightNode(), intuitionistic));

    // If the node is of type A -> B
    if (tree->getType() == ExpTree::Type::Implies)
    {
        // A -> A: @
        if (*tree->getLeftNode() == *tree->getRightNode())
            return ExpTree::True;
        // A -> !A: !A
        else if (tree->getLeftNode()->getType() == ExpTree::Type::Negation &&
            *tree->getRightNode() == *tree->getLeftNode()->getLeftNode())
            return tree->getRightNode();
        // !A -> A: A
        else if (tree->getRightNode()->getType() == ExpTree::Type::Negation &&
            *tree->getLeftNode() == *tree->getRightNode()->getLeftNode())
            return tree->getRightNode();
        // @ -> A: A
        // A -> @: @
        else if (tree->getLeftNode()->isTruehood() || tree->getRightNode()->isTruehood())
            return tree->getRightNode();
        // A -> #: !A
        else if (tree->getRightNode()->isFalsehood())
            return std::make_shared<ExpTree>(ExpTree::Type::Negation, tree->getLeftNode());
        // # -> A: @
        else if (tree->getLeftNode()->isFalsehood())
            return ExpTree::True;
    }
    // If the node is of type !A
    else if (tree->getType() == ExpTree::Type::Negation)
    {
        // in classical mode, !!A: A; in intuitionistic mode, !!!A: !A
        if (tree->getLeftNode()->getType() == ExpTree::Type::Negation &&
            (!intuitionistic || tree->getLeftNode()->getLeftNode()->getType() == ExpTree::Type::Negation))
                return tree->getLeftNode()->getLeftNode();
        // !#: @
        else if (tree->getLeftNode()->isFalsehood())
            return ExpTree::True;
        // !@: #
        else if (tree->getLeftNode()->isTruehood())
            return ExpTree::False;
    }
    // If the node is of type A & B
    else if (tree->getType() == ExpTree::Type::And)
    {
        // # & A: #
        // A & #: #
        if (tree->getLeftNode()->isFalsehood() || tree->getRightNode()->isFalsehood())
            return ExpTree::False;
        // !A & A: #
        else if (tree->getLeftNode()->getType() == ExpTree::Type::Negation &&
            *tree->getRightNode() == *tree->getLeftNode()->getLeftNode())
            return ExpTree::False;
        // A & !A: #
        else if (tree->getRightNode()->getType() == ExpTree::Type::Negation &&
            *tree->getLeftNode() == *tree->getRightNode()->getLeftNode())
            return ExpTree::False;
        // A & A: A
        else if (*tree->getLeftNode() == *tree->getRightNode())
            return tree->getLeftNode();
        // @ & A: A
        else if (tree->getLeftNode()->isTruehood())
            return tree->getRightNode();
        // A & @: A
        else if (tree->getRightNode()->isTruehood())
            return tree->getLeftNode();
    }
    // If the node is of type A | B
    else if (tree->getType() == ExpTree::Type::Or)
    {
        auto node = tree;
    
        // @ | A: @
        // A | @: @
        if (tree->getLeftNode()->isTruehood() || tree->getRightNode()->isTruehood())
            return ExpTree::True;
        // !A | A: @, not valid in intuitionistic mode
        else if (!intuitionistic && tree->getLeftNode()->getType() == ExpTree::Type::Negation &&
            *tree->getRightNode() == *tree->getLeftNode()->getLeftNode())
            return ExpTree::True;
        // A | !A: @, not valid in intuitionistic mode
        else if (!intuitionistic && tree->getRightNode()->getType() == ExpTree::Type::Negation &&
            *tree->getLeftNode() == *tree->getRightNode()->getLeftNode())
            return ExpTree::True;
        // A | A: A
        else if (*tree->getLeftNode() == *tree->getRightNode())
            return tree->getLeftNode();
        // # | A: A
        else if (tree->getLeftNode()->isFalsehood())
            return tree->getRightNode();
        // A | #: A
        else if (tree->getRightNode()->isFalsehood())
            return tree->getLeftNode();
    }
    
    return tree;
}
