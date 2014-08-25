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
    Log::message(
      Log::Error,
      String::from("Image \"%s\" could not be loaded", Path::get(path).c_str())
    );
    exit(0);
  }
}

Image::~Image() {
  SDL_FreeSurface(image);
}

Texture2D* Image::generateTexture() const {
  GLenum format;
  if (image->format->BytesPerPixel == 1) {
    format = GL_LUMINANCE;
  }
  else if (image->format->BytesPerPixel == 3) {
    format = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? GL_BGR : GL_RGB;
  }
  else {
    format = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? GL_BGRA : GL_RGBA;
  }
  return new Texture2D(format, image->w, image->h, image->pixels);
}

shared_ptr<Texture2D> Image::getTexture(const string& path) {
  shared_ptr<Texture2D> tex = Assets::get<Texture2D>(path);
  if (tex.get()) {
    return tex;
  }
  return Assets::put(
    path, shared_ptr<Texture2D>(Image(path).generateTexture())
  );
}

} // namespace metallicar
