#include "wave.hpp"
#include "physconstants.hpp"
namespace phys {

WavyEnvironment::WavyEnvironment(Frequency f, RefractiveIndex n, Length loss) :
    waveLength(consts::c / (f * n)),
    eLossCoeff(loss)
{

}


EWave::EWave(EFieldVal val, NoUnit phase_0) : m_wave{val}{
    m_wave.rotate(phase_0);
}


}
