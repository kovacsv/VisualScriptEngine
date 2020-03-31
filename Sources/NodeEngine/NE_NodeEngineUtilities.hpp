#ifndef NE_UTILITIES_HPP
#define NE_UTILITIES_HPP

#include <functional>
#include <vector>

namespace NE
{

bool EnumerateVariationIndices (const std::vector<size_t>& maxIndices,
								const std::function<bool (const std::vector<size_t>&)>& processVariation);

}

#endif
