#ifndef CHAMBER_HPP
#define CHAMBER_HPP

#include "surface.hpp"

namespace phys {

class Chamber {
  std::vector<Surface*> m_surfaces;

 public:
  struct Metrics {
    std::vector<LightSource> sources;
    LengthVal diff;
  };

  void AddSurface(Surface* surface);

  Metrics GetMetrics();

  ~Chamber();
};

}  // namespace phys

#endif
