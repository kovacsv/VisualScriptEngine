#include "NE_NodeEngineUtilities.hpp"

namespace NE
{

bool EnumerateVariationIndices (const std::vector<size_t>& maxIndices,
								const std::function<bool (const std::vector<size_t>&)>& processVariation)
{
	size_t length = maxIndices.size ();
	if (length == 0) {
		return false;
	}
	std::vector<size_t> indices;
	for (size_t i = 0; i < length; i++) {
		indices.push_back (0);
	}
	int columnIndex = (int) length - 1;
	while (columnIndex != -1) {
		if (!processVariation (indices)) {
			return false;
		}
		columnIndex = (int) length - 1;
		while (columnIndex >= 0) {
			if (indices[columnIndex] < maxIndices[columnIndex]) {
				indices[columnIndex] += 1;
				break;
			} else {
				indices[columnIndex] = 0;
			}
			columnIndex -= 1;
		}
	}
	return true;
}

}
