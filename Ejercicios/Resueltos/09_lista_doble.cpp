/* ============================================================================
   EJERCICIO 09 - Lista doblemente enlazada completa
   Tema   : Lista doble - punteros anterior/siguiente, head y tail
   Archivo: 09_lista_doble.cpp
   ============================================================================

   ENUNCIADO
   --------
   Implementar una lista doblemente enlazada con las siguientes operaciones:

   1) agregarAlInicio(v)     : O(1)
   2) agregarAlFinal(v)      : O(1)  gracias al puntero `tail`
   3) insertarAntesDe(ref,v) : O(n)
   4) eliminarValor(v)        : O(n)
   5) eliminarNodo(puntero)   : O(1)  <- la ventaja de la lista doble
   6) buscar(v)              : O(n)
   7) invertir()             : O(n)  reusando los nodos, sin crear ni borrar
   8) concatenar(otra)       : O(1)  une el final de A con el principio de B
   9) estaOrdenada()         : O(n)
   10) mostrarAdelante() / mostrarAtras() : recorrido en ambos sentidos

   VENTAJAS QUE DEMUESTRA ESTE EJERCICIO
   --------------------------------------
   - Insertar y eliminar al final pasa de O(n) a O(1) por tener `tail`.
   - Se puede recorrer hacia atras, lo que la lista simple no permite.
   - Con el puntero de un nodo se puede eliminar en O(1).

   ========================================================================== */

#include <iostream>
#include <initializer_list>

// ---------------------------------------------------------------------------
// Nodo con dos punteros
// ---------------------------------------------------------------------------
template <typename T>
class NodoD {
public:
    T dato;
    NodoD<T>* anterior;    // hacia atras
    NodoD<T>* siguiente;   // hacia adelante

    NodoD(T valor) : dato(valor), anterior(nullptr), siguiente(nullptr) {}
};

// ---------------------------------------------------------------------------
// Lista doblemente enlazada
// ---------------------------------------------------------------------------
template <typename T>
class ListaDoble {
private:
    NodoD<T>* head;   // primer nodo
    NodoD<T>* tail;   // ultimo nodo

public:
    ListaDoble() : head(nullptr), tail(nullptr) {}

    ~ListaDoble() {
        NodoD<T>* a = head;
        while (a != nullptr) {
            NodoD<T>* sig = a->siguiente;
            delete a;
            a = sig;
        }
        head = tail = nullptr;
    }

    bool estaVacia() const { return head == nullptr; }

