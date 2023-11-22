#include "chamber.hpp"

namespace phys {

void Chamber::AddSurface(Surface* surface) {
  if (!m_surfaces.empty()) {
    if (!surface->setParent(m_surfaces.back())) {
      std::cerr << "You're adding already inited surf\n";
      return;
    }
  }

  m_surfaces.push_back(surface);
}

auto Chamber::GetMetrics() -> Metrics {
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
  return Metrics{std::move(sources), period};
}

Chamber::~Chamber() {
  for (auto* surf : m_surfaces) {
    delete surf;
  }
}

}
