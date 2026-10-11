/* ============================================================================
   PLANTILLA 03 - Lista circular simplemente enlazada
   Archivo : 03_lista_circular.cpp

   Librerias: SOLO <iostream>. Nada de vector, list, string ni assert.

   Estructura: el ULTIMO nodo apunta al primero, no a nullptr.
       head
        |
        v
      [1] -> [2] -> [3] -+
        ^                 |
        +-----------------+

   OJO: por eso el recorrido se detiene comparando con head, NUNCA con nullptr.

   Metodos: agregarAlInicio, agregarAlFinal, eliminarValor, eliminarNodo,
            buscar, contiene, largo, estaOrdenada, invertir (conserva el
            circulo), concatenar, estaVacia, limpiar, mostrar
   Incluye un ejemplo de Josephus, que es el uso clasico de esta estructura.
   ========================================================================== */

#include <iostream>

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
// La lista circular
// ---------------------------------------------------------------------------
template <typename T>
class ListaCircular {
private:
    Nodo<T>* head;
    Nodo<T>* tail;

public:
    ListaCircular() : head(nullptr), tail(nullptr) {}

    // do-while porque hay que dar al menos una vuelta; se para al volver a head
    ~ListaCircular() {
        if (head == nullptr) return;
        Nodo<T>* a = head;
        do {
            Nodo<T>* siguiente = a->siguiente;
            delete a;
            a = siguiente;
        } while (a != head);
        head = tail = nullptr;
    }

    // ===================== INSERTAR =====================

    void agregarAlFinal(T valor) {         // O(1)
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (head == nullptr) {              // lista vacia: el nodo se apunta a si mismo
            head = tail = nuevo;
            nuevo->siguiente = nuevo;
            return;
        }

        tail->siguiente = nuevo;            // la vieja cola entra al nuevo
        nuevo->siguiente = head;            // el nuevo cierra el circulo
        tail = nuevo;
    }

    void agregarAlInicio(T valor) {        // O(1)
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (head == nullptr) {
            head = tail = nuevo;
            nuevo->siguiente = nuevo;
            return;
        }

        nuevo->siguiente = head;
        tail->siguiente = nuevo;            // hay que reconectar la cola
        head = nuevo;
    }

    // ===================== ELIMINAR =====================

    bool eliminarValor(T valor) {          // O(n)
        if (head == nullptr) return false;

        if (head->dato == valor) {          // caso 1: es la cabeza
            if (head->siguiente == head) {  // era el unico nodo
                delete head;
                head = tail = nullptr;
                return true;
            }
            Nodo<T>* aBorrar = head;
            head = head->siguiente;
            tail->siguiente = head;         // el cierre del circulo se rehace
            delete aBorrar;
            return true;
        }

        Nodo<T>* a = head;
        do {                              // caso 2: en el medio o al final
            if (a->siguiente->dato == valor) {
                Nodo<T>* aBorrar = a->siguiente;
                a->siguiente = aBorrar->siguiente;
                if (aBorrar == tail) tail = a;      // si era la cola, se actualiza
                delete aBorrar;
                return true;
            }
            a = a->siguiente;
        } while (a != head);

        return false;                      // no existe
    }

    bool eliminarNodo(Nodo<T>* nodo) {
        if (nodo == nullptr) return false;
        return eliminarValor(nodo->dato);
    }

    void limpiar() {
        while (head != nullptr) eliminarValor(head->dato);
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
        int n = 1;                          // la cabeza ya cuenta
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
            if (b != head && a->dato > b->dato) return false;   // el cierre no se compara
            a = b;
        } while (a != head);
        return true;
    }

    // ===================== TRANSFORMAR =====================

    void invertir() {                      // O(n) y el circulo se mantiene
        if (head == nullptr || head->siguiente == head) return;

        Nodo<T>* cabezaOriginal = head;

        // 1) romper SOLO el enlace del ultimo (poner nullptr ahi)
        Nodo<T>* a = head;
        do {
            Nodo<T>* siguiente = a->siguiente;
            if (siguiente == head) a->siguiente = nullptr;   // este era el cierre
            a = siguiente;
        } while (a != head);

        // 2) invertir la cadena, empezando por la cabeza
        Nodo<T>* anterior = nullptr;
        Nodo<T>* actual = cabezaOriginal;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            actual->siguiente = anterior;
            anterior = actual;
            actual = siguiente;
        }

        // 3) la cabeza y la cola originales cambian de papel, y se cierra
        head = anterior;                    // la cola original pasa a ser la cabeza
        tail = cabezaOriginal;              // la cabeza original pasa a ser la cola
        tail->siguiente = head;
    }

    void concatenar(ListaCircular& otra) { // O(1). "otra" queda vacia.
        if (otra.estaVacia()) return;

        if (estaVacia()) {
            head = otra.head;
            tail = otra.tail;
        } else {
            tail->siguiente = otra.head;
            otra.tail->siguiente = head;    // el circulo de B entra en el de A
            tail = otra.tail;
        }
        otra.head = nullptr;
        otra.tail = nullptr;
    }

    // ===================== MOSTRAR =====================

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        if (head == nullptr) {
            std::cout << "(vacia)\n";
            return;
        }
        Nodo<T>* a = head;
        do {
            std::cout << a->dato << " -> ";
            a = a->siguiente;
        } while (a != head);
        std::cout << "(vuelve a " << head->dato << ")\n";
    }
};

