#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <string_view>

namespace image_processor::io {
class Reader {
public:
    explicit Reader(std::string_view filename);
    template <std::integral T>
    T Read();
    template <std::integral T>
    Reader& operator>>(T& el);

private:
    std::ifstream stream_;
};
template <std::integral T>
T Reader::Read() {
    char buffer[sizeof(T)];
    stream_.read(buffer, sizeof(buffer));

    if constexpr (std::endian::native == std::endian::big) {
        std::reverse(std::begin(buffer), std::end(buffer));
    }

    return *reinterpret_cast<T*>(buffer);
}
template <std::integral T>
Reader& Reader::operator>>(T& el) {
    el = Read<T>();
    return *this;
};
}  // namespace image_processor::io
