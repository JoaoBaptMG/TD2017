//=========================================================================
// step2.cpp
//=========================================================================
// Essa é a parte mais difícil, confesso...
//=========================================================================

#include "ExpTree.hpp"
#include <new>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cassert>

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

void traverseTerminals(ExpPtr tree, std::unordered_map<ExpPtr,ExpPtr>& terminalList, unsigned long& number)
{
    if (!tree) return;

    traverseTerminals(tree->getLeftNode(), terminalList, number);
    traverseTerminals(tree->getRightNode(), terminalList, number);
    
    auto it = std::find_if(terminalList.begin(), terminalList.end(),
    [&tree](const std::pair<ExpPtr,ExpPtr>& pair)
    {
        return *pair.first == *tree;
    });
    
    terminalList.emplace(tree, it != terminalList.end() ? it->second : std::make_shared<ExpTree>(std::to_string(++number)));
}

void buildFormulas(ExpPtr tree, std::unordered_map<ExpPtr,ExpPtr>& terminalList, std::vector<ExpPtr>& formulaList)
{
    if (!tree) return;

    buildFormulas(tree->getLeftNode(), terminalList, formulaList);
    buildFormulas(tree->getRightNode(), terminalList, formulaList);
    
    assert(tree->getType() != ExpTree::Type::Negation);
    assert(!tree->isTruehood());
    
    auto it = terminalList.find(tree);
    assert(it != terminalList.end());
    auto terminal = it->second;
    
    switch (tree->getType())
    {
        case ExpTree::Type::Terminal:
            formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, tree, terminal));
            formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, terminal, tree));
            break;
        case ExpTree::Type::Implies:
        {
            auto it1 = terminalList.find(tree->getLeftNode()), it2 = terminalList.find(tree->getRightNode());
            assert(it1 != terminalList.end() && it2 != terminalList.end());
            auto imply = std::make_shared<ExpTree>(ExpTree::Type::Implies, it1->second, it2->second);
            formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, terminal, imply));
            formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, imply, terminal));
        } break;
        case ExpTree::Type::And:
        {
            auto it1 = terminalList.find(tree->getLeftNode()), it2 = terminalList.find(tree->getRightNode());
            assert(it1 != terminalList.end() && it2 != terminalList.end());
            formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, terminal, it1->second));
            formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, terminal, it2->second));
            formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, it1->second,
                std::make_shared<ExpTree>(ExpTree::Type::Implies, it2->second, terminal)));
        } break;
        case ExpTree::Type::Or:
        {
            auto it1 = terminalList.find(tree->getLeftNode()), it2 = terminalList.find(tree->getRightNode());
            assert(it1 != terminalList.end() && it2 != terminalList.end());
            formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, it1->second, terminal));
            formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, it2->second, terminal));
            
            for (const auto& p : terminalList)
            {
                auto newterm = p.second;
                
                auto imp1 = std::make_shared<ExpTree>(ExpTree::Type::Implies, it2->second, newterm);
                auto imp2 = std::make_shared<ExpTree>(ExpTree::Type::Implies, imp1, newterm);
                auto imp3 = std::make_shared<ExpTree>(ExpTree::Type::Implies, it1->second, newterm);
                auto imp4 = std::make_shared<ExpTree>(ExpTree::Type::Implies, imp3, imp2);
                formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, terminal, imp4));
            }
        } break;
        default: break;
    }
    
    auto falseIt = terminalList.find(ExpTree::False);
    assert(falseIt != terminalList.end());
    formulaList.emplace_back(std::make_shared<ExpTree>(ExpTree::Type::Implies, falseIt->second, terminal));
}

ExpPtr convertImplicational(ExpPtr tree)
{
    // Transforma negações e @s em fórmulas "palpáveis" pelo algoritmo apresentado
    tree = preprocess(tree);
    
    std::unordered_map<ExpPtr,ExpPtr> terminalList;
    std::vector<ExpPtr> formulaList;

    unsigned long number = 0;
    traverseTerminals(tree, terminalList, number);
    if (terminalList.find(ExpTree::False) == terminalList.end())
        terminalList.emplace(ExpTree::False, std::make_shared<ExpTree>(std::to_string(++number)));
    
    buildFormulas(tree, terminalList, formulaList);
    
    std::reverse(formulaList.begin(), formulaList.end());
    auto result = terminalList.find(tree)->second;
    for (ExpPtr formula : formulaList)
        result = std::make_shared<ExpTree>(ExpTree::Type::Implies, formula, result);

    return simplify(result, SimplifyMode::Implicational);
}
