/* ============================================================================
   PLANTILLA 09 - Cola (queue, FIFO), cola circular y deque - completa
   Archivo : 09_cola_completa.cpp

   Librerias: SOLO <iostream>. Nada de queue, deque, vector, string ni assert.

   La PLANTILLA 06 trae lo basico. Esta 09 agrega lo que suele caer en el examen:

       Cola<T>        : operaciones basicas + copia profunda, buscar, contar,
                        rotar, invertir, limpiar, mostrar
       ColaCircular<T>: arreglo fijo generico con indices que dan la vuelta
       Deque<T>       : empujar y sacar por LOS DOS extremos
       Aplicaciones   : BFS en un laberinto, palindromo con deque y
                        cola implementada con DOS PILAS

   Idea central de la cola: entra por la cola (back) y sale por el frente
   (front). Por eso se guardan LOS DOS extremos y todo es O(1):

       frente                                  cola
          |                                      |
          v                                      v
        [10] -> [20] -> [30] -> [40] -> [50] -> nullptr
       sale                                   entra
   ========================================================================== */

#include <iostream>

static const int FILAS = 5;
static const int COLS  = 7;

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
// Pila minima, solo para la aplicacion "cola con dos pilas"
// ---------------------------------------------------------------------------
template <typename T>
class Pila {
private:
    Nodo<T>* top;

public:
    Pila() : top(nullptr) {}

    ~Pila() {
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

    bool estaVacia() const { return top == nullptr; }
};

// ---------------------------------------------------------------------------
// Cola completa
// ---------------------------------------------------------------------------
template <typename T>
class Cola {
private:
    Nodo<T>* frente;   // sale por aqui
    Nodo<T>* cola;     // entra por aqui

    // Copia recursiva en orden: frente, luego el resto. encolar() va al final.
    void copiarDesde(Nodo<T>* nodo) {
        if (nodo == nullptr) return;
        encolar(nodo->dato);
        copiarDesde(nodo->siguiente);
    }

public:
    Cola() : frente(nullptr), cola(nullptr) {}

    // Copia PROFUNDA: cada cola tiene sus propios nodos.
    Cola(const Cola& otra) : frente(nullptr), cola(nullptr) {
        copiarDesde(otra.frente);
    }

    Cola& operator=(const Cola& otra) {
        if (this != &otra) {
            limpiar();
            copiarDesde(otra.frente);
        }
        return *this;
    }

    ~Cola() { limpiar(); }

    // ===================== OPERACIONES BASICAS ============================

    void encolar(T valor) {                  // O(1)
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = nullptr;          // sera el ultimo

        if (cola == nullptr) {
            frente = cola = nuevo;           // estaba vacia
        } else {
            cola->siguiente = nuevo;         // el viejo ultimo apunta al nuevo
            cola = nuevo;
        }
    }

    bool desencolar(T& salida) {             // O(1)
        if (frente == nullptr) return false;

        Nodo<T>* aBorrar = frente;
        salida = aBorrar->dato;
        frente = aBorrar->siguiente;
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

    int contar() const { return tamano(); }

    void limpiar() {
        while (frente != nullptr) {
            Nodo<T>* siguiente = frente->siguiente;
            delete frente;
            frente = siguiente;
        }
        cola = nullptr;
    }

    // ===================== CONSULTAR ======================================

    bool busca(T valor) const {
        for (Nodo<T>* a = frente; a != nullptr; a = a->siguiente)
            if (a->dato == valor) return true;
        return false;
    }

    bool contiene(T valor) const { return busca(valor); }

    // ===================== TRANSFORMAR ===================================

    // Invierte la cola: el primero pasa a ser el ultimo. O(n).
    void invertir() {
        Nodo<T>* anterior = nullptr;
        Nodo<T>* actual = frente;

        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            actual->siguiente = anterior;
            anterior = actual;
            actual = siguiente;
        }

        Nodo<T>* temporal = frente;
        frente = cola;
        cola = temporal;
    }

    // Rotar a la izquierda: el frente pasa al final, k veces.
    //   [1 2 3 4 5] rotar(2) -> [3 4 5 1 2]
    void rotar(int k) {
        int n = tamano();
        if (n == 0) return;
        k %= n;
        for (int i = 0; i < k; i++) {
            T valor = T();
            desencolar(valor);
            encolar(valor);
        }
    }

    // ===================== MOSTRAR ========================================

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (Nodo<T>* a = frente; a != nullptr; a = a->siguiente)
            std::cout << a->dato << " ";
        if (frente == nullptr) std::cout << "(vacia)";
        std::cout << "     <- el de la izquierda sale primero\n";
    }
};

// ---------------------------------------------------------------------------
// Cola circular con arreglo de capacidad fija.
// Los indices dan la vuelta con el modulo (i + 1) % capacidad.
// El contador `cantidad` distingue "vacia" de "llena".
// ---------------------------------------------------------------------------
template <typename T>
class ColaCircular {
private:
    T* datos;
    int capacidad;
    int frente;
    int final;       // posicion donde entrara el siguiente
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

