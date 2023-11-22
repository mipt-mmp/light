#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP

#include "surface.hpp"

namespace phys {

class Engine {
    std::vector<Surface* > m_surfaces;

public:
    void calculate();
};

}

#endif /* ENGINE_ENGINE_HPP */
