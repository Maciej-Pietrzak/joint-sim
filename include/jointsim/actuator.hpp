#pragma once

namespace jointsim {

// Element wykonawczy przegubu (np. silnik ze sterownikiem prądu).
// Sterownik zna tylko ten interfejs, a nie konkretny silnik.
class IActuator {
public:
    virtual ~IActuator() = default;

    // Zadaje moment obrotowy w niutonometrach [Nm].
    virtual void setTorque(double torqueNm) = 0;
};

}  // namespace jointsim
