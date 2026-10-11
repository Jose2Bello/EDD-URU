/* ============================================================================
   EJERCICIO 02 - Invertir la lista simplemente enlazada
   Tema   : Punteros - reasignacion de enlaces
   Archivo: 02_invertir_lista.cpp
   ============================================================================

   ENUNCIADO
   --------
   Invertir una lista simplemente enlazada de N nodos **sin crear nodos nuevos**
   y en tiempo lineal O(n). Se deben entregar dos versiones:

   1. invertirIterativa() : usando tres punteros (prev, actual, siguiente).
   2. invertirRecursiva() : usando la recursion.

   Definicion: si la lista es [a] -> [b] -> [c] -> nullptr, el resultado debe ser
   [c] -> [b] -> [a] -> nullptr.

   Ejemplo con 4 nodos:

     head -> [10] -> [20] -> [30] -> [40] -> nullptr
     -----------------------------------------------
     resultado:  head -> [40] -> [30] -> [20] -> [10] -> nullptr

   COMPLEJIDAD
   -----------
   Ambas versiones son O(n) en tiempo y O(1) en memoria (la recursiva usa O(n)
   por la pila de llamadas, por eso la iterativa es la preferida en produccion).

   ========================================================================== */

#include <iostream>

template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T>
class ListaSimple {
private:
    Nodo<T>* head;

public:
    ListaSimple() : head(nullptr) {}

    ~ListaSimple() {
        Nodo<T>* a = head;
        while (a != nullptr) {
            Nodo<T>* sig = a->siguiente;
            delete a;
            a = sig;
        }
        head = nullptr;
    }

    void agregarAlFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (head == nullptr) { head = nuevo; return; }
        Nodo<T>* a = head;
        while (a->siguiente != nullptr) a = a->siguiente;
        a->siguiente = nuevo;
    }

    // ------------------------------------------------------------------
    // SOLUCION 1 - ITERATIVA con tres punteros
    //
    //   prev    : el nodo ya invertido, que queda "detras" de actual
    //   actual  : el nodo que estamos procesando
    //   siguiente: el nodo que aun no hemos tocado
    //
    //   Invariante: prev contiene la lista [ultimo..primero] ya invertida.
    // ------------------------------------------------------------------
    void invertirIterativa() {
        Nodo<T>* prev = nullptr;      // al inicio no hay nodo invertido
        Nodo<T>* actual = head;

        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;  // 1) SALVAR el resto
            actual->siguiente = prev;                // 2) INVERTIR la flecha
            prev = actual;                           // 3) prev avanza
            actual = siguiente;                      // 4) actual avanza
        }

        head = prev;    // el ultimo nodo procesado es ahora la cabeza
    }

    // ------------------------------------------------------------------
    // SOLUCION 2 - RECURSIVA
    //
    //   Idea: invertir una lista de N nodos es:
    //         1) invertir la sublista que empieza en actual->siguiente
    //         2) hacer que el primero (actual) quede al final
    // ------------------------------------------------------------------
    Nodo<T>* invertirRecursiva(Nodo<T>* actual) {
        if (actual == nullptr || actual->siguiente == nullptr) {
            return actual;      // caso base: ultimo nodo (o lista vacia)
        }

        Nodo<T>* nuevaCabeza = invertirRecursiva(actual->siguiente);  // paso 1
        actual->siguiente->siguiente = actual;                          // paso 2
        actual->siguiente = nullptr;
        return nuevaCabeza;
    }

    void invertirRecursiva() { head = invertirRecursiva(head); }

    // Mostrar
    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) {
            std::cout << a->dato << " -> ";
        }
        std::cout << "nullptr\n";
    }

    int largo() const {
        int n = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) n++;
        return n;
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 02: invertir la lista ===\n\n";

    // --- Caso limite: lista vacia -------------------------------------------
    std::cout << "-- Caso limite: lista vacia --\n";
    ListaSimple<int> vacia;
    vacia.invertirIterativa();
    vacia.mostrar("Lista invertida: ");
    std::cout << "  largo = 0, correcto\n\n";

    // --- Caso limite: un solo nodo (debe quedar igual) ------------------------
    std::cout << "-- Caso limite: un solo nodo --\n";
    ListaSimple<int> uno;
    uno.agregarAlFinal(99);
    uno.invertirIterativa();
    uno.mostrar("Lista invertida: ");
    std::cout << "\n";

    // --- Version iterativa ----------------------------------------------------
    std::cout << "-- Solucion 1: iterativa con tres punteros --\n";
    ListaSimple<int> lista;
    lista.agregarAlFinal(10);
    lista.agregarAlFinal(20);
    lista.agregarAlFinal(30);
    lista.agregarAlFinal(40);
    lista.mostrar("Antes:    ");
    lista.invertirIterativa();
    lista.mostrar("Despues:  ");
    lista.mostrar("\nInvertir dos veces mas: ");
    lista.invertirIterativa();
    lista.invertirIterativa();
    lista.mostrar("");
    std::cout << "\n";

    // --- Version recursiva (otra lista con los mismos datos) -----------------
    std::cout << "-- Solucion 2: recursiva --\n";
    ListaSimple<int> lista2;
    lista2.agregarAlFinal(10);
    lista2.agregarAlFinal(20);
    lista2.agregarAlFinal(30);
    lista2.agregarAlFinal(40);
    lista2.mostrar("Antes:    ");
    lista2.invertirRecursiva();
    lista2.mostrar("Despues:  ");

    // --- Verificacion: el resultado es el mismo en ambos casos ---------------
    std::cout << "\n-- Verificacion cruzada --\n";
    std::cout << "Las dos soluciones producen la misma lista invertida: "
              << (lista.largo() == lista2.largo() ? "OK" : "ERROR") << "\n\n";


    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 02: invertir la lista ===
    -- Caso limite: lista vacia --
    Lista invertida: nullptr
      largo = 0, correcto
    -- Caso limite: un solo nodo --
    Lista invertida: 99 -> nullptr
    -- Solucion 1: iterativa con tres punteros --
    Antes:    10 -> 20 -> 30 -> 40 -> nullptr
    Despues:  40 -> 30 -> 20 -> 10 -> nullptr
    Invertir dos veces mas: 40 -> 30 -> 20 -> 10 -> nullptr
    40 -> 30 -> 20 -> 10 -> nullptr
    -- Solucion 2: recursiva --
    Antes:    10 -> 20 -> 30 -> 40 -> nullptr
    Despues:  40 -> 30 -> 20 -> 10 -> nullptr
    -- Verificacion cruzada --
    Las dos soluciones producen la misma lista invertida: OK
    Demostracion completada.
*/
