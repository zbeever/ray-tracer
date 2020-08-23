#pragma once

#include "../engine/Spectrum.h"
#include <math.h>

class PeakSpectrum: public Spectrum
{
public:
	PeakSpectrum(const double mean, const double amplitude, const double spread)
	{
		for (int i = 0; i < 73; ++i) {
			double wavelength = 380.0 + 5.0 * i;
			double exponent = (wavelength - mean) / spread;
			exponent *= exponent;
			exponent *= -0.5;
			wavelength_vals[i] = amplitude * exp(exponent);
		}
	}
};
