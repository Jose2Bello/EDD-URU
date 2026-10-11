/* ============================================================================
   EJERCICIO 07 - Fusionar dos listas ordenadas
   Tema   : Listas - fusion (merge) con nodo centinela y recursion
   Archivo: 07_fusionar_listas.cpp
   ============================================================================

   ENUNCIADO
   --------
   Se tienen dos listas simplemente enlazadas, ambas ordenadas de menor a
   mayor y sin repetidos dentro de cada una. Se pide:

   1) fusionarIterativa(a, b) : devuelve una lista ordenada con todos los
      elementos de ambas, usando un nodo centinela (dummy).            O(n+m)
   2) fusionarRecursiva(a, b): la misma logica resuelta con recursion. O(n+m)
   3) fusionarEnPlace(a, b)   : cada nodo de b se inserta ordenado en a.
                                                                     O(n*m)
   4) interseccion(a, b)      : elementos comunes a ambas.            O(n+m)
   5) diferencia(a, b)        : elementos de a que NO estan en b.     O(n+m)

   EL TRUCO DEL NODO CENTINELA (dummy)
   -----------------------------------
   Sin el hay que escribir dos casos especiales ("si a es nullptr" y "si b es
   nullptr"). Con un nodo centinela al que se le cuelga todo, el algoritmo es
   un unico bucle y al final se devuelve el siguiente del dummy. El dummy se
   crea y se borra: nunca forma parte del resultado.

   IMPORTANTE - PROPIEDAD DE LA MEMORIA
   ------------------------------------
   fusionarIterativa, fusionarRecursiva y fusionarEnPlace REUTILIZAN los nodos
   de las listas de entrada: no crean nodos nuevos. Por eso, despues de
   fusionar, las listas de entrada quedan invalidas y hay que "devolverles" la
   propiedad de sus nodos con propiedadDeLosNodos() = nullptr. Si se olvida, el
   destructor intentara borrar los mismos nodos dos veces.

   interseccion y diferencia, en cambio, solo LEEN las entradas: crean sus
   propios nodos y dejan intactas las listas originales.

   ========================================================================== */

#include <iostream>
#include <initializer_list>

template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}

    // Constructor vacio: se usa para el nodo centinela (dummy), que nunca
    // guarda un dato real, solo sirve de "ancla" para la cabeza de la lista.
    Nodo() : dato(T()), siguiente(nullptr) {}
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

    Nodo<T>* cabeza() const { return head; }

    // Asigna la cabeza directamente. Se usa cuando un algoritmo devuelve
    // la cabeza de la lista ya construida.
    void fijarCabeza(Nodo<T>* n) { head = n; }

    // Referencia al puntero head: sirve para transferir la propiedad de los
    // nodos cuando un algoritmo los reasigna a otra lista.
    Nodo<T>*& propiedadDeLosNodos() { return head; }

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

    bool estaOrdenada() const {
        for (Nodo<T>* a = head; a != nullptr && a->siguiente != nullptr; a = a->siguiente) {
            if (a->dato > a->siguiente->dato) return false;
        }
        return true;
    }
};

// Cargar una lista con datos (evita devolver objetos Lista por valor)
template <typename T>
void cargar(ListaSimple<T>& lista, std::initializer_list<T> datos) {
    for (T d : datos) lista.agregarAlFinal(d);
}

// ---------------------------------------------------------------------------
// 1) FUSION ITERATIVA con nodo centinela
//
//    while (a != nullptr && b != nullptr)
//        se cuelga el menor de a->dato y b->dato en la cola
//    al salir, queda una sola lista con datos: se cuelga entera
// ---------------------------------------------------------------------------
template <typename T>
Nodo<T>* fusionarIterativa(Nodo<T>* a, Nodo<T>* b) {
    Nodo<T>* centinela = new Nodo<T>();   // Nodo<T>() -> dato por defecto
    Nodo<T>* cola = centinela;

    while (a != nullptr && b != nullptr) {
        if (a->dato <= b->dato) {
            cola->siguiente = a;         // se REUTILIZA el nodo existente de a
            a = a->siguiente;
        } else {
            cola->siguiente = b;
            b = b->siguiente;
        }
        cola = cola->siguiente;
    }

    cola->siguiente = (a != nullptr) ? a : b;   // cuelga lo que sobre

    Nodo<T>* resultado = centinela->siguiente;
    delete centinela;                          // el centinela nunca se devuelve
    return resultado;
}

