#ifndef ENGINE_WAVE_HPP
#define ENGINE_WAVE_HPP
#include "units.hpp"
namespace phys {

// Now spheric wave
class EWave {
    Complex<EFieldVal> m_wave;
    Frequency m_frequency; // rad / s
public:
    EWave(Frequency frequency, EFieldVal val = {}, NoUnit phase_0 = {}) : m_wave(val, EFieldVal{0}), m_frequency(frequency) {
        m_wave.rotate(phase_0);
    }

    EWave(EFieldVal val, Length len_wave, NoUnit phase_0);
    

    EWave traveled(Distance l) const;

    EWave& operator+=(const EWave& oth) {
        assert(oth.m_frequency == this->m_frequency); // Overwise we can't sum waves like vectors;
        m_wave += oth.m_wave;
        return *this;
    }

    auto getIntensity() const {
        return m_wave.Len2();
    }
};

}

#endif /* ENGINE_WAVE_HPP */
