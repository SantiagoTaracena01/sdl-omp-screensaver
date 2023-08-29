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

// Librerías necesarias para el desarrollo del proyecto.
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Constantes que representan la base y altura de la pantalla.
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Constante que representa los milisegundos para crear una nueva línea.
const int NEW_LINE_INTERVAL_MS = 2000;

// Constantes de las direcciones a actualizar por la línea.
const int MIN_DIRECTION_CHANGE_DISTANCE = 20;
const double DIRECTION_CHANGE_PROBABILITY = 0.1;

// Estructura que representa una línea.
struct Line {

  // Propiedades de posición, velocidad y color de la línea.
  int x1, y1, x2, y2;
  int vx, vy;
  SDL_Color color;

  // Constructor de la línea.
  Line(int startX, int startY, int endX, int endY) : x1(startX), y1(startY), x2(endX), y2(endY), vx(1), vy(0) {
    color.r = rand() % 256;
    color.g = rand() % 256;
    color.b = rand() % 256;
    color.a = 255;
  }

  // Función para actualizar la posición de la línea.
  void update() {

    // Cambio de la posición de la línea según la velocidad.
    x1 += vx;
    y1 += vy;
    x2 += vx;
    y2 += vy;

    // Chequeo de la posibilidad de cambiar la dirección de la líena.
    if (((x1 % MIN_DIRECTION_CHANGE_DISTANCE) == 0) && ((y1 % MIN_DIRECTION_CHANGE_DISTANCE) == 0)) {
      if (rand() / static_cast<double>(RAND_MAX) < DIRECTION_CHANGE_PROBABILITY) {
        changeDirection();
      }
    }

    // Revisión de los límites de la pantalla.
    if ((x1 < 0) || (x1 >= SCREEN_WIDTH) || (y1 < 0) || (y1 >= SCREEN_HEIGHT)) {
      changeDirection();
    }
  }

  // Función para realizar un cambio de dirección.
  void changeDirection() {

    // Próxima dirección de la línea.
    int direction = rand() % 4;

    // Análisis y cambio de la dirección.
    switch (direction) {
      case 0:
        vx = 0;
        vy = -1;
        break;
      case 1:
        vx = 0;
        vy = 1;
        break;
      case 2:
        vx = -1;
        vy = 0;
        break;
      case 3:
        vx = 1;
        vy = 0;
        break;
    }
  }
};

int main(int argc, char* argv[]) {
  srand(static_cast<unsigned int>(time(nullptr))); // Initialize random seed

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("Moving Line Screensaver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == nullptr) {
    std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == nullptr) {
    std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  std::vector<Line> lines;

  bool quit = false;
  SDL_Event e;

  Uint32 lastNewLineTime = 0;
  Uint32 currentTime = SDL_GetTicks();

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    currentTime = SDL_GetTicks();

    int xFactor = rand() % 2;
    int yFactor = !xFactor;

    if (currentTime - lastNewLineTime >= NEW_LINE_INTERVAL_MS) {
      lastNewLineTime = currentTime;
      Line newLine(
        rand() % SCREEN_WIDTH * xFactor,
        rand() % SCREEN_HEIGHT * yFactor,
        rand() % SCREEN_WIDTH * xFactor,
        rand() % SCREEN_HEIGHT * yFactor
      );
      lines.push_back(newLine);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (Line& line : lines) {
      line.update();
      SDL_SetRenderDrawColor(renderer, line.color.r, line.color.g, line.color.b, line.color.a);
      SDL_RenderDrawLine(renderer, line.x1, line.y1, line.x2, line.y2);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}