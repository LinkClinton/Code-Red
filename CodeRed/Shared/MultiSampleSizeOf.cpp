#include "MultiSampleSizeOf.hpp"

auto CodeRed::MultiSampleSizeOf::get(const MultiSample sample) -> size_t
{
	return static_cast<size_t>(sample);
}
