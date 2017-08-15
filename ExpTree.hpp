#pragma once

#include <iostream>
#include <string>
#include <memory>

class ExpTree
{
public:
    enum class Type { Terminal, Negation, Or, And, Implies };
    
private:
    Type type;
    std::string name;
    std::unique_ptr<ExpTree> node1, node2;
    ExpTree* parent;
    
public:
    ExpTree(std::string name) : type(Type::Terminal), name(name), node1(), node2() {}
    ExpTree(Type type, ExpTree* node1, ExpTree* node2 = nullptr)
    : type(type), name() { setLeftNode(node1); setRightNode(node2); }
    ~ExpTree() {}
    
    auto getType() const { return type; }
    auto getName() const { return name; }
    auto getLeftNode() const { return node1.get(); }
    auto getRightNode() const { return node2.get(); }
    
    auto releaseLeftNode()
    {
        node1->parent = nullptr; 
        return node1.release();
    }
    auto releaseRightNode()
    {
        node2->parent = nullptr;
        return node2.release();
    }
    void setLeftNode(ExpTree* n)
    {
        node1.reset(n);
        if (n) n->parent = this;
    }
    void setRightNode(ExpTree* n)
    {
        node2.reset(n);
        if (n) n->parent = this;
    }
    
    auto getParent() const { return parent; }
};

