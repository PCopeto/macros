#include "Nuclear_Mass.h"

// Physical constants
constexpr double MeV_to_eV = 1.0e6;
constexpr double atomic_mass_unit_MeV = 931.494;

// Semi-Empirical Mass Formula coefficients (in MeV)
constexpr double a_v = 15.75;
constexpr double a_s = 17.8;
constexpr double a_c = 0.711;
constexpr double a_a = 23.7;
constexpr double a_p = 12.0;

double Nuke_Mass(int A, int Z) {
    int N = A - Z;
    double volume_term = a_v * A;
    double surface_term = a_s * pow(A, 2.0 / 3.0);
    double coulomb_term = a_c * Z * (Z - 1) / pow(A, 1.0 / 3.0);
    double asymmetry_term = a_a * pow((A - 2 * Z), 2) / A;
    double pairing_term = 0.0;
    
    if (A % 2 == 0) {
        if (Z % 2 == 0) {
            pairing_term = a_p / sqrt(A); // Even-even nuclei
        } else {
            pairing_term = -a_p / sqrt(A); // Odd-odd nuclei
        }
    }
    
    double binding_energy = volume_term - surface_term - coulomb_term - asymmetry_term + pairing_term;
    double mass_MeV = A * atomic_mass_unit_MeV - binding_energy;
    return mass_MeV * MeV_to_eV; // Convert to eV
}
