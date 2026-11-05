/* ============================================================================
   PLANTILLA 02 - Lista doblemente enlazada
   Archivo : 02_lista_doblemente_enlazada.cpp

   Librerias: SOLO <iostream>. Nada de vector, list, string, assert ni otras.

   Estructura:
              head                          tail
               |                             |
               v                             v
        nullptr <-+---+      +---+------+      +---+---+-> nullptr
                  | 10| <-- | 20 |      | <-- | 30 |   |
                  +---+      +---+------+      +---+---+

   Cada nodo conoce al de atras y al de adelante. Gracias a "tail",
   agregar al final es O(1) en vez de O(n).

   Metodos: agregarAlInicio, agregarAlFinal, insertarAntesDe, insertarDespuesDe,
            eliminarValor, eliminarNodo, buscar, contiene, largo, estaOrdenada,
            invertir, concatenar, estaVacia, limpiar,
            mostrarAdelante, mostrarAtras
   ========================================================================== */

#include <iostream>

// ---------------------------------------------------------------------------
// Nodo con los dos punteros
// ---------------------------------------------------------------------------
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* anterior;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), anterior(nullptr), siguiente(nullptr) {}
};

// ---------------------------------------------------------------------------
// Intercambiar dos valores sin usar la biblioteca
// ---------------------------------------------------------------------------
template <typename T>
void intercambiar(T& a, T& b) {
    T temporal = a;
    a = b;
    b = temporal;
}

// ---------------------------------------------------------------------------
// La lista
// ---------------------------------------------------------------------------
template <typename T>
class ListaDoble {
private:
    Nodo<T>* head;
    Nodo<T>* tail;

public:
    ListaDoble() : head(nullptr), tail(nullptr) {}

    ~ListaDoble() {
        while (head != nullptr) {
            Nodo<T>* siguiente = head->siguiente;
            delete head;
            head = siguiente;
        }
        head = tail = nullptr;
    }

    // ===================== INSERTAR =====================

    void agregarAlInicio(T valor) {        // O(1)
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = head;

        if (head != nullptr) head->anterior = nuevo;
        else                 tail = nuevo;  // estaba vacia: nuevo es cabeza y cola

        head = nuevo;
    }

    void agregarAlFinal(T valor) {         // O(1) gracias a "tail"
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->anterior = tail;

        if (tail != nullptr) tail->siguiente = nuevo;
        else                 head = nuevo;  // estaba vacia

        tail = nuevo;
    }

    void insertarAntesDe(T referencia, T valor) {   // O(n)
        Nodo<T>* objetivo = buscar(referencia);
        if (objetivo == nullptr) { agregarAlFinal(valor); return; }
        if (objetivo == head)   { agregarAlInicio(valor); return; }

        Nodo<T>* nuevo     = new Nodo<T>(valor);
        Nodo<T>* previo    = objetivo->anterior;
        nuevo->anterior       = previo;
        nuevo->siguiente      = objetivo;
        previo->siguiente     = nuevo;
        objetivo->anterior    = nuevo;   // hay que arreglar tambien el de atras
    }

    void insertarDespuesDe(T referencia, T valor) {  // O(n)
        Nodo<T>* objetivo = buscar(referencia);
        if (objetivo == nullptr) { agregarAlFinal(valor); return; }
        if (objetivo == tail)   { agregarAlFinal(valor); return; }

        Nodo<T>* nuevo  = new Nodo<T>(valor);
        Nodo<T>* sig    = objetivo->siguiente;
        nuevo->anterior      = objetivo;
        nuevo->siguiente     = sig;
        objetivo->siguiente  = nuevo;
        sig->anterior        = nuevo;
    }

    // ===================== ELIMINAR =====================

    bool eliminarValor(T valor) {           // O(n) para buscar, O(1) para borrar
        return eliminarNodo(buscar(valor));
    }

    bool eliminarNodo(Nodo<T>* nodo) {     // O(1): ya tenemos el puntero
        if (nodo == nullptr) return false;

        if (nodo->anterior != nullptr) nodo->anterior->siguiente = nodo->siguiente;
        else                          head = nodo->siguiente;    // era la cabeza

        if (nodo->siguiente != nullptr) nodo->siguiente->anterior = nodo->anterior;
        else                           tail = nodo->anterior;     // era la cola

        delete nodo;
        return true;
    }

    void limpiar() {
        while (head != nullptr) eliminarNodo(head);
    }

    // ===================== CONSULTAR =====================

    Nodo<T>* buscar(T valor) const {
        Nodo<T>* a = head;
        while (a != nullptr) {
            if (a->dato == valor) return a;
            a = a->siguiente;
        }
        return nullptr;
    }

    bool contiene(T valor) const { return buscar(valor) != nullptr; }

