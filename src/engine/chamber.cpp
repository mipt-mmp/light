#include "chamber.hpp"

namespace phys {

void Chamber::addSurface(Surface* surface) {
  m_surfaces.push_back(surface);
  m_ns.push_back(1.__);
}

void 
Chamber::update() {
  if(m_surfaces.empty()) {
    return;
  }
  
  std::sort(m_surfaces.begin(), m_surfaces.end(), [](const Surface* lhs, const Surface* rhs) -> bool {return lhs->getZ() < rhs->getZ();});
  for(size_t i = 0; i < m_surfaces.size()-1; ++i) {
    m_surfaces[i+1]->setEnvironment({m_frequency, m_ns[i]});
    m_surfaces[i+1]->update(m_surfaces[i]->getSrcs());
  }
}

Chamber::~Chamber() {
  /*Fuck you memleaks*/
}

}
