/* ============================================================================
   PLANTILLA 04 - Lista circular doblemente enlazada

   Estructura: los dos extremos estan unidos en las dos direcciones.
       head.previous ----> tail          (no es nullptr)
             |                              ^
             v                              |
           [1] <----> [2] <----> [3] <------+
             ^                              |
             +------------------------------+

   head->anterior  = el ultimo nodo
   tail->siguiente = el primero

   Ventaja: desde cualquier nodo se puede seguir hacia adelante o hacia atras
   indefinidamente, y con el puntero de un nodo insertar o eliminar es O(1).

   Metodos: agregarAlInicio, agregarAlFinal, insertarAntesDe, insertarDespuesDe,
            eliminarValor, eliminarNodo, buscar, contiene, largo, estaOrdenada,
            invertir, concatenar, estaVacia, limpiar, cabeza, cola,
            mostrarAdelante, mostrarAtras
   ========================================================================== */

#include <iostream>

// ---------------------------------------------------------------------------
// Nodo
// ---------------------------------------------------------------------------
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* anterior;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), anterior(nullptr), siguiente(nullptr) {}
};

template <typename T>
void intercambiar(T& a, T& b) {
    T temporal = a;
    a = b;
    b = temporal;
}

template <typename T>
class ListaCircularDoble {
private:
    Nodo<T>* head;
    Nodo<T>* tail;

public:
    ListaCircularDoble() : head(nullptr), tail(nullptr) {}

    ~ListaCircularDoble() {
        if (tail != nullptr) tail->siguiente = nullptr;
        Nodo<T>* a = head;
        while (a != nullptr) {
            Nodo<T>* siguiente = a->siguiente;
            delete a;
            a = siguiente;
        }
        head = tail = nullptr;
    }



    void agregarAlFinal(T valor) {         // O(1)
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (head == nullptr) {              // vacia: el nodo se cierra solo
            head = tail = nuevo;
            nuevo->anterior = nuevo;
            nuevo->siguiente = nuevo;
            return;
        }

        nuevo->anterior  = tail;
        nuevo->siguiente = head;            // cierra por adelante
        tail->siguiente  = nuevo;           // cierra por atras
        head->anterior   = nuevo;           // el nuevo pasa a ser el anterior de head
        tail = nuevo;
    }

    void agregarAlInicio(T valor) {        // O(1)
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (head == nullptr) {
            head = tail = nuevo;
            nuevo->anterior = nuevo;
            nuevo->siguiente = nuevo;
            return;
        }

        nuevo->siguiente = head;
        nuevo->anterior  = tail;
        head->anterior   = nuevo;
        tail->siguiente   = nuevo;
        head = nuevo;
    }

    void insertarAntesDe(T referencia, T valor) {   // O(n)
        Nodo<T>* objetivo = buscar(referencia);
        if (objetivo == nullptr) { agregarAlFinal(valor); return; }

        Nodo<T>* nuevo  = new Nodo<T>(valor);
        Nodo<T>* previo = objetivo->anterior;
        nuevo->anterior    = previo;
        nuevo->siguiente   = objetivo;
        previo->siguiente  = nuevo;
        objetivo->anterior = nuevo;
    }

    void insertarDespuesDe(T referencia, T valor) {  // O(n)
        Nodo<T>* objetivo = buscar(referencia);
        if (objetivo == nullptr) { agregarAlFinal(valor); return; }

        Nodo<T>* nuevo = new Nodo<T>(valor);
        Nodo<T>* sig   = objetivo->siguiente;
        nuevo->anterior      = objetivo;
        nuevo->siguiente     = sig;
        objetivo->siguiente  = nuevo;
        sig->anterior        = nuevo;
    }

    // ===================== ELIMINAR =====================

    bool eliminarValor(T valor) {
        return eliminarNodo(buscar(valor));
    }

    bool eliminarNodo(Nodo<T>* nodo) {     // O(1): 4 enlaces que arreglar
        if (nodo == nullptr) return false;

        if (head == tail) {                 // era el unico nodo
            delete nodo;
            head = tail = nullptr;
            return true;
        }

        nodo->anterior->siguiente = nodo->siguiente;   // salta el nodo
        nodo->siguiente->anterior = nodo->anterior;   // en las dos direcciones

        if (nodo == head) head = nodo->siguiente;
        if (nodo == tail) tail = nodo->anterior;

        delete nodo;
        return true;
    }

    void limpiar() {
        while (head != nullptr) eliminarNodo(head);
    }

    // ===================== CONSULTAR =====================

    Nodo<T>* buscar(T valor) const {
        if (head == nullptr) return nullptr;
        Nodo<T>* a = head;
        do {
            if (a->dato == valor) return a;
            a = a->siguiente;
        } while (a != head);
        return nullptr;
    }

    bool contiene(T valor) const { return buscar(valor) != nullptr; }

    int largo() const {                    // O(n)
        if (head == nullptr) return 0;
        int n = 1;
        Nodo<T>* a = head->siguiente;
        while (a != head) {
            n++;
            a = a->siguiente;
        }
        return n;
    }

    bool estaVacia() const { return head == nullptr; }

    bool estaOrdenada() const {
        if (head == nullptr) return true;
        Nodo<T>* a = head;
        do {
            Nodo<T>* b = a->siguiente;
            if (b != head && a->dato > b->dato) return false;
            a = b;
        } while (a != head);
        return true;
    }

