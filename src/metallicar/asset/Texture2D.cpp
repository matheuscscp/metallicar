/*
 * Texture2D.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

namespace metallicar {

Texture2D::Texture2D(GLenum format, int w, int h, void* pixels) : w(w), h(h) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(
    GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, pixels
  );
}

Texture2D::~Texture2D() {
  glDeleteTextures(1, &texture);
}

int Texture2D::width() const {
  return w;
}

int Texture2D::height() const {
  return h;
}

GLuint Texture2D::id() const {
  return texture;
}

} // namespace metallicar
