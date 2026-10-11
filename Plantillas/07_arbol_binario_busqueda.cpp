/* ============================================================================
   PLANTILLA 07 - Arbol binario de busqueda (ABB)
   Archivo : 07_arbol_binario_busqueda.cpp

   Librerias: SOLO <iostream>. Nada de stack, queue, vector ni assert.
   Para el recorrido iterativo se incluyen aqui dos estructuras pequenas
   (una pila y una cola) que son copias reducidas de las plantillas 05 y 06:
   si prefieres, borralas y usa std::stack y std::queue con <stack> y <queue>.

   PROPIEDAD DEL ABB
   Para todo nodo: todo lo del subarbol izquierdo es MENOR que el, y todo lo
   del subarbol derecho es MAYOR. Por eso:
       - buscar es O(log n)  (en cada paso se descarta la mitad)
       - el recorrido inOrden sale ordenado de menor a mayor

   Metodos: insertar, buscar, borrar, preOrden, inOrden, postOrden,
            preOrdenIterativo, nivelOrden, altura, esLleno, esBalanceado,
            contarNodos, sumaValores, minimo, maximo, sonIdenticos,
            obtenerRaiz, estaVacia, limpiar, mostrarInOrden
   ========================================================================== */

#include <iostream>

// ===========================================================================
//  Pila y cola minimas, solo para los dos recorridos iterativos
// ===========================================================================
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T>
class PilaMin {
private:
    Nodo<T>* top;
public:
    PilaMin() : top(nullptr) {}
    ~PilaMin() {
        while (top != nullptr) {
            Nodo<T>* base = top->siguiente;
            delete top;
            top = base;
        }
    }
    void apilar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = top;
        top = nuevo;
    }
    bool desapilar(T& salida) {
        if (top == nullptr) return false;
        Nodo<T>* aBorrar = top;
        salida = aBorrar->dato;
        top = aBorrar->siguiente;
        delete aBorrar;
        return true;
    }
    bool vacia() const { return top == nullptr; }
};

template <typename T>
class ColaMin {
private:
    Nodo<T>* frente;
    Nodo<T>* cola;
public:
    ColaMin() : frente(nullptr), cola(nullptr) {}
    ~ColaMin() {
        while (frente != nullptr) {
            Nodo<T>* siguiente = frente->siguiente;
            delete frente;
            frente = siguiente;
        }
        cola = nullptr;
    }
    void encolar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = nullptr;
        if (cola == nullptr) frente = cola = nuevo;
        else { cola->siguiente = nuevo; cola = nuevo; }
    }
    bool desencolar(T& salida) {
        if (frente == nullptr) return false;
        Nodo<T>* aBorrar = frente;
        salida = aBorrar->dato;
        frente = aBorrar->siguiente;
        delete aBorrar;
        if (frente == nullptr) cola = nullptr;
        return true;
    }
    bool vacia() const { return frente == nullptr; }
};

// ===========================================================================
//  Nodo del arbol
// ===========================================================================
template <typename T>
class NodoArbol {
public:
    T dato;
    NodoArbol<T>* izquierda;
    NodoArbol<T>* derecha;
    NodoArbol(T valor) : dato(valor), izquierda(nullptr), derecha(nullptr) {}
};

// ===========================================================================
//  El arbol
// ===========================================================================
template <typename T>
class ABB {
private:
    NodoArbol<T>* raiz;

public:
    ABB() : raiz(nullptr) {}

    // Para destruir un arbol hay que hacerlo en POST-ORDEN:
    // primero los hijos y despues el padre. Si se borra el padre primero,
    // se pierde la direccion de los hijos y quedan fugas de memoria.
    ~ABB() { destruir(raiz); }

    void destruir(NodoArbol<T>* nodo) {
        if (nodo == nullptr) return;
        destruir(nodo->izquierda);
        destruir(nodo->derecha);
        delete nodo;
    }

    // ===================== INSERTAR =====================

