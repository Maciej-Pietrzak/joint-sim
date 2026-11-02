#pragma once

#include <cstdint>

namespace jointsim {

// Czujnik pozycji przegubu (enkoder inkrementalny).
// Sterownik zna tylko ten interfejs, a nie konkretny czujnik.
class IEncoder {
public:
    virtual ~IEncoder() = default;

    // Odczytuje bieżącą pozycję w impulsach (liczba całkowita).
    // Nie jest const: odczyt prawdziwego czujnika to operacja, a nie zwykły getter.
    virtual std::int32_t readCounts() = 0;

    // Liczba impulsów na jeden pełny obrót (rozdzielczość czujnika).
    virtual std::int32_t countsPerRevolution() const = 0;
};

}  // namespace jointsim
