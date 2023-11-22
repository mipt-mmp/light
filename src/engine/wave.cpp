#include "wave.hpp"
#include "physconstants.hpp"
namespace phys {

EWave::EWave(EFieldVal val, Length len_wave, NoUnit phase_0) : m_wave(val, EFieldVal{0}), m_frequency(num_t{2.} * consts::pi * consts::c / len_wave) {
    m_wave.rotate(phase_0);
}

EWave
EWave::traveled(Distance l) const {
    //m_wave /= l; // FIXME: We must lose energy ~ 1 / r
    EWave wave = *this;
    wave.m_wave.rotate(l * m_frequency / consts::c);
    return wave;
}

}
