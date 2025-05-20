#include <args/args.h>
#include <image_format/bmp.h>
#include <filters/registry.h>
#include <stdexcept>

int main(int argc, const char** argv) {
    try {
        image_processor::args::Args args(argc, argv);
        image_processor::image_format::BMP bmp;
        auto image = bmp.Load(args.InputFile());

        for (const auto& filter_params : args.Filters()) {
            try {
                auto factory = image_processor::filters::Registry::Instance().Get(filter_params.Name());
                auto filter = factory(filter_params.Parameters());
                image = filter->Apply(image);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error argument filter: " << e.what() << std::endl;
                return 0;
            }
        }

        bmp.Save(image, args.OutputFile());
    } catch (const std::runtime_error& e) {
        std::cerr << "Error filter: " << e.what() << std::endl;
        return 0;
    }
    return 0;
}