    int largo() const {
        int n = 0;
        for (NodoD<T>* a = head; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    // ------------------------------------------------------------------
    // 1) AGREGAR AL INICIO                                        O(1)
    //    Detalle: si la lista esta vacia, tail tambien debe moverse.
    // ------------------------------------------------------------------
    void agregarAlInicio(T valor) {
        NodoD<T>* nuevo = new NodoD<T>(valor);

        nuevo->siguiente = head;                 // el nuevo apunta a la cabeza vieja

        if (head != nullptr) {
            head->anterior = nuevo;              // la cabeza vieja mira hacia atras
        } else {
            tail = nuevo;                        // lista vacia: nuevo es tambien la cola
        }

        head = nuevo;                            // el nuevo pasa a ser la cabeza
    }

    // ------------------------------------------------------------------
    // 2) AGREGAR AL FINAL                                         O(1)
    // ------------------------------------------------------------------
    void agregarAlFinal(T valor) {
        NodoD<T>* nuevo = new NodoD<T>(valor);

        nuevo->anterior = tail;                  // el nuevo apunta a la cola vieja

        if (tail != nullptr) {
            tail->siguiente = nuevo;             // la cola vieja mira hacia adelante
        } else {
            head = nuevo;                        // lista vacia: nuevo es tambien la cabeza
        }

        tail = nuevo;                            // el nuevo pasa a ser la cola
    }

    // ------------------------------------------------------------------
    // 3) INSERTAR ANTES DE UN VALOR                               O(n)
    // ------------------------------------------------------------------
    void insertarAntesDe(T ref, T valor) {
        if (head == nullptr) { agregarAlFinal(valor); return; }   // lista vacia

        if (head->dato == ref) {                                  // ref es la cabeza
            agregarAlInicio(valor);
            return;
        }

        NodoD<T>* actual = head;
        while (actual->siguiente != nullptr && actual->siguiente->dato != ref) {
            actual = actual->siguiente;
        }

        if (actual->siguiente == nullptr) {     // ref no existe: va al final
            agregarAlFinal(valor);
            return;
        }

        NodoD<T>* nuevo = new NodoD<T>(valor);

        NodoD<T>* objetivo = actual->siguiente;      // el nodo con el dato ref
        nuevo->anterior    = actual;
        nuevo->siguiente   = objetivo;
        actual->siguiente  = nuevo;
        objetivo->anterior = nuevo;                  // hay que arreglar TAMBEN el atras
    }

    // ------------------------------------------------------------------
    // 4) ELIMINAR POR VALOR                                      O(n)
    //    Se localiza el nodo y se delega en eliminarNodo (que es O(1)).
    // ------------------------------------------------------------------
    bool eliminarValor(T valor) {
        NodoD<T>* actual = head;
        while (actual != nullptr && actual->dato != valor) {
            actual = actual->siguiente;
        }
        return eliminarNodo(actual);
    }

    // ------------------------------------------------------------------
    // 5) ELIMINAR POR PUNTERO                                     O(1)
    //    ESTA es la operacion que distingue a la lista doble: como ya se
    //    tiene el nodo, los dos vecinos se puentEAN directamente.
    // ------------------------------------------------------------------
    bool eliminarNodo(NodoD<T>* nodo) {
        if (nodo == nullptr) return false;

        if (nodo->anterior != nullptr) {
            nodo->anterior->siguiente = nodo->siguiente;   // puente hacia adelante
        } else {
            head = nodo->siguiente;                        // era la cabeza
        }

        if (nodo->siguiente != nullptr) {
            nodo->siguiente->anterior = nodo->anterior;    // puente hacia atras
        } else {
            tail = nodo->anterior;                         // era la cola
        }

        delete nodo;
        return true;
    }

    // ------------------------------------------------------------------
    // 6) BUSCAR                                                  O(n)
    // ------------------------------------------------------------------
    NodoD<T>* buscar(T valor) const {
        NodoD<T>* a = head;
        while (a != nullptr) {
            if (a->dato == valor) return a;
            a = a->siguiente;
        }
        return nullptr;
    }

    // ------------------------------------------------------------------
    // 7) INVERTIR                                                O(n)
    //    Se intercambian head/tail y se voltea cada nodo usando el metodo
    //    de los tres punteros, ahora con un puntero "anterior" que hace de
    //    "siguiente" al momento de invertir.
    // ------------------------------------------------------------------
    void revertir() {
        NodoD<T>* actual = head;
        NodoD<T>* guardado = nullptr;

        // se intercambian los extremos
        NodoD<T>* temp = head;
        head = tail;
        tail = temp;

        while (actual != nullptr) {
            guardado        = actual->siguiente;   // salvar el siguiente
            actual->siguiente = actual->anterior;  // la flecha de adelante pasa atras
            actual->anterior  = guardado;          // y la de atras pasa adelante
            actual = guardado;
        }
    }

    // ------------------------------------------------------------------
    // 8) CONCATENAR                                              O(1)
    //    Se enlaza tail con el head de la otra lista y se une la cola.
    //    Ojo: `otra` queda VACIA porque sus nodos ahora son de esta lista.
    // ------------------------------------------------------------------
    void concatenar(ListaDoble& otra) {
        if (otra.estaVacia()) return;     // unir con una lista vacia no hace nada

        if (estaVacia()) {
            head = otra.head;
            tail = otra.tail;
        } else {
            tail->siguiente = otra.head;      // el final de A entra a B
            otra.head->anterior = tail;       // y el inicio de B mira atras
            tail = otra.tail;
        }

        otra.head = nullptr;                  // B ya no es dueña de sus nodos
        otra.tail = nullptr;
    }

    // ------------------------------------------------------------------
    // 9) VERIFICAR EL ORDEN                                      O(n)
    // ------------------------------------------------------------------
    bool estaOrdenada() const {
        for (NodoD<T>* a = head; a != nullptr && a->siguiente != nullptr; a = a->siguiente) {
            if (a->dato > a->siguiente->dato) return false;
        }
        return true;
    }

    // ------------------------------------------------------------------
    // 10) IMPRIMIR EN AMBOS SENTIDOS
    // ------------------------------------------------------------------
    void mostrarAdelante(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (NodoD<T>* a = head; a != nullptr; a = a->siguiente) {
            std::cout << a->dato << " <-> ";
        }
        std::cout << "nullptr\n";
    }

    void mostrarAtras(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (NodoD<T>* a = tail; a != nullptr; a = a->anterior) {
            std::cout << a->dato << " <- ";
        }
        std::cout << "nullptr\n";
    }
};

template <typename T>
void cargar(ListaDoble<T>& lista, std::initializer_list<T> datos) {
    for (T d : datos) lista.agregarAlFinal(d);
}

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 09: lista doblemente enlazada ===\n\n";

    // --- Caso limite: lista vacia --------------------------------------------
    std::cout << "-- Caso limite: lista vacia --\n";
    ListaDoble<int> vacia;
    vacia.mostrarAdelante("Adelante: ");
    vacia.mostrarAtras("Atras:    ");
    std::cout << "  estaVacia = " << std::boolalpha << vacia.estaVacia() << "\n\n";

    // --- Agregar al inicio y al final ----------------------------------------
    std::cout << "-- 1) y 2) agregar al inicio y al final --\n";
    ListaDoble<int> lista;
    lista.agregarAlFinal(20);
    lista.agregarAlFinal(30);
    lista.agregarAlInicio(10);
    lista.mostrarAdelante("Adelante: ");
    lista.mostrarAtras("Atras:    ");
    std::cout << "  largo = " << lista.largo() << "  (esperado 3)\n\n";

    // --- Insertar antes de un valor ------------------------------------------
    std::cout << "-- 3) insertarAntesDe --\n";
    lista.insertarAntesDe(20, 15);
    std::cout << "  insertarAntesDe(20, 15): ";
    lista.mostrarAdelante("");
    lista.insertarAntesDe(10, 5);
    std::cout << "  insertarAntesDe(10, 5) (la cabeza): ";
    lista.mostrarAdelante("");
    lista.insertarAntesDe(99, 35);
    std::cout << "  ref inexistente (99): ";
    lista.mostrarAdelante("\n");

    // --- Verificar que los enlaces hacia atras son correctos -----------------
    std::cout << "  Comprobacion de que atras es el espejo de adelante:\n";
    lista.mostrarAtras("    ");
    std::cout << "\n";

    // --- Eliminar por valor y por puntero ------------------------------------
    std::cout << "-- 4) y 5) eliminar --\n";
    std::cout << "  eliminarValor(20):   " << lista.eliminarValor(20)
              << "   ";
    lista.mostrarAdelante("");
    std::cout << "  eliminarValor(1000): " << lista.eliminarValor(1000)
              << "  (no existe)\n";
    std::cout << "  eliminarValor(5) (cabeza): " << lista.eliminarValor(5) << "  ";
    lista.mostrarAdelante("");
    std::cout << "  eliminarValor(35) (cola):   " << lista.eliminarValor(35) << "  ";
    lista.mostrarAdelante("");

    // --- Eliminar por puntero (O(1)) -----------------------------------------
    NodoD<int>* nodo = lista.buscar(30);
    std::cout << "\n  eliminarNodo(puntero a 30): " << lista.eliminarNodo(nodo) << "  ";
    lista.mostrarAdelante("");
    lista.mostrarAtras("  Atras: ");
    std::cout << "  eliminarNodo(nullptr): " << lista.eliminarNodo(nullptr) << "  (protegido)\n\n";

    // --- Invertir -------------------------------------------------------------
    std::cout << "-- 7) invertir --\n";
    lista.mostrarAdelante("  Antes:  ");
    lista.revertir();
    lista.mostrarAdelante("  Despues: ");
    lista.mostrarAtras("  Atras:   ");
    lista.revertir();                    // volver a invertir debe restaurar
    lista.mostrarAdelante("  Restaurada: ");

    // --- Concatenar -----------------------------------------------------------
    std::cout << "\n-- 8) concatenar --\n";
    ListaDoble<int> a, b;
    cargar(a, {1, 2, 3});
    cargar(b, {7, 8, 9});
    a.mostrarAdelante("  A: ");
    b.mostrarAdelante("  B: ");
    a.concatenar(b);
    a.mostrarAdelante("  A + B:  ");
    a.mostrarAtras("  Atras:   ");
    b.mostrarAdelante("  B ahora: ");
    std::cout << "  largo de A = " << a.largo() << "  (esperado 6)\n";

    // --- Concatenar con lista vacia ------------------------------------------
    ListaDoble<int> vacia2;
    a.concatenar(vacia2);
    std::cout << "  concatenar con lista vacia -> largo = " << a.largo() << "  (esperado 6)\n\n";

    // --- Verificar orden ------------------------------------------------------
    std::cout << "-- 9) estaOrdenada --\n";
    {
        ListaDoble<int> ord;
        cargar(ord, {1, 2, 3, 4, 5});
        std::cout << "  [1,2,3,4,5] -> " << ord.estaOrdenada() << "\n";
    }
    {
        ListaDoble<int> des;
        cargar(des, {1, 5, 3, 4});
        std::cout << "  [1,5,3,4]   -> " << des.estaOrdenada() << "\n\n";
    }

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 09: lista doblemente enlazada ===
    -- Caso limite: lista vacia --
    Adelante: nullptr
    Atras:    nullptr
      estaVacia = true
    -- 1) y 2) agregar al inicio y al final --
    Adelante: 10 <-> 20 <-> 30 <-> nullptr
    Atras:    30 <- 20 <- 10 <- nullptr
      largo = 3  (esperado 3)
    -- 3) insertarAntesDe --
      insertarAntesDe(20, 15): 10 <-> 15 <-> 20 <-> 30 <-> nullptr
      insertarAntesDe(10, 5) (la cabeza): 5 <-> 10 <-> 15 <-> 20 <-> 30 <-> nullptr
      ref inexistente (99):
    5 <-> 10 <-> 15 <-> 20 <-> 30 <-> 35 <-> nullptr
      Comprobacion de que atras es el espejo de adelante:
        35 <- 30 <- 20 <- 15 <- 10 <- 5 <- nullptr
    -- 4) y 5) eliminar --
      eliminarValor(20):   true   5 <-> 10 <-> 15 <-> 30 <-> 35 <-> nullptr
      eliminarValor(1000): false  (no existe)
      eliminarValor(5) (cabeza): true  10 <-> 15 <-> 30 <-> 35 <-> nullptr
      eliminarValor(35) (cola):   true  10 <-> 15 <-> 30 <-> nullptr
      eliminarNodo(puntero a 30): true  10 <-> 15 <-> nullptr
      Atras: 15 <- 10 <- nullptr
      eliminarNodo(nullptr): false  (protegido)
    -- 7) invertir --
      Antes:  10 <-> 15 <-> nullptr
      Despues: 15 <-> 10 <-> nullptr
      Atras:   10 <- 15 <- nullptr
      Restaurada: 10 <-> 15 <-> nullptr
    -- 8) concatenar --
      A: 1 <-> 2 <-> 3 <-> nullptr
      B: 7 <-> 8 <-> 9 <-> nullptr
      A + B:  1 <-> 2 <-> 3 <-> 7 <-> 8 <-> 9 <-> nullptr
      Atras:   9 <- 8 <- 7 <- 3 <- 2 <- 1 <- nullptr
      B ahora: nullptr
      largo de A = 6  (esperado 6)
      concatenar con lista vacia -> largo = 6  (esperado 6)
    -- 9) estaOrdenada --
      [1,2,3,4,5] -> true
      [1,5,3,4]   -> false
    Demostracion completada.
*/
