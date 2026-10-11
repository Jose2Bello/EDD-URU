/* ============================================================================
   EJERCICIO 12 - Cola, cola circular y deque
   Tema   : Cola (queue) - FIFO, y deque
   Archivo: 12_cola_ventanilla.cpp
   ============================================================================

   ENUNCIADO
   --------
   PARTE 1 - Cola con dos punteros (front y back)
   Implementar una cola con:
     encolar, desencolar, primero, ultimo, estaVacia, tamano, invertir
   y demostrar que todas las operaciones son O(1) gracias a tener los dos
   extremos guardados.

   PARTE 2 - Cola circular con arreglo de capacidad fija
   Implementar encolar/desencolar usando dos indices que dan la vuelta con
   el modulo, distinguiendo bien el caso "vacia" del caso "llena".

   PARTE 3 - Cola de prioridad (urgencia)
   Los clientes con mayor prioridad se atienden primero.

   PARTE 4 - Simulacion de una ventanilla con menus
   Menus: 1) Llegar a la fila, 2) Ser atendido, 3) Ver la fila, 4) Salir.
   Al salir, se informa quantas personas quedaban delante y cuantas se atendieron.

   PARTE 5 - Deque: cola por los dos extremos
   Implementar un deque minimo con empujarFrente, empujarFinal, sacarFrente,
   sacarFinal e imprimir.

   ========================================================================== */

#include <iostream>
#include <string>
#include <utility>   // std::swap

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
// PARTE 1 - COLA con dos punteros
//
//    frente : sale por aqui
//    cola   : entra por aqui
//
//    Si solo se guardara `frente`, encolar seria O(n) porque habria que
//    recorrer hasta el final. Tener los dos punteros lo hace O(1).
// ---------------------------------------------------------------------------
template <typename T>
class Cola {
private:
    Nodo<T>* frente;
    Nodo<T>* cola;

public:
    Cola() : frente(nullptr), cola(nullptr) {}

    ~Cola() {
        while (frente != nullptr) {
            Nodo<T>* sig = frente->siguiente;
            delete frente;
            frente = sig;
        }
        cola = nullptr;
    }

    // ENCOLAR: el nuevo nodo siempre queda al final                    O(1)
    void encolar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = nullptr;          // sera el ultimo

        if (cola == nullptr) {
            frente = cola = nuevo;           // estaba vacia
        } else {
            cola->siguiente = nuevo;         // el anterior ultimo pasa a apuntar al nuevo
            cola = nuevo;                    // el nuevo es el ultimo
        }
    }

    // DESENCOLAR: sale el primero                                     O(1)
    bool desencolar(T& salida) {
        if (frente == nullptr) return false;

        Nodo<T>* aBorrar = frente;
        salida = frente->dato;
        frente = frente->siguiente;
        delete aBorrar;

        if (frente == nullptr) cola = nullptr;   // se vacio: la cola tambien
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

    // INVERTIR: O(n). Se invierte la cadena y se intercambian los extremos.
    void invertir() {
        Nodo<T>* anterior = nullptr;
        Nodo<T>* actual   = frente;

        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            actual->siguiente = anterior;
            anterior = actual;
            actual   = siguiente;
        }

        std::swap(frente, cola);   // el primero invertido es la nueva cola
    }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (Nodo<T>* a = frente; a != nullptr; a = a->siguiente) {
            std::cout << a->dato << " ";
        }
        if (frente == nullptr) std::cout << "(vacia)";
        std::cout << "\n";
    }
};

// ---------------------------------------------------------------------------
// PARTE 2 - COLA CIRCULAR con arreglo de capacidad fija
//
//    Los indices "dan la vuelta" gracias al modulo: (i + 1) % capacidad.
//    Se necesita un contador `cantidad` para distinguir vacia de llena
//    (en un arreglo circular, en ambos casos frente == final).
// ---------------------------------------------------------------------------
class ColaCircular {
private:
    std::string datos;
    int capacidad;
    int frente;
    int final;      // posicion donde entrara el siguiente elemento
    int cantidad;

public:
    explicit ColaCircular(int cap)
        : capacidad(cap), frente(0), final(0), cantidad(0) {
        datos.resize(cap, ' ');   // se reserva el arreglo
    }

