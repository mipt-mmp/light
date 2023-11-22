#ifndef ENGINE_SURFACE_HPP
#define ENGINE_SURFACE_HPP
#include "units.hpp"
#include "wave.hpp"

namespace phys {

using LightSource = std::pair<EWave, Position>;

class EWave;
class Surface {
 public:
  virtual ~Surface();

  virtual std::vector<LightSource> getSrcs() const = 0;
  virtual bool setParent(Surface* parent) = 0;
};

class Lights : public Surface {
 public:
  void AddSource(LightSource source) {
    if (!m_sources.empty()) {
      if (source.second.Z() != m_sources.front().second.Z()) {
        std::cerr << "You should add sources only with same Z coord\n";
        abort();
      }
    }

    m_sources.push_back(std::move(source));
  }

  virtual std::vector<LightSource> getSrcs() const override {
    return m_sources;
  }

  virtual bool setParent(Surface*) override { return true; }

  virtual ~Lights() override;

 private:
  std::vector<LightSource> m_sources;
};

class Barrier : public Surface {
 public:
  void AddHole(Position hole);
  virtual std::vector<LightSource> getSrcs() const override;

  virtual bool setParent(Surface* surface) override {
    if (m_parent != nullptr) {
      return false;
    }

    m_parent = surface;
    return true;
  }

  virtual ~Barrier() override;

 private:
  Surface* m_parent = nullptr;

  std::vector<Position> m_holes;
};

class Displayer : public Surface {
  static constexpr Unit<num_t> XSteps = Unit<num_t>{1000.0};
  static constexpr Unit<num_t> YSteps = Unit<num_t>{1000.0};

 public:
  Displayer(Position corner, Position sizes)
      : m_corner(corner), m_sizes(sizes) {}

  Position GetCorner() { return m_corner; }

  Position GetSizes() { return m_sizes; }

  // We assume that every point of displayer is a light source (we will draw
  // intensivity of them)
  virtual std::vector<LightSource> getSrcs() const override;

  virtual bool setParent(Surface* surface) override {
    if (m_parent != nullptr) {
      return false;
    }

    m_parent = surface;
    return true;
  }

  virtual ~Displayer() override;

 private:
  Surface* m_parent = nullptr;

  Position m_corner;
  Position m_sizes;
};

}  // namespace phys

#endif /* ENGINE_SURFACE_HPP */
