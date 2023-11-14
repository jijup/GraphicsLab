// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <stdio.h>

#ifdef USE_PNG
#include <png.h>
#else
#include <OpenGP/external/LodePNG/lodepng.h>
#endif

#include "Image.h"



//=============================================================================
namespace OpenGP {
//=============================================================================

#ifdef USE_PNG

namespace {

    struct PNGReaderData {

        FILE *fid = nullptr;
        png_struct *read_struct = nullptr;
        png_info *info_struct = nullptr;

    };

    struct PNGWriterData {

        FILE *fid = nullptr;
        png_struct *write_struct = nullptr;
        png_info *info_struct = nullptr;

    };

}

PNGReader::~PNGReader() {

    PNGReaderData *data = (PNGReaderData*)private_data;

    if (data != nullptr) {
        png_read_end(data->read_struct, nullptr);
        png_destroy_read_struct(&(data->read_struct), &(data->info_struct), nullptr);
        fclose(data->fid);
    }

    delete data;

}

PNGReader::PNGReader(const std::string &path) {

    private_data = new PNGReaderData();

    PNGReaderData *data = (PNGReaderData*)private_data;

    using filep = FILE*;

    filep &fid = data->fid;

    fid = fopen(path.c_str(), "rb");
    if (fid == nullptr)
        mFatal() << "Could not open file";

    uint8_t header[8];
    if( fread(header, sizeof(header), 1, fid) != 1 )
        mFatal() << "Cannot read PNG header";

    if (png_sig_cmp(header, 0, 8))
        mFatal() << "File is not a PNG image";

    png_structp &read_struct = data->read_struct;
    png_infop &info_struct = data->info_struct;

    read_struct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    info_struct = png_create_info_struct(read_struct);

    png_init_io(read_struct, fid);
    png_set_sig_bytes(read_struct, 8);

    png_read_info(read_struct, info_struct);

    width = png_get_image_width(read_struct, info_struct);
    height = png_get_image_height(read_struct, info_struct);
    components = png_get_channels(read_struct, info_struct);

}

void PNGReader::read(const ReadFunction &read_function) {

    PNGReaderData *data = (PNGReaderData*)private_data;

    using filep = FILE*;

    filep &fid = data->fid;
    png_structp &read_struct = data->read_struct;
    png_infop &info_struct = data->info_struct;

    int passes = png_set_interlace_handling(read_struct);
    png_read_update_info(read_struct, info_struct);

    Eigen::Matrix<uint8_t*, Eigen::Dynamic, Eigen::Dynamic> row_ptrs(height, 1);
    for (int i = 0;i < height;i++) {
        row_ptrs(i) = (uint8_t*)malloc(png_get_rowbytes(read_struct, info_struct));
    }

    png_read_image(read_struct, row_ptrs.data());

    int bit_depth = png_get_bit_depth(read_struct, info_struct);
    int row_width = components * width * 8;

    for (int row=0;row < height;row++) {
        for (int col=0;col < width;col++) {
            for (int c=0;c < components;c++) {

                uint32_t int_val = 0;

                int start_idx = (components * col + c) * (bit_depth / 8);
                for (int i=0;i < bit_depth / 8;i++) {
                    int_val <<= 8;
                    int_val |= row_ptrs(row)[start_idx + i];
                }

                double val = ((double)int_val) / ((((uint64_t)0x1) << bit_depth) - 1);
                read_function(row, col, c, val);
            }
        }
    }

    for (int i = 0;i < height;i++) {
        free(row_ptrs(i));
    }

}

PNGWriter::PNGWriter(const std::string &path) {

    private_data = new PNGWriterData();

    PNGWriterData *data = (PNGWriterData*)private_data;

    using filep = FILE*;

    filep &fid = data->fid;

    fid = fopen(path.c_str(), "wb");
    if (fid == nullptr)
        mFatal() << "Could not open file";

}

PNGWriter::~PNGWriter() {

    PNGWriterData *data = (PNGWriterData*)private_data;

    if (data != nullptr) {
        if (data->write_struct != nullptr) {
            png_destroy_write_struct(&(data->write_struct), &(data->info_struct));
        }
        fclose(data->fid);
    }

    delete data;

}

void PNGWriter::write(const WriteFunction &write_function) {

    PNGWriterData *data = (PNGWriterData*)private_data;

    using filep = FILE*;

    filep &fid = data->fid;
    png_structp &write_struct = data->write_struct;
    png_infop &info_struct = data->info_struct;

    write_struct = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    info_struct = png_create_info_struct(write_struct);

    png_init_io(write_struct, fid);

    uint8_t color_type;

    if (components == 1) {
        color_type = PNG_COLOR_TYPE_GRAY;
    } else if (components == 3) {
        color_type = PNG_COLOR_TYPE_RGB;
    } else if (components == 4) {
        color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    } else {
        mFatal() << "Unsupported PNG color type";
    }

    if (!(bit_depth == 8 || bit_depth == 16)) {
        bit_depth = 16;
    }

    png_set_IHDR(write_struct, info_struct, width, height,
                     bit_depth, color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);


    png_write_info(write_struct, info_struct);

    Eigen::Matrix<uint8_t*, Eigen::Dynamic, Eigen::Dynamic> row_ptrs(height, 1);
    for (int i = 0;i < height;i++) {
        row_ptrs(i) = (uint8_t*)malloc(png_get_rowbytes(write_struct, info_struct));
    }

    int row_width = components * width * 8;

    for (int row=0;row < height;row++) {
        for (int col=0;col < width;col++) {
            for (int c=0;c < components;c++) {

                double val = write_function(row, col, c);

                uint32_t int_val = val * ((((uint64_t)0x1) << bit_depth) - 1);

                int start_idx = (components * col + c) * (bit_depth / 8);
                for (int i=(bit_depth/8)-1;i >= 0;i--) {
                    row_ptrs(row)[start_idx + i] = int_val;
                    int_val >>= 8;
                }
            }
        }
    }

    png_write_image(write_struct, row_ptrs.data());

    png_write_end(data->write_struct, nullptr);

    for (int i = 0;i < height;i++) {
        free(row_ptrs(i));
    }


}

#else

namespace {

