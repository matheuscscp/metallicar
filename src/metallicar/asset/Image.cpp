/*
 * Image.cpp
 *
 *  Created on: Aug 10, 2014
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

Image::Image(const string& path) :
path(path),
image(IMG_Load(Path::get(path).c_str()))
{
  if (!image) {
    Log::message(Log::Error, String::from("Image \"%s\" could not be loaded", Path::get(path).c_str()));
    exit(0);
  }
}

Image::~Image() {
  SDL_FreeSurface(image);
}

Texture2D* Image::generateTexture() const {
  GLenum format;
  if (image->format->BytesPerPixel == 4) {
    format = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? GL_BGRA : GL_RGBA;
  }
  else if (image->format->BytesPerPixel == 3) {
    format = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? GL_BGR : GL_RGB;
  }
  else {
    Log::message(Log::Error, String::from("Image \"%s\": %d byte(s) per pixel format is not supported", Path::get(path).c_str(), image->format->BytesPerPixel));
    exit(0);
  }
  return new Texture2D(format, image->w, image->h, image->pixels);
}

Texture2D* Image::createTexture(const string& path) {
  return Image(path).generateTexture();
}

} // namespace metallicar
