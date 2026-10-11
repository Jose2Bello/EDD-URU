/* ============================================================================
   EJERCICIO 05 - Eliminar duplicados de una lista
   Tema   : Listas - doble puntero (nodo actual + nodo siguiente)
   Archivo: 05_eliminar_duplicados.cpp
   ============================================================================

   ENUNCIADO
   --------
   1) eliminarDuplicadosOrdenada() : la lista esta ordenada de menor a mayor y
      puede tener repetidos. Quitar todas las repeticiones dejando un solo
      ejemplar de cada valor.

         1 -> 1 -> 2 -> 3 -> 3 -> 3 -> 4    debe quedar   1 -> 2 -> 3 -> 4

   2) eliminarDuplicadosNoOrdenada() : la lista NO esta ordenada y puede tener
      repetidos en cualquier posicion.

         3 -> 1 -> 3 -> 1 -> 2 -> 3    debe quedar   3 -> 1 -> 2
         (se conserva el ORDEN ORIGINAL del primer ejemplar de cada valor)

   3) Cuantas veces aparece un valor x en la lista (version con lista ordenada
      y version general con una tabla de visitados).

   EL PUNTO CLAVE DEL EJERCICIO
   -----------------------------
   Cuando se borra un nodo, `actual` NO debe avanzar: hay que volver a comparar
   el nodo que quedo en su lugar contra el siguiente. Si se avanza, el valor
   duplicado se "escapa" de la comparacion.

   ========================================================================== */

