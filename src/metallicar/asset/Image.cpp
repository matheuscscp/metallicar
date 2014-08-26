/*
 * Image.cpp
 *
 *  Created on: Aug 10, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

// lib
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// local
#include "Path.hpp"
#include "Log.hpp"
#include "String.hpp"

using namespace std;

namespace metallicar {

Image::Image(const string& path) :
image(stbi_load(Path::get(path).c_str(), &w, &h, &comp, 0))
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
  stbi_image_free(image);
}

Texture2D* Image::generateTexture() const {
  GLenum format;
  if (comp == 1) {
    format = GL_LUMINANCE;
  }
  else if (comp == 3) {
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    format = GL_RGB;
#else
    format = GL_BGR;
#endif
  }
  else {
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    format = GL_RGBA;
#else
    format = GL_BGRA;
#endif
  }
  return new Texture2D(format, w, h, image);
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

SDL_Surface* Image::createSDL_Surface(const string& path) {
  uint32_t rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#else
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#endif
  Image tmp(path);
  SDL_Surface* ret;
  if (tmp.comp == 4) {
    ret = SDL_CreateRGBSurface(0, tmp.w, tmp.h, 32, rmask, gmask, bmask, amask);
  }
  else {
    ret = SDL_CreateRGBSurface(0, tmp.w, tmp.h, 24, rmask, gmask, bmask, 0);
  }
  memcpy(ret->pixels, tmp.image, tmp.comp*tmp.w*tmp.h);
  return ret;
}

} // namespace metallicar
