#ifndef IF_HPP
#define IF_HPP

#include "ElementIf.hpp"

#include <vector>

namespace TAU
{

class If
{
public:
private:
    Condition                condition;
    std::vector<ElementIf *> elements;
};

}

#endif