    void insertar(T valor) { raiz = insertarEn(raiz, valor); }

    NodoArbol<T>* insertarEn(NodoArbol<T>* nodo, T valor) {
        if (nodo == nullptr) return new NodoArbol<T>(valor);   // se hallo el hueco
        if (valor < nodo->dato)      nodo->izquierda = insertarEn(nodo->izquierda, valor);
        else if (valor > nodo->dato) nodo->derecha  = insertarEn(nodo->derecha, valor);
        // si es igual no se inserta: el ABB no guarda duplicados
        return nodo;
    }

    // ===================== BUSCAR =====================

    bool buscar(T valor) const {
        NodoArbol<T>* a = raiz;
        while (a != nullptr) {
            if      (valor == a->dato) return true;
            else if (valor <  a->dato) a = a->izquierda;
            else                        a = a->derecha;
        }
        return false;
    }

    // ===================== LOS TRES RECORRIDOS =====================
    // El nombre indica DONDE aparece la raiz respecto a sus hijos.

    void preOrden(NodoArbol<T>* nodo) const {          // RAIZ - IZQ - DER
        if (nodo == nullptr) return;
        std::cout << nodo->dato << " ";
        preOrden(nodo->izquierda);
        preOrden(nodo->derecha);
    }

    void inOrden(NodoArbol<T>* nodo) const {           // IZQ - RAIZ - DER
        if (nodo == nullptr) return;
        inOrden(nodo->izquierda);
        std::cout << nodo->dato << " ";
        inOrden(nodo->derecha);
    }

    void postOrden(NodoArbol<T>* nodo) const {         // IZQ - DER - RAIZ
        if (nodo == nullptr) return;
        postOrden(nodo->izquierda);
        postOrden(nodo->derecha);
        std::cout << nodo->dato << " ";
    }

    // Pre-orden SIN recursion: se apila el derecho y luego el izquierdo,
    // porque la pila es LIFO y el izquierdo tiene que salir primero.
    void preOrdenIterativo() const {
        if (raiz == nullptr) return;
        PilaMin<NodoArbol<T>*> pila;
        pila.apilar(raiz);

        while (!pila.vacia()) {
            NodoArbol<T>* nodo;
            pila.desapilar(nodo);
            std::cout << nodo->dato << " ";
            if (nodo->derecha  != nullptr) pila.apilar(nodo->derecha);
            if (nodo->izquierda != nullptr) pila.apilar(nodo->izquierda);
        }
    }

    // Recorrido por niveles (de arriba hacia abajo) usando una cola
    void nivelOrden() const {
        if (raiz == nullptr) return;
        ColaMin<NodoArbol<T>*> pendientes;
        pendientes.encolar(raiz);

        while (!pendientes.vacia()) {
            NodoArbol<T>* nodo;
            pendientes.desencolar(nodo);
            std::cout << nodo->dato << " ";
            if (nodo->izquierda != nullptr) pendientes.encolar(nodo->izquierda);
            if (nodo->derecha  != nullptr) pendientes.encolar(nodo->derecha);
        }
    }

    // ===================== MEDIDAS =====================

    int altura(NodoArbol<T>* nodo) const {             // vacio = 0, hoja = 1
        if (nodo == nullptr) return 0;
        int izq = altura(nodo->izquierda);
        int der = altura(nodo->derecha);
        return 1 + (izq > der ? izq : der);
    }

    // Todos los niveles completos, incluido el ultimo
    bool esLleno(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return true;                                    // subarbol vacio
        if (nodo->izquierda == nullptr && nodo->derecha == nullptr) return true;   // hoja
        if (nodo->izquierda == nullptr ||  nodo->derecha == nullptr) return false;  // medio lleno
        return esLleno(nodo->izquierda) && esLleno(nodo->derecha);
    }

    // La diferencia de alturas de los hijos es 1 o menos, en todos los nodos
    bool esBalanceado(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return true;
        int izq = altura(nodo->izquierda);
        int der = altura(nodo->derecha);
        if (izq - der > 1 || der - izq > 1) return false;
        return esBalanceado(nodo->izquierda) && esBalanceado(nodo->derecha);
    }

