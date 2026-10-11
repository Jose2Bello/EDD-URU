/* ============================================================================
   PLANTILLA 01 - Lista simplemente enlazada
   Archivo : 01_lista_simplemente_enlazada.cpp

   Librerias: SOLO <iostream>. Nada de vector, list, string, assert ni otras
   librerias: todo son nodos creados con new.

   Estructura:
       head -> [10] -> [20] -> [30] -> nullptr

   Metodos: agregarAlInicio, agregarAlFinal, agregarOrdenado,
            insertarAntesDe, insertarEnPosicion,
            eliminar, eliminarPrimero, buscar, contiene, indiceDe,
            largo, sumar, maximo, minimo, estaOrdenada,
            invertir, invertirRecursivo, concatenar, eliminarDuplicados,
            estaVacia, limpiar, mostrar
   ========================================================================== */

#include <iostream>

// ---------------------------------------------------------------------------
// El nodo: guarda un dato y la direccion del siguiente
// ---------------------------------------------------------------------------
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;   // nullptr cuando es el ultimo

    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

// ---------------------------------------------------------------------------
// La lista: solo necesita guardar la direccion del primer nodo
// ---------------------------------------------------------------------------
template <typename T>
class ListaSimple {
private:
    Nodo<T>* head;

public:
    ListaSimple() : head(nullptr) {}

    // El destructor borra nodo por nodo. Guardar el siguiente ANTES de borrar.
    ~ListaSimple() {
        while (head != nullptr) {
            Nodo<T>* siguiente = head->siguiente;
            delete head;
            head = siguiente;
        }
    }

    // ===================== INSERTAR =====================

    void agregarAlInicio(T valor) {        // O(1)
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = head;            // 1) el nuevo apunta a la cabeza vieja
        head = nuevo;                       // 2) el nuevo es la nueva cabeza
    }

    void agregarAlFinal(T valor) {         // O(n)
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (head == nullptr) {              // lista vacia
            head = nuevo;
            return;
        }
        Nodo<T>* a = head;
        while (a->siguiente != nullptr) a = a->siguiente;   // ir hasta el ultimo
        a->siguiente = nuevo;
    }

    void agregarOrdenado(T valor) {         // O(n) y deja la lista ordenada
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (head == nullptr || valor <= head->dato) {   // vacia, o es el menor
            nuevo->siguiente = head;
            head = nuevo;
            return;
        }
        Nodo<T>* a = head;
        while (a->siguiente != nullptr && a->siguiente->dato < valor) a = a->siguiente;
        nuevo->siguiente = a->siguiente;
        a->siguiente = nuevo;
    }

    void insertarAntesDe(T referencia, T valor) {   // O(n)
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (head == nullptr) { head = nuevo; return; }            // lista vacia

        if (head->dato == referencia) {                            // referencia es la cabeza
            agregarAlInicio(valor);
            delete nuevo;
            return;
        }

        Nodo<T>* a = head;
        while (a->siguiente != nullptr && a->siguiente->dato != referencia) a = a->siguiente;

        if (a->siguiente == nullptr) {    // la referencia no existe -> va al final
            a->siguiente = nuevo;
            return;
        }

        nuevo->siguiente = a->siguiente;  // puente por delante
        a->siguiente = nuevo;             // puente por detras
    }

    void insertarEnPosicion(T valor, int posicion) {   // O(n), posicion 0 = cabeza
        if (posicion <= 0) { agregarAlInicio(valor); return; }

        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (head == nullptr) { head = nuevo; return; }

        Nodo<T>* a = head;
        for (int i = 0; i < posicion - 1 && a->siguiente != nullptr; i++) a = a->siguiente;
        nuevo->siguiente = a->siguiente;
        a->siguiente = nuevo;
    }

    // ===================== ELIMINAR =====================

