#include "surface.hpp"

namespace phys {

Surface::~Surface() {}

Lights::~Lights() {}

Barrier::~Barrier() { m_parent = nullptr; }

void Barrier::AddHole(Position hole) {
  if (!m_holes.empty()) {
    if (hole.Z() != m_holes.front().Z()) {
      std::cerr << "You should add holes only with same Z coord\n";
      abort();
    }
  }

  m_holes.push_back(std::move(hole));
}

void Barrier::SetHolePos(size_t i, Position hole)
{
  m_holes[i] = hole;
}

std::vector<std::pair<EWave, Position>> Barrier::getSrcs() const {
  std::vector<std::pair<EWave, Position>> parentSources = m_parent->getSrcs();
  std::vector<std::pair<EWave, Position>> result = {};

  if (parentSources.empty()) {
    return result;
  }

  for (auto& hole : m_holes) {
    EWave newWave{parentSources.front().first.getFreq()};

    for (auto& parentWave : parentSources) {
      newWave += parentWave.first.traveled((hole - parentWave.second).Len());
    }

    result.push_back(std::make_pair(newWave, hole));
  }

  return result;
}

Displayer::~Displayer() { m_parent = nullptr; }

std::vector<std::pair<EWave, Position>> Displayer::getSrcs() const {
  std::vector<std::pair<EWave, Position>> parentSources = m_parent->getSrcs();
  std::vector<std::pair<EWave, Position>> result = {};

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
}  // namespace phys
