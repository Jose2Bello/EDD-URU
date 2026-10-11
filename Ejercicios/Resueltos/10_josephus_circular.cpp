/* ============================================================================
   EJERCICIO 10 - Lista circular y problema de Josephus
   Tema   : Lista circular - recorrido sin nullptr
   Archivo: 10_josephus_circular.cpp
   ============================================================================

   ENUNCIADO
   --------
   PARTE 1 - Lista circular
   Implementar una lista circular simplemente enlazada (el ultimo nodo apunta
   al primero, no a nullptr) con:
     - agregarAlFinal(v), agregarAlInicio(v)
     - recorrerMostrar() protegido contra recorridos infinitos
     - largo()
     - eliminarValor(v)
     - invertir() conservando la circularidad

   PARTE 2 - Problema de Josephus
   N personas (1..N) se colocan en circulo. Se empieza contando en 1 y cada vez
   que se cuentan K personas se elimina a la que le toca, y el conteo se reinicia
   en la persona siguiente. Se pide:
     a) imprimir el orden en que se van eliminando
     b) indicar quien es el ultimo sobreviviente

   Ejemplo clasico con N = 7 y K = 3:
     orden de eliminacion: 3, 6, 2, 7, 5, 1  ->  sobrevive el 4

   ========================================================================== */

#include <iostream>
#include <initializer_list>
#include <set>

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
// Lista circular
// ---------------------------------------------------------------------------
template <typename T>
class ListaCircular {
private:
    Nodo<T>* head;
    Nodo<T>* tail;

public:
    ListaCircular() : head(nullptr), tail(nullptr) {}

    // El recorrido de liberacion usa do-while: al menos una vuelta, y se detiene
    // al volver a la cabeza. Con `while (a != nullptr)` seria infinito.
    ~ListaCircular() {
        if (head == nullptr) return;

        Nodo<T>* actual = head;
        do {
            Nodo<T>* siguiente = actual->siguiente;   // salvar antes de borrar
            delete actual;
            actual = siguiente;
        } while (actual != head);
        head = tail = nullptr;
    }

    bool estaVacia() const { return head == nullptr; }

    // ------------------------------------------------------------------
    // AGREGAR AL FINAL
    //    Si la lista estaba vacia, el nodo se apunta a SI MISMO: esa es
    //    la definicion de lista circular de un solo elemento.
    // ------------------------------------------------------------------
    void agregarAlFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (head == nullptr) {
            head = tail = nuevo;
            nuevo->siguiente = nuevo;     // circulo de un elemento
            return;
        }

        tail->siguiente = nuevo;   // el viejo final entra al nuevo
        nuevo->siguiente = head;   // el nuevo cierra el circulo
        tail = nuevo;
    }

    void agregarAlInicio(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (head == nullptr) {
            head = tail = nuevo;
            nuevo->siguiente = nuevo;
            return;
        }

        nuevo->siguiente = head;
        tail->siguiente = nuevo;   // hay que reconectar la cola
        head = nuevo;
    }

    // ------------------------------------------------------------------
    // RECORRER: do-while comparando contra head, NUNCA contra nullptr
    // ------------------------------------------------------------------
    void recorrerMostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        if (head == nullptr) { std::cout << "vacia\n"; return; }

        Nodo<T>* actual = head;
        std::set<Nodo<T>*> vistos;      // proteccion extra
        int pasos = 0;

        do {
            std::cout << actual->dato << " -> ";
            if (pasos > 0 && vistos.count(actual) > 0) { std::cout << "(ciclo cerrado)\n"; return; }
            vistos.insert(actual);
            actual = actual->siguiente;
            pasos++;
        } while (actual != head);

        std::cout << "(vuelve a " << head->dato << ")\n";
    }

    int largo() const {
        if (head == nullptr) return 0;

        int n = 1;
        Nodo<T>* actual = head->siguiente;
        while (actual != head && actual != nullptr) { n++; actual = actual->siguiente; }
        return n;
    }

    // ------------------------------------------------------------------
    // ELIMINAR POR VALOR
    //    Caso especial: eliminar la CABEZA exige mover head Y reconectar tail.
    // ------------------------------------------------------------------
    bool eliminarValor(T valor) {
        if (head == nullptr) return false;

        if (head->dato == valor) {
            if (head->siguiente == head) {     // era el unico nodo
                delete head;
                head = tail = nullptr;
                return true;
            }
            Nodo<T>* aBorrar = head;
            head = head->siguiente;
            tail->siguiente = head;           // el ultimo pasa a apuntar a la nueva cabeza
            delete aBorrar;
            return true;
        }

        Nodo<T>* actual = head;
        do {
            if (actual->siguiente->dato == valor) {
                Nodo<T>* aBorrar = actual->siguiente;
                actual->siguiente = aBorrar->siguiente;
                if (aBorrar == tail) tail = actual;   // si era la cola, se actualiza
                delete aBorrar;
                return true;
            }
            actual = actual->siguiente;
        } while (actual != head);

        return false;
    }

