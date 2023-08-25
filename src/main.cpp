/**
 * Universidad del Valle de Guatemala
 * (CC3069) Computación Paralela y Distribuida
 * Proyecto 01 - Screensaver con SDL y OpenMP
 * 
 * Miembros del equipo de trabajo:
 * - José Rodrigo Barrera García (20807)
 * - Marco Pablo Orozco Saravia (20857)
 * - Santiago Taracena Puga (20017)
 */

// Librerías necesarias para el proyecto.
#include <SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>

// Constantes necesarias para el proyecto.
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Estructura para representar las líneas a dibujar.
struct Line {
  int x1, y1, x2, y2;
  int vx, vy;
  void update() {
    x1 += vx;
    x2 += vx;
    y1 += vy;
    y2 += vy;
    // Collision logic
  }
}

// Función main que ejecuta todo el código.
int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window = SDL_CreateWindow(
    "Screensaver",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN,
  );

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  std::vector<Line> lines;

  for (int i = 0; i < 50, i++) {
    Line line;
    line.x1 = rand() % SCREEN_WIDTH;
    line.x2 = rand() % SCREEN_WIDTH;
    line.y1 = rand() % SCREEN_HEIGHT;
    line.y2 = rand() % SCREEN_HEIGHT;
    line.vx = rand() % 5 - 2;
    line.vy = rand() % 5 - 2;
    lines.push_back(line);
  }

  bool quit = false;
  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (Line& line: lines) {
      line.update();
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderDrawLine(renderer, line.x1, line.y1, line.x2, line.y2);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
