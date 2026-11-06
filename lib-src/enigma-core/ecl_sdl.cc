#include "ecl_sdl.hh"

namespace sdl {

// Dispatch an event to the suitable virtual function. Returns
// true if event was handled.
bool EventHandler::dispatch_event(SDL_Event &e) {
    bool handled = false;

    switch (e.type) {
    case SDL_EVENT_KEY_DOWN: handled = on_keydown(e); break;
    case SDL_EVENT_KEY_UP: handled = on_keyup(e); break;
    case SDL_EVENT_MOUSE_MOTION: handled = on_mousemotion(e); break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP: handled = on_mousebutton(e); break;
    case SDL_EVENT_QUIT: handled = on_quit(e); break;
    }
    return handled || on_event(e);
}

void FlushEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e))
        ;
}

}  // namespace sdl