// ---------------------------------------------------------------------------
// 2) FUSION RECURSIVA
//    Caso base: si una lista esta vacia, devuelve la otra.
// ---------------------------------------------------------------------------
template <typename T>
Nodo<T>* fusionarRecursiva(Nodo<T>* a, Nodo<T>* b) {
    if (a == nullptr) return b;                       // caso base
    if (b == nullptr) return a;                       // caso base

    if (a->dato <= b->dato) {
        a->siguiente = fusionarRecursiva(a->siguiente, b);   // caso recursivo
        return a;
    }
    b->siguiente = fusionarRecursiva(a, b->siguiente);
    return b;
}

// ---------------------------------------------------------------------------
// 3) FUSION "EN EL LUGAR"
//    Cada nodo de b se inserta ordenado dentro de a. O(n*m).
// ---------------------------------------------------------------------------
template <typename T>
Nodo<T>* fusionarEnPlace(Nodo<T>* a, Nodo<T>* b) {
    while (b != nullptr) {
        Nodo<T>* porInsertar = b;
        b = b->siguiente;                            // se saca de b

        if (a == nullptr || porInsertar->dato < a->dato) {
            porInsertar->siguiente = a;              // entra por la cabeza
            a = porInsertar;
        } else {
            Nodo<T>* actual = a;
            while (actual->siguiente != nullptr &&
                   actual->siguiente->dato < porInsertar->dato) {
                actual = actual->siguiente;
            }
            porInsertar->siguiente = actual->siguiente;
            actual->siguiente = porInsertar;
        }
    }
    return a;
}

// ---------------------------------------------------------------------------
// 4) INTERSECCION y 5) DIFERENCIA
//    Aqui SI se crean nodos nuevos: las entradas solo se leen.
// ---------------------------------------------------------------------------
template <typename T>
Nodo<T>* interseccion(Nodo<T>* a, Nodo<T>* b) {
    Nodo<T>* centinela = new Nodo<T>();
    Nodo<T>* cola = centinela;

    while (a != nullptr && b != nullptr) {
        if (a->dato == b->dato) {
            cola->siguiente = new Nodo<T>(a->dato);
            cola = cola->siguiente;
            a = a->siguiente;
            b = b->siguiente;
        } else if (a->dato < b->dato) {
            a = a->siguiente;      // a tiene algo que b no tiene
        } else {
            b = b->siguiente;      // b tiene algo que a no tiene
        }
    }

    Nodo<T>* resultado = centinela->siguiente;
    delete centinela;
    return resultado;
}