    bool eliminar(T valor) {               // O(n), borra la primera aparicion
        if (head == nullptr) return false;

        if (head->dato == valor) {         // caso 1: es la cabeza
            Nodo<T>* aBorrar = head;
            head = head->siguiente;
            delete aBorrar;
            return true;
        }

        Nodo<T>* a = head;
        while (a->siguiente != nullptr && a->siguiente->dato != valor) a = a->siguiente;
        if (a->siguiente == nullptr) return false;      // no existe

        Nodo<T>* aBorrar = a->siguiente;                // caso 2: esta en el medio o al final
        a->siguiente = aBorrar->siguiente;              // puentear
        delete aBorrar;
        return true;
    }

    bool eliminarPrimero(T& salida) {      // O(1)
        if (head == nullptr) return false;
        Nodo<T>* aBorrar = head;
        salida = aBorrar->dato;
        head = aBorrar->siguiente;
        delete aBorrar;
        return true;
    }

    void eliminarDuplicados() {            // O(n)
        // Al borrar, el nodo actual NO avanza: hay que comparar otra vez
        // el nodo que quedo en su lugar con el siguiente.
        Nodo<T>* a = head;
        while (a != nullptr && a->siguiente != nullptr) {
            if (a->dato == a->siguiente->dato) {
                Nodo<T>* repetido = a->siguiente;
                a->siguiente = repetido->siguiente;
                delete repetido;
            } else {
                a = a->siguiente;
            }
        }
    }

    void limpiar() {                       // vacia la lista entera
        while (head != nullptr) {
            Nodo<T>* siguiente = head->siguiente;
            delete head;
            head = siguiente;
        }
    }

    // ===================== CONSULTAR =====================

    Nodo<T>* buscar(T valor) const {       // O(n), nullptr si no existe
        Nodo<T>* a = head;
        while (a != nullptr) {
            if (a->dato == valor) return a;
            a = a->siguiente;
        }
        return nullptr;
    }

    bool contiene(T valor) const { return buscar(valor) != nullptr; }

    int indiceDe(T valor) const {          // O(n), -1 si no existe
        int posicion = 0;
        Nodo<T>* a = head;
        while (a != nullptr) {
            if (a->dato == valor) return posicion;
            a = a->siguiente;
            posicion++;
        }
        return -1;
    }

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

    // ===================== RECORRER Y CALCULAR =====================

    int sumar() const {
        int total = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) total += a->dato;
        return total;
    }

    T maximo() const {
        if (head == nullptr) return T();      // se parte del primer dato, no de 0
        T maximo = head->dato;
        Nodo<T>* a = head->siguiente;
        while (a != nullptr) {
            if (a->dato > maximo) maximo = a->dato;
            a = a->siguiente;
        }
        return maximo;
    }

    T minimo() const {
        if (head == nullptr) return T();
        T minimo = head->dato;
        Nodo<T>* a = head->siguiente;
        while (a != nullptr) {
            if (a->dato < minimo) minimo = a->dato;
            a = a->siguiente;
        }
        return minimo;
    }

    // ===================== TRANSFORMAR =====================

    void invertir() {                      // O(n) con tres punteros
        Nodo<T>* anterior = nullptr;
        Nodo<T>* actual = head;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;   // 1) salvar el resto
            actual->siguiente = anterior;              // 2) invertir la flecha
            anterior = actual;                         // 3) avanzar
            actual = siguiente;                        // 4) avanzar
        }
        head = anterior;        // el ultimo procesado es la nueva cabeza
    }

    void invertirRecursivo() {             // lo mismo, con recursion
        head = invertirDesde(head);
    }

    Nodo<T>* invertirDesde(Nodo<T>* actual) {
        if (actual == nullptr || actual->siguiente == nullptr) return actual;  // caso base
        Nodo<T>* nuevaCabeza = invertirDesde(actual->siguiente);
        actual->siguiente->siguiente = actual;   // el resto queda detras de actual
        actual->siguiente = nullptr;
        return nuevaCabeza;
    }

    void concatenar(ListaSimple& otra) {   // O(n). "otra" queda vacia.
        if (otra.head == nullptr) return;   // no hay nada que unir

        if (head == nullptr) {
            head = otra.head;
            otra.head = nullptr;
            return;
        }

        Nodo<T>* a = head;
        while (a->siguiente != nullptr) a = a->siguiente;
        a->siguiente = otra.head;
        otra.head = nullptr;    // A ya es duena de esos nodos: hay que avisarlo
    }

    // ===================== MOSTRAR =====================

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        Nodo<T>* a = head;
        while (a != nullptr) {
            std::cout << a->dato << " -> ";
            a = a->siguiente;
        }
        std::cout << "nullptr\n";
    }
};

