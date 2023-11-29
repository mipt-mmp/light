#include "surface.hpp"

namespace phys {

Surface::~Surface() {}

void 
Surface::update(const std::vector<LightSource>&) {}

void 
Surface::recalculate(const std::vector<LightSource>& src, std::vector<LightSource>& dst, WavyEnvironment env) {
  for(auto& [wave, pos] : dst) {
    wave.clear();
    for(const auto& [light, start] : src) {
      wave += light.traveled((start - pos).Len(), env);
    }
  }
}


//====================================================================================/
//================================< Point Lights >====================================/
//====================================================================================/

PointLights::~PointLights() {}

//====================================================================================/
//================================< Point Barrier >===================================/
//====================================================================================/

PointsBarrier::~PointsBarrier() {}

void 
PointsBarrier::addHole(Position hole) {
  if (!m_sources.empty()) {
    if (hole.Z() != m_sources.front().second.Z()) {
      std::cerr << "You should add holes only with same Z coord\n";
      abort();
    }
  }

  m_sources.emplace_back(EWave{}, std::move(hole));
  updateRect();
}

void 
PointsBarrier::setHolePos(size_t i, Position hole) {
  m_sources[i].second = hole;
  updateRect();
}

void 
PointsBarrier::update(const std::vector<LightSource>& src) {
  recalculate(src, m_sources, m_env);
}

void
PointsBarrier::updateRect() {
  m_rect = {m_sources[0].second, m_sources[0].second};
  for(auto [_, pos] : m_sources) {
    m_rect.first  = std::min(m_rect.first,  pos);
    m_rect.second = std::max(m_rect.second, pos);
  }
}


//====================================================================================/
//===================================< Displayer >====================================/
//====================================================================================/

Displayer::~Displayer() {}

void 
Displayer::genDisplayMatrix() {

}
#if 0
std::vector<LightSource> Displayer::getSrcs() const {
  std::vector<LightSource> parentSources = m_parent->getSrcs();
  std::vector<LightSource> result = {};

  if (parentSources.empty()) {
    return result;
  }

  Position xShift = m_sizes / XSteps;
  xShift.Y() = Unit<num_t, 1>{0.0};

  Position yShift = m_sizes / YSteps;
  yShift.X() = Unit<num_t, 1>{0.0};

  Position endPos = m_sizes + m_corner;

  for (Position curPos = m_corner; curPos.X() <= endPos.X(); curPos += xShift) {
    for (; curPos.Y() <= endPos.Y(); curPos += yShift) {
      EWave newWave{parentSources[0].first.getFreq()};

      for (auto& source : parentSources) {
        newWave += source.first.traveled((source.second - curPos).Len());
      }

      result.push_back(std::make_pair(newWave, curPos));
    }
    curPos.Y() = m_corner.Y();
  }

  return result;
}
#endif

void 
Displayer::update(const std::vector<LightSource>& src) {

}


//====================================================================================/
//===================================< Contig Surface >===============================/
//====================================================================================/


void 
ContigSurface::update(const std::vector<LightSource>& srcs) {
  recalculate(srcs, m_srcs, m_env);  
}


ContigSurface::ContigSurface(Position pos) : m_corner(pos) {genSurface();}

ContigSurface::ContigSurface(Position pos, std::function<Position(Position)> transform  ) : m_corner(pos), m_transformation(transform) {genSurface();}

ContigSurface::ContigSurface(Position pos, std::function<bool    (Position)> transparent) : m_corner(pos), m_isTransparent(transparent) {genSurface();}

ContigSurface::ContigSurface(Position pos, std::function<Position(Position)> transform, std::function<bool (Position)> transparent)
  : m_corner(pos), m_isTransparent(transparent), m_transformation(transform) {
    genSurface();
}

void 
ContigSurface::genSurface() {
  m_rect = {m_corner, m_corner};
  m_srcs.clear();
  for(size_t i = 0; i < m_resolution; ++i) {
    for(size_t j = 0; j < m_resolution; ++j) {
      num_t x = static_cast<double>(i) / static_cast<double>(m_resolution);
      num_t y = static_cast<double>(j) / static_cast<double>(m_resolution);
      Position pos = m_corner;
      pos[0] *= x;
      pos[1] *= y;
      if(m_isTransparent(pos)) {
        m_srcs.emplace_back(EWave{}, m_transformation(pos));
      }
      m_rect.first  = std::min(m_rect.first , m_srcs.back().second);
      m_rect.second = std::max(m_rect.second, m_srcs.back().second);
    }
  } 
}

}  // namespace phys
