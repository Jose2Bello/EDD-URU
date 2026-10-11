/* ============================================================================
   PLANTILLA 06 - Cola (queue, FIFO) y Deque
   Archivo : 06_cola.cpp

   Librerias: SOLO <iostream>. Nada de queue, deque, string, vector ni assert.

   Estructura: entra por la cola y sale por el frente, todo O(1).
       frente                              cola
          |                                  |
          v                                  v
        [10] -> [20] -> [30] -> [40] -> [50]
       sale                              entra

   Por eso guardamos los DOS extremos: si solo guardamos el frente,
   encolar seria O(n).

   Metodos de Cola: encolar, desencolar, primero, ultimo, estaVacia,
                    tamano, limpiar, invertir, mostrar
   Ademas: ColaCircular (arreglo de capacidad fija), ColaPrioridad y Deque.
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
// Intercambiar dos valores sin usar la biblioteca
// ---------------------------------------------------------------------------
template <typename T>
void intercambiar(T& a, T& b) {
    T temporal = a;
    a = b;
    b = temporal;
}

// ---------------------------------------------------------------------------
// Cola
// ---------------------------------------------------------------------------
template <typename T>
class Cola {
private:
    Nodo<T>* frente;   // sale por aqui
    Nodo<T>* cola;     // entra por aqui

public:
    Cola() : frente(nullptr), cola(nullptr) {}

    ~Cola() {
        while (frente != nullptr) {
            Nodo<T>* siguiente = frente->siguiente;
            delete frente;
            frente = siguiente;
        }
        cola = nullptr;
    }

    void encolar(T valor) {                 // O(1)
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = nullptr;          // sera el ultimo

        if (cola == nullptr) {
            frente = cola = nuevo;           // estaba vacia
        } else {
            cola->siguiente = nuevo;         // la vieja cola entra al nuevo
            cola = nuevo;
        }
    }

    bool desencolar(T& salida) {             // O(1)
        if (frente == nullptr) return false;
        Nodo<T>* aBorrar = frente;
        salida = aBorrar->dato;
        frente = aBorrar->siguiente;
        delete aBorrar;
        if (frente == nullptr) cola = nullptr;   // se vacio
        return true;
    }

    bool primero(T& salida) const {
        if (frente == nullptr) return false;
        salida = frente->dato;
        return true;
    }

    bool ultimo(T& salida) const {
        if (cola == nullptr) return false;
        salida = cola->dato;
        return true;
    }

    bool estaVacia() const { return frente == nullptr; }

    int tamano() const {
        int n = 0;
        for (Nodo<T>* a = frente; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    void limpiar() {
        T nada;
        while (desencolar(nada)) { /* se repite hasta vaciar */ }
    }

    void invertir() {                        // O(n): invierte y cambia los extremos
        Nodo<T>* anterior = nullptr;
        Nodo<T>* actual = frente;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            actual->siguiente = anterior;
            anterior = actual;
            actual = siguiente;
        }
        intercambiar(frente, cola);          // el primero invertido es la nueva cola
    }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        Nodo<T>* a = frente;
        while (a != nullptr) {
            std::cout << a->dato << " ";
            a = a->siguiente;
        }
        if (frente == nullptr) std::cout << "(vacia)";
        std::cout << "     <- el de la izquierda sale primero\n";
    }
};

// ---------------------------------------------------------------------------
// Cola circular con arreglo de capacidad fija.
// Los indices "dan la vuelta" con el modulo: (i + 1) % capacidad.
// El contador "cantidad" distingue el caso vacia del caso llena.
// ---------------------------------------------------------------------------
template <typename T>
class ColaCircular {
private:
    T* datos;
    int capacidad;
    int frente;
    int final;       // lugar donde entrara el siguiente elemento
    int cantidad;

public:
    ColaCircular(int capacidadInicial)
        : capacidad(capacidadInicial), frente(0), final(0), cantidad(0) {
        datos = new T[capacidad];
    }

    ~ColaCircular() { delete[] datos; }

    bool encolar(T valor) {                  // O(1)
        if (llena()) return false;
        datos[final] = valor;
        final = (final + 1) % capacidad;
        cantidad++;
        return true;
    }

