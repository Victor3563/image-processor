#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include "filters/filter.h"
#include "image/color.h"

#include <image/image.h>

namespace image_processor::filters {

namespace {

const int COUNTELEMENT = 9;

};  // namespace

class FilterMatrix9Element : public Filter {
public:
    FilterMatrix9Element() = default;

protected:
    virtual void CorectPixel(const image::Image& image, image::Color& a, size_t& row, size_t& col);

    void RunMatrix(const image::Image& image, image::Image& new_image);

    image::Image Apply(const image::Image& image) override = 0;

protected:
    std::vector<float> matricacoef_{
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    };
};

}  // namespace image_processor::filters
