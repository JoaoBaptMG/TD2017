#include "ExpTree.hpp"

ExpTree _True("@"), _False("#");

ExpPtr ExpTree::True(&_True, [](ExpTree*){});
ExpPtr ExpTree::False(&_False, [](ExpTree*){});

bool ExpTree::operator==(const ExpTree& other) const
{
    if (hash != other.hash) return false;

    switch (type)
    {
        case Type::Terminal: return name == other.name;
        case Type::Negation: return *node1 == *other.node1;
        default: return *node1 == *other.node1 && *node2 == *other.node2;
    }
}

std::size_t ExpTreeHash::operator()(const ExpTree& tree) const
{
    return tree.hash;
}

void ExpTree::setLeftNode(const std::shared_ptr<ExpTree>& node)
{
    node1 = node;
    computeHash();
}

void ExpTree::setRightNode(const std::shared_ptr<ExpTree>& node)
{
    node2 = node;
    computeHash();
}

void ExpTree::setNode(size_t i, const std::shared_ptr<ExpTree>& node)
{
    (i == 0 ? node1 : node2) = node;
    computeHash();
}

void ExpTree::computeHash()
{
    using std::hash;
    using std::string;

    size_t h = hash<size_t>()((size_t)type);
    
    switch (type)
    {
        case ExpTree::Type::Terminal:
            h = (h * 5) ^ hash<std::string>()(name);
            break;
        default:
            h = (h * 23) ^ node2->hash;
        case ExpTree::Type::Negation:
            h = (h * 11) ^ node1->hash;
            break;
    }
    
    this->hash = h;
}
