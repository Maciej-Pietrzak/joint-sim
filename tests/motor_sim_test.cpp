#include <gtest/gtest.h>

#include "jointsim/sim/motor_sim.hpp"

using jointsim::sim::MotorParams;
using jointsim::sim::MotorSim;

namespace {

constexpr double kDtS = 1e-4;  // krok fizyki: 0.1 ms (10 kHz)

// Pomocnik: wykonuje tyle kroków, ile mieści się w podanym czasie.
void runFor(MotorSim& motor, double durationS) {
    const int steps = static_cast<int>(durationS / kDtS);
    for (int i = 0; i < steps; ++i) {
        motor.step(kDtS);
    }
}

}  // namespace

TEST(MotorSim, StaysAtRestWithoutTorque) {
    MotorSim motor{MotorParams{}};

    runFor(motor, 1.0);

    EXPECT_DOUBLE_EQ(motor.positionRad(), 0.0);
    EXPECT_DOUBLE_EQ(motor.velocityRadPerS(), 0.0);
}

TEST(MotorSim, TorqueBelowCoulombFrictionDoesNotMove) {
    MotorParams params;
    params.coulombNm = 0.1;
    MotorSim motor{params};

    motor.setTorque(0.05);  // połowa tarcia suchego
    runFor(motor, 1.0);

    EXPECT_DOUBLE_EQ(motor.positionRad(), 0.0);
}

TEST(MotorSim, CommandedTorqueIsClampedToLimit) {
    MotorParams params;
    params.torqueLimitNm = 2.0;
    MotorSim motor{params};

    motor.setTorque(100.0);
    EXPECT_DOUBLE_EQ(motor.appliedTorqueNm(), 2.0);

    motor.setTorque(-100.0);
    EXPECT_DOUBLE_EQ(motor.appliedTorqueNm(), -2.0);
}

TEST(MotorSim, ReachesTerminalVelocityAtFullTorque) {
    const MotorParams params;  // domyślne: b = 0.05, Tc = 0.1, limit = 2.0
    MotorSim motor{params};

    motor.setTorque(params.torqueLimitNm);
    runFor(motor, 3.0);  // 15 stałych czasowych J/b = 0.2 s

    // Przyspieszenie = 0, gdy tau = b * w + Tc  =>  w = (tau - Tc) / b = 38 rad/s
    const double expected = (params.torqueLimitNm - params.coulombNm) / params.viscousNmsPerRad;
    EXPECT_NEAR(motor.velocityRadPerS(), expected, 1e-3);
}

TEST(MotorSim, FrictionStopsMotionWithoutReversing) {
    MotorSim motor{MotorParams{}};

    motor.setTorque(1.0);
    runFor(motor, 0.5);
    ASSERT_GT(motor.velocityRadPerS(), 0.0);  // przegub się kręci

    motor.setTorque(0.0);  // odcinamy moment, hamuje tylko tarcie
    for (int i = 0; i < 50000; ++i) {
        motor.step(kDtS);
        ASSERT_GE(motor.velocityRadPerS(), 0.0) << "prędkość zmieniła znak w kroku " << i;
    }
    EXPECT_DOUBLE_EQ(motor.velocityRadPerS(), 0.0);  // stoi dokładnie
}