    int largo() const {
        int n = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    bool estaVacia() const { return head == nullptr; }

    bool estaOrdenada() const {
        Nodo<T>* a = head;
        while (a != nullptr && a->siguiente != nullptr) {
            if (a->dato > a->siguiente->dato) return false;
            a = a->siguiente;
        }
        return true;
    }

    // ===================== TRANSFORMAR =====================

    void revertir() {                      // O(n) reusando los mismos nodos
        Nodo<T>* actual = head;             // se parte de la cabeza ORIGINAL
        intercambiar(head, tail);           // las puntas cambian de lugar

        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            intercambiar(actual->anterior, actual->siguiente);   // se voltea el nodo
            actual = siguiente;
        }
    }

    void concatenar(ListaDoble& otra) {     // O(1). "otra" queda vacia.
        if (otra.estaVacia()) return;

        if (estaVacia()) {
            head = otra.head;
            tail = otra.tail;
        } else {
            tail->siguiente     = otra.head;
            otra.head->anterior = tail;
            tail = otra.tail;
        }
        otra.head = nullptr;   // A ya es duena de esos nodos: hay que avisarlo
        otra.tail = nullptr;
    }

    // ===================== MOSTRAR =====================

    void mostrarAdelante(const char* etiqueta = "") const {
        std::cout << etiqueta;
        Nodo<T>* a = head;
        while (a != nullptr) {
            std::cout << a->dato << " <-> ";
            a = a->siguiente;
        }
        std::cout << "nullptr\n";
    }

    void mostrarAtras(const char* etiqueta = "") const {
        std::cout << etiqueta;
        Nodo<T>* a = tail;
        while (a != nullptr) {
            std::cout << a->dato << " <- ";
            a = a->anterior;
        }
        std::cout << "nullptr\n";
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== PLANTILLA 02: lista doblemente enlazada ===\n\n";

    // 1) Empezar vacia
    ListaDoble<int> lista;
    std::cout << "1) La lista nueva ya esta vacia\n";
    lista.mostrarAdelante("   ");

    // 2) Insertar
    std::cout << "\n2) Insertar\n";
    for (int v : {20, 30, 40}) lista.agregarAlFinal(v);
    lista.agregarAlInicio(10);
    lista.agregarAlFinal(50);
    lista.insertarAntesDe(30, 25);
    lista.insertarDespuesDe(30, 35);
    lista.mostrarAdelante("   ");
    lista.mostrarAtras("   atras:  ");

    // 3) Comprobar que "atras" es el espejo de "adelante"
    std::cout << "\n3) Los dos recorridos deben ser el mismo dato al reves\n";

    // 4) Eliminar
    std::cout << "\n4) Eliminar\n";
    std::cout << "   eliminarValor(10)  = " << lista.eliminarValor(10)  << "  (cabeza)\n";
    std::cout << "   eliminarValor(50)  = " << lista.eliminarValor(50)  << "  (cola)\n";
    std::cout << "   eliminarValor(35)  = " << lista.eliminarValor(35)  << "  (medio)\n";
    std::cout << "   eliminarValor(999) = " << lista.eliminarValor(999) << "  (no existe)\n";
    lista.mostrarAdelante("   ");
    lista.mostrarAtras("   atras:  ");

    // 5) Eliminar por puntero: O(1)
    std::cout << "\n5) eliminarNodo(puntero): es O(1) porque ya tenemos el nodo\n";
    Nodo<int>* nodo = lista.buscar(25);
    std::cout << "   eliminarNodo(el nodo con 25) = " << lista.eliminarNodo(nodo) << "\n";
    std::cout << "   eliminarNodo(nullptr)        = " << lista.eliminarNodo(nullptr) << "\n";
    lista.mostrarAdelante("   ");
    lista.mostrarAtras("   atras:  ");

    // 6) Invertir
    std::cout << "\n6) Invertir\n";
    lista.mostrarAdelante("   antes:   ");
    lista.revertir();
    lista.mostrarAdelante("   despues: ");
    lista.mostrarAtras("   atras:   ");
    lista.revertir();
    lista.mostrarAdelante("   otra vez (vuelve al inicio): ");

    // 7) Concatenar
    std::cout << "\n7) Concatenar\n";
    ListaDoble<int> a, b;
    for (int v : {1, 3, 5}) a.agregarAlFinal(v);
    for (int v : {2, 4, 6}) b.agregarAlFinal(v);
    a.concatenar(b);
    a.mostrarAdelante("   a + b = ");
    a.mostrarAtras("   atras:  ");
    b.mostrarAdelante("   b ahora = ");

    // 8) Limpiar
    std::cout << "\n8) Limpiar\n";
    a.limpiar();
    a.mostrarAdelante("   ");

    std::cout << "\nFin de la plantilla." << std::endl;
    return 0;
}
