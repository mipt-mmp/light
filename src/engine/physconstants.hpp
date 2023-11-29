#ifndef ENGINE_PHYSCONSTANTS_HPP
#define ENGINE_PHYSCONSTANTS_HPP

#include <numbers>
#include "units.hpp"

namespace phys {
namespace consts {

constexpr const auto G = 6.67430e-11_H * 1._m * 1._m / 1._kg / 1._kg;
constexpr const auto k = 1.38064e-23_J / 1_K;
constexpr const auto Dalton = 1.660e-27_kg;
constexpr const auto c = 299'792'458_m / 1_sec;
constexpr const NoUnit pi{std::numbers::pi};

constexpr const Frequency red   = 450_Hz * Tera;
constexpr const Frequency green = 550_Hz * Tera;
constexpr const Frequency blue  = 650_Hz * Tera;

} // namespace consts

namespace config {

constexpr const num_t DT_NORMALIZER = 4e-5;
constexpr const num_t FLUCTATION_DEGREE = 1e3;

} // namespace config
} // namespace phys

#endif /* ENGINE_PHYSCONSTANTS_HPP */