    bool desencolar(T& salida) {             // O(1)
        if (vacia()) return false;
        salida = datos[frente];
        frente = (frente + 1) % capacidad;
        cantidad--;
        return true;
    }

    bool vacia() const { return cantidad == 0; }
    bool llena() const { return cantidad == capacidad; }
    int  tamano() const { return cantidad; }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (int i = 0; i < cantidad; i++)
            std::cout << datos[(frente + i) % capacidad] << " ";   // se recorre circular
        if (cantidad == 0) std::cout << "(vacia)";
        std::cout << "     (frente = " << frente << ", final = " << final << ")\n";
    }
};

// ---------------------------------------------------------------------------
// Cola de prioridad: sale primero el de MENOR prioridad (1 = mas urgente).
// El nombre se guarda como texto en un arreglo de char, sin usar string.
// ---------------------------------------------------------------------------
struct Cliente {
    char nombre[16];
    int prioridad;
};

class ColaPrioridad {
private:
    Cliente* datos;
    int cantidad;
    int capacidad;

public:
    ColaPrioridad(int capacidadInicial)
        : cantidad(0), capacidad(capacidadInicial) {
        datos = new Cliente[capacidad];
    }

    ~ColaPrioridad() { delete[] datos; }

    void encolar(const Cliente& cliente) {   // insercion ordenada O(cantidad)
        if (cantidad >= capacidad) return;

        int i = cantidad - 1;
        while (i >= 0 && datos[i].prioridad > cliente.prioridad) {
            datos[i + 1] = datos[i];          // se corre el bloque una posicion
            i--;
        }
        datos[i + 1] = cliente;
        cantidad++;
    }

    bool desencolar(Cliente& salida) {       // sale la mas urgente
        if (cantidad == 0) return false;
        salida = datos[0];
        cantidad--;
        for (int i = 0; i < cantidad; i++) datos[i] = datos[i + 1];
        return true;
    }

    bool estaVacia() const { return cantidad == 0; }
    int  tamano() const { return cantidad; }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (int i = 0; i < cantidad; i++)
            std::cout << datos[i].nombre << "(" << datos[i].prioridad << ") ";
        if (cantidad == 0) std::cout << "(vacia)";
        std::cout << "\n";
    }
};

// ---------------------------------------------------------------------------
// Arma un Cliente a partir de su nombre en texto.
// El nombre va en un arreglo de char, para no usar la clase string.
// ---------------------------------------------------------------------------
Cliente crearCliente(const char* nombre, int prioridad) {
    Cliente cliente;
    cliente.prioridad = prioridad;
    int k = 0;
    while (nombre[k] != '\0' && k < 15) {
        cliente.nombre[k] = nombre[k];
        k++;
    }
    cliente.nombre[k] = '\0';
    return cliente;
}

// ---------------------------------------------------------------------------
// Deque: se puede encolar y desencolar por los DOS extremos
// ---------------------------------------------------------------------------
template <typename T>
class Deque {
private:
    Nodo<T>* frente;
    Nodo<T>* cola;

public:
    Deque() : frente(nullptr), cola(nullptr) {}

    ~Deque() {
        while (frente != nullptr) {
            Nodo<T>* siguiente = frente->siguiente;
            delete frente;
            frente = siguiente;
        }
        cola = nullptr;
    }

    void empujarFrente(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = frente;
        if (frente == nullptr) cola = nuevo;   // vacia: nuevo es frente y cola
        frente = nuevo;
    }

