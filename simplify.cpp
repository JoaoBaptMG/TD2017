//=====================================================================
// simplify.cpp
//=====================================================================
// Nesse código é feita a simplificação a partir de algumas regras.
// Isso foi feito mais como um exercício e um exemplo para a
// manipulação de árvores de expressão
//=====================================================================

#include "ExpTree.hpp"
#include <new>

// Substitua para "false" para desabilitar a simplificação
constexpr bool GlobalSimplificationEnabled = true;

ExpPtr simplify(ExpPtr tree, bool intuitionistic)
{
    if (!GlobalSimplificationEnabled) return tree;
    // Ignorar ponteiros vazios
    if (!tree) return nullptr;

    // Simplificar recursivamente
    tree->setLeftNode(simplify(tree->getLeftNode(), intuitionistic));
    tree->setRightNode(simplify(tree->getRightNode(), intuitionistic));

    // Se o nó for do tipo A -> B
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
    // Se o nó for do tipo !A
    else if (tree->getType() == ExpTree::Type::Negation)
    {
        // na lógica clássica, !!A: A; na intuicionista, !!!A: !A
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
    // Se o nó for do tipo A & B
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
    // Se o nó for do tipo A | B
    else if (tree->getType() == ExpTree::Type::Or)
    {
        auto node = tree;
    
        // @ | A: @
        // A | @: @
        if (tree->getLeftNode()->isTruehood() || tree->getRightNode()->isTruehood())
            return ExpTree::True;
        // !A | A: @, não é válido no intuicionista
        else if (!intuitionistic && tree->getLeftNode()->getType() == ExpTree::Type::Negation &&
            *tree->getRightNode() == *tree->getLeftNode()->getLeftNode())
            return ExpTree::True;
        // A | !A: @, não é válido no intuicionista
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
