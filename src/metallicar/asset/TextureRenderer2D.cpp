/*
 * TextureRenderer2D.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

// local
#include "metallicar_core.hpp"

using namespace std;

namespace metallicar {

TextureRenderer2D::TextureRenderer2D(const shared_ptr<Texture2D>& texture) :
tex(texture),
widthTexture(float(texture->width())),
heightTexture(float(texture->height())),
filter(GL_LINEAR),
color(Color::WHITE),
rawPosition(geometry::Point2(0.0f, 0.0f)),
spot(geometry::Rectangle::Spot::TOP_LEFT),
angle(0.0f),
scale(geometry::Point2(1.0f, 1.0f)),
horizontalFlip(0.0f),
verticalFlip(0.0f)
{
  resetClip();
}

TextureRenderer2D::~TextureRenderer2D() {
  
}

shared_ptr<Texture2D> TextureRenderer2D::texture() const {
  return tex;
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
  rawPosition = position;
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
  adjustPosition();
}

void TextureRenderer2D::setFlip(bool horizontal, bool vertical) {
  horizontalFlip = horizontal ? 180.0f : 0.0f;
  verticalFlip = vertical ? 180.0f : 0.0f;
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

void TextureRenderer2D::render(double z) const {
  TextureRenderer2D tmp(*this);
  Game::addRenderer(z, [tmp]() {
    // bind
    glBindTexture(GL_TEXTURE_2D, tmp.tex->id());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tmp.filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tmp.filter);
    
    glColor4f(tmp.color.r, tmp.color.g, tmp.color.b, tmp.color.a);
    
    glPushMatrix();
      // transform
      glLoadIdentity();
      glTranslatef(tmp.position.x, tmp.position.y, 0.0f);
      glRotatef(tmp.angle, 0.0f, 0.0f, 1.0f);
      glRotatef(tmp.horizontalFlip, 0.0f, 1.0f, 0.0f);
      glRotatef(tmp.verticalFlip, 1.0f, 0.0f, 0.0f);
      glScalef(tmp.scale.x, tmp.scale.y, 1.0f);
      
      // render
      glBegin(GL_QUADS);
        glTexCoord2f(tmp.texCoordX0, tmp.texCoordY0);
        glVertex2f(tmp.vertexCoordX0, tmp.vertexCoordY0);
        glTexCoord2f(tmp.texCoordX1, tmp.texCoordY0);
        glVertex2f(tmp.vertexCoordX1, tmp.vertexCoordY0);
        glTexCoord2f(tmp.texCoordX1, tmp.texCoordY1);
        glVertex2f(tmp.vertexCoordX1, tmp.vertexCoordY1);
        glTexCoord2f(tmp.texCoordX0, tmp.texCoordY1);
        glVertex2f(tmp.vertexCoordX0, tmp.vertexCoordY1);
      glEnd();
    glPopMatrix();
  });
}

void TextureRenderer2D::adjustPosition() {
  position = rawPosition;
  switch (spot) {
    case geometry::Rectangle::Spot::TOP_LEFT:
      this->position.x += clipHalfWidth*scale.x;
      this->position.y += clipHalfHeight*scale.y;
      break;
      
    case geometry::Rectangle::Spot::TOP_RIGHT:
      this->position.x -= clipHalfWidth*scale.x;
      this->position.y += clipHalfHeight*scale.y;
      break;
      
    case geometry::Rectangle::Spot::BOTTOM_LEFT:
      this->position.x += clipHalfWidth*scale.x;
      this->position.y -= clipHalfHeight*scale.y;
      break;
      
    case geometry::Rectangle::Spot::BOTTOM_RIGHT:
      this->position.x -= clipHalfWidth*scale.x;
      this->position.y -= clipHalfHeight*scale.y;
      break;
      
    default:
      break;
  }
}

} // namespace metallicar
