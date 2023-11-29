#ifndef ENGINE_SURFACE_HPP
#define ENGINE_SURFACE_HPP
#include "units.hpp"
#include "wave.hpp"
#include <functional>

namespace phys {

using LightSource = std::pair<EWave, Position>;

class EWave;
class Surface {
 public:
  virtual ~Surface();

  virtual const std::vector<LightSource>& getSrcs() const = 0;

  virtual void update(const std::vector<LightSource>& srcs);

  virtual std::pair<Position, Position> getRect() const = 0;

  static void recalculate(const std::vector<LightSource>& src, std::vector<LightSource>& dst, WavyEnvironment env);

  [[deprecated("Use update")]] virtual bool setParent(Surface* ) {return true;}

  virtual void 
  setEnvironment(WavyEnvironment env) {
    m_env = env;
  }

  LengthVal getZ() const {
    auto [beg, end] = getRect();
    return (beg.Z() + end.Z()) / 2;
  }

protected:
  WavyEnvironment m_env;
};

//====================================================================================/
//================================< Point Lights >====================================/
//====================================================================================/

class PointLights : public Surface {
 public:
  void 
  addSource(LightSource source) {
    if (!m_sources.empty()) {
      if (source.second.Z() != m_sources.front().second.Z()) {
        std::cerr << "You should add sources only with same Z coord\n";
        abort();
      }
    }

    m_rect.first  = std::min(m_rect.first,  source.second);
    m_rect.second = std::max(m_rect.second, source.second);
    m_sources.push_back(std::move(source));
  }

  virtual const std::vector<LightSource>& 
  getSrcs() const override {
    return m_sources;
  }

  virtual std::pair<Position, Position> 
  getRect() const override {return m_rect;}

  virtual void 
  update(const std::vector<LightSource>&) override {}

  virtual ~PointLights() override;

 private:
  std::vector<LightSource> m_sources;
  std::pair<Position, Position> m_rect;
  Frequency m_frequency;
};


//====================================================================================/
//=====================================< Points Barrier >=============================/
//====================================================================================/


class PointsBarrier : public Surface {
 public:
  void addHole(Position hole);

  void setHolePos(size_t i, Position hole);

  virtual const std::vector<LightSource>& 
  getSrcs() const override {return m_sources;}

  virtual void update(const std::vector<LightSource>&) override;

  virtual std::pair<Position, Position> 
  getRect() const override {return m_rect;}

  virtual ~PointsBarrier() override;

 private:
  void updateRect();

  std::vector<LightSource> m_sources;
  std::pair<Position, Position> m_rect;
};


//====================================================================================/
//===================================< Displayer >====================================/
//====================================================================================/


class Displayer : public Surface {
  static constexpr Unit<num_t> XSteps = Unit<num_t>{1000.0};
  static constexpr Unit<num_t> YSteps = Unit<num_t>{1000.0};

 public:
  Displayer(Position corner, Position sizes)
      : m_corner(corner), m_sizes(sizes) {}

  Position getCorner() { return m_corner; }

  Position getSizes() { return m_sizes; }

  void setZPos(Length val) {
    m_corner[2] = val;
  }

  virtual void update(const std::vector<LightSource>&) override;

  virtual std::pair<Position, Position> 
  getRect() const override {return {m_corner, m_corner};}

  // We assume that every point of displayer is a light source (we will draw
  // intensivity of them)
  virtual const std::vector<LightSource>& 
  getSrcs() const override {return m_dst;}

  virtual ~Displayer() override;

 private:
  void genDisplayMatrix();

  std::vector<LightSource> m_dst;
  Position m_corner;
  Position m_sizes;
};

//====================================================================================/
//===================================< Contig Surface >===============================/
//====================================================================================/

class ContigSurface : public Surface {
public:
  ContigSurface(Position pos);
  ContigSurface(Position pos, std::function<Position(Position)> transform  );
  ContigSurface(Position pos, std::function<bool    (Position)> transparent);
  ContigSurface(Position pos, std::function<Position(Position)> transform, std::function<bool (Position)> transparent);

  virtual void update(const std::vector<LightSource>& src) override;
  
  virtual const std::vector<LightSource>& 
  getSrcs() const override {return m_srcs;}

  virtual std::pair<Position, Position> 
  getRect() const override {return m_rect;}

  void setResolution(size_t resolution) {
    m_resolution = resolution;
    genSurface();
  }

  void setPosition(Position pos) {
    m_corner = pos;
    genSurface();
  }




private:
  Position m_corner;
  std::function<bool    (Position)> m_isTransparent  = [](Position){return true;};
  std::function<Position(Position)> m_transformation = [](Position p){return p;};

  size_t m_resolution = 1;
  void genSurface();

  std::pair<Position, Position> m_rect;
  std::vector<LightSource> m_srcs;
};


}  // namespace phys

#endif /* ENGINE_SURFACE_HPP */
