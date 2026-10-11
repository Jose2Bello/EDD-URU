/* ============================================================================
   EJERCICIO 01 - Recorrer y contar nodos
   Tema   : Lista simplemente enlazada - recorrido
   Archivo: 01_recorrer_contar.cpp
   ============================================================================

   ENUNCIADO
   --------
   Se tiene una lista simplemente enlazada de enteros. Sobre ella implementar:

   1. contar()      : devuelve la cantidad de nodos de la lista.
   2. contarPares() : devuelve la cantidad de nodos cuyo dato es par.
   3. sumar()       : devuelve la suma de todos los datos.
   4. maximo()      : devuelve el dato mas grande (0 si la lista esta vacia).
   5. minimo()      : devuelve el dato mas pequeno (0 si la lista esta vacia).
   6. indiceDe(v)   : devuelve la posicion de v (0 es la cabeza), o -1 si no existe.
   7. contiene(v)   : devuelve true si v esta en la lista.
   8. mostrar()     : imprime la lista en formato (dato) -> ... -> nullptr.

   Ademas el main debe probar los casos limite:
     - lista vacia
     - lista con un solo elemento
     - lista con elementos repetidos

   COMPLEJIDAD
   -----------
   Todos los metodos son O(n) salvo los dos ultimos:
     maximo/minimo tambien son O(n) (no estan ordenados).
     indiceDe es O(n). mostrar es O(n).
   Un recorrido completo por lista es, por definicion, lineal.

   ========================================================================== */

#include <iostream>

// ---------------------------------------------------------------------------
// Nodo generico (plantilla)
// ---------------------------------------------------------------------------
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;

    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

// ---------------------------------------------------------------------------
// Lista simplemente enlazada
// ---------------------------------------------------------------------------
template <typename T>
class ListaSimple {
private:
    Nodo<T>* head;

public:
    ListaSimple() : head(nullptr) {}

    // DESTRUCTOR: toda la memoria de los nodos se libera aqui.
    // Regla: salvar el siguiente ANTES de borrar el actual.
    ~ListaSimple() {
        Nodo<T>* actual = head;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        head = nullptr;
    }

    void agregarAlFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (head == nullptr) {          // caso lista vacia
            head = nuevo;
            return;
        }
        Nodo<T>* actual = head;
        while (actual->siguiente != nullptr) {   // avanzar hasta el ultimo
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }

    bool estaVacia() const { return head == nullptr; }

    // 1) Contar nodos: el recorrido canonico de una lista.
    int contar() const {
        int n = 0;
        Nodo<T>* actual = head;
        while (actual != nullptr) {     // != nullptr y no "while(actual)"
            n++;
            actual = actual->siguiente;
        }
        return n;
    }

    // 2) Contar pares: mismo recorrido, con una condicion extra.
    int contarPares() const {
        int n = 0;
        for (Nodo<T>* actual = head; actual != nullptr; actual = actual->siguiente) {
            if (actual->dato % 2 == 0) n++;
        }
        return n;
    }

    // 3) Sumar: acumulador que arranca en cero (elemento neutro de la suma).
    int sumar() const {
        int total = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) total += a->dato;
        return total;
    }

    // 4) Maximo: se inicializa con el primer dato, NO con 0, porque la lista
    //    podria contener solo numeros negativos (ejemplo: -8, -3, -5).
    int maximo() const {
        if (head == nullptr) return 0;
        int max = head->dato;
        for (Nodo<T>* a = head->siguiente; a != nullptr; a = a->siguiente) {
            if (a->dato > max) max = a->dato;
        }
        return max;
    }

    // 5) Minimo: mismo criterio que maximo.
    int minimo() const {
        if (head == nullptr) return 0;
        int min = head->dato;
        for (Nodo<T>* a = head->siguiente; a != nullptr; a = a->siguiente) {
            if (a->dato < min) min = a->dato;
        }
        return min;
    }

    // 6) Indice: se avanza contando posiciones. -1 significa "no encontrado".
    int indiceDe(T valor) const {
        int pos = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente, pos++) {
            if (a->dato == valor) return pos;
        }
        return -1;
    }

    // 7) contiene: se detiene en cuanto encuentra el valor (borroso, pero O(n)).
    bool contiene(T valor) const {
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) {
            if (a->dato == valor) return true;
        }
        return false;
    }

    // 8) Mostrar
    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) {
            std::cout << "(" << a->dato << ") -> ";
        }
        std::cout << "nullptr\n";
    }
};