template <typename T>
Nodo<T>* diferencia(Nodo<T>* a, Nodo<T>* b) {
    Nodo<T>* centinela = new Nodo<T>();
    Nodo<T>* cola = centinela;

    while (a != nullptr) {
        while (b != nullptr && b->dato < a->dato) b = b->siguiente;   // descartar menores

        if (b == nullptr || b->dato != a->dato) {                     // no esta en b
            cola->siguiente = new Nodo<T>(a->dato);
            cola = cola->siguiente;
        }
        a = a->siguiente;
    }

    Nodo<T>* resultado = centinela->siguiente;
    delete centinela;
    return resultado;
}

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 07: fusionar listas ordenadas ===\n\n";

    // --- 1) Fusion iterativa --------------------------------------------------
    std::cout << "-- 1) Fusion iterativa con centinela --\n";
    {
        ListaSimple<int> a, b;
        cargar(a, {1, 3, 5, 7, 9});
        cargar(b, {2, 4, 6, 8, 10});
        a.mostrar("  A: ");
        b.mostrar("  B: ");

        ListaSimple<int> resultado;
        resultado.fijarCabeza(fusionarIterativa(a.cabeza(), b.cabeza()));
        a.propiedadDeLosNodos() = nullptr;   // los nodos ahora son de resultado
        b.propiedadDeLosNodos() = nullptr;

        resultado.mostrar("  A + B: ");
        std::cout << "  largo = " << resultado.largo() << "  (esperado 10)\n";
        std::cout << "  estaOrdenada = " << std::boolalpha << resultado.estaOrdenada() << "\n\n";
    }

    // Casos limite de la fusion
    std::cout << "  Casos limite:\n";
    {
        ListaSimple<int> vacia, b;
        cargar(b, {2, 4, 6, 8, 10});
        ListaSimple<int> res;
        res.fijarCabeza(fusionarIterativa(vacia.cabeza(), b.cabeza()));
        b.propiedadDeLosNodos() = nullptr;
        res.mostrar("    fusion(vacia, B): ");
    }
    {
        ListaSimple<int> a;
        cargar(a, {1, 3, 5, 7, 9});
        ListaSimple<int> res;
        res.fijarCabeza(fusionarIterativa<int>(a.cabeza(), nullptr));
        a.propiedadDeLosNodos() = nullptr;
        res.mostrar("    fusion(A, vacia): ");
    }
    {
        ListaSimple<int> res;
        res.fijarCabeza(fusionarIterativa<int>(nullptr, nullptr));
        res.mostrar("    fusion(vacia, vacia): ");
    }

    // --- 2) Fusion recursiva --------------------------------------------------
    std::cout << "\n-- 2) Fusion recursiva --\n";
    {
        ListaSimple<int> a, b;
        cargar(a, {1, 3, 5, 7, 9});
        cargar(b, {2, 4, 6, 8, 10});

        ListaSimple<int> res;
        res.fijarCabeza(fusionarRecursiva(a.cabeza(), b.cabeza()));
        a.propiedadDeLosNodos() = nullptr;
        b.propiedadDeLosNodos() = nullptr;

        res.mostrar("  A + B: ");
        std::cout << "  largo = " << res.largo() << "  (esperado 10)\n";
    }

    // --- 3) Fusion en el lugar ------------------------------------------------
    std::cout << "\n-- 3) Fusion en el lugar --\n";
    {
        ListaSimple<int> a, b;
        cargar(a, {1, 3, 5, 7, 9});
        cargar(b, {2, 4, 6, 8, 10});

        ListaSimple<int> res;
        res.fijarCabeza(fusionarEnPlace(a.cabeza(), b.cabeza()));
        a.propiedadDeLosNodos() = nullptr;
        b.propiedadDeLosNodos() = nullptr;

        res.mostrar("  A + B: ");
        std::cout << "  largo = " << res.largo() << "  (esperado 10)\n\n";
    }

    // --- 4) Interseccion ------------------------------------------------------
    // Estas dos NO reutilizan nodos, asi que las listas de entrada quedan intactas.
    std::cout << "-- 4) Interseccion --\n";
    {
        ListaSimple<int> a, c;
        cargar(a, {1, 3, 5, 7, 9});
        cargar(c, {3, 4, 5, 20, 25});

        ListaSimple<int> res;
        res.fijarCabeza(interseccion(a.cabeza(), c.cabeza()));
        res.mostrar("    comun = ");

        ListaSimple<int> sinNada;
        res.fijarCabeza(interseccion(a.cabeza(), sinNada.cabeza()));
        res.mostrar("    A con lista vacia = ");
    }

    // --- 5) Diferencia --------------------------------------------------------
    std::cout << "\n-- 5) Diferencia (elementos de A que no estan en C) --\n";
    {
        ListaSimple<int> a, c;
        cargar(a, {1, 3, 5, 7, 9});
        cargar(c, {3, 4, 5, 20, 25});

        ListaSimple<int> res;
        res.fijarCabeza(diferencia(a.cabeza(), c.cabeza()));
        res.mostrar("    A - C = ");
    }

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 07: fusionar listas ordenadas ===
    -- 1) Fusion iterativa con centinela --
      A: 1 -> 3 -> 5 -> 7 -> 9 -> nullptr
      B: 2 -> 4 -> 6 -> 8 -> 10 -> nullptr
      A + B: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> nullptr
      largo = 10  (esperado 10)
      estaOrdenada = true
      Casos limite:
        fusion(vacia, B): 2 -> 4 -> 6 -> 8 -> 10 -> nullptr
        fusion(A, vacia): 1 -> 3 -> 5 -> 7 -> 9 -> nullptr
        fusion(vacia, vacia): nullptr
    -- 2) Fusion recursiva --
      A + B: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> nullptr
      largo = 10  (esperado 10)
    -- 3) Fusion en el lugar --
      A + B: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> nullptr
      largo = 10  (esperado 10)
    -- 4) Interseccion --
        comun = 3 -> 5 -> nullptr
        A con lista vacia = nullptr
    -- 5) Diferencia (elementos de A que no estan en C) --
        A - C = 1 -> 7 -> 9 -> nullptr
    Demostracion completada.
*/
