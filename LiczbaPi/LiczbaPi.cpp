#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>

/**
 * @brief Funkcja licząca wartość części całki na zadanym przedziale
 *
 * Ta funkcja oblicza część całki funkcji \(\frac{4}{1 + x^2}\) w przedziale \([start, end]\)
 * z krokiem całkowania \texttt{step}. Wynik obliczeń jest zapisywany do zmiennej \texttt{result}.
 *
 * @param start Początek przedziału (wartość początkowa zmiennej \(x\))
 * @param end Koniec przedziału (wartość końcowa zmiennej \(x\))
 * @param step Krok, który określa rozdzielczość przybliżenia całki
 * @param result Referencja do zmiennej, w której zostanie zapisany wynik obliczenia części całki
 */
void calculate_partial_integral(double start, double end, double step, double& result) {
    double partial_sum = 0.0;
    // Sumowanie wartości funkcji na zadanym przedziale
    for (double x = start; x < end; x += step) {
        partial_sum += 4.0 / (1.0 + x * x) * step;
    }
    result = partial_sum;
}

/**
 * @brief Funkcja główna programu
 *
 * Funkcja główna programu, która:
 * - przyjmuje dane wejściowe od użytkownika (liczba przedziałów oraz liczba wątków),
 * - oblicza krok całkowania,
 * - tworzy wątki i przydziela im odpowiednie zakresy przedziału do obliczeń,
 * - sumuje wyniki z poszczególnych wątków,
 * - wyświetla wynik (przybliżoną wartość liczby PI) oraz czas obliczeń.
 *
 * @return Zwraca 0 po zakończeniu programu
 */
int main() {
    size_t num_intervals;  /**< Liczba przedziałów (np. 100 mln, 1 mld, 3 mld) */
    int num_threads;       /**< Liczba wątków (1-50) */

    // Wczytywanie danych wejściowych
    std::cout << "Podaj liczbe przedzialow: ";
    std::cin >> num_intervals;
    std::cout << "Podaj liczbe watkow: ";
    std::cin >> num_threads;

    // Obliczanie kroku całkowania
    double step = 1.0 / num_intervals;

    // Wektory przechowujące wątki i wyniki
    std::vector<std::thread> threads;
    std::vector<double> results(num_threads, 0.0);

    // Mierzenie czasu rozpoczęcia obliczeń
    auto start_time = std::chrono::high_resolution_clock::now();

    // Przydzielanie przedziałów dla każdego wątku
    for (int i = 0; i < num_threads; ++i) {
        double thread_start = i * (1.0 / num_threads);
        double thread_end = (i + 1) * (1.0 / num_threads);
        threads.emplace_back(calculate_partial_integral, thread_start, thread_end, step, std::ref(results[i]));
    }

    // Czekanie na zakończenie pracy wątków
    for (auto& thread : threads) {
        thread.join();
    }

    // Sumowanie wyników z poszczególnych wątków
    double pi = 0.0;
    for (const auto& result : results) {
        pi += result;
    }

    // Mierzenie czasu zakończenia obliczeń
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // Wyświetlanie wyników
    std::cout << "Przyblizona wartosc liczby PI: " << pi << std::endl;
    std::cout << "Czas obliczen: " << elapsed.count() << " sekund" << std::endl;

    return 0;
}