    bool primero(T& salida) const {
        if (cantidad == 0) return false;
        salida = datos[frente];
        return true;
    }

    bool ultimo(T& salida) const {
        if (cantidad == 0) return false;
        int pos = (final - 1 + capacidad) % capacidad;
        salida = datos[pos];
        return true;
    }

    bool vacia()  const { return cantidad == 0; }
    bool llena()  const { return cantidad == capacidad; }
    int  tamano() const { return cantidad; }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (int i = 0; i < cantidad; i++)
            std::cout << datos[(frente + i) % capacidad] << " ";
        if (cantidad == 0) std::cout << "(vacia)";
        std::cout << "     (frente = " << frente << ", final = " << final << ")\n";
    }
};

// ---------------------------------------------------------------------------
// Deque: se empuja y se saca por los DOS extremos
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
        if (frente == nullptr) cola = nuevo;   // estaba vacio
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

        if (cola == frente) {                  // queda un solo nodo
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

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (Nodo<T>* a = frente; a != nullptr; a = a->siguiente)
            std::cout << a->dato << " ";
        if (frente == nullptr) std::cout << "(vacio)";
        std::cout << "\n";
    }
};

// ===========================================================================
//  APLICACION 1: BFS en un laberinto (ruta mas corta)
//  'S' inicio, 'E' salida, '#' pared, '.' libre.
//  La cola garantiza visitar por niveles: la primera vez que se llega a una
//  casilla, se llego por el camino mas corto.
// ===========================================================================
struct Posicion {
    int fila;
    int columna;
};

int distanciaMinima(char mapa[FILAS][COLS]) {
    int distancia[FILAS][COLS];
    for (int f = 0; f < FILAS; f++)
        for (int c = 0; c < COLS; c++)
            distancia[f][c] = -1;

    Posicion inicio = {-1, -1};
    Posicion salida = {-1, -1};
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLS; c++) {
            if (mapa[f][c] == 'S') inicio = {f, c};
            if (mapa[f][c] == 'E') salida = {f, c};
        }
    }
    if (inicio.fila == -1 || salida.fila == -1) return -1;

    Cola<Posicion> pendientes;
    pendientes.encolar(inicio);
    distancia[inicio.fila][inicio.columna] = 0;

    const int df[] = {-1, 1, 0, 0};   // arriba, abajo, izquierda, derecha
    const int dc[] = {0, 0, -1, 1};

    while (!pendientes.estaVacia()) {
        Posicion actual;
        pendientes.desencolar(actual);

        if (actual.fila == salida.fila && actual.columna == salida.columna)
            return distancia[actual.fila][actual.columna];

        for (int d = 0; d < 4; d++) {
            int nf = actual.fila + df[d];
            int nc = actual.columna + dc[d];

            if (nf < 0 || nf >= FILAS || nc < 0 || nc >= COLS) continue;
            if (mapa[nf][nc] == '#') continue;
            if (distancia[nf][nc] != -1) continue;   // ya visitada

            distancia[nf][nc] = distancia[actual.fila][actual.columna] + 1;
            pendientes.encolar({nf, nc});
        }
    }

    return -1;   // no hay camino
}

// ===========================================================================
//  APLICACION 2: palindromo con un deque
//  Se compara el frente con el final, sacando por los dos lados.
// ===========================================================================
bool esPalindromoDeque(const char* cadena) {
    Deque<char> deque;
    for (int i = 0; cadena[i] != '\0'; i++) deque.empujarFinal(cadena[i]);

    char primero = '\0';
    char ultimo = '\0';
    while (deque.tamano() > 1) {
        deque.sacarFrente(primero);
        deque.sacarFinal(ultimo);
        if (primero != ultimo) return false;
    }
    return true;
}

// ===========================================================================
//  APLICACION 3: cola implementada con DOS PILAS
//  Se apila en `entrada`; para sacar, si `salida` esta vacia se pasa todo
//  de entrada a salida (eso invierte el orden y deja el mas viejo arriba).
//  Coste amortizado O(1) por operacion.
// ===========================================================================
template <typename T>
class ColaConDosPilas {
private:
    Pila<T> entrada;
    Pila<T> salida;

    void pasarDeEntradaASalida() {
        T valor;
        while (entrada.desapilar(valor)) salida.apilar(valor);
    }

public:
    void encolar(T valor) {
        entrada.apilar(valor);
    }

    bool desencolar(T& salidaValor) {
        if (salida.estaVacia()) pasarDeEntradaASalida();
        return salida.desapilar(salidaValor);
    }

