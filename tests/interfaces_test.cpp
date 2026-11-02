#include <gtest/gtest.h>

#include <cstdint>

#include "jointsim/actuator.hpp"
#include "jointsim/encoder.hpp"

namespace {

// Atrapa silnika: niczego nie symuluje, tylko zapamiętuje ostatni moment.
class FakeActuator : public jointsim::IActuator {
public:
    void setTorque(double torqueNm) override { lastTorqueNm = torqueNm; }

    double lastTorqueNm = 0.0;
};

// Atrapa enkodera: zwraca liczbę impulsów ustawioną w teście.
class FakeEncoder : public jointsim::IEncoder {
public:
    std::int32_t readCounts() override { return counts; }
    std::int32_t countsPerRevolution() const override { return 4096; }

    std::int32_t counts = 0;
};

// Funkcja, która zna TYLKO interfejsy, tak jak później sterownik.
void pushIfBelowZero(jointsim::IEncoder& encoder, jointsim::IActuator& actuator) {
    if (encoder.readCounts() < 0) {
        actuator.setTorque(1.0);
    } else {
        actuator.setTorque(0.0);
    }
}

}  // namespace

TEST(Interfaces, CodeUsingInterfacesWorksWithFakes) {
    FakeEncoder encoder;
    FakeActuator actuator;

    encoder.counts = -10;
    pushIfBelowZero(encoder, actuator);
    EXPECT_DOUBLE_EQ(actuator.lastTorqueNm, 1.0);

    encoder.counts = 10;
    pushIfBelowZero(encoder, actuator);
    EXPECT_DOUBLE_EQ(actuator.lastTorqueNm, 0.0);
}