    bool encolar(char valor) {
        if (llena()) return false;
        datos[final] = valor;
        final = (final + 1) % capacidad;   // el indice da la vuelta
        cantidad++;
        return true;
    }

    bool desencolar(char& salida) {
        if (vacia()) return false;
        salida = datos[frente];
        frente = (frente + 1) % capacidad;
        cantidad--;
        return true;
    }

    bool vacia() const { return cantidad == 0; }
    bool llena() const { return cantidad == capacidad; }
    int  tamano() const { return cantidad; }

    // Como el arreglo se recorre de forma circular, hay que empezar en frente
    // y dar la vuelta hasta final.
    std::string contenido() const {
        std::string s = "[ ";
        for (int i = 0; i < cantidad; i++) {
            s += datos[(frente + i) % capacidad];
            s += ' ';
        }
        s += "]";
        return s;
    }
};

// ---------------------------------------------------------------------------
// PARTE 3 - COLA DE PRIORIDAD
//    Cada cliente tiene una prioridad: 1 es la mas urgente.
// ---------------------------------------------------------------------------
struct Cliente {
    std::string nombre;
    int prioridad;

    // Constructor por defecto: necesario para poder crear el arreglo
    // `new Cliente[capacidad]`, que exige constructores sin argumentos.
    Cliente() : nombre(""), prioridad(0) {}

    Cliente(const std::string& n, int p) : nombre(n), prioridad(p) {}
};

class ColaPrioridad {
private:
    Cliente* datos;
    int cantidad;
    int capacidad;

public:
    explicit ColaPrioridad(int cap = 50)
        : cantidad(0), capacidad(cap) {
        datos = new Cliente[capacidad];
    }

    ~ColaPrioridad() { delete[] datos; }   // los arreglos usan delete[]

    // Insercion ordenada por prioridad: se busca la posicion y se corre el bloque.
    void encolar(const Cliente& cliente) {
        if (cantidad >= capacidad) return;

        int i = cantidad - 1;
        while (i >= 0 && datos[i].prioridad > cliente.prioridad) {
            datos[i + 1] = datos[i];   // se corre un lugar a la derecha
            i--;
        }
        datos[i + 1] = cliente;
        cantidad++;
    }

    bool desencolar(Cliente& salida) {
        if (cantidad == 0) return false;
        salida = datos[0];   // la mas urgente esta al frente
        cantidad--;
        // se corre el bloque hacia la izquierda
        for (int i = 0; i < cantidad; i++) datos[i] = datos[i + 1];
        return true;
    }

    bool estaVacia() const { return cantidad == 0; }
    int  tamano() const { return cantidad; }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (int i = 0; i < cantidad; i++) {
            std::cout << datos[i].nombre << "(" << datos[i].prioridad << ") ";
        }
        if (cantidad == 0) std::cout << "(vacia)";
        std::cout << "\n";
    }
};

