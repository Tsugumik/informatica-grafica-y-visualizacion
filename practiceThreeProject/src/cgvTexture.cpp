#include "cgvTexture.h"
#include "lodepng.h"

#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm> // For std::swap_ranges

cgvTexture::cgvTexture(std::string file) {
    std::vector<unsigned char> image_data;
    unsigned int error = lodepng::decode(image_data, width, height, file);

    if (error) {
        throw std::runtime_error("Error reading file " + file + ": " + lodepng_error_text(error));
    }

    // Flip the image vertically
    std::vector<unsigned char> flipped_image_data(image_data.size());
    for (unsigned int y = 0; y < height; y++) {
        unsigned int src_y = height - 1 - y;
        std::copy(image_data.begin() + src_y * width * 4,
                  image_data.begin() + (src_y + 1) * width * 4,
                  flipped_image_data.begin() + y * width * 4);
    }

    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Set texture parameters with mipmapping for better quality
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Use gluBuild2DMipmaps to automatically generate mipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, flipped_image_data.data());
}

cgvTexture::~cgvTexture() {
    if (idTexture != 0) {
        glDeleteTextures(1, &idTexture);
    }
}

void cgvTexture::apply() {
    glBindTexture(GL_TEXTURE_2D, idTexture);
}

void cgvTexture::setTextureId(unsigned int id) {
    this->idTexture = id;
}

unsigned int cgvTexture::getTextureId() {
    return this->idTexture;
}
