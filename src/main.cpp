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

// Constantes para el ancho y alto de la ventana.
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Intervalo de tiempo para que aparezca un nuevo elemento en pantalla.
const int NEW_LINE_INTERVAL_MS = 2000;

// Constantes relacionadas al cambio de dirección de un elemento.
const int MIN_DIRECTION_CHANGE_DISTANCE = 20;
const double DIRECTION_CHANGE_PROBABILITY = 0.1;

// Estructura que define el comportamiento de una línea.
struct Line {

  // Propiedades que indican la posición y velocidad del elemento.
  int x1, y1, x2, y2;
  int vx, vy;

  // Dirección inicial del elemento.
  int initialDirection;

  // Color del elemento.
  SDL_Color color;

  // Constructor del elemento, inicializa un color aleatorio.
  Line(int startX, int startY, int direction) : x1(startX), y1(startY), x2(startX), y2(startY), vx(1), vy(0), initialDirection(direction) {
    color.r = rand() % 256;
    color.g = rand() % 256;
    color.b = rand() % 256;
    color.a = 255;
  }

  // Función que maneja la lógica de actualizar la posición del elemento.
  void update() {

    /**
     * Todo lo que está en esta función es magia que al final no funcionó.
     * Hay qué cambiar toda la animación y la lógica de los elementos de la pantalla.
     */

    int xLength = x2 - x1;
    int yLength = y2 - y1;

    if (xLength <= 20 || yLength <= 20) {
      switch (initialDirection) {
        case 0:
          y2 -= vy;
          break;
        case 1:
          y2 += vy;
          break;
        case 2:
          x2 -= vx;
          break;
        case 3:
          x2 += vx;
          break;
      }
    }

    if (x1 % MIN_DIRECTION_CHANGE_DISTANCE == 0 && y1 % MIN_DIRECTION_CHANGE_DISTANCE == 0) {
      if (rand() / static_cast<double>(RAND_MAX) < DIRECTION_CHANGE_PROBABILITY) {
        changeDirection();
      }
    }

    if (x1 < 20 || x1 >= SCREEN_WIDTH - 20 || y1 < 20 || y1 >= SCREEN_HEIGHT - 20) {
      changeDirection();
    }
  }

  // Función que maneja la lógica de cambiar la dirección del elemento.
  void changeDirection() {

    /**
     * Todo lo que está en esta función es magia que al final no funcionó.
     * Hay qué cambiar toda la animación y la lógica de los elementos de la pantalla.
     */

    int direction = rand() % 4;
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

// Función main que ejecuta el código.
int main(int argc, char* argv[]) {

  // Seed aleatoria para generar números pseudoaleatorios para el proyecto.
  srand(static_cast<unsigned int>(time(nullptr)));

  // Inicialización de SDL y verificación de errores.
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Instancia de la ventana del proyecto.
  SDL_Window* window = SDL_CreateWindow("Moving Line Screensaver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  // Verificación de que la ventana se haya creado correctamente.
  if (window == nullptr) {
    std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // Instancia del renderer del proyecto.
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Verificación de que el renderer se haya creado correctamente.
  if (renderer == nullptr) {
    std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Vector de líneas a renderizar.
  std::vector<Line> lines;

  // Variables útiles para cerrar la ventana.
  bool quit = false;
  SDL_Event e;

  // Tiempo transcurrido para generar un nuevo elemento.
  Uint32 lastNewLineTime = 0;
  Uint32 currentTime = SDL_GetTicks();

  // Ciclos que mantienen la ventana abierta.
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    // Tiempo en el frame que esté ocurriendo.
    currentTime = SDL_GetTicks();

    // Verificación de que el tiempo haya transcurrido para generar un nuevo elemento.
    if (currentTime - lastNewLineTime >= NEW_LINE_INTERVAL_MS) {
      lastNewLineTime = currentTime;

      /**
       * Todo lo que está en esta función es magia que al final no funcionó.
       * Hay qué cambiar toda la animación y la lógica de los elementos de la pantalla.
       */

      int startingPoint = rand() % 4;
      int north, south, east, west;
      int initialX, initialY;

      switch (startingPoint) {
        case 0:
          initialX = rand() % (SCREEN_WIDTH - 100) + 50;
          initialY = 0;
          break;
        case 1:
          initialX = rand() % (SCREEN_WIDTH - 100) + 50;
          initialY = SCREEN_HEIGHT;
          break;
        case 2:
          initialX = 0;
          initialY = rand() % (SCREEN_HEIGHT - 100) + 50;
          break;
        case 3:
          initialX = SCREEN_WIDTH;
          initialY = rand() % (SCREEN_HEIGHT - 100) + 50;
          break;
      }

      // Llamada al constructor de la línea y almacenamiento de la misma en el vector de líneas.
      Line newLine(
        initialX,
        initialY,
        startingPoint
      );
      lines.push_back(newLine);
    }

    // Funciones de SDL para limpiar la ventana y renderizar el siguiente frame.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Ciclo que renderiza cada línea del proyecto.
    for (Line& line : lines) {
      line.update();
      SDL_SetRenderDrawColor(renderer, line.color.r, line.color.g, line.color.b, line.color.a);
      SDL_RenderDrawLine(renderer, line.x1, line.y1, line.x2, line.y2);
    }

    // Función para mostrar el frame.
    SDL_RenderPresent(renderer);
  }

  // Funciones para finalizar la ejecución de un proyecto de SDL.
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  // Retorno de la función.
  return 0;
}
