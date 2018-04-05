#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <functional>
#include <vector>

namespace NE
{

bool EnumerateVariationIndices (std::vector<size_t> maxIndices,
								const std::function<void (const std::vector<size_t>&)>& processVariation);

}

#endif