// ---------------------------------------------------------------------------
// Josephus: N personas en circulo, se elimina una cada K y se reinicia el
// conteo en la siguiente. Se imprime el orden de salidas y el superviviente.
// ---------------------------------------------------------------------------
void josephus(int personas, int salto) {
    ListaCircular<int> circulo;
    for (int i = 1; i <= personas; i++) circulo.agregarAlFinal(i);

    Nodo<int>* actual = circulo.buscar(1);      // el conteo arranca en el 1
    int restantes = personas;

    std::cout << "   " << personas << " personas, se cuenta de " << salto
              << " en " << salto << ". Eliminados: ";

    while (restantes > 1) {
        for (int paso = 1; paso < salto; paso++) actual = actual->siguiente;

        int eliminado = actual->dato;            // copiar el dato ANTES de borrar
        actual = actual->siguiente;              // el siguiente reinicia el conteo
        circulo.eliminarValor(eliminado);
        restantes--;

        std::cout << eliminado;
        if (restantes > 1) std::cout << ", ";
    }

    std::cout << " | sobrevive el " << actual->dato << "\n";
}

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== PLANTILLA 03: lista circular ===\n\n";

    // 1) Empezar vacia
    ListaCircular<int> lista;
    std::cout << "1) La lista nueva ya esta vacia\n";
    lista.mostrar("   ");
    std::cout << "   largo = " << lista.largo() << "\n";

    // 2) Insertar
    std::cout << "\n2) Insertar\n";
    for (int v : {2, 3, 4, 5}) lista.agregarAlFinal(v);
    lista.agregarAlInicio(1);
    lista.agregarAlFinal(6);
    lista.mostrar("   ");
    std::cout << "   largo = " << lista.largo() << "\n";

    // 3) Consultar
    std::cout << "\n3) Consultar\n";
    std::cout << "   contiene(4) = " << std::boolalpha << lista.contiene(4)
              << "   contiene(99) = " << lista.contiene(99)
              << "   estaOrdenada = " << lista.estaOrdenada() << "\n";

    // 4) Eliminar
    std::cout << "\n4) Eliminar\n";
    std::cout << "   eliminarValor(1)  = " << lista.eliminarValor(1)  << "  (cabeza)\n";
    std::cout << "   eliminarValor(6)  = " << lista.eliminarValor(6)  << "  (cola)\n";
    std::cout << "   eliminarValor(4)  = " << lista.eliminarValor(4)  << "  (medio)\n";
    std::cout << "   eliminarValor(99) = " << lista.eliminarValor(99) << "  (no existe)\n";
    lista.mostrar("   ");

    // 5) Caso limite: quedar vacia
    std::cout << "\n5) Caso limite: vaciar del todo y empezar de nuevo\n";
    lista.limpiar();
    lista.mostrar("   ");
    lista.agregarAlFinal(9);
    lista.mostrar("   tras agregar 9: ");

    // 6) Invertir
    std::cout << "\n6) Invertir (el circulo se mantiene)\n";
    ListaCircular<int> c;
    for (int v : {1, 2, 3, 4, 5}) c.agregarAlFinal(v);
    c.mostrar("   antes:            ");
    c.invertir();
    c.mostrar("   despues:          ");
    c.invertir();
    c.mostrar("   invertido x2:     ");

    // 7) Concatenar
    std::cout << "\n7) Concatenar\n";
    ListaCircular<int> a, b;
    a.agregarAlFinal(1);
    a.agregarAlFinal(2);
    b.agregarAlFinal(8);
    b.agregarAlFinal(9);
    a.concatenar(b);
    a.mostrar("   a + b = ");
    b.mostrar("   b ahora = ");

    // 8) Josephus: el uso clasico de la lista circular
    std::cout << "\n8) Josephus\n";
    josephus(7, 3);
    josephus(5, 2);
    josephus(5, 1);

    std::cout << "\nFin de la plantilla.\n";
    return 0;
}