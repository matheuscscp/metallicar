/*
 * Font.cpp
 *
 *  Created on: Aug 25, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

// local
#include "Path.hpp"
#include "Log.hpp"
#include "String.hpp"

using namespace std;

namespace metallicar {

Font::Font(const string& path, int ptsize) :
path(path),
ptsize(ptsize),
font(TTF_OpenFont(Path::get(path).c_str(), ptsize))
{
  if (!font) {
    Log::message(
      Log::Error,
      String::from("Font \"%s\" could not be loaded", Path::get(path).c_str())
    );
    exit(0);
  }
}

Font::~Font() {
  TTF_CloseFont(font);
}

Texture2D* Font::render(const string& text, int style, bool filter) const {
  TTF_SetFontStyle(font, style);
  SDL_Surface* tmp;
  if (filter) {
    tmp = TTF_RenderUTF8_Shaded(font, text.c_str(), SDL_Color(), SDL_Color());
  }
  else {
    tmp = TTF_RenderUTF8_Solid(font, text.c_str(), SDL_Color());
    uint8_t* pix = (uint8_t*)tmp->pixels;
    int max = tmp->w*tmp->h;
    for (int i = 0; i < max; i++) {
      pix[i] = ~(pix[i] - 1);
    }
  }
  Texture2D* tex = new Texture2D(GL_ALPHA, tmp->w, tmp->h, tmp->pixels);
  SDL_FreeSurface(tmp);
  return tex;
}

shared_ptr<Font> Font::getFont(const string& path, int ptsize) {
  string name = String::from("%s%d", path.c_str(), ptsize);
  shared_ptr<Font> font = Assets::get<Font>(name);
  if (font.get()) {
    return font;
  }
  return Assets::put(name, shared_ptr<Font>(new Font(path, ptsize)));
}

} // namespace metallicar
