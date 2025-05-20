#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "filters/matrix_filtr.h"
#include "image/image.h"

#include <filters/filter.h>
#include <filters/registry.h>

namespace image_processor::filters {

namespace {

const float COEFVALUE = 4;
const float REDCOLOURCOEF = 0.299;
const float BLUECOLOURCOEF = 0.114;
const float GREENCOLOURCOEF = 0.587;

const std::vector<float> MATRIXDEV{0.0f, -1.0f, 0.0f, -1.0f, COEFVALUE, -1.0f, 0.0f, -1.0f, 0.0f};

float CastFloat(std::string_view s) {
    std::stringstream sstream(s.data());
    float result = 0.0f;
    sstream >> result;
    return result;
}

}  // namespace

class Edge : public FilterMatrix9Element {
public:
    class Factory : public Filter::Factory {
    public:
        std::unique_ptr<Filter> Construct(const std::vector<std::string>& parameters) override {
            if (parameters.size() != 1) {
                throw std::invalid_argument("USAGE: -edge threshold");
            }
            const float borderline_value = CastFloat(parameters[0]);
            if (0 > borderline_value || borderline_value > 1) {
                throw std::invalid_argument("USAGE: -edge 1>threshold>0");
            }
            return std::make_unique<Edge>(borderline_value);
        }
    };

public:
    explicit Edge(float borderline_value) : borderline_value_(borderline_value) {
        matricacoef_ = MATRIXDEV;
    };
    ~Edge() override = default;

protected:
    image::Image Apply(const image::Image& image) override {
        image::Image work_image{image.GetWidth(), image.GetHeight()};
        image::Image new_image{image.GetWidth(), image.GetHeight()};
        GrayscaleAlg(image, work_image);
        RunMatrix(work_image, new_image);
        return new_image;
    }

    void CorectPixel(const image::Image& image, image::Color& a, size_t& row, size_t& col) override {
        for (int ycor = 0; ycor < 3; ycor++) {
            for (int xcor = 0; xcor < 3; xcor++) {
                size_t ycorectcoordinait = static_cast<size_t>(
                    std::clamp(static_cast<int>(row) + 1 - ycor, 0, static_cast<int>(image.GetHeight()) - 1));
                size_t xcorectcoordinait = static_cast<size_t>(
                    std::clamp(static_cast<int>(col) - 1 + xcor, 0, static_cast<int>(image.GetWidth()) - 1));
                a = a + matricacoef_[ycor * 3 + xcor] * image(ycorectcoordinait, xcorectcoordinait);
            }
        }
        a.r = CheckValue(a.r);
        a.g = a.r;
        a.b = a.r;
    }

    float CheckValue(float value) const {
        if (value > borderline_value_) {
            return 1.0f;
        }
        return 0.0f;
    }

    void GrayscaleAlg(const image::Image& image, image::Image& new_image) {
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
    }

protected:
    [[maybe_unused]] float borderline_value_;
};
REGISTER_FILTER("edge", Edge::Factory)

}  // namespace image_processor::filters
