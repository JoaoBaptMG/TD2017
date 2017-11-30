//=====================================================================
// ExpTree.hpp
//=====================================================================
// Aqui é feita a definição da classe ExpTree, que representa um nó na
// árvore sintática usada para a transformação das expressões lógicas
//=====================================================================

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <functional>

class ExpTree
{
public:
    enum class Type { Terminal, Negation, Or, And, Implies };
    
private:
    Type type;
    std::string name;
    std::shared_ptr<ExpTree> node1 = nullptr, node2 = nullptr;
    size_t hash;
    void computeHash();
    
public:
    ExpTree(std::string name) : type(Type::Terminal), name(name), node1(), node2() { computeHash(); }
    ExpTree(Type type, std::shared_ptr<ExpTree> node1, std::shared_ptr<ExpTree> node2 = nullptr)
    : type(type), name(), node1(node1), node2(node2) { computeHash(); }
    ~ExpTree() {}

    static std::shared_ptr<ExpTree> True;
    static std::shared_ptr<ExpTree> False;

    bool isTruehood() const { return type == Type::Terminal && name == "@"; }
    bool isFalsehood() const { return type == Type::Terminal && name == "#"; }
    auto getType() const { return type; }
    auto getName() const { return name; }
    auto getLeftNode() const { return node1; }
    auto getRightNode() const { return node2; }
    auto getNode(size_t i) const { return i == 0 ? node1 : node2; }
    bool operator==(const ExpTree& other) const;
    
    void setLeftNode(const std::shared_ptr<ExpTree>& node1);
    void setRightNode(const std::shared_ptr<ExpTree>& node2);
    void setNode(size_t i, const std::shared_ptr<ExpTree>& node);
    
    friend struct ExpTreeHash;
};

using ExpPtr = std::shared_ptr<ExpTree>;

struct ExpTreeHash
{
    using argument_type = ExpTree;
    using result_type = size_t;
    
    std::size_t operator()(const ExpTree& tree) const;
};

namespace std
{
    template <> struct hash<ExpTree>
    {
        std::size_t operator()(const ExpTree& tree) const
        {
            return ExpTreeHash()(tree);
        }
    };
}

enum class SimplifyMode { Classical, Intuitionistic, Implicational };
ExpPtr simplify(ExpPtr, SimplifyMode mode = SimplifyMode::Classical);