    // Acceso a los extremos: util cuando ya se tiene el puntero del nodo
    Nodo<T>* cabeza() const { return head; }
    Nodo<T>* cola()   const { return tail; }

    // ===================== TRANSFORMAR =====================

    void invertir() {                      // O(n) y el circulo se mantiene
        if (head == nullptr) return;
        intercambiar(head, tail);          // las puntas cambian de lugar

        Nodo<T>* inicio = head;
        Nodo<T>* a = head;
        do {                               // exactamente UNA vuelta
            Nodo<T>* siguiente = a->siguiente;
            intercambiar(a->anterior, a->siguiente);
            a = siguiente;
        } while (a != inicio);

        head->anterior  = tail;            // se reafirman los enlaces del circulo
        tail->siguiente = head;
    }

    void concatenar(ListaCircularDoble& otra) {   // O(1). "otra" queda vacia.
        if (otra.estaVacia()) return;

        if (estaVacia()) {
            head = otra.head;
            tail = otra.tail;
        } else {
            tail->siguiente      = otra.head;
            otra.head->anterior  = tail;
            otra.tail->siguiente = head;   // el circulo de B entra en el de A
            tail = otra.tail;
            head->anterior = tail;           // y se cierra por atras
        }
        otra.head = nullptr;
        otra.tail = nullptr;
    }

    // ===================== MOSTRAR =====================

    void mostrarAdelante(const char* etiqueta = "") const {
        std::cout << etiqueta;
        if (head == nullptr) {
            std::cout << "(vacia)\n";
            return;
        }
        Nodo<T>* a = head;
        do {
            std::cout << a->dato << " <-> ";
            a = a->siguiente;
        } while (a != head);
        std::cout << "(vuelve a " << head->dato << ")\n";
    }

    void mostrarAtras(const char* etiqueta = "") const {
        std::cout << etiqueta;
        if (tail == nullptr) {
            std::cout << "(vacia)\n";
            return;
        }
        Nodo<T>* a = tail;
        do {
            std::cout << a->dato << " <- ";
            a = a->anterior;
        } while (a != tail);
        std::cout << "(vuelve a " << tail->dato << ")\n";
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== PLANTILLA 04: lista circular doblemente enlazada ===\n\n";

    // 1) Empezar vacia
    ListaCircularDoble<int> lista;
    std::cout << "1) La lista nueva ya esta vacia\n";
    lista.mostrarAdelante("   ");


    std::cout << "\n2) Insertar\n";
    lista.agregarAlFinal(67);
    lista.agregarAlInicio(1);
    lista.agregarAlFinal(5);
    lista.mostrarAdelante("   ");
    lista.mostrarAtras("   atras:  ");
    std::cout << "   largo = " << lista.largo() << "\n";

    // 3) Recorrer en ambos sentidos desde un nodo del medio
    std::cout << "\n3) Desde el nodo 3 se puede seguir hacia los dos lados\n";
    Nodo<int>* medio = lista.buscar(3);
    std::cout << "   dos pasos adelante: " << medio->siguiente->siguiente->dato
              << "   dos pasos atras:  " << medio->anterior->anterior->dato << "\n";

    // 4) Eliminar
    std::cout << "\n4) Eliminar\n";
    std::cout << "   eliminarValor(1)  = " << lista.eliminarValor(1)  << "  (cabeza)\n";
    std::cout << "   eliminarValor(5)  = " << lista.eliminarValor(5)  << "  (cola)\n";
    std::cout << "   eliminarValor(3)  = " << lista.eliminarValor(3)  << "  (medio)\n";
    std::cout << "   eliminarValor(99) = " << lista.eliminarValor(99) << "  (no existe)\n";
    lista.mostrarAdelante("   ");
    lista.mostrarAtras("   atras:  ");

    // 5) Invertir
    std::cout << "\n5) Invertir\n";
    lista.mostrarAdelante("   antes:   ");
    lista.invertir();
    lista.mostrarAdelante("   despues: ");
    lista.mostrarAtras("   atras:   ");
    lista.invertir();
    lista.mostrarAdelante("   vuelta al inicio: ");

    // 6) Concatenar
    std::cout << "\n6) Concatenar\n";
    ListaCircularDoble<int> a, b;
    a.agregarAlFinal(1);
    a.agregarAlFinal(2);
    b.agregarAlFinal(8);
    b.agregarAlFinal(9);
    a.concatenar(b);
    a.mostrarAdelante("   a + b = ");
    a.mostrarAtras("   atras:  ");
    b.mostrarAdelante("   b ahora = ");

    // 7) Caso limite: reducir a un solo nodo y luego vaciar
    std::cout << "\n7) Caso limite: quedarse con un solo nodo\n";
    while (lista.largo() > 1) lista.eliminarNodo(lista.cabeza()->siguiente);
    lista.mostrarAdelante("   ");
    lista.mostrarAtras("   ");
    std::cout << "   eliminarValor(" << lista.cabeza()->dato << ") = "
              << lista.eliminarValor(lista.cabeza()->dato) << "\n";
    lista.mostrarAdelante("   ");

    std::cout << "\nFin de la plantilla.\n";
    return 0;
}