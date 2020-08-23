#include "Spectrum.h"

std::ostream& operator<<(std::ostream& os, const Spectrum& s)
{
	os << "[";
	for (int i = 0; i < 72; ++i) {
		os << s.get(i) << ", ";
	}
	os << s.get(72) << "]";
	return os;
}
