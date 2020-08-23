#pragma once

class FlatSpectrum: public Spectrum
{
public:
	FlatSpectrum(const double w)
	{
		for (int i = 0; i < 73; ++i) {
			wavelength_vals[i] = w;
		}
	}

};
