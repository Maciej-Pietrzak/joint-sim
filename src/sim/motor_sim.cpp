#include "jointsim/sim/motor_sim.hpp"

#include <algorithm>
#include <cmath>

namespace jointsim::sim {

MotorSim::MotorSim(const MotorParams& params) : params_(params) {}

void MotorSim::setTorque(double torqueNm) {
    // Silnik fizycznie nie da więcej niż swój limit.
    appliedTorqueNm_ = std::clamp(torqueNm, -params_.torqueLimitNm, params_.torqueLimitNm);
}

void MotorSim::step(double dtS) {
    const double tau = appliedTorqueNm_;

    // 1) Spoczynek: tarcie suche trzyma przegub, dopóki moment go nie przekroczy.
    if (velocityRadPerS_ == 0.0 && std::abs(tau) <= params_.coulombNm) {
        return;
    }

    // 2) Kierunek tarcia: przeciwny do ruchu, a gdy stoimy, przeciwny do momentu.
    const double direction =
        (velocityRadPerS_ != 0.0) ? std::copysign(1.0, velocityRadPerS_) : std::copysign(1.0, tau);
    const double frictionNm =
        params_.viscousNmsPerRad * velocityRadPerS_ + params_.coulombNm * direction;

    // 3) Newton: J * alpha = tau - tarcie.
    const double accel = (tau - frictionNm) / params_.inertiaKgM2;

    // 4) Euler semi-implicit: najpierw prędkość, potem pozycja z NOWEJ prędkości.
    double newVelocity = velocityRadPerS_ + accel * dtS;

    // 5) Tarcie może zatrzymać ruch, ale nie odwrócić go.
    if (velocityRadPerS_ != 0.0 && newVelocity * velocityRadPerS_ < 0.0) {
        newVelocity = 0.0;
    }

    velocityRadPerS_ = newVelocity;
    positionRad_ += velocityRadPerS_ * dtS;
}

}  // namespace jointsim::sim
