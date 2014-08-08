/*
 * Texture.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

// lib
#include "SDL_image.h"

// local
#include "Path.hpp"
#include "Log.hpp"
#include "String.hpp"

using namespace std;

namespace metallicar {

Texture::Texture(const std::string& path) {
  // load image
  SDL_Surface* image = IMG_Load(Path::get(path).c_str());
  if (!image) {
    Log::message(Log::Error, String::from("Image \"%s\" could not be loaded", Path::get(path).c_str()));
    exit(0);
  }
  w = image->w;
  h = image->h;
  
  // check format
  GLenum texFormat;
  if (image->format->BytesPerPixel == 4) {
    texFormat = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? GL_BGRA : GL_RGBA;
  }
  else if (image->format->BytesPerPixel == 3) {
    texFormat = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? GL_BGR : GL_RGB;
  }
  else {
    Log::message(Log::Error, String::from("Image \"%s\": %d byte(s) per pixel format is not supported", Path::get(path).c_str(), image->format->BytesPerPixel));
    exit(0);
  }
  
  // generate texture
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(
    GL_TEXTURE_2D, 0, texFormat, w, h, 0,
    texFormat, GL_UNSIGNED_BYTE, image->pixels
  );
  
  SDL_FreeSurface(image);
}

Texture::~Texture() {
  glDeleteTextures(1, &texture);
}

int Texture::width() const {
  return w;
}

int Texture::height() const {
  return h;
}

GLuint Texture::id() const {
  return texture;
}

} // namespace metallicar
