#pragma once

#include "jointsim/actuator.hpp"

namespace jointsim::sim {

// Parametry fizyczne symulowanego silnika z przegubem.
struct MotorParams {
    double inertiaKgM2 = 0.01;          // J: bezwładność
    double viscousNmsPerRad = 0.05;     // b: tarcie lepkie (proporcjonalne do prędkości)
    double coulombNm = 0.1;             // Tc: tarcie suche (stała wartość)
    double torqueLimitNm = 2.0;         // maksymalny moment, jaki silnik może dać
};

// Symulowany silnik: implementuje IActuator, więc sterownik nie odróżni go od prawdziwego.
class MotorSim : public IActuator {
public:
    explicit MotorSim(const MotorParams& params);

    void setTorque(double torqueNm) override;

    // Przesuwa symulację o jeden krok czasu dt [s].
    void step(double dtS);

    double positionRad() const { return positionRad_; }
    double velocityRadPerS() const { return velocityRadPerS_; }
    double appliedTorqueNm() const { return appliedTorqueNm_; }

private:
    MotorParams params_;
    double appliedTorqueNm_ = 0.0;  // moment po ograniczeniu do limitu
    double positionRad_ = 0.0;
    double velocityRadPerS_ = 0.0;
};

}  // namespace jointsim::sim
