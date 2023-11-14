// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <regex>
#include <memory>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <limits>
#include <cassert>

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <OpenGP/headeronly.h>
#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/gl.h>
#include <OpenGP/GL/GlfwWindow.h>
#include <OpenGP/GL/FullscreenQuad.h>
#include <OpenGP/GL/SceneObject.h>
#include <OpenGP/Image/ImageType.h>

//=============================================================================
namespace OpenGP {
//=============================================================================

template <class ImageType>
inline int imshow(const ImageType &I, const std::string &title="") {

    GlfwWindow window(title, I.cols(), I.rows());

    constexpr int components = ImageTypeInfo<ImageType>::component_count;
    using Scalar = typename ImageTypeInfo<ImageType>::Scalar;
    using TextureType = typename TextureTypeBuilder<ImageType>::Type;

    TextureType texture;
    texture.upload(I);

    FullscreenQuad fs_quad;

    window.set_user_update_fn([&](){
        fs_quad.draw_texture(texture);
    });

    return window.run();
}

class ImageReader {
protected:

    void *private_data;

    int components;
    int width, height;

public:

    using ReadFunction = std::function<void(int, int, int, double)>;

    int get_components() const { return components; }
    int get_width() const { return width; }
    int get_height() const { return height; }

    virtual ~ImageReader() {}

    virtual void read(const ReadFunction&) = 0;

};

class PNGReader : public ImageReader {
public:
    HEADERONLY_INLINE PNGReader(const std::string &path);
    HEADERONLY_INLINE ~PNGReader();
    HEADERONLY_INLINE void read(const ReadFunction&);
};

class TGAReader : public ImageReader {
public:
    HEADERONLY_INLINE TGAReader(const std::string &path);
    HEADERONLY_INLINE ~TGAReader();
    HEADERONLY_INLINE void read(const ReadFunction&);
};

class ImageWriter {
protected:

    void *private_data;

    int components, bit_depth;
    int width, height;

public:

    using WriteFunction = std::function<double(int, int, int)>;

    virtual ~ImageWriter() {}

    void set_components(int components) { this->components = components; }
    void set_bit_depth(int bit_depth) { this->bit_depth = bit_depth; }
    void set_width(int width) { this->width = width; }
    void set_height(int height) { this->height = height; }

    virtual void write(const WriteFunction&) = 0;

};

class PNGWriter : public ImageWriter {
public:
    HEADERONLY_INLINE PNGWriter(const std::string &path);
    HEADERONLY_INLINE ~PNGWriter();
    HEADERONLY_INLINE void write(const WriteFunction&);
};

class TGAWriter : public ImageWriter {
public:
    HEADERONLY_INLINE TGAWriter(const std::string &path);
    HEADERONLY_INLINE ~TGAWriter();
    HEADERONLY_INLINE void write(const WriteFunction&);
};


template <typename Scalar>
typename std::enable_if<std::is_integral<Scalar>::value, Scalar>::type scalar_transfer_read(double val) {
    return val * std::numeric_limits<Scalar>::max();
}

template <typename Scalar>
typename std::enable_if<!std::is_integral<Scalar>::value, Scalar>::type scalar_transfer_read(double val) {
    return Scalar(val);
}

template <typename Scalar>
typename std::enable_if<std::is_integral<Scalar>::value, double>::type scalar_transfer_write(Scalar val) {
    return ((double)val) / std::numeric_limits<Scalar>::max();
}

template <typename Scalar>
typename std::enable_if<!std::is_integral<Scalar>::value, double>::type scalar_transfer_write(Scalar val) {
    return (double)val;
}


template <typename ImageType>
void imread(const char* path, ImageType& I) {

    std::regex suffix_regex(R"regex([\S\s]*\.([^\.]+)$)regex");

    std::cmatch match;
    if (!std::regex_match(path, match, suffix_regex))
        mFatal() << "Could not identify the image file type";

    std::string suffix = match[1];
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);

    std::unique_ptr<ImageReader> reader;

    if (suffix == "png") {
        reader = std::unique_ptr<ImageReader>(new PNGReader(path));
    } else if (suffix == "tga") {
        reader = std::unique_ptr<ImageReader>(new TGAReader(path));
    } else {
        mFatal() << "Unknown image type suffix";
    }

    I.resize(reader->get_height(), reader->get_width());

    int image_type_components = ImageTypeInfo<ImageType>::component_count;
    using Scalar = typename ImageTypeInfo<ImageType>::Scalar;

    ImageReader::ReadFunction read_function;

