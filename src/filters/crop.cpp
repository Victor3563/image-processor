#include <algorithm>
#include <cstddef>
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

size_t Cast(std::string_view s) {
    std::stringstream sstream(s.data());
    int result = 0;
    sstream >> result;
    if (result < 0) {
        throw std::runtime_error("USAGE: -crop width>0 height>0");
    }

    return static_cast<size_t>(result);
}

}  // namespace

class Crop : public Filter {
public:
    class Factory : public Filter::Factory {
    public:
        std::unique_ptr<Filter> Construct(const std::vector<std::string>& parameters) override {
            if (parameters.size() != 2) {
                throw std::invalid_argument("USAGE: -crop width height");
            }

            const size_t width = Cast(parameters[0]);
            const size_t height = Cast(parameters[1]);
            return std::make_unique<Crop>(width, height);
        }
    };

public:
    Crop(size_t width, size_t height) : width_(width), height_(height) {
    }

    image::Image Apply(const image::Image& image) override {

        if (image.GetHeight() > height_ || image.GetWidth() > width_) {
            height_ = std::min(height_, image.GetHeight());
            width_ = std::min(width_, image.GetWidth());
            image::Image new_image{width_, height_};
            for (size_t row = image.GetHeight() - height_; row < image.GetHeight(); ++row) {
                for (size_t col = 0; col < width_; ++col) {
                    new_image(row - (image.GetHeight() - height_), col) = image(row, col);
                }
            }
            return new_image;
        }
        return image;
    }

private:
    [[maybe_unused]] size_t width_;
    [[maybe_unused]] size_t height_;
};

REGISTER_FILTER("crop", Crop::Factory)

}  // namespace image_processor::filters
