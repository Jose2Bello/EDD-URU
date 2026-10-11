/* ============================================================================
   EJERCICIO 04 - Detectar ciclos con el algoritmo de Floyd
   Tema   : Lista enlazada - ciclos / tortuga y liebre
   Archivo: 04_detectar_ciclo.cpp
   ============================================================================

   ENUNCIADO
   --------
   Una lista mal construida puede formar un ciclo si algun `siguiente` apunta a un
   nodo ya recorrido. Cuando eso pasa, cualquier recorrido del tipo
   `while (actual != nullptr)` se vuelve INFINITO.

   Implementar:

   1) tieneCiclo()              : ALGORITMO DE FLOYD (tortuga y liebre), O(n) O(1).
   2) tieneCicloPorVisitados()  : metodo alternativo con tabla de visitados,
                                  O(n) tiempo pero O(n) memoria.
   3) inicioDelCiclo()          : nodo donde COMIENZA el ciclo.
   4) longitudDelCiclo()        : cantidad de nodos del ciclo.
   5) romperCiclo()             : dejar la lista como una lista simple valida.
   6) mostrar()                 : impresion protegida (nunca infinita).

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

    // El destructor debe proteger a la lista: si quedara un ciclo, el recorrido
    // de liberacion seria infinito.
    ~ListaSimple() {
        if (tieneCiclo()) romperCiclo();      // se corta el ciclo primero
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
    // 1) ALGORITMO DE FLOYD (tortuga y liebre)            O(n) tiempo O(1) memoria
    //
    //    lento  avanza 1 nodo por vuelta
    //    rapido avanza 2 nodos por vuelta
    //
    //    Si NO hay ciclo, el rapido llega a nullptr y el metodo termina.
    //    Si HAY ciclo, los dos giran dentro y coinciden en algun momento.
    // ------------------------------------------------------------------
    bool tieneCiclo() const {
        Nodo<T>* lento  = head;
        Nodo<T>* rapido = head;

        while (rapido != nullptr && rapido->siguiente != nullptr) {
            lento  = lento->siguiente;
            rapido = rapido->siguiente->siguiente;

            if (lento == rapido) return true;   // se encontraron
        }
        return false;
    }

    // ------------------------------------------------------------------
    // 2) METODO CON TABLA DE VISITADOS             O(n) tiempo pero O(n) memoria
    //
    //    Se recorren los nodos guardando cada direccion en un conjunto.
    //    Si alguna direccion se repite, hay ciclo.
    //    Es mas lento que Floyd, pero mucho mas facil de explicar.
    // ------------------------------------------------------------------
    bool tieneCicloPorVisitados() const {
        std::set<Nodo<T>*> visitados;
        Nodo<T>* a = head;
        while (a != nullptr) {
            if (visitados.count(a) > 0) return true;   // ya habiamos pasado por aqui
            visitados.insert(a);
            a = a->siguiente;
        }
        return false;
    }

    // ------------------------------------------------------------------
    // 3) INICIO DEL CICLO
    //
    //    Cuando lento y rapido se encuentran, ese nodo esta SIEMPRE dentro
    //    del ciclo. La idea clasica: se reinicia lento en head y se avanzan
    //    los dos de a uno; el punto donde vuelven a coincidir es el inicio
    //    del ciclo (teorema del "hueco" de Floyd).
    // ------------------------------------------------------------------
    Nodo<T>* inicioDelCiclo() const {
        Nodo<T>* lento  = head;
        Nodo<T>* rapido = head;

        while (rapido != nullptr && rapido->siguiente != nullptr) {
            lento  = lento->siguiente;
            rapido = rapido->siguiente->siguiente;

            if (lento == rapido) {
                Nodo<T>* inicio = head;      // reinicio en la cabeza
                while (inicio != lento) {    // avanzar de a uno
                    inicio = inicio->siguiente;
                    lento  = lento->siguiente;
                }
                return inicio;                // nodo de entrada al ciclo
            }
        }
        return nullptr;   // no hay ciclo
    }

    // ------------------------------------------------------------------
    // 4) LONGITUD DEL CICLO
    //    Desde el nodo de inicio se recorre hasta volver a el, contando.
    // ------------------------------------------------------------------
    int longitudDelCiclo() const {
        Nodo<T>* inicio = inicioDelCiclo();
        if (inicio == nullptr) return 0;

        int longitud = 1;
        Nodo<T>* a = inicio->siguiente;
        while (a != inicio) {
            a = a->siguiente;
            longitud++;
        }
        return longitud;
    }

    // ------------------------------------------------------------------
    // 5) ROMPER EL CICLO
    //
    //    El ciclo se cierra porque UN nodo del ciclo (el ultimo) apunta de
    //    vuelta al nodo de inicio. La solucion es localizar ese nodo y poner
    //    su `siguiente` en nullptr. Se conservan TODOS los nodos: la lista
    //    queda como una lista simple valida que empieza en head.
    //
    //        1 -> 2 -> 3 -> 4
    //                  ^         (n4->siguiente = n2, la flecha que cierra)
    //         1 -> 2 -> 3 -> 4 -> nullptr
    // ------------------------------------------------------------------
    void romperCiclo() {
        Nodo<T>* inicio = inicioDelCiclo();
        if (inicio == nullptr) return;      // no hay ciclo, no hay nada que hacer

        Nodo<T>* ultimo = inicio;
        while (ultimo->siguiente != inicio) {   // avanzar hasta el ultimo del ciclo
            ultimo = ultimo->siguiente;
        }

        ultimo->siguiente = nullptr;   // se corta la flecha que cerraba el circulo
    }

    // ------------------------------------------------------------------
    // 6) MOSTRAR PROTEGIDO
    //    Si se pasa el mismo nodo dos veces, se detecta y se corta la impresion.
    //    Asi el programa nunca se queda colgado mostrando un ciclo.
    // ------------------------------------------------------------------
    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        std::set<Nodo<T>*> vistos;
        Nodo<T>* a = head;
        while (a != nullptr) {
            if (vistos.count(a) > 0) {
                std::cout << "(vuelve al nodo " << a->dato << ": CIERRE DE CICLO)\n";
                return;
            }
            vistos.insert(a);
            std::cout << a->dato << " -> ";
            a = a->siguiente;
        }
        std::cout << "nullptr\n";
    }

    int largo() const {
        if (tieneCiclo()) return -1;   // un ciclo no tiene "largo" definido
        int n = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    Nodo<T>* cabeza() const { return head; }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 04: detectar ciclos (Floyd) ===\n\n";

    // --- Lista SIN ciclo ------------------------------------------------------
    std::cout << "-- Lista SIN ciclo --\n";
    ListaSimple<int> normal;
    normal.agregarAlFinal(1);
    normal.agregarAlFinal(2);
    normal.agregarAlFinal(3);
    normal.agregarAlFinal(4);
    normal.mostrar("Lista: ");
    std::cout << "  tieneCiclo()             = " << std::boolalpha << normal.tieneCiclo() << "   (esperado false)\n";
    std::cout << "  tieneCicloPorVisitados() = " << normal.tieneCicloPorVisitados() << "   (esperado false)\n";
    std::cout << "  inicioDelCiclo()         = " << (normal.inicioDelCiclo() != nullptr ? "nodo" : "nullptr") << "   (esperado nullptr)\n";
    std::cout << "  longitudDelCiclo()       = " << normal.longitudDelCiclo() << "   (esperado 0)\n";
    std::cout << "  largo()                  = " << normal.largo() << "   (esperado 4)\n\n";


    // --- Construccion manual de una lista CON ciclo --------------------------
    //   1 -> 2 -> 3 -> 4 -> (vuelve a 2)
    //   Camino previo al ciclo: 1        (1 nodo)
    //   Ciclo: 2 -> 3 -> 4 -> 2          (3 nodos)
    std::cout << "-- Lista CON ciclo: 1 -> 2 -> 3 -> 4 -> (vuelve a 2) --\n";
    ListaSimple<int> conCiclo;
    conCiclo.agregarAlFinal(1);
    conCiclo.agregarAlFinal(2);
    conCiclo.agregarAlFinal(3);
    conCiclo.agregarAlFinal(4);

    Nodo<int>* n1 = conCiclo.cabeza();               // dato 1
    Nodo<int>* n2 = n1->siguiente;                   // dato 2
    Nodo<int>* n3 = n2->siguiente;                   // dato 3
    Nodo<int>* n4 = n3->siguiente;                   // dato 4
    n4->siguiente = n2;      // <-- aqui se cierra el ciclo

    conCiclo.mostrar("Lista: ");
    std::cout << "  tieneCiclo()             = " << conCiclo.tieneCiclo() << "   (esperado true)\n";
    std::cout << "  tieneCicloPorVisitados() = " << conCiclo.tieneCicloPorVisitados() << "   (esperado true)\n";
    std::cout << "  inicioDelCiclo()->dato   = " << conCiclo.inicioDelCiclo()->dato << "   (esperado 2)\n";
    std::cout << "  longitudDelCiclo()       = " << conCiclo.longitudDelCiclo() << "   (esperado 3)\n\n";


    // --- Romper el ciclo -----------------------------------------------------
    std::cout << "-- Romper el ciclo --\n";
    conCiclo.romperCiclo();
    conCiclo.mostrar("Lista resultante: ");
    std::cout << "  tieneCiclo() = " << conCiclo.tieneCiclo() << "   (esperado false)\n";
    std::cout << "  largo()      = " << conCiclo.largo() << "   (esperado 4)\n\n";


    // --- Ciclo que empieza en la cabeza --------------------------------------
    std::cout << "-- Ciclo que empieza en la cabeza: 7 -> 8 -> 9 -> (vuelve a 7) --\n";
    ListaSimple<int> cicloEnCabeza;
    cicloEnCabeza.agregarAlFinal(7);
    cicloEnCabeza.agregarAlFinal(8);
    cicloEnCabeza.agregarAlFinal(9);
    Nodo<int>* a1 = cicloEnCabeza.cabeza();
    Nodo<int>* a3 = a1->siguiente->siguiente;
    a3->siguiente = a1;      // el ultimo vuelve a la cabeza

    std::cout << "  longitudDelCiclo() = " << cicloEnCabeza.longitudDelCiclo() << "   (esperado 3)\n";

    cicloEnCabeza.romperCiclo();
    cicloEnCabeza.mostrar("Lista resultante: ");
    std::cout << "  largo() = " << cicloEnCabeza.largo() << "   (esperado 3)\n\n";

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 04: detectar ciclos (Floyd) ===
    -- Lista SIN ciclo --
    Lista: 1 -> 2 -> 3 -> 4 -> nullptr
      tieneCiclo()             = false   (esperado false)
      tieneCicloPorVisitados() = false   (esperado false)
      inicioDelCiclo()         = nullptr   (esperado nullptr)
      longitudDelCiclo()       = 0   (esperado 0)
      largo()                  = 4   (esperado 4)
    -- Lista CON ciclo: 1 -> 2 -> 3 -> 4 -> (vuelve a 2) --
    Lista: 1 -> 2 -> 3 -> 4 -> (vuelve al nodo 2: CIERRE DE CICLO)
      tieneCiclo()             = true   (esperado true)
      tieneCicloPorVisitados() = true   (esperado true)
      inicioDelCiclo()->dato   = 2   (esperado 2)
      longitudDelCiclo()       = 3   (esperado 3)
    -- Romper el ciclo --
    Lista resultante: 1 -> 2 -> 3 -> 4 -> nullptr
      tieneCiclo() = false   (esperado false)
      largo()      = 4   (esperado 4)
    -- Ciclo que empieza en la cabeza: 7 -> 8 -> 9 -> (vuelve a 7) --
      longitudDelCiclo() = 3   (esperado 3)
    Lista resultante: 7 -> 8 -> 9 -> nullptr
      largo() = 3   (esperado 3)
    Demostracion completada.
*/