// ---------------------------------------------------------------------------
// Prueba de todos los metodos
// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== PLANTILLA 01: lista simplemente enlazada ===\n\n";

    // 1) Empezar vacia
    ListaSimple<int> lista;
    std::cout << "1) La lista nueva ya esta vacia\n";
    lista.mostrar("   ");

    // 2) Insertar
    std::cout << "\n2) Insertar\n";
    for (int v : {30, 10, 50, 20, 40}) lista.agregarAlFinal(v);
    lista.mostrar("   agregarAlFinal 30,10,50,20,40 -> ");

    lista.agregarAlInicio(5);
    lista.mostrar("   agregarAlInicio(5)           -> ");

    lista.agregarOrdenado(35);
    lista.mostrar("   agregarOrdenado(35)           -> ");

    lista.insertarAntesDe(40, 38);
    lista.mostrar("   insertarAntesDe(40, 38)       -> ");

    lista.insertarAntesDe(999, 60);
    lista.mostrar("   referencia inexistente -> 60  -> ");

    lista.insertarEnPosicion(7, 2);
    lista.mostrar("   insertarEnPosicion(7, 2)      -> ");

    // 3) Consultar
    std::cout << "\n3) Consultar\n";
    std::cout << "   largo = " << lista.largo()
              << "   sumar = " << lista.sumar()
              << "   maximo = " << lista.maximo()
              << "   minimo = " << lista.minimo() << "\n";
    std::cout << "   indiceDe(30) = " << lista.indiceDe(30)
              << "   indiceDe(999) = " << lista.indiceDe(999)
              << "   contiene(20) = " << std::boolalpha << lista.contiene(20)
              << "   contiene(21) = " << lista.contiene(21) << "\n";

    // 4) Eliminar
    std::cout << "\n4) Eliminar\n";
    std::cout << "   eliminar(5)   = " << lista.eliminar(5) << "  (cabeza)\n";
    std::cout << "   eliminar(60)  = " << lista.eliminar(60) << "  (ultimo)\n";
    std::cout << "   eliminar(35)  = " << lista.eliminar(35) << "  (medio)\n";
    std::cout << "   eliminar(999) = " << lista.eliminar(999) << "  (no existe)\n";
    lista.mostrar("   ");

    int primero;
    lista.eliminarPrimero(primero);
    std::cout << "   eliminarPrimero() devolvio " << primero << "\n";
    lista.mostrar("   ");

    // 5) Duplicados
    std::cout << "\n5) Quitar duplicados\n";
    ListaSimple<int> conRepetidos;
    for (int v : {3, 1, 3, 1, 2, 3}) conRepetidos.agregarAlFinal(v);
    conRepetidos.mostrar("   antes:    ");
    conRepetidos.eliminarDuplicados();
    conRepetidos.mostrar("   despues:  ");

    // 6) Invertir
    std::cout << "\n6) Invertir\n";
    lista.mostrar("   antes:                ");
    lista.invertir();
    lista.mostrar("   invertir():           ");
    lista.invertirRecursivo();
    lista.mostrar("   invertirRecursivo():  ");

    // 7) Concatenar
    std::cout << "\n7) Concatenar\n";
    ListaSimple<int> a, b;
    for (int v : {1, 3, 5}) a.agregarAlFinal(v);
    for (int v : {2, 4, 6}) b.agregarAlFinal(v);
    a.concatenar(b);
    a.mostrar("   a + b = ");
    b.mostrar("   b ahora = ");

    // 8) Limpiar
    std::cout << "\n8) Limpiar\n";
    a.limpiar();
    a.mostrar("   ");

    std::cout << "\nFin de la plantilla." << std::endl;
    return 0;
}
