/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>
#include "m_time.h"

#ifdef _WIN32
  #include <windows.h>
#else
  #include <sys/time.h>
#endif

static double m_time_last = 0;
static double m_time_delta = 0;
static double m_time_average = 0;
static double m_time_avgTimer = 0;
static double m_time_avgAcc = 1;
static double m_time_avgCount = 1;


double time_getNow(void) {
  double t;
#ifdef _WIN32
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  t = (ft.dwHighDateTime * 4294967296.0 / 1e7) + ft.dwLowDateTime / 1e7;
  t -= 11644473600.0;
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  t = tv.tv_sec + tv.tv_usec / 1e6;
#endif
  return t;
}


double time_getTime(void) {
  // printf("TIME: %f\n", SDL_GetTicks() / 1000.);
  return SDL_GetTicks() / 1000.;
}


int time_sleep(double t) {
  // printf("SLEEP: %f\n", t);
  SDL_Delay(t * 1000.);
  return 0;
}


void time_step(void) {
  int now = time_getTime();
  if (m_time_last == 0) m_time_last = now;
  m_time_delta = now - m_time_last;
  m_time_last = now;
  m_time_avgTimer -= m_time_delta;
  m_time_avgAcc += m_time_delta;
  m_time_avgCount += 1;;
  if (m_time_avgTimer <= 0) {
    m_time_average = m_time_avgAcc / m_time_avgCount;
    m_time_avgTimer = m_time_avgTimer + 1;
    m_time_avgCount = 0;
    m_time_avgAcc = 0;
  }
}


double time_getDelta(void) {
  return m_time_delta;
}



double time_getAverage(void){
  return m_time_average;
}


int time_getFps(void){
  return floor(1 / m_time_average + .5);
}
