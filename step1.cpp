#include "ExpTree.hpp"

ExpPtr convertIntuitionistic(ExpPtr tree)
{
    return simplify(std::make_shared<ExpTree>(ExpTree::Type::Negation, std::make_shared<ExpTree>(ExpTree::Type::Negation, tree)), true);
}