// ------------------------------------------------------------------
    // INVERTIR conservando la circularidad                     O(n)
    //
    //    Pasada 1: se rompe la circularidad poniendo nullptr en el ultimo
    //              enlace, dejando la lista como una cadena lineal.
    //    Pasada 2: se invierte esa cadena con tres punteros (prev/actual/sig).
    //    Pasada 3: la cabeza y la cola originales intercambian su papel y se
    //              vuelve a cerrar el circulo, ahora en el sentido opuesto.
    // ------------------------------------------------------------------
    void invertir() {
        if (head == nullptr || head->siguiente == head) return;   // 0 o 1 nodo

        Nodo<T>* primero = head;

        // Pasada 1: romper SOLO el enlace del ultimo nodo (-> nullptr).
        //           Si se pusieran todos a nullptr, la cadena quedaria
        //           partida en trozos y la pasada 2 no tendria nada que invertir.
        Nodo<T>* a = head;
        do {
            Nodo<T>* siguiente = a->siguiente;
            if (siguiente == head) {
                a->siguiente = nullptr;   // este era el enlace que cerraba el circulo
            }
            a = siguiente;
        } while (a != head);

        // Pasada 2: invertir la cadena lineal con tres punteros.
        //           OJO: se parte de la CABEZA y se sigue la cadena hacia el
        //           final. Empezar por `ultimo` no serviria, porque su
        //           `siguiente` ya es nullptr.
        Nodo<T>* anterior = nullptr;
        Nodo<T>* actual   = primero;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            actual->siguiente = anterior;
            anterior = actual;
            actual   = siguiente;
        }

        // Pasada 3: cerrar el circulo en el nuevo sentido
        head = anterior;                // la cola original pasa a ser la cabeza
        tail = primero;                 // la cabeza original pasa a ser la cola
        tail->siguiente = head;
    }

    Nodo<T>* cabeza() const { return head; }
    Nodo<T>* cola() const { return tail; }
};

// ---------------------------------------------------------------------------
// PARTE 2 - JOSEPHUS
//
//   El nodo `actual` marca a quien le toca contar. Se avanza k-1 veces y se
//   elimina ese nodo; el conteo se reinicia en el siguiente.
// ---------------------------------------------------------------------------
void josephus(int n, int k) {
    std::cout << "  Josephus con N = " << n << " y K = " << k << ":\n";

    ListaCircular<int> circulo;
    for (int i = 1; i <= n; i++) circulo.agregarAlFinal(i);

    std::cout << "    Circulo inicial: ";
    circulo.recorrerMostrar("");

    Nodo<int>* actual = circulo.cabeza();
    int restantes = n;
    std::cout << "    Eliminados: ";
    bool primeroEnSalida = true;

    while (restantes > 1) {
        // avanzar (k-1) pasos desde `actual`: el que cae es el que se elimina
        for (int paso = 1; paso < k; paso++) {
            actual = actual->siguiente;
        }

        // OJO: se copia el dato ANTES de borrar el nodo. Si se imprimiera
        // aEliminar->dato despues del delete, se estaria leyendo memoria
        // liberada y el programa puede fallar.
        int eliminado = actual->dato;
        actual = actual->siguiente;             // el siguiente reinicia el conteo
        circulo.eliminarValor(eliminado);       // sale del circulo
        restantes--;

        if (!primeroEnSalida) std::cout << ", ";
        std::cout << eliminado;
        primeroEnSalida = false;
    }

    std::cout << "\n    Ultimo sobreviviente: " << actual->dato << "\n\n";
}