// ---------------------------------------------------------------------------
// PARTE 5 - DEQUE: cola por los dos extremos
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
            Nodo<T>* sig = frente->siguiente;
            delete frente;
            frente = sig;
        }
        cola = nullptr;
    }

    void empujarFrente(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = frente;

        if (frente == nullptr) cola = nuevo;   // estaba vacia: nuevo = frente y cola

        frente = nuevo;
    }

    void empujarFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = nullptr;
        if (cola == nullptr) {
            frente = nuevo;
        } else {
            cola->siguiente = nuevo;
        }
        cola = nuevo;
    }

    bool sacarFrente(T& salida) {
        if (frente == nullptr) return false;
        Nodo<T>* aBorrar = frente;
        salida = frente->dato;
        frente = frente->siguiente;
        delete aBorrar;
        if (frente == nullptr) cola = nullptr;
        return true;
    }

    bool sacarFinal(T& salida) {
        if (cola == nullptr) return false;

        // caso especial: queda un solo nodo
        if (cola == frente) {
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
        anterior->siguiente = nullptr;   // se corta el enlace
        cola = anterior;
        delete aBorrar;
        return true;
    }

    bool estaVacia() const { return frente == nullptr; }
    int  tamano() const {
        int n = 0;
        for (Nodo<T>* a = frente; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (Nodo<T>* a = frente; a != nullptr; a = a->siguiente) std::cout << a->dato << " ";
        if (frente == nullptr) std::cout << "(vacio)";
        std::cout << "\n";
    }
};

// ---------------------------------------------------------------------------
// PARTE 4 - SIMULACION DE UNA VENTANILLA
// ---------------------------------------------------------------------------
class Ventanilla {
private:
    Cola<std::string> fila;

public:
    void llegar(const std::string& nombre) {
        fila.encolar(nombre);
        std::cout << "  Llega " << nombre
                  << " -> quedan " << fila.tamano() << " persona(s) en la fila\n";
    }

    // Atender: sale el primero (FIFO) y se informa cuantas quedaban delante.
    void atender() {
        std::string nombre;
        if (!fila.desencolar(nombre)) {
            std::cout << "  No hay nadie en la fila\n";
            return;
        }
        std::cout << "  Se atiende a " << nombre
                  << " -> quedaban " << fila.tamano() << " persona(s) delante\n";
    }

    void verFila() const { fila.mostrar("  Fila: "); }
    bool estaVacia() const { return fila.estaVacia(); }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 12: cola, cola circular y deque ===\n\n";

    // --- 1) Cola con dos punteros ---------------------------------------------
    std::cout << "-- 1) Cola con front y back --\n";
    Cola<int> cola;
    std::cout << "  estaVacia al inicio = " << std::boolalpha << cola.estaVacia() << "\n";

    for (int i = 1; i <= 5; i++) cola.encolar(i * 10);
    cola.mostrar("  Tras encolar 10..50: ");

    int primero, ultimo, valor;
    cola.primero(primero);
    cola.ultimo(ultimo);
    std::cout << "  primero = " << primero << "  ultimo = " << ultimo
              << "  tamano = " << cola.tamano() << "\n";

    cola.desencolar(valor);
    std::cout << "  Tras desencolar    : sale el " << valor << "\n";
    cola.mostrar("  Cola ahora:        ");

    cola.desencolar(valor);
    cola.desencolar(valor);
    std::cout << "  Tras 3 desencolares: salen " << valor << " (ultimo), tamano = "
              << cola.tamano() << "\n";

    // Invertir
    cola.invertir();
    cola.mostrar("  Tras invertir:     ");
    cola.invertir();
    cola.mostrar("  Invertir de nuevo: ");
    std::cout << "\n";

    // Caso limite: vaciar del todo y volver a encolar
    while (cola.desencolar(valor)) { /* vaciar */ }
    std::cout << "  Tras vaciar: estaVacia = " << cola.estaVacia()
              << "  desencolar en vacia = " << cola.desencolar(valor) << "\n";
    std::cout << "  Encolar de nuevo    -> ";
    cola.encolar(99);
    cola.mostrar("");

    // Una sola encolada y desencolada debe dejar ambos punteros a nullptr
    cola.desencolar(valor);
    std::cout << "\n";

    // --- 2) Cola circular -----------------------------------------------------
    std::cout << "-- 2) Cola circular con arreglo (capacidad 5) --\n";
    ColaCircular cc(5);
    std::cout << "  Inicial: " << cc.contenido() << "\n";
    for (char c : {'A', 'B', 'C'}) cc.encolar(c);
    std::cout << "  Encolar A, B, C     -> " << cc.contenido() << "\n";
    char sacado;
    cc.desencolar(sacado);
    cc.desencolar(sacado);
    std::cout << "  Desencolar x2 (salen A y B) -> " << cc.contenido() << "\n";
    std::cout << "  Los indices Dieron la vuelta: el frente ahora esta en la posicion "
              << "del final del arreglo\n";
    cc.encolar('D');
    cc.encolar('E');
    cc.encolar('F');
    std::cout << "  Encolar D, E, F    -> " << cc.contenido() << "  (llena = "
              << cc.llena() << ")\n";
    std::cout << "  Encolar G          -> " << cc.encolar('G')
              << "  " << cc.contenido() << "  (llena = " << cc.llena() << ")\n";
    std::cout << "  Encolar H con la cola llena: " << cc.encolar('H') << "  (esperado false)\n\n";

    // --- 3) Cola de prioridad -------------------------------------------------
    std::cout << "-- 3) Cola de prioridad (1 = mas urgente) --\n";
    ColaPrioridad cp;
    cp.encolar(Cliente("Rosa",    4));
    cp.encolar(Cliente("Carlos",  1));
    cp.encolar(Cliente("Miguel",  3));
    cp.encolar(Cliente("Lucia",   1));
    cp.encolar(Cliente("Andres",  5));
    cp.mostrar("  Orden de llegada: ");

    std::cout << "  Orden de atencion: ";
    Cliente cliente;
    while (cp.desencolar(cliente)) {
        std::cout << cliente.nombre << " ";
    }
    std::cout << "\n  (Carlos y Lucia tenian prioridad 1 y salen primero)\n\n";

    // --- 4) Simulacion de la ventanilla --------------------------------------
    std::cout << "-- 4) Simulacion de la ventanilla --\n";
    Ventanilla v;
    v.llegar("Ana");
    v.llegar("Luis");
    v.llegar("Sofia");
    v.llegar("Diego");
    v.verFila();
    std::cout << "\n  Atendiendo a todos:\n";
    v.atender();
    v.atender();
    v.atender();
    v.atender();
    v.atender();   // ya no queda nadie
    std::cout << "\n";

    // --- 5) Deque -------------------------------------------------------------
    std::cout << "-- 5) Deque (cola por los dos extremos) --\n";
    Deque<int> dq;
    dq.empujarFinal(2);
    dq.empujarFinal(3);
    dq.empujarFrente(1);
    dq.mostrar("  Tras empujar 1, 2, 3: ");

    dq.sacarFrente(valor);
    std::cout << "  Sacar por el frente: " << valor << " -> ";
    dq.mostrar("");

    dq.sacarFinal(valor);
    std::cout << "  Sacar por el final:  " << valor << " -> ";
    dq.mostrar("");

    // Caso limite: vaciar el deque por los dos lados
    while (dq.sacarFrente(valor))  { /* vaciar por el frente */ }
    while (dq.sacarFinal(valor))   { /* vaciar por el final  */ }
    std::cout << "  Vacio = " << std::boolalpha << dq.estaVacia()
              << "  sacarFrente en vacio = " << dq.sacarFrente(valor) << "\n";

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 12: cola, cola circular y deque ===
    -- 1) Cola con front y back --
      estaVacia al inicio = true
      Tras encolar 10..50: 10 20 30 40 50
      primero = 10  ultimo = 50  tamano = 5
      Tras desencolar    : sale el 10
      Cola ahora:        20 30 40 50
      Tras 3 desencolares: salen 30 (ultimo), tamano = 2
      Tras invertir:     50 40
      Invertir de nuevo: 40 50
      Tras vaciar: estaVacia = true  desencolar en vacia = false
      Encolar de nuevo    -> 99
    -- 2) Cola circular con arreglo (capacidad 5) --
      Inicial: [ ]
      Encolar A, B, C     -> [ A B C ]
      Desencolar x2 (salen A y B) -> [ C ]
      Los indices Dieron la vuelta: el frente ahora esta en la posicion del final del arreglo
      Encolar D, E, F    -> [ C D E F ]  (llena = false)
      Encolar G          -> true  [ C D E F G ]  (llena = true)
      Encolar H con la cola llena: false  (esperado false)
    -- 3) Cola de prioridad (1 = mas urgente) --
      Orden de llegada: Carlos(1) Lucia(1) Miguel(3) Rosa(4) Andres(5)
      Orden de atencion: Carlos Lucia Miguel Rosa Andres
      (Carlos y Lucia tenian prioridad 1 y salen primero)
    -- 4) Simulacion de la ventanilla --
      Llega Ana -> quedan 1 persona(s) en la fila
      Llega Luis -> quedan 2 persona(s) en la fila
      Llega Sofia -> quedan 3 persona(s) en la fila
      Llega Diego -> quedan 4 persona(s) en la fila
      Fila: Ana Luis Sofia Diego
      Atendiendo a todos:
      Se atiende a Ana -> quedaban 3 persona(s) delante
      Se atiende a Luis -> quedaban 2 persona(s) delante
      Se atiende a Sofia -> quedaban 1 persona(s) delante
      Se atiende a Diego -> quedaban 0 persona(s) delante
      No hay nadie en la fila
    -- 5) Deque (cola por los dos extremos) --
      Tras empujar 1, 2, 3: 1 2 3
      Sacar por el frente: 1 -> 2 3
      Sacar por el final:  3 -> 2
      Vacio = true  sacarFrente en vacio = false
    Demostracion completada.
*/
