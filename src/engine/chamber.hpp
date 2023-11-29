#ifndef CHAMBER_HPP
#define CHAMBER_HPP

#include "surface.hpp"
#include "physconstants.hpp"

namespace phys {

class Chamber {
  std::vector<Surface*> m_surfaces;
  WavyEnvironment m_env;
 public:
  struct Metrics {
    std::vector<LightSource> sources;
    LengthVal diff;
  };

  void addSurface(Surface* surface);

  void update();

  void setLight(Frequency f) {
    m_env = WavyEnvironment(f);
    std::cerr << m_env.waveLength << '\n';
  }

  [[deprecated]] Metrics GetMetrics();

  ~Chamber();
};

}  // namespace phys

#endif