template <typename T>
void cargar(ListaCircular<T>& lista, std::initializer_list<T> datos) {
    for (T d : datos) lista.agregarAlFinal(d);
}

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 10: lista circular y Josephus ===\n\n";

    // --- Caso limite: lista circular vacia -----------------------------------
    std::cout << "-- Caso limite: lista vacia --\n";
    ListaCircular<int> vacia;
    vacia.recorrerMostrar("  ");
    std::cout << "  largo = " << vacia.largo() << "  (esperado 0)\n";
    vacia.eliminarValor(5);
    std::cout << "\n";

    // --- Caso limite: un solo nodo (circular consigo mismo) ------------------
    std::cout << "-- Caso limite: un solo nodo --\n";
    ListaCircular<int> uno;
    uno.agregarAlFinal(42);
    uno.recorrerMostrar("  ");
    std::cout << "  largo = " << uno.largo() << "  (esperado 1)\n";
    std::cout << "  eliminarValor(42): " << std::boolalpha << uno.eliminarValor(42) << "\n";
    uno.recorrerMostrar("  ");
    std::cout << "\n";

    // --- Operaciones basicas ---------------------------------------------------
    std::cout << "-- Agregar al inicio y al final --\n";
    ListaCircular<int> circulo;
    circulo.agregarAlFinal(20);
    circulo.agregarAlFinal(30);
    circulo.agregarAlInicio(10);
    circulo.recorrerMostrar("  ");
    std::cout << "  largo = " << circulo.largo() << "  (esperado 3)\n\n";

    // --- Eliminar (incluida la cabeza y la cola) ------------------------------
    std::cout << "-- eliminarValor --\n";
    std::cout << "  eliminarValor(20) (medio):   " << circulo.eliminarValor(20) << "  ";
    circulo.recorrerMostrar("");
    std::cout << "  eliminarValor(10) (cabeza):  " << circulo.eliminarValor(10) << "  ";
    circulo.recorrerMostrar("");
    std::cout << "  eliminarValor(30) (ultimo):  " << circulo.eliminarValor(30) << "  ";
    circulo.recorrerMostrar("");
    std::cout << "  eliminarValor(99) (no existe): " << circulo.eliminarValor(99) << "\n\n";

    // --- Invertir manteniendo la circularidad ---------------------------------
    std::cout << "-- invertir conservando la circularidad --\n";
    ListaCircular<int> c2;
    cargar(c2, {1, 2, 3, 4, 5});
    c2.recorrerMostrar("  Antes:  ");
    c2.invertir();
    c2.recorrerMostrar("  Despues: ");
    c2.invertir();
    c2.recorrerMostrar("  De nuevo: ");
    std::cout << "  largo = " << c2.largo() << "  (esperado 5)\n\n";

    // --- Josephus --------------------------------------------------------------
    std::cout << "-- Problema de Josephus --\n";
    josephus(7, 3);    // clasico: sobrevive el 7
    josephus(5, 2);
    josephus(5, 1);    // k = 1: se va eliminando en orden
    josephus(1, 5);    // caso limite de una sola persona

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 10: lista circular y Josephus ===
    -- Caso limite: lista vacia --
      vacia
      largo = 0  (esperado 0)
    -- Caso limite: un solo nodo --
      42 -> (vuelve a 42)
      largo = 1  (esperado 1)
      eliminarValor(42): true
      vacia
    -- Agregar al inicio y al final --
      10 -> 20 -> 30 -> (vuelve a 10)
      largo = 3  (esperado 3)
    -- eliminarValor --
      eliminarValor(20) (medio):   true  10 -> 30 -> (vuelve a 10)
      eliminarValor(10) (cabeza):  true  30 -> (vuelve a 30)
      eliminarValor(30) (ultimo):  true  vacia
      eliminarValor(99) (no existe): false
    -- invertir conservando la circularidad --
      Antes:  1 -> 2 -> 3 -> 4 -> 5 -> (vuelve a 1)
      Despues: 5 -> 4 -> 3 -> 2 -> 1 -> (vuelve a 5)
      De nuevo: 1 -> 2 -> 3 -> 4 -> 5 -> (vuelve a 1)
      largo = 5  (esperado 5)
    -- Problema de Josephus --
      Josephus con N = 7 y K = 3:
        Circulo inicial: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> (vuelve a 1)
        Eliminados: 3, 6, 2, 7, 5, 1
        Ultimo sobreviviente: 4
      Josephus con N = 5 y K = 2:
        Circulo inicial: 1 -> 2 -> 3 -> 4 -> 5 -> (vuelve a 1)
        Eliminados: 2, 4, 1, 5
        Ultimo sobreviviente: 3
      Josephus con N = 5 y K = 1:
        Circulo inicial: 1 -> 2 -> 3 -> 4 -> 5 -> (vuelve a 1)
        Eliminados: 1, 2, 3, 4
        Ultimo sobreviviente: 5
      Josephus con N = 1 y K = 5:
        Circulo inicial: 1 -> (vuelve a 1)
        Eliminados:
        Ultimo sobreviviente: 1
    Demostracion completada.
*/
