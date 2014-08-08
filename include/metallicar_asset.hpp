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

namespace metallicar {

class Asset {
  public:
    virtual ~Asset();
};

class Assets {
  private:
    std::map<std::string, Asset*> assets;
  public:
    Assets();
    ~Assets();
    
    template <class AssetClass, typename... Args>
    AssetClass* get(const std::string& path, Args&&... args) {
      auto& asset = assets[path];
      if (!asset) {
        asset = new AssetClass(path, std::forward<Args>(args)...);
      }
      return (AssetClass*)asset;
    }
    
    static Assets& instance();
};

class Texture : public Asset {
  protected:
    int w, h;
    GLuint texture;
  public:
    Texture(const std::string& path);
    virtual ~Texture();
    virtual int width() const;
    virtual int height() const;
    virtual GLuint id() const;
};

class Sprite {
  protected:
    Texture* texture;
    float widthTexture, heightTexture;
    float texCoordX0, texCoordX1;
    float texCoordY0, texCoordY1;
    float vertexCoordX0, vertexCoordX1;
    float vertexCoordY0, vertexCoordY1;
    int width, height;
    float halfWidth, halfHeight;
  public:
    Sprite(const std::string& path);
    virtual ~Sprite();
    virtual int getWidth() const;
    virtual int getHeight() const;
    virtual void render(
      float x = 0.0f,
      float y = 0.0f,
      Corner corner = Corner::TOP_LEFT,
      float opacity = 1.0f,
      float angle = 0.0f,
      float scaleX = 1.0f,
      float scaleY = 1.0f,
      const Color& color = Color::WHITE,
      bool linearFilter = true
    );
    virtual void clip(float x, float y, float w, float h);
    virtual void resetClip();
};

} // namespace metallicar

#endif /* METALLICAR_ASSET_HPP_ */