    void empujarFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = nullptr;
        if (cola == nullptr) frente = nuevo;
        else                 cola->siguiente = nuevo;
        cola = nuevo;
    }

    bool sacarFrente(T& salida) {
        if (frente == nullptr) return false;
        Nodo<T>* aBorrar = frente;
        salida = aBorrar->dato;
        frente = aBorrar->siguiente;
        delete aBorrar;
        if (frente == nullptr) cola = nullptr;
        return true;
    }

    bool sacarFinal(T& salida) {
        if (cola == nullptr) return false;

        if (cola == frente) {                 // queda un solo nodo
            Nodo<T>* aBorrar = cola;
            salida = aBorrar->dato;
            delete aBorrar;
            frente = cola = nullptr;
            return true;
        }

        Nodo<T>* anterior = frente;
        while (anterior->siguiente != cola) anterior = anterior->siguiente;

        Nodo<T>* aBorrar = cola;
        salida = aBorrar->dato;
        anterior->siguiente = nullptr;
        cola = anterior;
        delete aBorrar;
        return true;
    }

    bool estaVacia() const { return frente == nullptr; }

    int tamano() const {
        int n = 0;
        for (Nodo<T>* a = frente; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    void limpiar() {
        while (frente != nullptr) {
            Nodo<T>* siguiente = frente->siguiente;
            delete frente;
            frente = siguiente;
        }
        cola = nullptr;
    }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        Nodo<T>* a = frente;
        while (a != nullptr) {
            std::cout << a->dato << " ";
            a = a->siguiente;
        }
        if (frente == nullptr) std::cout << "(vacio)";
        std::cout << "\n";
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== PLANTILLA 06: cola y deque ===\n\n";

    // 1) Cola
    std::cout << "1) Cola (sale el primero que entro)\n";
    Cola<int> cola;
    cola.mostrar("   ");
    for (int v : {10, 20, 30, 40, 50}) cola.encolar(v);
    cola.mostrar("   encolar 10, 20, 30, 40, 50 -> ");

    int primero, ultimo, v;
    cola.primero(primero);
    cola.ultimo(ultimo);
    std::cout << "   primero = " << primero << "   ultimo = " << ultimo
              << "   tamano = " << cola.tamano() << "\n";

    cola.desencolar(v);
    std::cout << "   desencolar() devolvio " << v << "\n";
    cola.mostrar("   ");

    std::cout << "\n2) Invertir la cola\n";
    cola.invertir();
    cola.mostrar("   ");
    cola.invertir();
    cola.mostrar("   invertida otra vez: ");

    std::cout << "\n3) Caso limite: vaciarla del todo\n";
    cola.limpiar();
    std::cout << "   estaVacia = " << std::boolalpha << cola.estaVacia()
              << "   desencolar en vacia = " << cola.desencolar(v) << "\n\n";

    // 4) Cola circular
    std::cout << "4) Cola circular con 5 cupos\n";
    ColaCircular<int> circular(5);
    circular.mostrar("   ");
    for (int x : {1, 2, 3}) circular.encolar(x);
    circular.mostrar("   encolar 1, 2, 3:      ");

    circular.desencolar(v);
    circular.desencolar(v);
    std::cout << "   desencolar dos veces: sale " << v << "\n";
    circular.mostrar("   quedan:               ");

    circular.encolar(4);
    circular.encolar(5);
    circular.encolar(6);
    circular.encolar(7);
    circular.mostrar("   encolar 4, 5, 6, 7:   ");
    std::cout << "   encolar(8) con la cola llena = " << circular.encolar(8) << "  (falso: no cabe)\n\n";

    // 5) Cola de prioridad
    std::cout << "5) Cola de prioridad (1 = mas urgente)\n";
    ColaPrioridad prioridad(10);
    prioridad.encolar(crearCliente("Rosa", 4));
    prioridad.encolar(crearCliente("Carlos", 1));
    prioridad.encolar(crearCliente("Miguel", 3));
    prioridad.encolar(crearCliente("Lucia", 1));
    prioridad.mostrar("   orden de llegada: ");

    std::cout << "   orden de atencion:  ";
    Cliente cliente;
    while (prioridad.desencolar(cliente)) std::cout << cliente.nombre << " ";
    std::cout << "   (los de prioridad 1 salen primero)\n\n";

    // 6) Deque
    std::cout << "6) Deque (se opera por los dos extremos)\n";
    Deque<int> dq;
    dq.empujarFinal(2);
    dq.empujarFinal(3);
    dq.empujarFrente(1);
    dq.mostrar("   ");

    dq.sacarFrente(v);
    std::cout << "   sacarFrente() = " << v << " -> ";
    dq.mostrar("");

    dq.sacarFinal(v);
    std::cout << "   sacarFinal()  = " << v << " -> ";
    dq.mostrar("");

    dq.limpiar();
    std::cout << "   vacio = " << std::boolalpha << dq.estaVacia() << "\n";

    std::cout << "\nFin de la plantilla.\n";
    return 0;
}