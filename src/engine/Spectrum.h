#pragma once

#include <array>
#include <iostream>

class Spectrum
{
public:
	std::array<double, 73> wavelength_vals; // Discretizes the visible spectrum (380 nm to 740 nm) in steps of 5 nm

	Spectrum(const std::array<double, 73>& vals)
	{
		for (int i = 0; i < 73; ++i) {
			wavelength_vals[i] = vals[i];
		}
	}

	Spectrum(const Spectrum& s)
	{
		for (int i = 0; i < 73; ++i) {
			wavelength_vals[i] = s.get(i);
		}
	}

	Spectrum()
	{
		for (int i = 0; i < 73; ++i) {
			wavelength_vals[i] = 0.0;
		}
	}

	void add(double val, int bin)
	{
		wavelength_vals[bin] += val;
		return;
	}

	void set(double val, int bin)
	{
		wavelength_vals[bin] = val;
		return;
	}

	double get(int bin) const
	{
		return wavelength_vals[bin];
	}

	double sum() const
	{
		double total = 0.0;
		for (int i = 0; i < 73; ++i) {
			total += wavelength_vals[i];
		}
		return total;
	}

	double integrate() const
	{
		return this->sum() * 5; // Each dlambda contributes 5
	}

	void normalize()
	{
		double factor = this->sum();
		*this /= factor;
	}

	double operator[](const int i) const
	{
		return wavelength_vals[i];
	}

	double& operator[](const int i)
	{
		return wavelength_vals[i];
	}

	Spectrum& operator+=(const Spectrum& s)
	{
		for (int i = 0; i < 73; ++i) {
			wavelength_vals[i] += s.get(i);
		}

		return *this;
	}

	Spectrum operator+(const Spectrum& s) const
	{
		std::array<double, 73> new_wavelengths;

		for (int i = 0; i < 73; ++i) {
			new_wavelengths[i] = wavelength_vals[i] + s.get(i);
		}

		return Spectrum(new_wavelengths);
	}

	Spectrum& operator*=(const Spectrum& s)
	{
		for (int i = 0; i < 73; ++i) {
			wavelength_vals[i] *= s.get(i);
		}

		return *this;
	}

	Spectrum operator*(const Spectrum& s) const
	{
		std::array<double, 73> new_wavelengths;

		for (int i = 0; i < 73; ++i) {
			new_wavelengths[i] = wavelength_vals[i] * s.get(i);
		}

		return Spectrum(new_wavelengths);
	}

	Spectrum& operator*=(const double w)
	{
		for (int i = 0; i < 73; ++i) {
			wavelength_vals[i] *= w;
		}

		return *this;
	}

	Spectrum operator*(const double w) const
	{
		std::array<double, 73> new_wavelengths;

		for (int i = 0; i < 73; ++i) {
			new_wavelengths[i] = wavelength_vals[i] * w;
		}

		return Spectrum(new_wavelengths);
	}

	Spectrum& operator/=(const double w)
	{
		double k = 1.0 / w;
		for (int i = 0; i < 73; ++i) {
			wavelength_vals[i] *= k;
		}

		return *this;
	}

	Spectrum operator/(const double w) const
	{
		double k = 1.0 / w;
		std::array<double, 73> new_wavelengths;

		for (int i = 0; i < 73; ++i) {
			new_wavelengths[i] = wavelength_vals[i] * k;
		}

		return Spectrum(new_wavelengths);
	}
};

std::ostream& operator<<(std::ostream& os, const Spectrum& s);