    if (reader->get_components() == 1) {

        // duplicate single channel into first 3 provided channels
        if (image_type_components < 4) {
            read_function = [&](int row, int col, int c, double val) {
                for (int i = 0;i < image_type_components;i++) {
                    ImageTypeInfo<ImageType>::channel_ref(I, row, col, i) = scalar_transfer_read<Scalar>(val);
                }
            };
        } else if (image_type_components == 4) {
            // set alpha to 1
            read_function = [&](int row, int col, int c, double val) {
                for (int i = 0;i < 3;i++) {
                    ImageTypeInfo<ImageType>::channel_ref(I, row, col, i) = scalar_transfer_read<Scalar>(val);
                }
                ImageTypeInfo<ImageType>::channel_ref(I, row, col, 3) = scalar_transfer_read<Scalar>(1.0);
            };
        } else {
            mFatal() << "Image type and image file are incompatible";
        }

    } else if (reader->get_components() == 3) {

        if (image_type_components == 1) {
            // use average of rgb values
            ImageTypeInfo<ImageType>::set_zero(I);
            read_function = [&](int row, int col, int c, double val) {
                ImageTypeInfo<ImageType>::channel_ref(I, row, col, 0) += scalar_transfer_read<Scalar>(val / 3);
            };
        } else if (image_type_components == 3) {
            read_function = [&](int row, int col, int c, double val) {
                ImageTypeInfo<ImageType>::channel_ref(I, row, col, c) = scalar_transfer_read<Scalar>(val);
            };
        } else if (image_type_components == 4) {
            // set alpha to 1
            read_function = [&](int row, int col, int c, double val) {
                ImageTypeInfo<ImageType>::channel_ref(I, row, col, c) = scalar_transfer_read<Scalar>(val);
                if (c == 2)
                    ImageTypeInfo<ImageType>::channel_ref(I, row, col, 3) = scalar_transfer_read<Scalar>(1.0);
            };
        } else {
            mFatal() << "Image type and image file are incompatible";
        }

    } else if (reader->get_components() == 4) {

        if (image_type_components == 1) {
            // use average of rgb values
            ImageTypeInfo<ImageType>::set_zero(I);
            read_function = [&](int row, int col, int c, double val) {
                if (c != 3)
                    ImageTypeInfo<ImageType>::channel_ref(I, row, col, 0) += scalar_transfer_read<Scalar>(val / 3);
            };
        } else if (image_type_components == 3) {
            // drop alpha channel
            read_function = [&](int row, int col, int c, double val) {
                if (c < 3)
                    ImageTypeInfo<ImageType>::channel_ref(I, row, col, c) = scalar_transfer_read<Scalar>(val);
            };
        } else if (image_type_components == 4) {
            read_function = [&](int row, int col, int c, double val) {
                ImageTypeInfo<ImageType>::channel_ref(I, row, col, c) = scalar_transfer_read<Scalar>(val);
            };
        } else {
            mFatal() << "Image type and image file are incompatible";
        }

    } else if (image_type_components == reader->get_components()) {

        read_function = [&](int row, int col, int c, double val) {
            ImageTypeInfo<ImageType>::channel_ref(I, row, col, c) = scalar_transfer_read<Scalar>(val);
        };

    } else {
        mFatal() << "Image type and image file are incompatible";
    }

    reader->read(read_function);

}

template <typename ImageType>
void imwrite(const char* path, ImageType &I) {

    std::regex suffix_regex(R"regex([\S\s]*\.([^\.]+)$)regex");

    std::cmatch match;
    if (!std::regex_match(path, match, suffix_regex))
        mFatal() << "Could not identify the image file type";

    std::string suffix = match[1];
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);

    std::unique_ptr<ImageWriter> writer;

    if (suffix == "png") {
        writer = std::unique_ptr<ImageWriter>(new PNGWriter(path));
    } else if (suffix == "tga") {
        writer = std::unique_ptr<ImageWriter>(new TGAWriter(path));
    } else {
        mFatal() << "Unknown image type suffix";
    }

    int image_type_components = ImageTypeInfo<ImageType>::component_count;
    using Scalar = typename ImageTypeInfo<ImageType>::Scalar;

    //mLogger() << image_type_components;

    writer->set_width(I.cols());
    writer->set_height(I.rows());
    writer->set_components(image_type_components);
    writer->set_bit_depth(8 * sizeof(Scalar));

    ImageWriter::WriteFunction write_function;

    if (image_type_components == 1) {
        write_function = [&](int row, int col, int c) {
            if (c < 3)
                return scalar_transfer_write<Scalar>(ImageTypeInfo<ImageType>::channel_ref(I, row, col, 0));
            else
                return 1.0;
        };
    } else if (image_type_components == 3) {
        write_function = [&](int row, int col, int c) {
            if (c < 3)
                return scalar_transfer_write<Scalar>(ImageTypeInfo<ImageType>::channel_ref(I, row, col, c));
            else
                return 1.0;
        };
    } else {
        write_function = [&](int row, int col, int c) {
            if (c < image_type_components)
                return scalar_transfer_write<Scalar>(ImageTypeInfo<ImageType>::channel_ref(I, row, col, c));
            else{
                mFatal() << "Image type and image file are incompatible";
                return 0.0;
            }
        };
    }

    writer->write(write_function);

}

//=============================================================================
} // namespace OpenGP
//=============================================================================

#ifdef HEADERONLY
    #include "PNGImage.cpp"
    #include "TGAImage.cpp"
#endif
