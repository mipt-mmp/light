#include "chamber.hpp"

namespace phys {

void Chamber::addSurface(Surface* surface) {
  m_surfaces.push_back(surface);
}

void 
Chamber::update() {
  if(m_surfaces.empty()) {
    return;
  }
  
  std::sort(m_surfaces.begin(), m_surfaces.end(), [](const Surface* lhs, const Surface* rhs) -> bool {return lhs->getZ() < rhs->getZ();});
  for(size_t i = 0; i < m_surfaces.size()-1; ++i) {
    m_surfaces[i+1]->setEnvironment(m_env);
    m_surfaces[i+1]->update(m_surfaces[i]->getSrcs());
  }
}



auto Chamber::GetMetrics() -> Metrics {
  #if 0
  auto sources = m_surfaces.back()->getSrcs();

  auto minDiff = *sources.front().first.getIntensity();
  auto intens = minDiff;

  auto bestOne = sources.front().second;

  for (auto& source : sources) {
    auto diff = std::abs(*source.first.getIntensity() - intens);

    if ((diff < minDiff) && (source.second != bestOne)) {
      minDiff = diff;
      bestOne = source.second;
    }
  }

  auto period = (sources.front().second - bestOne).Y();
#endif
  return Metrics{};
}

Chamber::~Chamber() {
  for (auto* surf : m_surfaces) {
    delete surf;
  }
}
}
