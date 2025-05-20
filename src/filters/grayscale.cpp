#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <filters/filter.h>
#include <filters/registry.h>

namespace image_processor::filters {

namespace {

const float REDCOLOURCOEF = 0.299;
const float BLUECOLOURCOEF = 0.114;
const float GREENCOLOURCOEF = 0.587;

}  // namespace

class Grayscale : public Filter {
public:
    class Factory : public Filter::Factory {
    public:
        std::unique_ptr<Filter> Construct(const std::vector<std::string>& parameters) override {
            if (!parameters.empty()) {
                throw std::invalid_argument("USAGE: -gs");
            }
            return std::make_unique<Grayscale>();
        }
    };

public:
    Grayscale() = default;
    image::Image Apply(const image::Image& image) override {
        return GrayscaleAlg(image);
    }

    image::Image GrayscaleAlg(const image::Image& image) {
        image::Image new_image{image.GetWidth(), image.GetHeight()};
        for (size_t row = 0; row < image.GetHeight(); ++row) {
            for (size_t col = 0; col < image.GetWidth(); ++col) {
                float new_color_value = BLUECOLOURCOEF * image(row, col).b + REDCOLOURCOEF * image(row, col).r +
                                        GREENCOLOURCOEF * image(row, col).g;
                new_image(row, col) = image::Color{
                    .r = new_color_value,
                    .g = new_color_value,
                    .b = new_color_value,
                };
            }
        }
        return new_image;
    }
};

REGISTER_FILTER("gs", Grayscale::Factory)

}  // namespace image_processor::filters
