# Aproximación de Pi con Monte Carlo

Este código implementa el método de Monte Carlo para aproximar el valor de pi. Incluye implementaciones secuenciales y paralelas utilizando OpenMP.

## Integrantes

- Mauricio Bustos Miranda
- Álvaro Ubilla Oliva

## Requisitos

Para compilar y ejecutar este código, necesitas:

- Un compilador de C++ que admita el estándar C++11.
- OpenMP (opcional, para ejecución paralela).

## Uso

1. Clona el repositorio o descarga el archivo de código fuente.

2. Compila el código usando el siguiente comando:

   ```bash
   $ g++ -std=c++11 -fopenmp main.cpp -o pi_montecarlo
   ```

3. Ejecuta el archivo compilado:

   ```bash
   $ ./pi_montecarlo
   ```

4. El programa generará un archivo llamado `resultados.csv` que contiene los resultados de la aproximación de Monte Carlo para diferentes iteraciones y cantidades de hilos.

## Algoritmo

El método de Monte Carlo estima el valor de pi generando puntos aleatorios dentro de un cuadrado unitario y contando la cantidad de puntos que caen dentro de un cuarto de un círculo unitario. La proporción de puntos dentro del círculo con respecto al número total de puntos proporciona una aproximación del valor de pi.

El código proporciona dos implementaciones:

1. Secuencial: `monteCarloSecuencial(int puntos_totales)`

   Esta función utiliza un enfoque secuencial para generar puntos aleatorios y contar la cantidad de puntos dentro del círculo.

2. Paralelo: `monteCarloParalelo(int puntos_totales)`

   Esta función utiliza OpenMP para paralelizar el proceso de generación y conteo de puntos. Divide el número total de puntos entre múltiples hilos y combina los resultados para obtener la aproximación final.

## Resultados

El código realiza una serie de iteraciones, aumentando la cantidad de hilos utilizados en cada iteración. Para cada iteración, mide el tiempo de ejecución y calcula diversas métricas de rendimiento, incluyendo speedup, eficiencia relativa, mejora máxima y overhead. Los resultados se guardan en el archivo `resultados.csv`.

## Licencia

Este código se distribuye bajo la [Licencia MIT](https://choosealicense.com/licenses/mit/). Siéntete libre de modificarlo y distribuirlo según tus necesidades.
