//=========================================================================
// step2.cpp
//=========================================================================
// Essa é a parte mais difícil, confesso...
//=========================================================================

#include "ExpTree.hpp"
#include <new>
#include <unordered_map>

static ExpPtr preprocess(ExpPtr tree)
{
    if (!tree) return nullptr;
        
    tree->setLeftNode(preprocess(tree->getLeftNode()));
    tree->setRightNode(preprocess(tree->getRightNode()));
        
    // !A vira A -> #
    if (tree->getType() == ExpTree::Type::Negation)
        return std::make_shared<ExpTree>(ExpTree::Type::Implies, tree->getLeftNode(), ExpTree::False);
    // @ vira # -> #
    else if (tree->isTruehood())
        return std::make_shared<ExpTree>(ExpTree::Type::Implies, ExpTree::False, ExpTree::False);
    
    return tree;
}

ExpPtr convertImplicational(ExpPtr tree)
{
    // Transforma negações e @s em fórmulas "palpáveis" pelo algoritmo apresentado
    tree = preprocess(tree);

    // TODO
    // Para criar uma subexpressão (uma árvore "solta"), você usa o código
    // auto node = std::make_shared<ExpTree>(TIPO, nó1, nó2);
    // Tipo pode ser ExpTree::Type::And, Or ou Implies, mas aqui você só
    // vai se preocupar com tipos Implies.
    // Para criar um terminal (ou seja, uma variável), você só coloca o nome
    // dela: auto term = std::make_shared<ExpTree>("nome");
    // Para se referir aos terminais @ e # use ExpTree::True e ExpTree::False
    // Como você vai mexer com nomes "autogerados", para transformar um número
    // em uma string: auto str = std::to_string(número);
    // Funções interessantes:
    // tree->getLeftNode() e tree->getRightNode(), é o seu meio de fazer
    // a travessia na árvore
    // tree->getType(), retorna o tipo do nó, ExpTree::Type::And, Or, Implies,
    // Negation ou Terminal. Aqui não existirão mais nós Negation
    // Para fazer IGUALDADE entre árvores, compare os apontados, não os
    // ponteiros (ou seja, faça *tree1 == *tree2, não tree1 == tree2)
    // tree->isTruehood(): vê se a árvore é o terminal @
    // tree->isFalsehood(): vê se a árvore é o terminal #
    // Por fim, aconselho estudar os algoritmos std::find e um pouco mais do
    // std::vector e std::pair
    // Boa sorte!

    return tree;
}
