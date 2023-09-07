# sdl-omp-screensaver

# Proyecto de Screensaver con SDL y OpenMP

Este proyecto es un screensaver simple que utiliza la biblioteca SDL (Simple DirectMedia Layer) y OpenMP para crear una animación de partículas coloridas en una ventana. Las partículas rebotan en las paredes y se generan de manera aleatoria en la pantalla, creando un efecto visual atractivo.

## Miembros del equipo

- José Rodrigo Barrera García (20807)
- Marco Pablo Orozco Saravia (20857)
- Santiago Taracena Puga (20017)

## Requisitos

Para compilar y ejecutar este proyecto, necesitarás:

- **SDL2**: Una biblioteca multimedia utilizada para crear la ventana y renderizar gráficos.
- **OpenMP**: Una API de programación paralela utilizada para acelerar el procesamiento de partículas.

Asegúrate de tener estas bibliotecas instaladas en tu sistema antes de compilar el proyecto.

## Compilación y ejecución

Para compilar el proyecto, puedes utilizar un comando similar al siguiente:

```bash
g++ -o parallel-main parallel-main.cpp -lSDL2 -fopenmp
```
Una vez compilado, puedes ejecutar el screensaver con:

```bash
./screensaver
```

## Uso

- El screensaver mostrará una ventana en la que las partículas se moverán y rebotarán en las paredes.
- El número de partículas generadas se controla mediante la constante NUM_NEW_PARTICLES en el código fuente.
- Puedes ajustar otros parámetros, como el tamaño de la ventana y la velocidad de fotogramas, modificando las constantes correspondientes en el código.
- El FPS (fotogramas por segundo) se muestra en la consola mientras se ejecuta el screensaver.

## Personalización

Si deseas personalizar este screensaver, aquí hay algunas ideas:

- Cambia los colores y tamaños de las partículas.
- Experimenta con diferentes ecuaciones de movimiento para las partículas.
- Agrega efectos visuales adicionales, como trazas o efectos de partículas especiales.
- Añade interacción del usuario, como eventos de clic del ratón o teclado.

## Problemas conocidos

- Puedes encontrar problemas de rendimiento si aumentas significativamente el número de partículas generadas debido a la carga en la CPU.
- Si el screensaver se bloquea o experimenta problemas de memoria, asegúrate de que la gestión de memoria sea adecuada.

## Licencia

Este proyecto se distribuye bajo la Licencia MIT. Consulta el archivo LICENSE para obtener más detalles.

## Conclusión

Disfruta de tu screensaver con SDL y OpenMP y diviértete explorando las posibilidades de personalización y mejora. Si tienes alguna pregunta o problema, no dudes en ponerte en contacto con los miembros del equipo mencionados anteriormente.
