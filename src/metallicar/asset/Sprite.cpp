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

Sprite::Sprite(Texture2D* texture) :
texture(texture),
widthTexture(float(texture->width())),
heightTexture(float(texture->height())),
filter(GL_LINEAR),
color(Color::WHITE),
position(geometry::Point2(0.0f, 0.0f)),
corner(Corner::TOP_LEFT),
angle(0.0f),
scale(geometry::Point2(1.0f, 1.0f))
{
  resetClip();
}

Sprite::~Sprite() {
  
}

int Sprite::width() const {
  return texture->width();
}

int Sprite::height() const {
  return texture->height();
}

GLuint Sprite::textureID() const {
  return texture->id();
}

void Sprite::setFilter(bool linear) {
  filter = linear ? GL_LINEAR : GL_NEAREST;
}

void Sprite::setColor(const Color& color) {
  float opacity = this->color.a;
  this->color = color;
  this->color.a = opacity;
}

void Sprite::setOpacity(float opacity) {
  color.a = opacity;
}

void Sprite::setPosition(const geometry::Point2& position) {
  this->position = position;
  adjustPosition();
}

void Sprite::setCorner(Corner corner) {
  this->corner = corner;
  adjustPosition();
}

void Sprite::setAngle(float angle) {
  this->angle = angle;
}

void Sprite::setScale(const geometry::Point2& scale) {
  this->scale = scale;
}

void Sprite::clip(const geometry::Rectangle& clipRect) {
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

void Sprite::resetClip() {
  clip(geometry::Rectangle(0.0f, 0.0f, widthTexture, heightTexture));
}

void Sprite::render() {
  texture->render2D(
    filter,
    color,
    position,
    angle,
    scale,
    texCoordX0,
    texCoordX1,
    texCoordY0,
    texCoordY1,
    vertexCoordX0,
    vertexCoordX1,
    vertexCoordY0,
    vertexCoordY1
  );
}

void Sprite::adjustPosition() {
  switch (corner) {
    case TOP_LEFT:
      this->position.x += clipHalfWidth;
      this->position.y += clipHalfHeight;
      break;
      
    case TOP_RIGHT:
      this->position.x -= clipHalfWidth;
      this->position.y += clipHalfHeight;
      break;
      
    case BOTTOM_LEFT:
      this->position.x += clipHalfWidth;
      this->position.y -= clipHalfHeight;
      break;
      
    case BOTTOM_RIGHT:
      this->position.x -= clipHalfWidth;
      this->position.y -= clipHalfHeight;
      break;
      
    default:
      break;
  }
}

} // namespace metallicar
