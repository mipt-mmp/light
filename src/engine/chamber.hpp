#ifndef CHAMBER_HPP
#define CHAMBER_HPP

#include "surface.hpp"
#include "physconstants.hpp"

namespace phys {

class Chamber {
  std::vector<Surface*> m_surfaces;
  std::vector<RefractiveIndex> m_ns;
  Frequency m_frequency;
 public:
  struct Metrics {
    std::vector<LightSource> sources;
    LengthVal diff;
  };

  void addSurface(Surface* surface);

  void update();

  void setZ(int n, Length z) {
    m_surfaces[n]->setZ(z);
  }

  void setRefractiveIndex(size_t i, RefractiveIndex n) {
    m_ns[i] = n;
  }

  void setLight(Frequency f) {
    m_frequency = f;
  }

  void clear() {
    m_surfaces.clear();
    m_ns.clear();
  }

  ~Chamber();
};

}  // namespace phys

#endif
