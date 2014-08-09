/*
 * metallicar_io.hpp
 *
 *  Created on: Jul 25, 2014
 *      Author: Pimenta
 */

#ifndef METALLICAR_IO_HPP_
#define METALLICAR_IO_HPP_

// lib
#include "SDL.h"

// local
#include "observer.hpp"
#include "Point2.hpp"

namespace metallicar {

struct WindowOptions {
  std::string title;
  int width;
  int height;
  int gameWidth;
  int gameHeight;
  bool fullscreen;
  std::string icon;
  bool cursor;
  WindowOptions(
    const std::string& title = "metallicar game",
    int width = 640,
    int height = 480,
    int gameWidth = 640,
    int gameHeight = 480,
    bool fullscreen = false,
    const std::string& icon = "asset/icon.png",
    bool cursor = true
  );
};

class Window {
  public:
    static void init(const WindowOptions& options = WindowOptions());
    static void close();
    
    static WindowOptions getOptions();
    static void setOptions(const WindowOptions& options);
    
    static void setOpenGLVersion(int major, int minor);
    
    static SDL_GLContext createOpenGLContext();
    static void setOpenGLContext(SDL_GLContext glContext);
    static SDL_GLContext getOpenGLContext();
    static void destroyOpenGLContext(SDL_GLContext glContext);
    
    static void update();
};

class Input {
  public:
    class KeyDownEvent : public observer::Event<KeyDownEvent> {
      private:
        SDL_Keycode keycode;
      public:
        KeyDownEvent(SDL_Keycode keycode);
        SDL_Keycode key() const;
    };
    
    class KeyUpEvent : public observer::Event<KeyUpEvent> {
      private:
        SDL_Keycode keycode;
      public:
        KeyUpEvent(SDL_Keycode keycode);
        SDL_Keycode key() const;
    };
    
    class ButtonDownEvent : public observer::Event<ButtonDownEvent> {
      private:
        uint8_t buttoncode;
      public:
        ButtonDownEvent(uint8_t buttoncode);
        uint32_t button() const;
    };
    
    class ButtonUpEvent : public observer::Event<ButtonUpEvent> {
      private:
        uint8_t buttoncode;
      public:
        ButtonUpEvent(uint8_t buttoncode);
        uint32_t button() const;
    };
    
    class QuitEvent : public observer::Event<QuitEvent> {
      
    };
  private:
    static observer::Subject subject;
  public:
    static void update();
    
    static bool quitRequested();
    static void resetQuitRequest();
    
    static bool key(SDL_Keycode keycode);
    static bool button(uint8_t buttoncode);
    static Point2 mouse();
    static Point2 mouseDown();
    static Point2 mouseUp();
    
    template <class T>
    static observer::Connection connect(
      const std::function<void(const observer::EventBase&)>& callback
    ) {
      return subject.connect<T>(callback);
    }
};

} // namespace metallicar

#endif /* METALLICAR_IO_HPP_ */
