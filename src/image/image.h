#pragma once

#include <utils/matrix.h>
#include <image/color.h>

namespace image_processor::image {

using RowImage = std::vector<Color>;
using Image = utils::Matrix<Color>;

}  // namespace image_processor::image
