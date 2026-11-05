/* ============================================================================
   EJERCICIO 17 - Deque: maximo de cada ventana deslizante
   Tema   : Deque (cola de doble extremo)
   Archivo: 17_deque_ventana_maxima.cpp
   ============================================================================

   ENUNCIADO
   --------
   Dado un arreglo de enteros y un tamano de ventana k, devolver el MAXIMO de
   cada ventana de k elementos consecutivos.

       datos = [1, 3, -1, -3, 5, 3, 6, 7],  k = 3
       ventanas:  [1 3 -1] [-1 ... ] ...
       salida :   3  3  5  5  6  7

   Se exige O(n), usando un deque de INDICES con los valores en orden
   decreciente:
      - antes de meter el indice i, se quitan por DETRAS los indices cuyos
        valores son menores o iguales que datos[i] (ya nunca podran ser maximos)
      - se quitan por DELANTE los indices que ya salieron de la ventana
      - el frente del deque es siempre el indice del maximo de la ventana

   Casos limite: k = 1, k = n, k mayor que n (no hay ventanas) y k <= 0.
   ========================================================================== */

#include <iostream>
#include <cassert>

// ---------------------------------------------------------------------------
// Nodo
// ---------------------------------------------------------------------------
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

// ---------------------------------------------------------------------------
// Deque minimo: se opera por los dos extremos
// ---------------------------------------------------------------------------
template <typename T>
class Deque {
private:
    Nodo<T>* frente_;
    Nodo<T>* final_;

public:
    Deque() : frente_(nullptr), final_(nullptr) {}

    ~Deque() {
        while (frente_ != nullptr) {
            Nodo<T>* siguiente = frente_->siguiente;
            delete frente_;
            frente_ = siguiente;
        }
        final_ = nullptr;
    }

    void empujarFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (final_ == nullptr) frente_ = nuevo;
        else                   final_->siguiente = nuevo;
        final_ = nuevo;
    }

    bool sacarFrente(T& salida) {
        if (frente_ == nullptr) return false;
        Nodo<T>* aBorrar = frente_;
        salida = aBorrar->dato;
        frente_ = aBorrar->siguiente;
        delete aBorrar;
        if (frente_ == nullptr) final_ = nullptr;
        return true;
    }

    bool sacarFinal(T& salida) {
        if (final_ == nullptr) return false;

        if (frente_ == final_) {                 // un solo nodo
            salida = final_->dato;
            delete final_;
            frente_ = final_ = nullptr;
            return true;
        }

        Nodo<T>* anterior = frente_;
        while (anterior->siguiente != final_) anterior = anterior->siguiente;

        salida = final_->dato;
        delete final_;
        anterior->siguiente = nullptr;
        final_ = anterior;
        return true;
    }

    bool frente(T& salida) const {
        if (frente_ == nullptr) return false;
        salida = frente_->dato;
        return true;
    }

    bool ultimo(T& salida) const {
        if (final_ == nullptr) return false;
        salida = final_->dato;
        return true;
    }

    bool estaVacia() const { return frente_ == nullptr; }

    int tamano() const {
        int n = 0;
        for (Nodo<T>* a = frente_; a != nullptr; a = a->siguiente) n++;
        return n;
    }
};

// ---------------------------------------------------------------------------
// Maximo de cada ventana. Devuelve cuantos maximos se guardaron en `salida`.
// ---------------------------------------------------------------------------
int maximosVentana(const int* datos, int n, int k, int* salida) {
    if (k <= 0 || k > n) return 0;

    Deque<int> indices;      // indices, no valores
    int cantidad = 0;

    for (int i = 0; i < n; i++) {
        // 1) Por detras: fuera los que no pueden ser maximos (valor <= actual).
        int atras = 0;
        while (indices.ultimo(atras) && datos[atras] <= datos[i]) {
            int descartado = 0;
            indices.sacarFinal(descartado);
        }

        // 2) Entra el indice actual.
        indices.empujarFinal(i);

        // 3) Por delante: fuera el indice que ya salio de la ventana.
        int delante = 0;
        indices.frente(delante);
        if (delante <= i - k) {
            int descartado = 0;
            indices.sacarFrente(descartado);
        }

        // 4) La ventana ya esta completa: el frente es el maximo.
        if (i >= k - 1) {
            indices.frente(delante);
            salida[cantidad++] = datos[delante];
        }
    }

    return cantidad;
}

// ---------------------------------------------------------------------------
void probar(const int* datos, int n, int k, const int* esperado, int esperados) {
    int salida[64];
    int cantidad = maximosVentana(datos, n, k, salida);

    std::cout << "   k = " << k << " -> [ ";
    for (int i = 0; i < cantidad; i++) std::cout << salida[i] << " ";
    std::cout << "]\n";

    assert(cantidad == esperados);
    for (int i = 0; i < esperados; i++) assert(salida[i] == esperado[i]);
}

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 17: maximo de cada ventana con deque ===\n\n";

    const int datos[] = {1, 3, -1, -3, 5, 3, 6, 7};
    const int n = static_cast<int>(sizeof(datos) / sizeof(datos[0]));

    std::cout << "datos = [1, 3, -1, -3, 5, 3, 6, 7]\n";
    std::cout << "Ventanas de tamano k:\n";

    const int esperado3[] = {3, 3, 5, 5, 6, 7};
    probar(datos, n, 3, esperado3, 6);

    const int esperado1[] = {1, 3, -1, -3, 5, 3, 6, 7};
    probar(datos, n, 1, esperado1, 8);

    const int esperado8[] = {7};
    probar(datos, n, 8, esperado8, 1);

    const int esperado2[] = {3, 3, -1, 5, 5, 6, 7};
    probar(datos, n, 2, esperado2, 7);

    // Casos limite
    std::cout << "\nCasos limite:\n";
    int salida[64];
    assert(maximosVentana(datos, n, 0, salida) == 0);
    std::cout << "   k = 0  -> 0 ventanas (no tiene sentido)\n";
    assert(maximosVentana(datos, n, n + 5, salida) == 0);
    std::cout << "   k > n  -> 0 ventanas\n";

    std::cout << "\nComplejidad: O(n). Cada indice entra y sale del deque a lo sumo una vez.\n";
    std::cout << "Todos los assert pasaron.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 17: maximo de cada ventana con deque ===

    datos = [1, 3, -1, -3, 5, 3, 6, 7]
    Ventanas de tamano k:
       k = 3 -> [ 3 3 5 5 6 7 ]
       k = 1 -> [ 1 3 -1 -3 5 3 6 7 ]
       k = 8 -> [ 7 ]
       k = 2 -> [ 3 3 -1 5 5 6 7 ]

    Casos limite:
       k = 0  -> 0 ventanas (no tiene sentido)
       k > n  -> 0 ventanas

    Complejidad: O(n). Cada indice entra y sale del deque a lo sumo una vez.
    Todos los assert pasaron.
*/
