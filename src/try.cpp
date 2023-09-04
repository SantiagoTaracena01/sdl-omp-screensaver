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

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PARTICLE_RADIUS = 10;
const int GRAVITY = 1;
const int FPS_DELAY = 1000 / 60;  // 60 FPS

// Intervalo de tiempo para que aparezca un nuevo elemento en pantalla.
const int NEW_LINE_INTERVAL_MS = 2000;

// Estructura que define el comportamiento de una aprticula
struct Particle {
  int x, y;
  int vx, vy;

  // Color del elemento.
  SDL_Color color;

  // Constructor del elemento, inicializa un color aleatorio
  Particle() {
    x = rand() % (SCREEN_WIDTH - PARTICLE_RADIUS * 2) + PARTICLE_RADIUS;
    y = rand() % (SCREEN_HEIGHT - PARTICLE_RADIUS * 2) + PARTICLE_RADIUS;
    vx = rand() % 5 - 2;
    vy = rand() % 5 - 2;

    // Genera un color aleatorio para cada partícula
    color.r = rand() % 256;
    color.g = rand() % 256;
    color.b = rand() % 256;
    color.a = 255;
}

    void update() {
        // Aplicar gravedad
        vy += GRAVITY;

        // Actualizar posición
        x += vx;
        y += vy;

        // Rebote en las paredes
        if (x - PARTICLE_RADIUS < 0 || x + PARTICLE_RADIUS > SCREEN_WIDTH) {
            vx = -vx;
        }
        if (y - PARTICLE_RADIUS < 0 || y + PARTICLE_RADIUS > SCREEN_HEIGHT) {
            vy = -vy;
        }

        // Actualizar la posición en función de la velocidad
        x += vx;
        y += vy;

    }
};

// Dibujar un circulo
void SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    // Este bucle itera a través de cada píxel en un cuadrado con lados de longitud (radius * 2)
    for (int w = 0; w < radius * 2; ++w) {
        for (int h = 0; h < radius * 2; ++h) {
            // Calcula la distancia desde el centro del círculo al píxel actual
            int dx = radius - w; // Desplazamiento horizontal desde el centro
            int dy = radius - h; // Desplazamiento vertical desde el centro

            // Verifica si el píxel actual está dentro del círculo utilizando la ecuación del círculo:
            // (dx*dx + dy*dy) <= (radius * radius)
            // Si la distancia al cuadrado desde el centro es menor o igual al radio al cuadrado,
            // entonces el píxel está dentro del círculo
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                // Dibuja el píxel en la posición correspondiente
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}



int main(int argc, char* argv[]) {
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

        if (currentTime - lastFrameTime >= FPS_DELAY) {
            lastFrameTime = currentTime;

            // Verifica si ha pasado al menos 2 segundos desde la última creación de partícula.
            if (currentTime - lastNewParticleTime >= NEW_LINE_INTERVAL_MS) {
                lastNewParticleTime = currentTime;

                particles.push_back(Particle());

            }

            for (Particle& particle : particles) {
                particle.update();
            }

            // Limpiar la pantalla
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Dibujar las partículas con colores aleatorios
            for (const Particle& particle : particles) {
                SDL_SetRenderDrawColor(renderer, particle.color.r, particle.color.g, particle.color.b, particle.color.a);
                SDL_RenderDrawCircle(renderer, particle.x, particle.y, PARTICLE_RADIUS);
            }

            // Mostrar FPS
            float fps = 1000.0f / (currentTime - lastFrameTime);
            std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));
            SDL_SetWindowTitle(window, fpsText.c_str());

            // Renderizar la pantalla
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}