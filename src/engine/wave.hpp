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
    

    EWave traveled(LengthVal l) const;

    EWave& operator+=(const EWave& oth) {
        if (oth.m_frequency != this->m_frequency) {
            std::cerr << "Can't sum waves with different freq\n";
            abort();
        } // Overwise we can't sum waves like vectors;
        
        m_wave += oth.m_wave;
        return *this;
    }

    auto getIntensity() const {
        return m_wave.Len2();
    }

    auto getFreq() const {
        return m_frequency;
    }
};

}

#endif /* ENGINE_WAVE_HPP */
