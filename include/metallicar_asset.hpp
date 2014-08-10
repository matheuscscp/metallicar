/*
 * metallicar_asset.hpp
 *
 *  Created on: Jul 14, 2014
 *      Author: Pimenta
 */

#ifndef METALLICAR_ASSET_HPP_
#define METALLICAR_ASSET_HPP_

// standard
#include <map>
#include <string>

// lib
#include "SDL_opengl.h"

// local
#include "Color.hpp"
#include "Corner.hpp"
#include "geometry.hpp"

namespace metallicar {

class Asset {
  public:
    virtual ~Asset();
};

class Assets {
  private:
    std::map<std::string, Asset*> assets;
    
    static Assets* instance;
  public:
    Assets();
    ~Assets();
    
    template <class AssetClass, typename... Args>
    static AssetClass* get(const std::string& name, Args&&... args) {
      auto& asset = instance->assets[name];
      if (!asset) {
        asset = new AssetClass(std::forward<Args>(args)...);
      }
      return (AssetClass*)asset;
    }
};

class Texture2D : public Asset {
  protected:
    int w, h;
    GLuint texture;
  public:
    Texture2D(const std::string& path);
    Texture2D(int w, int h, GLuint texture);
    virtual ~Texture2D();
    virtual int width() const;
    virtual int height() const;
    virtual GLuint id() const;
};

class TextureRenderer2D {
  protected:
    Texture2D* texture;
    float widthTexture, heightTexture;
    GLint filter;
    Color color;
    geometry::Point2 position;
    Corner corner;
    float angle;
    geometry::Point2 scale;
    float clipHalfWidth, clipHalfHeight;
    float texCoordX0, texCoordX1;
    float texCoordY0, texCoordY1;
    float vertexCoordX0, vertexCoordX1;
    float vertexCoordY0, vertexCoordY1;
  public:
    TextureRenderer2D(Texture2D* texture);
    virtual ~TextureRenderer2D();
    virtual int width() const;
    virtual int height() const;
    virtual GLuint textureID() const;
    virtual void setFilter(bool linear);
    virtual void setColor(const Color& color);
    virtual void setOpacity(float opacity);
    virtual void setPosition(const geometry::Point2& position);
    virtual void setCorner(Corner corner);
    virtual void setAngle(float angle);
    virtual void setScale(const geometry::Point2& scale);
    virtual void clip(const geometry::Rectangle& clipRect);
    virtual void resetClip();
    virtual void render() const;
  protected:
    virtual void adjustPosition();
};

} // namespace metallicar

#endif /* METALLICAR_ASSET_HPP_ */
