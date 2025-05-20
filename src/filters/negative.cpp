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

class Negative : public Filter {
public:
    class Factory : public Filter::Factory {
    public:
        std::unique_ptr<Filter> Construct(const std::vector<std::string>& parameters) override {
            if (!parameters.empty()) {
                throw std::invalid_argument("USAGE: -neg");
            }
            return std::make_unique<Negative>();
        }
    };

public:
    Negative() = default;
    image::Image Apply(const image::Image& image) override {
        image::Image new_image{image.GetWidth(), image.GetHeight()};
        for (size_t row = 0; row < image.GetHeight(); ++row) {
            for (size_t col = 0; col < image.GetWidth(); ++col) {
                new_image(row, col) = image::Color{
                    .r = 1 - image(row, col).r,
                    .g = 1 - image(row, col).g,
                    .b = 1 - image(row, col).b,
                };
            }
        }
        return new_image;
    }
};

REGISTER_FILTER("neg", Negative::Factory)

}  // namespace image_processor::filters
