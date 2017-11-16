/**
 * Copyright (c) 2017 emkoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include "m_system.h"
#include "m_graphics.h"

void system_poll() {
  /* Handle events */
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT:
        QUIT: exit(EXIT_SUCCESS);
        break;

      case SDL_VIDEORESIZE:
       graphics_setSize(e.resize.w, e.resize.h);
       break;

      case SDL_KEYUP:
        if (e.key.keysym.sym == SDLK_ESCAPE) goto QUIT;
        break;
    }
  }
}