    bool estaVacia() const {
        return entrada.estaVacia() && salida.estaVacia();
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== PLANTILLA 09: cola completa ===\n\n";

    // --- 1) Operaciones basicas ---------------------------------------------
    std::cout << "1) Cola basica\n";
    Cola<int> cola;
    cola.mostrar("   ");
    for (int v : {10, 20, 30, 40, 50}) cola.encolar(v);
    cola.mostrar("   encolar 10..50 -> ");

    int primero = 0, ultimo = 0, valor = 0;
    cola.primero(primero);
    cola.ultimo(ultimo);
    std::cout << "   primero = " << primero << "   ultimo = " << ultimo
              << "   tamano = " << cola.tamano() << "\n";
    std::cout << "   contiene(30) = " << std::boolalpha << cola.contiene(30)
              << "   contiene(99) = " << cola.contiene(99)
              << "   estaVacia = " << cola.estaVacia() << "\n\n";

    // --- 2) Copia profunda ---------------------------------------------------
    std::cout << "2) Copia profunda\n";
    Cola<int> copia = cola;
    copia.encolar(99);
    std::cout << "   original: "; cola.mostrar("");
    std::cout << "   copia   : "; copia.mostrar("");
    std::cout << "   (encolar en la copia NO toca la original)\n\n";

    // --- 3) Invertir y rotar -------------------------------------------------
    std::cout << "3) Invertir y rotar\n";
    Cola<int> numeros;
    for (int v : {1, 2, 3, 4, 5}) numeros.encolar(v);
    std::cout << "   antes:        "; numeros.mostrar("");
    numeros.invertir();
    std::cout << "   invertir():   "; numeros.mostrar("");
    numeros.invertir();
    numeros.rotar(2);
    std::cout << "   rotar(2):     "; numeros.mostrar("");
    numeros.rotar(7);   // 7 % 5 = 2
    std::cout << "   rotar(7)=2:   "; numeros.mostrar("");
    std::cout << "\n";

    // --- 4) Cola circular ----------------------------------------------------
    std::cout << "4) Cola circular (capacidad 4)\n";
    ColaCircular<int> circular(4);
    for (int v : {1, 2, 3}) circular.encolar(v);
    circular.mostrar("   encolar 1, 2, 3:    ");
    circular.desencolar(valor);
    circular.desencolar(valor);
    std::cout << "   desencolar x2 (salen 1 y 2)\n";
    circular.mostrar("   quedan:             ");
    for (int v : {4, 5, 6}) circular.encolar(v);
    circular.mostrar("   encolar 4, 5, 6:    ");
    std::cout << "   encolar(7) en cola llena = " << circular.encolar(7) << "\n\n";

    // --- 5) Deque ------------------------------------------------------------
    std::cout << "5) Deque (por los dos extremos)\n";
    Deque<int> deque;
    deque.empujarFinal(2);
    deque.empujarFinal(3);
    deque.empujarFrente(1);
    deque.empujarFinal(4);
    deque.mostrar("   deque: ");
    deque.sacarFrente(valor);
    std::cout << "   sacarFrente() = " << valor << " -> "; deque.mostrar("");
    deque.sacarFinal(valor);
    std::cout << "   sacarFinal()  = " << valor << " -> "; deque.mostrar("");
    std::cout << "\n";

    // --- 6) BFS en un laberinto ---------------------------------------------
    std::cout << "6) BFS: ruta mas corta en un laberinto\n";
    char mapa[FILAS][COLS] = {
        {'S', '.', '.', '#', '.', '.', '.'},
        {'.', '#', '.', '#', '.', '#', '.'},
        {'.', '#', '.', '.', '.', '#', '.'},
        {'.', '#', '#', '#', '#', '#', '.'},
        {'.', '.', '.', '.', '.', '.', 'E'}
    };
    for (int f = 0; f < FILAS; f++) {
        std::cout << "   ";
        for (int c = 0; c < COLS; c++) std::cout << mapa[f][c];
        std::cout << "\n";
    }
    int distancia = distanciaMinima(mapa);
    std::cout << "   distanciaMinima(S, E) = " << distancia << " pasos\n\n";

    // --- 7) Palindromo con deque --------------------------------------------
    std::cout << "7) Palindromo con deque\n";
    const char* pruebas[] = {"anilina", "reconocer", "abb", "casa", "radar"};
    for (int i = 0; i < 5; i++)
        std::cout << "   esPalindromoDeque(\"" << pruebas[i] << "\") = "
                  << std::boolalpha << esPalindromoDeque(pruebas[i]) << "\n";

    // --- 8) Cola con dos pilas ----------------------------------------------
    std::cout << "\n8) Cola con dos pilas\n";
    ColaConDosPilas<int> colaPilas;
    for (int v : {10, 20, 30}) colaPilas.encolar(v);
    std::cout << "   encolar 10, 20, 30 -> desencolando: ";
    while (!colaPilas.estaVacia()) {
        colaPilas.desencolar(valor);
        std::cout << valor << " ";
    }
    std::cout << "(salen en orden FIFO)\n";

    std::cout << "\nFin de la plantilla.\n";
    return 0;
}
