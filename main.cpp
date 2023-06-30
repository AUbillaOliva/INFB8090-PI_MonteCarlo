#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <chrono>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

double monteCarloSecuencial(int puntos_totales) {
    // Contador de puntos dentro del c�rculo
    double puntos_dentro_circulo = 0.0;

    // Inicializar la semilla aleatoria
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Generar puntos aleatorios y contar los que est�n dentro del c�rculo
    for (int i = 0; i < puntos_totales; i++) {
        double x = static_cast<double>(std::rand()) / RAND_MAX;
        double y = static_cast<double>(std::rand()) / RAND_MAX;
        double distancia = std::sqrt(x * x + y * y);

        if (distancia <= 1.0) puntos_dentro_circulo++;
    }

    // Calcular la aproximaci�n de pi
    double aproximacion = 4.0 * puntos_dentro_circulo / puntos_totales;

    return aproximacion;
}

double monteCarloParalelo(int puntos_totales) {
    // Contador de puntos dentro del c�rculo
    double puntos_dentro_circulo = 0.0;

    // Inicializar la semilla aleatoria
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    #pragma omp parallel
    {
        // Obtener el n�mero de hilos
        int numero_hilos = omp_get_num_threads();

        // Calcular el n�mero de puntos para cada hilo
        int puntos_por_hilo = puntos_totales / numero_hilos;
        double puntos_hilo_circulo = 0.0;

        // Generar puntos aleatorios y contar los que est�n dentro del c�rculo
        for (int i = 0; i < puntos_por_hilo; i++) {
            double x = static_cast<double>(std::rand()) / RAND_MAX;
            double y = static_cast<double>(std::rand()) / RAND_MAX;
            double distancia = std::sqrt(x * x + y * y);

            if (distancia <= 1.0) puntos_hilo_circulo++;
        }

        // Sumar los resultados parciales de cada hilo
        #pragma omp atomic
        puntos_dentro_circulo += puntos_hilo_circulo;
    }

    // Calcular la aproximaci�n de pi
    double aproximacion = 4.0 * puntos_dentro_circulo / puntos_totales;

    return aproximacion;
}

int main(){
    // Aumentar el tama�o de la pila asignada al programa (solo para Windows)
    #ifdef _WIN32
        DWORD tamanoStack = 16 * 1024 * 1024 * 1024;
        SetThreadStackGuarantee(&tamanoStack);
    #endif

    int iteraciones = 100;

    std::ofstream file("resultados.csv");
    file << "N� de iteraciones,Cantidad de hilos,Tiempo secuencial (ms),Resultado secuencial,Tiempo paralelo (ms),Resultado paralelo,Aceleraci�n (Speed Up),Eficiencia Relativa,Mejora M�xima,Overhead" << std::endl;

    // Realizar las iteraciones para cada cantidad de hilos
    for (int i = 0; i < iteraciones; i++)
    {
        // Establecer el n�mero de puntos para la iteraci�n actual
        long long numero_puntos = 10000000000;

        // Establecer el n�mero de hilos para la iteraci�n actual
        int numero_hilos_actual = pow(2, i);

        // Ejecuci�n secuencial
        auto inicio_secuencial = std::chrono::high_resolution_clock::now();
        double pi_secuencial = monteCarloSecuencial(numero_puntos);
        auto termino_secuencial = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duracion_secuencial = termino_secuencial - inicio_secuencial;
        double tiempo_secuencial = duracion_secuencial.count();

        // Ejecuci�n paralela
        // Establecer el n�mero de hilos para la regi�n paralela
        #ifdef _OPENMP
        omp_set_num_threads(numero_hilos_actual);
        #endif

        // Medir el tiempo de ejecuci�n para la versi�n paralela
        auto inicio_paralelo = std::chrono::high_resolution_clock::now();
        double pi_paralelo = monteCarloParalelo(numero_puntos);
        auto termino_paralelo = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracion_paralelo = termino_paralelo - inicio_paralelo;
        double tiempo_paralelo = duracion_paralelo.count();

        // Calculo de kpis
        double speedup = tiempo_secuencial / tiempo_paralelo;
        double eficiencia = speedup / numero_hilos_actual;
        double mejora = 1.0 / (1.0 - (1.0 / numero_hilos_actual));
        double overhead = (tiempo_paralelo - tiempo_secuencial) / tiempo_secuencial * 100.0;

        // Guardar los resultados en el archivo
        file << i + 1 << "," << numero_hilos_actual << "," << tiempo_secuencial << "," << std::fixed << std::setprecision(14) << pi_secuencial << "," << tiempo_paralelo << ","
             << std::fixed << std::setprecision(14) << pi_paralelo << ","
             << speedup << ","
             << eficiencia << ","
             << mejora << ","
             << overhead << std::endl;
    }

    file.close();
    std::cout << "Los resultados se han guardado en el archivo resultados.csv" << std::endl;

    return 0;
}
