/*
 * Sprite.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

using namespace std;

namespace metallicar {

Sprite::Sprite(const string& path) :
texture(Assets::instance().get<Texture>(path)),
widthTexture(float(texture->width())),
heightTexture(float(texture->height())),
width(texture->width()),
height(texture->height())
{
  resetClip();
}

Sprite::~Sprite() {
  
}

int Sprite::getWidth() const {
  return width;
}

int Sprite::getHeight() const {
  return height;
}

void Sprite::render(
  float x,
  float y,
  Corner corner,
  float opacity,
  float angle,
  float scaleX,
  float scaleY,
  const Color& color,
  bool linearFilter
) {
  // texture stuff
  {
    GLint param = linearFilter ? GL_LINEAR : GL_NEAREST;
    glBindTexture(GL_TEXTURE_2D, texture->id());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
  }
  
  glColor4f(color.r, color.g, color.b, opacity);
  
  // check corner
  switch (corner) {
    case TOP_LEFT:
      x += halfWidth;
      y += halfHeight;
      break;
      
    case TOP_RIGHT:
      x -= halfWidth;
      y += halfHeight;
      break;
      
    case BOTTOM_LEFT:
      x += halfWidth;
      y -= halfHeight;
      break;
      
    case BOTTOM_RIGHT:
      x -= halfWidth;
      y -= halfHeight;
      break;
      
    default:
      break;
  }
  
  glPushMatrix();
    // transform
    glLoadIdentity();
    glTranslatef(x, y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glScalef(scaleX, scaleY, 0.0f);
    
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

void Sprite::clip(float x, float y, float w, float h) {
  halfWidth = w/2;
  halfHeight = h/2;
  texCoordX0 = x/widthTexture; texCoordX1 = (x + w)/widthTexture;
  texCoordY0 = y/heightTexture; texCoordY1 = (y + h)/heightTexture;
  vertexCoordX0 = -halfWidth; vertexCoordX1 = halfWidth;
  vertexCoordY0 = -halfHeight; vertexCoordY1 = halfHeight;
}

void Sprite::resetClip() {
  clip(0.0f, 0.0f, widthTexture, heightTexture);
}

} // namespace metallicar
