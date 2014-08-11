/*
 * TextureRenderer2D.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

namespace metallicar {

TextureRenderer2D::TextureRenderer2D(Texture2D* texture) :
texture(texture),
widthTexture(float(texture->width())),
heightTexture(float(texture->height())),
filter(GL_LINEAR),
color(Color::WHITE),
position(geometry::Point2(0.0f, 0.0f)),
spot(geometry::Rectangle::Spot::TOP_LEFT),
angle(0.0f),
scale(geometry::Point2(1.0f, 1.0f))
{
  resetClip();
}

TextureRenderer2D::~TextureRenderer2D() {
  
}

int TextureRenderer2D::width() const {
  return texture->width();
}

int TextureRenderer2D::height() const {
  return texture->height();
}

GLuint TextureRenderer2D::textureID() const {
  return texture->id();
}

void TextureRenderer2D::setFilter(bool linear) {
  filter = linear ? GL_LINEAR : GL_NEAREST;
}

void TextureRenderer2D::setColor(const Color& color) {
  float opacity = this->color.a;
  this->color = color;
  this->color.a = opacity;
}

void TextureRenderer2D::setOpacity(float opacity) {
  color.a = opacity;
}

void TextureRenderer2D::setPosition(const geometry::Point2& position) {
  this->position = position;
  adjustPosition();
}

void TextureRenderer2D::setSpot(geometry::Rectangle::Spot spot) {
  this->spot = spot;
  adjustPosition();
}

void TextureRenderer2D::setAngle(float angle) {
  this->angle = angle;
}

void TextureRenderer2D::setScale(const geometry::Point2& scale) {
  this->scale = scale;
}

void TextureRenderer2D::clip(const geometry::Rectangle& clipRect) {
  clipHalfWidth = clipRect.w/2;
  clipHalfHeight = clipRect.h/2;
  texCoordX0 = clipRect.x/widthTexture;
  texCoordX1 = (clipRect.x + clipRect.w)/widthTexture;
  texCoordY0 = clipRect.y/heightTexture;
  texCoordY1 = (clipRect.y + clipRect.h)/heightTexture;
  vertexCoordX0 = -clipHalfWidth;
  vertexCoordX1 = clipHalfWidth;
  vertexCoordY0 = -clipHalfHeight;
  vertexCoordY1 = clipHalfHeight;
  adjustPosition();
}

void TextureRenderer2D::resetClip() {
  clip(geometry::Rectangle(0.0f, 0.0f, widthTexture, heightTexture));
}

void TextureRenderer2D::render() const {
  // bind
  glBindTexture(GL_TEXTURE_2D, texture->id());
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

void TextureRenderer2D::adjustPosition() {
  switch (spot) {
    case geometry::Rectangle::Spot::TOP_LEFT:
      this->position.x += clipHalfWidth;
      this->position.y += clipHalfHeight;
      break;
      
    case geometry::Rectangle::Spot::TOP_RIGHT:
      this->position.x -= clipHalfWidth;
      this->position.y += clipHalfHeight;
      break;
      
    case geometry::Rectangle::Spot::BOTTOM_LEFT:
      this->position.x += clipHalfWidth;
      this->position.y -= clipHalfHeight;
      break;
      
    case geometry::Rectangle::Spot::BOTTOM_RIGHT:
      this->position.x -= clipHalfWidth;
      this->position.y -= clipHalfHeight;
      break;
      
    default:
      break;
  }
}

} // namespace metallicar
