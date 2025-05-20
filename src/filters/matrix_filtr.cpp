#include "matrix_filtr.h"

void image_processor::filters::FilterMatrix9Element::RunMatrix(const image::Image& image, image::Image& new_image) {

    for (size_t row = 0; row < image.GetHeight(); ++row) {
        for (size_t col = 0; col < image.GetWidth(); ++col) {
            new_image(row, col) = 0 * image(row, col);
            CorectPixel(image, new_image(row, col), row, col);
        }
    }
}

void image_processor::filters::FilterMatrix9Element::CorectPixel(const image::Image& image, image::Color& a,
                                                                 size_t& row, size_t& col) {
    for (int ycor = 0; ycor < 3; ycor++) {
        for (int xcor = 0; xcor < 3; xcor++) {
            size_t ycorectcoordinait = static_cast<size_t>(
                std::clamp(static_cast<int>(row) + 1 - ycor, 0, static_cast<int>(image.GetHeight()) - 1));
            size_t xcorectcoordinait = static_cast<size_t>(
                std::clamp(static_cast<int>(col) - 1 + xcor, 0, static_cast<int>(image.GetWidth()) - 1));
            a = a + matricacoef_[ycor * 3 + xcor] * image(ycorectcoordinait, xcorectcoordinait);
        }
    }
    a.r = std::clamp(a.r, 0.0f, 1.0f);
    a.g = std::clamp(a.g, 0.0f, 1.0f);
    a.b = std::clamp(a.b, 0.0f, 1.0f);
}