    struct PNGReaderData {

        std::vector<uint8_t> pixels;

    };

    struct PNGWriterData {

        std::string path;

    };

}

PNGReader::~PNGReader() {

    if (private_data == nullptr)
        return;

    PNGReaderData *data = (PNGReaderData*)private_data;

    delete data;

}

PNGReader::PNGReader(const std::string &path) {

#ifdef HEADERONLY

    mFatal() << "PNG support is not availible: to enable PNG support in header-only mode, define USE_PNG and include libpng in your build system";

#else

    private_data = new PNGReaderData();

    PNGReaderData *data = (PNGReaderData*)private_data;

    uint32_t width, height;

    uint32_t err = lodepng::decode(data->pixels, width, height, path.c_str());

    this->width = width;
    this->height = height;

    components = 4;

    if(err) {
        mFatal() << "lodepng error:" << lodepng_error_text(err);
    }

#endif

}

void PNGReader::read(const ReadFunction &read_function) {

    PNGReaderData *data = (PNGReaderData*)private_data;

    int i = 0;

    for (int row=0;row < height;row++) {
        for (int col=0;col < width;col++) {
            for (int c=0;c < components;c++) {

                uint8_t val = data->pixels[i];

                read_function(row, col, c, ((double)val) / 255);

                i++;
            }
        }
    }

}

PNGWriter::PNGWriter(const std::string &path) {

#ifdef HEADERONLY

    mFatal() << "PNG support is not availible: to enable PNG support in header-only mode, define USE_PNG and include libpng in your build system";

#else

    private_data = new PNGWriterData();

    PNGWriterData *data = (PNGWriterData*)private_data;

    data->path = path;

#endif

}

PNGWriter::~PNGWriter() {

    if (private_data == nullptr)
        return;

    PNGWriterData *data = (PNGWriterData*)private_data;

    delete data;


}

void PNGWriter::write(const WriteFunction &write_function) {

#ifndef HEADERONLY

    PNGWriterData *data = (PNGWriterData*)private_data;

    std::vector<uint8_t> pixels;

    for (int row=0;row < height;row++) {
        for (int col=0;col < width;col++) {
            for (int c=0;c < 4;c++) {

                uint8_t val;

                if (c < components) {

                    val = write_function(row, col, c) * 255;

                } else if (c < 3) {

                    val = 0;

                } else {

                    val = 255;

                }

                pixels.push_back(val);
            }
        }
    }

    uint32_t err = lodepng::encode(data->path.c_str(), pixels, width, height);

    if(err) {
        mFatal() << "lodepng error:" << lodepng_error_text(err);
    }

#endif

}

#endif

//=============================================================================
} // OpenGP::
//=============================================================================
