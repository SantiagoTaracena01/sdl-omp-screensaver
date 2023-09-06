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

// Ancho y alto de la pantalla.
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Radio de las partículas y gravedad.
const int PARTICLE_RADIUS = 10;
const int GRAVITY = 1;

// Constantes relacionadas al cálculo del framerate.
const int SCREEN_FPS = 60;
const int FPS_DELAY = (1000 / SCREEN_FPS);

// Intervalo de tiempo para que aparezca un nuevo elemento en pantalla.
const int NEW_CIRCLE_INTERVAL_MS = 2000;

// Estructura que define el comportamiento de una aprticula
struct Particle {

  // Propiedades de posición y velocidad.
  int x, y;
  int vx, vy;

  // Color del elemento.
  SDL_Color color;

  // Constructor del elemento, inicializa un color aleatorio.
  Particle() {

    /**
     * Particle.Particle()
     * 
     * Método constructor de la estructura Particle. No recibe ningún parámetro debido
     * a que todas las propiedades de la partícula se inicializan de forma aleatoria.
     */

    // Posición y velocidad aleatoria.
    x = rand() % (SCREEN_WIDTH - PARTICLE_RADIUS * 2) + PARTICLE_RADIUS;
    y = rand() % (SCREEN_HEIGHT - PARTICLE_RADIUS * 2) + PARTICLE_RADIUS;
    vx = rand() % 5 - 2;
    vy = rand() % 5 - 2;

    // Generación de un color aleatorio para cada partícula.
    color.r = rand() % 256;
    color.g = rand() % 256;
    color.b = rand() % 256;
    color.a = 255;
  }

  // Función utilizada para actualizar la posición de la partícula.
  void update() {

    /**
     * Particle.update()
     * 
     * Función que se utiliza para actualizar la posición de una instancia de Particle
     * en la pantalla. No recibe parámetros ya que todas las propiedades de la partícula
     * cambian de manera interna.
     */

    // Aplicación de la gravedad a la velocidad.
    vy += GRAVITY;

    // Actualización de la posición según la velocidad.
    x += vx;
    y += vy;

    // Verificaciones para hacer que la partícula rebote en las paredes.
    if (((x - PARTICLE_RADIUS) < 0) || ((x + PARTICLE_RADIUS) > SCREEN_WIDTH)) {
      vx = -vx;
    }
    if (((y - PARTICLE_RADIUS) < 0) || ((y + PARTICLE_RADIUS) > SCREEN_HEIGHT)) {
      vy = -vy;
    }

    // Actualización de la posición en función de la velocidad.
    x += vx;
    y += vy;
  }
};

// Función utilizada para dibujar un círculo.
void SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius) {

  /**
   * SDL_RenderDrawCircle
   * 
   * Función utilizada para manejar la lógica de dibujar un círculo en la pantalla.
   * Recibe como parámetros el renderer a utilizar para dibujar el círculo, la posición
   * del mismo en los ejes x, y, y el radio del mismo círculo.
   */

  // Ciclos for que iteran a través de cada píxel en un cuadrado con lados de longitud (radius * 2).
  for (int w = 0; w < (radius * 2); ++w) {
    for (int h = 0; h < (radius * 2); ++h) {

      // Calcula la distancia desde el centro del círculo al píxel actual
      int dx = (radius - w); // Desplazamiento horizontal desde el centro
      int dy = (radius - h); // Desplazamiento vertical desde el centro

      // Verifica si el píxel actual está dentro del círculo utilizando la ecuación del círculo:
      // (dx*dx + dy*dy) <= (radius * radius)
      // Si la distancia al cuadrado desde el centro es menor o igual al radio al cuadrado,
      // entonces el píxel está dentro del círculo
      if (((dx * dx) + (dy * dy)) <= (radius * radius)) {

        // Dibuja el píxel en la posición correspondiente.
        SDL_RenderDrawPoint(renderer, (x + dx), (y + dy));
      }
    }
  }
}

// Función principal que ejecuta el código.
int main(int argc, char* argv[]) {

  /**
   * main
   * 
   * Función principal donde comienza la ejecución del código. Recibe los argumentos de siempre, con
   * el objetivo de poder ingresar el número de partículas a renderizar por pantalla, y el número de
   * threads a utilizar para la ejecución del código.
   */

  // Seed aleatoria para generar números pseudoaleatorios para el proyecto.
  srand(static_cast<unsigned int>(time(nullptr)));

  // Inicialización de SDL y verificación de errores.
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Instancia de la ventana del proyecto.
  SDL_Window* window = SDL_CreateWindow("Particle Screensaver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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

  // Vector de particulas a renderizar.
  std::vector<Particle> particles;

  // Variables útiles para cerrar la ventana.
  bool quit = false;
  SDL_Event e;
  Uint32 lastFrameTime = 0;

  // Tiempo transcurrido para generar un nuevo elemento.
  Uint32 lastNewParticleTime = 0;
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

    // Verificación de que el tiempo haya sobrepasado el delay para agregar partículas.
    if ((currentTime - lastFrameTime) >= FPS_DELAY) {

      // Verifica si ha pasado al menos 2 segundos desde la última creación de partícula.
      if (currentTime - lastNewParticleTime >= NEW_CIRCLE_INTERVAL_MS) {
        lastNewParticleTime = currentTime;
        particles.push_back(Particle());
      }

      // Ciclo for que se encarga de actualizar cada una de las partículas instanciadas.
      for (Particle& particle: particles) {
        particle.update();
      }

      // Funciones de SDL para limpiar la pantalla y el renderer en general.
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      // Ciclo for que dibuja las partículas con colores aleatorios.
      for (const Particle& particle : particles) {
        SDL_SetRenderDrawColor(renderer, particle.color.r, particle.color.g, particle.color.b, particle.color.a);
        SDL_RenderDrawCircle(renderer, particle.x, particle.y, PARTICLE_RADIUS);
      }

      // Verificación y cálculo del framerate a desplegar.
      if (currentTime != lastFrameTime) {
        float fps = 1000.0f / (currentTime - lastFrameTime);
        std::string fpsText = "FPS count: " + std::to_string(static_cast<int>(fps));
        SDL_SetWindowTitle(window, fpsText.c_str());
        std::cout << "FPS count: " << fps << std::endl;
      }

      // Renderizado de la pantalla y cambio de tiempo.
      SDL_RenderPresent(renderer);
      lastFrameTime = currentTime;
    }
  }

  // Funciones utilizadas para limpiar y cerrar todo lo utilizado de SDL.
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  // Salida del programa, retorno exitoso.
  return 0;
}
