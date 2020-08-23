#pragma once

#include "../engine/Spectrum.h"

class Sellmeier: public Spectrum
{
public:
	Sellmeier(const double A, const std::array<double, 3>& B, const std::array<double, 3>& C)
	{
		for (int i = 0; i < 73; ++i) {
			double wl = 0.380 + 0.005 * i;
			double eta = A;
			for (int j = 0; j < 3; ++j) {
				eta += (B[j] * wl * wl) / (wl * wl - C[j]);
			}
			eta = sqrt(eta);
			wavelength_vals[i] = eta;
		}
	}
};