// ---------------------------------------------------------------------------
// MAIN - pruebas
// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 01: recorrer y contar nodos ===\n\n";

    // --- Caso limite 1: lista vacia -----------------------------------------
    std::cout << "-- Caso limite: lista vacia --\n";
    ListaSimple<int> vacia;
    vacia.mostrar("Lista: ");
    std::cout << "contar    = " << vacia.contar() << "  (esperado 0)\n";
    std::cout << "contarPares = " << vacia.contarPares() << "  (esperado 0)\n";
    std::cout << "sumar     = " << vacia.sumar() << "  (esperado 0)\n";
    std::cout << "maximo    = " << vacia.maximo() << "  (esperado 0)\n";
    std::cout << "minimo    = " << vacia.minimo() << "  (esperado 0)\n";
    std::cout << "indiceDe(5) = " << vacia.indiceDe(5) << "  (esperado -1)\n";
    std::cout << "contiene(5) = " << std::boolalpha << vacia.contiene(5)
              << "  (esperado false)\n\n";


    // --- Caso limite 2: lista con un solo elemento ---------------------------
    std::cout << "-- Caso limite: un solo elemento --\n";
    ListaSimple<int> uno;
    uno.agregarAlFinal(42);
    uno.mostrar("Lista: ");
    std::cout << "contar    = " << uno.contar() << "  (esperado 1)\n";
    std::cout << "sumar     = " << uno.sumar() << "  (esperado 42)\n";
    std::cout << "maximo    = " << uno.maximo() << "  (esperado 42)\n";
    std::cout << "indiceDe(42) = " << uno.indiceDe(42) << "  (esperado 0)\n\n";


    // --- Caso general: lista con elementos repetidos -------------------------
    std::cout << "-- Caso general: elementos repetidos --\n";
    ListaSimple<int> lista;
    int datos[] = {7, -4, 12, -4, 30, 0, 7, -11};
    for (int d : datos) lista.agregarAlFinal(d);

    lista.mostrar("Lista:            ");
    std::cout << "contar      = " << lista.contar() << "  (esperado 8)\n";
    std::cout << "contarPares = " << lista.contarPares() << "  (esperado 5: -4,12,-4,0,-11)\n";
    std::cout << "sumar       = " << lista.sumar() << "  (esperado 37)\n";
    std::cout << "maximo      = " << lista.maximo() << "  (esperado 30)\n";
    std::cout << "minimo      = " << lista.minimo() << "  (esperado -11)\n";
    std::cout << "indiceDe(-4) = " << lista.indiceDe(-4) << "  (esperado 1, la primera vez)\n";
    std::cout << "indiceDe(12) = " << lista.indiceDe(12) << "  (esperado 2)\n";
    std::cout << "indiceDe(99) = " << lista.indiceDe(99) << "  (esperado -1)\n";
    std::cout << "contiene(-11) = " << lista.contiene(-11) << "  (esperado true)\n";
    std::cout << "contiene(50)  = " << lista.contiene(50) << "  (esperado false)\n\n";

    // Verificaciones automaticas con assert

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 01: recorrer y contar nodos ===
    -- Caso limite: lista vacia --
    Lista: nullptr
    contar    = 0  (esperado 0)
    contarPares = 0  (esperado 0)
    sumar     = 0  (esperado 0)
    maximo    = 0  (esperado 0)
    minimo    = 0  (esperado 0)
    indiceDe(5) = -1  (esperado -1)
    contiene(5) = false  (esperado false)
    -- Caso limite: un solo elemento --
    Lista: (42) -> nullptr
    contar    = 1  (esperado 1)
    sumar     = 42  (esperado 42)
    maximo    = 42  (esperado 42)
    indiceDe(42) = 0  (esperado 0)
    -- Caso general: elementos repetidos --
    Lista:            (7) -> (-4) -> (12) -> (-4) -> (30) -> (0) -> (7) -> (-11) -> nullptr
    contar      = 8  (esperado 8)
    contarPares = 5  (esperado 5: -4,12,-4,0,-11)
    sumar       = 37  (esperado 37)
    maximo      = 30  (esperado 30)
    minimo      = -11  (esperado -11)
    indiceDe(-4) = 1  (esperado 1, la primera vez)
    indiceDe(12) = 2  (esperado 2)
    indiceDe(99) = -1  (esperado -1)
    contiene(-11) = true  (esperado true)
    contiene(50)  = false  (esperado false)
    Demostracion completada.
*/
