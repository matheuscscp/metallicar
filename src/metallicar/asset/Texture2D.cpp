/*
 * Texture2D.cpp
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
using namespace geometry;

namespace metallicar {

Texture2D::Texture2D(const std::string& path) {
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

Texture2D::Texture2D(int w, int h, GLuint texture) :
w(w), h(h), texture(texture)
{
  
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

void Texture2D::render2D(
  GLint filter,
  const Color& color,
  const geometry::Point2& position,
  float angle,
  const geometry::Point2& scale,
  float texCoordX0,
  float texCoordX1,
  float texCoordY0,
  float texCoordY1,
  float vertexCoordX0,
  float vertexCoordX1,
  float vertexCoordY0,
  float vertexCoordY1
) const {
  // bind
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  
  glColor4f(color.r, color.g, color.b, color.a);
  
  glPushMatrix();
    // transform
    glLoadIdentity();
    glTranslatef(position.x, position.y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glScalef(scale.x, scale.y, 1.0f);
    
    // render
    glBegin(GL_QUADS);
      glTexCoord2f(texCoordX0, texCoordY0);
      glVertex2f(vertexCoordX0, vertexCoordY0);
      glTexCoord2f(texCoordX1, texCoordY0);
      glVertex2f(vertexCoordX1, vertexCoordY0);
      glTexCoord2f(texCoordX1, texCoordY1);
      glVertex2f(vertexCoordX1, vertexCoordY1);
      glTexCoord2f(texCoordX0, texCoordY1);
      glVertex2f(vertexCoordX0, vertexCoordY1);
    glEnd();
  glPopMatrix();
}

} // namespace metallicar
