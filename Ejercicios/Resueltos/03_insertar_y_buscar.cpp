/* ============================================================================
   EJERCICIO 03 - Insertar en cualquier posicion y buscar
   Tema   : Lista simplemente enlazada - operaciones de escritura
   Archivo: 03_insertar_y_buscar.cpp
   ============================================================================

   ENUNCIADO
   --------
   Implementar sobre una lista simplemente enlazada de enteros:

   1. agregarAlInicio(v)      : inserta en la cabeza.                        O(1)
   2. agregarAlFinal(v)       : inserta al final.                             O(n)
   3. agregarAntesDe(ref, v)  : inserta v justo antes de la primera aparicion
                                de ref. Si ref no existe, agrega al final.     O(n)
   4. agregarEnPosicion(v, p) : inserta v en la posicion p. Si p > largo,
                                agrega al final.                              O(n)
   5. agregarOrdenado(v)      : mantiene la lista ordenada de menor a mayor.  O(n)
   6. buscar(v)               : devuelve el Nodo* donde esta v, o nullptr.   O(n)
   7. estaOrdenada()          : indica si los datos van en orden creciente.  O(n)

   El main debe demostrar cada operacion, incluyendo los casos limite
   (posicion 0, posicion mayor que el largo, insertar en lista vacia).

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

    // --- 1) Insertar al inicio: O(1) ----------------------------------------
    // Dos asignaciones y listo: no hay que recorrer nada.
    void agregarAlInicio(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = head;   // el nuevo apunta a la cabeza anterior
        head = nuevo;              // el nuevo pasa a ser la cabeza
    }

    // --- 2) Insertar al final: O(n) -----------------------------------------
    void agregarAlFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (head == nullptr) { head = nuevo; return; }
        Nodo<T>* a = head;
        while (a->siguiente != nullptr) a = a->siguiente;
        a->siguiente = nuevo;
    }

    // --- 3) Insertar antes de un valor: O(n) --------------------------------
    // Caso especial: si ref es la cabeza, es un agregarAlInicio.
    void agregarAntesDe(T ref, T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (head == nullptr) { head = nuevo; return; }             // lista vacia

        if (head->dato == ref) {                                    // ref es la cabeza
            nuevo->siguiente = head;
            head = nuevo;
            return;
        }

        Nodo<T>* a = head;
        while (a->siguiente != nullptr && a->siguiente->dato != ref) {
            a = a->siguiente;                                      // avanzar hasta el previo
        }

        if (a->siguiente == nullptr) {  // ref no existe -> agregar al final
            a->siguiente = nuevo;
            return;
        }

        nuevo->siguiente = a->siguiente;   // puentear por delante
        a->siguiente = nuevo;              // puentear por detras
    }

    // --- 4) Insertar en una posicion: O(n) ---------------------------------
    void agregarEnPosicion(T valor, int posicion) {
        if (posicion <= 0) { agregarAlInicio(valor); return; }    // posicion 0 o negativa
        agregarAntesDe_porIndice(valor, posicion);
    }

    // Auxiliar: insertar antes del nodo de la posicion `posicion`.
    void agregarAntesDe_porIndice(T valor, int posicion) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (head == nullptr) { head = nuevo; return; }

        if (posicion == 0) {
            nuevo->siguiente = head;
            head = nuevo;
            return;
        }

        Nodo<T>* a = head;
        int i = 0;
        while (a->siguiente != nullptr && i < posicion - 1) {
            a = a->siguiente;
            i++;
        }
        nuevo->siguiente = a->siguiente;
        a->siguiente = nuevo;
    }

    // --- 5) Insertar ordenado: O(n) ----------------------------------------
    // Se avanza mientras el siguiente sea menor que el valor.
    void agregarOrdenado(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (head == nullptr || valor <= head->dato) {   // lista vacia o valor minimo
            nuevo->siguiente = head;
            head = nuevo;
            return;
        }

        Nodo<T>* a = head;
        while (a->siguiente != nullptr && a->siguiente->dato < valor) {
            a = a->siguiente;
        }

        nuevo->siguiente = a->siguiente;
        a->siguiente = nuevo;
    }

    // --- 6) Buscar: devuelve el puntero al nodo, o nullptr ------------------
    Nodo<T>* buscar(T valor) const {
        Nodo<T>* a = head;
        while (a != nullptr) {
            if (a->dato == valor) return a;
            a = a->siguiente;
        }
        return nullptr;
    }

    // --- 7) Verificar el orden ----------------------------------------------
    bool estaOrdenada() const {
        Nodo<T>* a = head;
        while (a != nullptr && a->siguiente != nullptr) {
            if (a->dato > a->siguiente->dato) return false;   // desorden detectada
            a = a->siguiente;
        }
        return true;
    }

    int largo() const {
        int n = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) {
            std::cout << a->dato << " -> ";
        }
        std::cout << "nullptr\n";
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 03: insertar y buscar ===\n\n";

    // --- 1) Agregar al inicio -------------------------------------------------
    std::cout << "-- 1) agregarAlInicio --\n";
    ListaSimple<int> lista;
    lista.mostrar("Lista vacia:        ");
    lista.agregarAlInicio(30);
    lista.agregarAlInicio(20);
    lista.agregarAlInicio(10);
    lista.mostrar("Tras 3 inserciones: ");
    std::cout << "\n";

    // --- 2) Agregar al final --------------------------------------------------
    std::cout << "-- 2) agregarAlFinal --\n";
    lista.agregarAlFinal(40);
    lista.agregarAlFinal(50);
    lista.mostrar("Lista: ");
    std::cout << "\n";

    // --- 3) Agregar antes de un valor ----------------------------------------
    std::cout << "-- 3) agregarAntesDe --\n";
    std::cout << "  3) agregarAntesDe(40, 35): ";
    lista.agregarAntesDe(40, 35);
    lista.mostrar("");
    std::cout << "  insertar antes de la CABEZA (ref=10, valor=5): ";
    lista.agregarAntesDe(10, 5);
    lista.mostrar("");
    std::cout << "  ref inexistente (ref=99, valor=60): ";
    lista.agregarAntesDe(99, 60);
    lista.mostrar("");
    std::cout << "\n";

    // --- 4) Agregar en posicion ----------------------------------------------
    std::cout << "-- 4) agregarEnPosicion --\n";
    std::cout << "  agregarEnPosicion(15, 1): ";
    lista.agregarEnPosicion(15, 1);
    lista.mostrar("");
    std::cout << "  agregarEnPosicion(25, 2): ";
    lista.agregarEnPosicion(25, 2);
    lista.mostrar("");
    std::cout << "  agregarEnPosicion(0, 0)   : ";
    lista.agregarEnPosicion(0, 0);
    lista.mostrar("");
    std::cout << "  agregarEnPosicion(999, 99): ";
    lista.agregarEnPosicion(999, 99);
    lista.mostrar("");
    std::cout << "\n";

    // --- 5) Agregar ordenado --------------------------------------------------
    std::cout << "-- 5) agregarOrdenado --\n";
    ListaSimple<int> ordenada;
    int datos[] = {50, 20, 80, 10, 60, 30, 90, 40, 70, 5};
    for (int d : datos) ordenada.agregarOrdenado(d);
    ordenada.mostrar("Insertados en desorden: 50 20 80 10 60 30 90 40 70 5 -> ");
    std::cout << "  estaOrdenada() = " << std::boolalpha << ordenada.estaOrdenada() << "\n\n";

    // --- 6) Buscar ------------------------------------------------------------
    std::cout << "-- 6) buscar --\n";
    Nodo<int>* encontrado = ordenada.buscar(60);
    std::cout << "  buscar(60) -> " << (encontrado != nullptr ? "encontrado" : "no existe") << "\n";
    std::cout << "  buscar(61) -> " << (ordenada.buscar(61) != nullptr ? "encontrado" : "no existe") << "\n";

    // --- 7) Verificar orden en una lista desordenada --------------------------
    std::cout << "\n-- 7) estaOrdenada sobre lista desordenada --\n";
    ListaSimple<int> desordenada;
    desordenada.agregarAlFinal(5);
    desordenada.agregarAlFinal(9);
    desordenada.agregarAlFinal(2);
    desordenada.mostrar("Lista: ");
    std::cout << "  estaOrdenada() = " << desordenada.estaOrdenada() << "  (esperado false)\n";

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 03: insertar y buscar ===
    -- 1) agregarAlInicio --
    Lista vacia:        nullptr
    Tras 3 inserciones: 10 -> 20 -> 30 -> nullptr
    -- 2) agregarAlFinal --
    Lista: 10 -> 20 -> 30 -> 40 -> 50 -> nullptr
    -- 3) agregarAntesDe --
      3) agregarAntesDe(40, 35): 10 -> 20 -> 30 -> 35 -> 40 -> 50 -> nullptr
      insertar antes de la CABEZA (ref=10, valor=5): 5 -> 10 -> 20 -> 30 -> 35 -> 40 -> 50 -> nullptr
      ref inexistente (ref=99, valor=60): 5 -> 10 -> 20 -> 30 -> 35 -> 40 -> 50 -> 60 -> nullptr
    -- 4) agregarEnPosicion --
      agregarEnPosicion(15, 1): 5 -> 15 -> 10 -> 20 -> 30 -> 35 -> 40 -> 50 -> 60 -> nullptr
      agregarEnPosicion(25, 2): 5 -> 15 -> 25 -> 10 -> 20 -> 30 -> 35 -> 40 -> 50 -> 60 -> nullptr
      agregarEnPosicion(0, 0)   : 0 -> 5 -> 15 -> 25 -> 10 -> 20 -> 30 -> 35 -> 40 -> 50 -> 60 -> nullptr
      agregarEnPosicion(999, 99): 0 -> 5 -> 15 -> 25 -> 10 -> 20 -> 30 -> 35 -> 40 -> 50 -> 60 -> 999 -> nullptr
    -- 5) agregarOrdenado --
    Insertados en desorden: 50 20 80 10 60 30 90 40 70 5 -> 5 -> 10 -> 20 -> 30 -> 40 -> 50 -> 60 -> 70 -> 80 -> 90 -> nullptr
      estaOrdenada() = true
    -- 6) buscar --
      buscar(60) -> encontrado
      buscar(61) -> no existe
    -- 7) estaOrdenada sobre lista desordenada --
    Lista: 5 -> 9 -> 2 -> nullptr
      estaOrdenada() = false  (esperado false)
    Demostracion completada.
*/
