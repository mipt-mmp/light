#ifndef ENGINE_SURFACE_HPP
#define ENGINE_SURFACE_HPP
#include "units.hpp"

namespace phys {

class EWave;
class Surface {
public:
    virtual ~Surface() = 0;
    
    virtual std::vector<std::pair<EWave, Position>> getSrcs() const = 0;

    virtual std::vector<Position> getDsts() const = 0;
    //FIXME: Interface is poor
    virtual void receiveWaves(const std::vector<EWave>& v) = 0;
};

}

#endif /* ENGINE_SURFACE_HPP */
