#include "engine.hpp"
#include "wave.hpp"
namespace phys {

void Engine::calculate() {
    Frequency f; // FIXME: 
    for(size_t i = 0; i < m_surfaces.size() - 1; ++i) {
        auto srcs = m_surfaces[i]->getSrcs();
        auto dstPoses = m_surfaces[i]->getDsts();
        std::vector<EWave> dstWaves;
        for(const auto& dstPos : dstPoses) {
            EWave wave{f};
            for(const auto& [srcWave, srcPos]: srcs) {
                wave += srcWave.traveled((dstPos - srcPos).Len());
            }
            dstWaves.push_back(wave);
        }
        m_surfaces[i+1]->receiveWaves(dstWaves);
    }
}

}
