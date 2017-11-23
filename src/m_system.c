/**
 * Copyright (c) 2017 emkoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <SDL/SDL.h>
#include "util.h"
#include "m_system.h"
#include "m_graphics.h"

int QUITC = 0;

int titleAnim(void *data) {
  //While the program is not over
  while(QUITC == 0) {
    //Do the caption animation
    SDL_WM_SetCaption("sdl_seraph", NULL);
    SDL_Delay(250);

    SDL_WM_SetCaption("sdl_seraph.", NULL);
    SDL_Delay(250);

    SDL_WM_SetCaption("sdl_seraph..", NULL);
    SDL_Delay(250);

    SDL_WM_SetCaption("sdl_seraph...", NULL);
    SDL_Delay(250);
  }
  return 0;
}

void system_poll() {
  /* Handle events */
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT:
        QUITC = 1;
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