    int contarNodos(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return 0;
        return 1 + contarNodos(nodo->izquierda) + contarNodos(nodo->derecha);
    }

    int sumaValores(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return 0;
        return nodo->dato + sumaValores(nodo->izquierda) + sumaValores(nodo->derecha);
    }

    T minimo() const {          // el menor esta en el nodo mas a la izquierda
        NodoArbol<T>* a = raiz;
        while (a != nullptr && a->izquierda != nullptr) a = a->izquierda;
        return a != nullptr ? a->dato : T();
    }

    T maximo() const {          // el mayor esta en el nodo mas a la derecha
        NodoArbol<T>* a = raiz;
        while (a != nullptr && a->derecha != nullptr) a = a->derecha;
        return a != nullptr ? a->dato : T();
    }

    bool estaVacia() const { return raiz == nullptr; }

    int altura() const { return altura(raiz); }
    bool esLleno() const { return esLleno(raiz); }
    bool esBalanceado() const { return esBalanceado(raiz); }
    int contarNodos() const { return contarNodos(raiz); }
    int sumaValores() const { return sumaValores(raiz); }

    NodoArbol<T>* obtenerRaiz() const { return raiz; }   // util para comparar arboles

    // ===================== BORRAR: los tres casos =====================

    NodoArbol<T>* minimoDe(NodoArbol<T>* nodo) {
        while (nodo->izquierda != nullptr) nodo = nodo->izquierda;
        return nodo;
    }

    void borrar(T valor) { raiz = borrarEn(raiz, valor); }

    NodoArbol<T>* borrarEn(NodoArbol<T>* nodo, T valor) {
        if (nodo == nullptr) return nullptr;        // no existia: no hay nada que hacer

        if (valor < nodo->dato) {
            nodo->izquierda = borrarEn(nodo->izquierda, valor);
        } else if (valor > nodo->dato) {
            nodo->derecha = borrarEn(nodo->derecha, valor);
        } else {
            // caso 1 y 2: sin hijos, o con un solo hijo -> el hijo ocupa su lugar
            if (nodo->izquierda == nullptr) return nodo->derecha;
            if (nodo->derecha  == nullptr) return nodo->izquierda;

            // caso 3: dos hijos -> se sube el sucesor in-orden
            // (el menor del subarbol derecho) y se borra el nodo original de ese
            NodoArbol<T>* sucesor = minimoDe(nodo->derecha);
            nodo->dato = sucesor->dato;                             // se copia el VALOR
            nodo->derecha = borrarEn(nodo->derecha, sucesor->dato); // y se borra el original
        }
        return nodo;
    }

    void limpiar() { destruir(raiz); raiz = nullptr; }

    // ===================== COMPARAR =====================

    static bool sonIdenticos(NodoArbol<T>* a, NodoArbol<T>* b) {
        if (a == nullptr && b == nullptr) return true;    // los dos vacios
        if (a == nullptr || b == nullptr) return false;   // solo uno vacio
        return a->dato == b->dato &&
               sonIdenticos(a->izquierda, b->izquierda) &&
               sonIdenticos(a->derecha,  b->derecha);
    }

    // ===================== MOSTRAR =====================

