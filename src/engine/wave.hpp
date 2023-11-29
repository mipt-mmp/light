#ifndef ENGINE_WAVE_HPP
#define ENGINE_WAVE_HPP
#include "physconstants.hpp"
#include "units.hpp"
namespace phys {

struct WavyEnvironment {
    WavyEnvironment(Length len, Length loss = 1_m) : waveLength(len), eLossCoeff(loss) {}

    WavyEnvironment(Frequency f = consts::red, RefractiveIndex n = 1__, Length loss = 1_m);

    Length waveLength;
    Length eLossCoeff;
};

// Now spheric wave
class EWave {
    Complex<EFieldVal> m_wave;
    EWave(Complex<EFieldVal> wave) : m_wave(wave) {}
public:
    EWave(EFieldVal val = EFieldVal{}, NoUnit phase_0 = NoUnit{});

    EWave traveled(LengthVal l, WavyEnvironment env) const {
        return EWave{(m_wave * (env.eLossCoeff / l)).rotate(l / env.waveLength)};
    }

    EWave& operator+=(const EWave& oth) {
        m_wave += oth.m_wave;
        return *this;
    }

    Brightness getIntensity() const {
        return m_wave.Len2();
    }

    void clear() {
        m_wave = {};
    }
};

}

#endif /* ENGINE_WAVE_HPP */
