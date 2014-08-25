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
#include <memory>

// lib
#include "SDL_opengl.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"

// local
#include "Color.hpp"
#include "geometry.hpp"

namespace metallicar {

class Asset {
  public:
    virtual ~Asset();
};

class Assets {
  private:
    static std::map<std::string, std::shared_ptr<Asset>> assets;
  public:
    template <class AssetClass>
    static std::shared_ptr<AssetClass> put(
      const std::string& name,
      const std::shared_ptr<AssetClass>& asset
    ) {
      assets[name] = asset;
      return asset;
    }
    
    template <class AssetClass>
    static std::shared_ptr<AssetClass> get(const std::string& name) {
      auto asset = assets.find(name);
      if (asset == assets.end()) {
        return nullptr;
      }
      return std::dynamic_pointer_cast<AssetClass>(asset->second);
    }
    
    static void clear();
};

class Texture2D : public Asset {
  protected:
    int w, h;
    GLuint texture;
  public:
    Texture2D(GLenum format, int w, int h, void* pixels);
    virtual ~Texture2D();
    virtual int width() const;
    virtual int height() const;
    virtual GLuint id() const;
};

class TextureRenderer2D {
  protected:
    std::shared_ptr<Texture2D> tex;
    float widthTexture, heightTexture;
    GLint filter;
    Color color;
    geometry::Point2 rawPosition, position;
    geometry::Rectangle::Spot spot;
    float angle;
    geometry::Point2 scale;
    float clipHalfWidth, clipHalfHeight;
    float texCoordX0, texCoordX1;
    float texCoordY0, texCoordY1;
    float vertexCoordX0, vertexCoordX1;
    float vertexCoordY0, vertexCoordY1;
    float horizontalFlip, verticalFlip;
  public:
    TextureRenderer2D(const std::shared_ptr<Texture2D>& texture);
    TextureRenderer2D(Texture2D* texture);
    virtual ~TextureRenderer2D();
    virtual std::shared_ptr<Texture2D> texture() const;
    virtual void setFilter(bool linear);
    virtual void setColor(const Color& color);
    virtual void setOpacity(float opacity);
    virtual void setPosition(const geometry::Point2& position);
    virtual void setSpot(geometry::Rectangle::Spot spot);
    virtual void setAngle(float angle);
    virtual void setScale(const geometry::Point2& scale);
    virtual void setFlip(bool horizontal, bool vertical);
    virtual void clip(const geometry::Rectangle& clipRect);
    virtual void resetClip();
    virtual void render() const;
    virtual void render(double z) const;
  protected:
    virtual void adjustPosition();
};

class Image : public Asset {
  protected:
    std::string path;
    SDL_Surface* image;
  public:
    Image(const std::string& path);
    virtual ~Image();
    virtual Texture2D* generateTexture() const;
    
    static std::shared_ptr<Texture2D> getTexture(const std::string& path);
};

class Font : public Asset {
  protected:
    std::string path;
    int ptsize;
    TTF_Font* font;
  public:
    Font(const std::string& path, int ptsize);
    virtual ~Font();
    virtual Texture2D* render(
      const std::string& text, int style, bool filter
    ) const;
    
    static std::shared_ptr<Font> getFont(const std::string& path, int ptsize);
};

} // namespace metallicar

#endif /* METALLICAR_ASSET_HPP_ */