    void mostrarInOrden(const char* etiqueta = "") const {
        std::cout << etiqueta;
        inOrden(raiz);
        std::cout << "\n";
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== PLANTILLA 07: arbol binario de busqueda ===\n\n";

    // 1) Montar el arbol
    std::cout << "1) Insertando 50, 30, 70, 20, 40, 60, 80\n\n";
    std::cout << "                     [50]\n";
    std::cout << "                    /    \\\n";
    std::cout << "               [30]      [70]\n";
    std::cout << "               /   \\      /   \\\n";
    std::cout << "          [20]  [40]  [60]  [80]\n\n";

    ABB<int> arbol;
    for (int v : {50, 30, 70, 20, 40, 60, 80}) arbol.insertar(v);

    // 2) Recorridos
    std::cout << "2) Recorridos\n";
    arbol.mostrarInOrden("   in-orden   (izq, raiz, der): ");
    std::cout << "   pre-orden  (raiz, izq, der): ";
    arbol.preOrden(arbol.obtenerRaiz());
    std::cout << "\n   post-orden (izq, der, raiz): ";
    arbol.postOrden(arbol.obtenerRaiz());
    std::cout << "\n   el in-orden sale ordenado, porque es un ABB\n\n";

    std::cout << "3) Recorridos sin recursion\n";
    std::cout << "   pre-orden iterativo (pila): ";
    arbol.preOrdenIterativo();
    std::cout << "\n   por niveles (cola):        ";
    arbol.nivelOrden();
    std::cout << "\n\n";

    // 4) Buscar
    std::cout << "4) Buscar\n";
    std::cout << "   buscar(20) = " << std::boolalpha << arbol.buscar(20)
              << "   buscar(99) = " << arbol.buscar(99) << "\n\n";

    // 5) Medidas
    std::cout << "5) Medidas\n";
    std::cout << "   altura = " << arbol.altura()
              << "   nodos = " << arbol.contarNodos()
              << "   suma = " << arbol.sumaValores() << "\n";
    std::cout << "   minimo = " << arbol.minimo()
              << "   maximo = " << arbol.maximo() << "\n";
    std::cout << "   esLleno = " << arbol.esLleno()
              << "   esBalanceado = " << arbol.esBalanceado() << "\n\n";

    // 6) Borrar: los tres casos
    std::cout << "6) Borrar\n";
    arbol.mostrarInOrden("   antes de borrar:       ");
    arbol.borrar(20);
    arbol.mostrarInOrden("   caso 1 (hoja 20):      ");
    arbol.borrar(30);
    arbol.mostrarInOrden("   caso 2 (un hijo, 30): ");
    arbol.borrar(50);
    arbol.mostrarInOrden("   caso 3 (dos hijos, 50): ");
    arbol.borrar(999);
    arbol.mostrarInOrden("   borrar 999 (no existe): ");

    // 7) Comparar dos arboles
    std::cout << "\n7) Comparar dos arboles\n";
    ABB<int> uno, otro;
    for (int v : {40, 20, 60, 10, 30}) uno.insertar(v);
    for (int v : {40, 20, 60, 10, 30}) otro.insertar(v);
    std::cout << "   mismo contenido = " << std::boolalpha
              << ABB<int>::sonIdenticos(uno.obtenerRaiz(), otro.obtenerRaiz()) << "\n";
    otro.borrar(30);
    std::cout << "   tras borrar 30 del segundo = "
              << ABB<int>::sonIdenticos(uno.obtenerRaiz(), otro.obtenerRaiz()) << "\n\n";

    // 8) El ABB se puede degenerar
    std::cout << "8) Ojo: si se inserta en orden, el arbol se vuelve una lista\n";
    ABB<int> cadena;
    for (int v : {1, 2, 3, 4, 5}) cadena.insertar(v);
    std::cout << "   insertando 1, 2, 3, 4, 5 -> altura = " << cadena.altura()
              << " (un arbol bien armado daria 3)\n";
    std::cout << "   esBalanceado = " << std::boolalpha << cadena.esBalanceado()
              << "   por eso existen los arboles AVL y rojo-negro\n\n";

    // 9) Caso limite: arbol vacio
    std::cout << "9) Arbol vacio\n";
    ABB<int> vacio;
    std::cout << "   estaVacia = " << vacio.estaVacia()
              << "   altura = " << vacio.altura()
              << "   buscar(1) = " << vacio.buscar(1) << "\n";
    vacio.borrar(5);   // no debe romper nada

    std::cout << "\nFin de la plantilla.\n";
    return 0;
}