#include <iostream>
#include <set>

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
    // 1) LISTA ORDENADA
    //    Los repetidos estan juntos: basta comparar actual con actual->siguiente
    //    y borrar el siguiente.
    // ------------------------------------------------------------------
    void eliminarDuplicadosOrdenada() {
        Nodo<T>* actual = head;

        while (actual != nullptr && actual->siguiente != nullptr) {
            if (actual->dato == actual->siguiente->dato) {
                // Este nodo es repetido: se borra el SIGUIENTE (no actual),
                // porque actual puede seguir siendo el unico ejemplar de su valor.
                Nodo<T>* repetido = actual->siguiente;      // 1) marcar el sobrante
                actual->siguiente = repetido->siguiente;   // 2) puentear
                delete repetido;                           // 3) borrar
                // IMPORTANTE: no se avanza `actual`; hay que comparar de nuevo
            } else {
                actual = actual->siguiente;   // solo se avanza si NO se borro nada
            }
        }
    }

    // ------------------------------------------------------------------
    // 2) LISTA NO ORDENADA
    //    Se recorre y se va quitando de la lista todo lo que ya se vio.
    //    Cada borrado es O(1) porque ya se tiene el nodo anterior.
    // ------------------------------------------------------------------
    void eliminarDuplicadosNoOrdenada() {
        Nodo<T>* actual = head;

        while (actual != nullptr) {
            Nodo<T>* anterior = nullptr;      // el que nos precede
            Nodo<T>* explorador = actual->siguiente;   // el que va despues

            while (explorador != nullptr) {
                if (explorador->dato == actual->dato) {
                    // quitar explorador de la cadena
                    if (anterior == nullptr) {
                        actual->siguiente = explorador->siguiente;
                    } else {
                        anterior->siguiente = explorador->siguiente;
                    }
                    Nodo<T>* aBorrar = explorador;
                    explorador = explorador->siguiente;   // se avanza igual
                    delete aBorrar;
                } else {
                    anterior = explorador;      // este nodo se conserva
                    explorador = explorador->siguiente;
                }
            }
            actual = actual->siguiente;
        }
    }

    // ------------------------------------------------------------------
    // 3) Contar apariciones de un valor
    // ------------------------------------------------------------------
    int vecesQueAparece(T valor) const {
        int n = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) {
            if (a->dato == valor) n++;
        }
        return n;
    }

    // Version eficiente solo para lista ordenada: se detiene al encontrar un
    // dato mayor que el buscado, porque ya no habra mas apariciones.
    int vecesQueApareceOrdenada(T valor) const {
        int n = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) {
            if (a->dato == valor) n++;
            if (a->dato > valor) break;   // al ser ordenada, se puede salir
        }
        return n;
    }

    bool estaOrdenada() const {
        for (Nodo<T>* a = head; a != nullptr && a->siguiente != nullptr; a = a->siguiente) {
            if (a->dato > a->siguiente->dato) return false;
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
    std::cout << "=== EJERCICIO 05: eliminar duplicados ===\n\n";

    // --- 1) Lista ordenada con duplicados ------------------------------------
    std::cout << "-- 1) Lista ORDENADA con repetidos --\n";
    ListaSimple<int> ordenada;
    int datos1[] = {1, 1, 2, 2, 2, 3, 3, 3, 4, 5, 5};
    for (int d : datos1) ordenada.agregarAlFinal(d);
    ordenada.mostrar("Antes:  ");
    std::cout << "  largo = " << ordenada.largo() << "\n";
    std::cout << "  vecesQueAparece(3) = " << ordenada.vecesQueAparece(3) << "\n";
    std::cout << "  vecesQueApareceOrdenada(3) = " << ordenada.vecesQueApareceOrdenada(3) << "\n\n";


    ordenada.eliminarDuplicadosOrdenada();
    ordenada.mostrar("Despues: ");
    std::cout << "  largo = " << ordenada.largo() << "  (esperado 5)\n\n";


    // --- Caso limite: todos los elementos iguales ---------------------------
    std::cout << "-- Caso limite: todos iguales --\n";
    ListaSimple<int> iguales;
    for (int i = 0; i < 6; i++) iguales.agregarAlFinal(7);
    iguales.mostrar("Antes:  ");
    iguales.eliminarDuplicadosOrdenada();
    iguales.mostrar("Despues: ");
    std::cout << "  largo = 1 (esperado 1)\n\n";

    // --- Caso limite: lista vacia --------------------------------------------
    std::cout << "-- Caso limite: lista vacia y lista sin repetidos --\n";
    ListaSimple<int> vacia;
    vacia.eliminarDuplicadosOrdenada();
    std::cout << "  lista vacia -> largo = " << vacia.largo() << " (esperado 0)\n";

    ListaSimple<int> sinRepetidos;
    sinRepetidos.agregarAlFinal(10);
    sinRepetidos.agregarAlFinal(20);
    sinRepetidos.eliminarDuplicadosOrdenada();
    sinRepetidos.mostrar("  sin repetidos tras el proceso: ");
    std::cout << "\n";

    // --- 2) Lista NO ordenada con duplicados --------------------------------
    std::cout << "-- 2) Lista NO ORDENADA con repetidos --\n";
    ListaSimple<int> desordenada;
    int datos2[] = {3, 1, 3, 1, 2, 3, 5, 1, 5};
    for (int d : datos2) desordenada.agregarAlFinal(d);
    desordenada.mostrar("Antes:  ");
    std::cout << "  largo = " << desordenada.largo() << "\n";
    desordenada.eliminarDuplicadosNoOrdenada();
    desordenada.mostrar("Despues: ");
    std::cout << "  largo = " << desordenada.largo() << "  (esperado 4: 3, 1, 2, 5)\n\n";


    // Se conserva el orden del primer ejemplar de cada valor:
    //   3 1 3 1 2 3 5 1 5  ->  3 1 2 5
    std::cout << "  Nota: se conserva el orden del primer ejemplar de cada valor.\n\n";

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 05: eliminar duplicados ===
    -- 1) Lista ORDENADA con repetidos --
    Antes:  1 -> 1 -> 2 -> 2 -> 2 -> 3 -> 3 -> 3 -> 4 -> 5 -> 5 -> nullptr
      largo = 11
      vecesQueAparece(3) = 3
      vecesQueApareceOrdenada(3) = 3
    Despues: 1 -> 2 -> 3 -> 4 -> 5 -> nullptr
      largo = 5  (esperado 5)
    -- Caso limite: todos iguales --
    Antes:  7 -> 7 -> 7 -> 7 -> 7 -> 7 -> nullptr
    Despues: 7 -> nullptr
      largo = 1 (esperado 1)
    -- Caso limite: lista vacia y lista sin repetidos --
      lista vacia -> largo = 0 (esperado 0)
      sin repetidos tras el proceso: 10 -> 20 -> nullptr
    -- 2) Lista NO ORDENADA con repetidos --
    Antes:  3 -> 1 -> 3 -> 1 -> 2 -> 3 -> 5 -> 1 -> 5 -> nullptr
      largo = 9
    Despues: 3 -> 1 -> 2 -> 5 -> nullptr
      largo = 4  (esperado 4: 3, 1, 2, 5)
      Nota: se conserva el orden del primer ejemplar de cada valor.
    Demostracion completada.
*/
