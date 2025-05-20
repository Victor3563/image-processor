#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "filters/matrix_filtr.h"

#include <filters/filter.h>
#include <filters/registry.h>

namespace image_processor::filters {

namespace {

const float COEFVALUE = 5;
const std::vector<float> MATRIX_DEF = {0.0f, -1.0f, 0.0f, -1.0f, COEFVALUE, -1.0f, 0.0f, -1.0f, 0.0f};

}  // namespace

class Sharping : public FilterMatrix9Element {
public:
    class Factory : public Filter::Factory {
    public:
        std::unique_ptr<Filter> Construct(const std::vector<std::string>& parameters) override {
            if (!parameters.empty()) {
                throw std::invalid_argument("");
            }
            return std::make_unique<Sharping>();
        }
    };

public:
    Sharping() {
        matricacoef_ = MATRIX_DEF;
    };

    image::Image Apply(const image::Image& image) override {
        image_processor::image::Image new_image{image.GetWidth(), image.GetHeight()};
        RunMatrix(image, new_image);
        return new_image;
    };
};

REGISTER_FILTER("sharp", Sharping::Factory)

}  // namespace image_processor::filters
