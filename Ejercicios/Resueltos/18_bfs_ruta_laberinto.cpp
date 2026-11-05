/* ============================================================================
   EJERCICIO 18 - Cola: BFS en un laberinto y reconstruccion de la ruta
   Tema   : Cola (FIFO) y busqueda en anchura
   Archivo: 18_bfs_ruta_laberinto.cpp
   ============================================================================

   ENUNCIADO
   --------
   Dado un laberinto representado por una matriz de caracteres:
       'S' = inicio      'E' = salida
       '.' = libre       '#' = pared

   1) Usando una COLA (BFS), calcular la distancia minima (en pasos) desde 'S'
      hasta 'E'. Si no hay camino, informar que no existe.

   2) Reconstruir el camino minimo con un arreglo de predecesores y mostrarlo.

   3) Comprobar con assert:
      - el laberinto con salida da una distancia conocida
      - un laberinto sin camino devuelve "sin camino"
      - el camino empieza en 'S', termina en 'E' y mide distancia + 1

   Por que una cola: BFS visita por niveles. La PRIMERA vez que se llega a una
   casilla se llega por el camino mas corto, asi que no hace falta probar todos
   los caminos (eso seria fuerza bruta).
   ========================================================================== */

#include <iostream>
#include <cassert>

static const int FILAS = 5;
static const int COLS  = 7;

struct Posicion {
    int fila;
    int columna;
};

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
// Cola (FIFO)
// ---------------------------------------------------------------------------
template <typename T>
class Cola {
private:
    Nodo<T>* frente_;
    Nodo<T>* cola_;
public:
    Cola() : frente_(nullptr), cola_(nullptr) {}
    ~Cola() {
        while (frente_ != nullptr) {
            Nodo<T>* siguiente = frente_->siguiente;
            delete frente_;
            frente_ = siguiente;
        }
        cola_ = nullptr;
    }
    void encolar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (cola_ == nullptr) frente_ = cola_ = nuevo;
        else { cola_->siguiente = nuevo; cola_ = nuevo; }
    }
    bool desencolar(T& salida) {
        if (frente_ == nullptr) return false;
        Nodo<T>* aBorrar = frente_;
        salida = aBorrar->dato;
        frente_ = aBorrar->siguiente;
        delete aBorrar;
        if (frente_ == nullptr) cola_ = nullptr;
        return true;
    }
    bool estaVacia() const { return frente_ == nullptr; }
};

// ---------------------------------------------------------------------------
// Pila (para invertir el camino al reconstruirlo)
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

// ===========================================================================
//  PARTE 1 y 2: BFS + reconstruccion del camino
// ===========================================================================
bool buscarSalida(char mapa[FILAS][COLS], int& distanciaFinal,
                  Posicion camino[], int& largoCamino) {
    int distancia[FILAS][COLS];
    int prevFila[FILAS][COLS];
    int prevColumna[FILAS][COLS];

    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLS; c++) {
            distancia[f][c]    = -1;      // -1 = no visitado
            prevFila[f][c]     = -1;
            prevColumna[f][c]  = -1;
        }
    }

    Posicion inicio = {-1, -1};
    Posicion salida = {-1, -1};
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLS; c++) {
            if (mapa[f][c] == 'S') inicio = {f, c};
            if (mapa[f][c] == 'E') salida = {f, c};
        }
    }
    if (inicio.fila == -1 || salida.fila == -1) return false;

    Cola<Posicion> pendientes;
    pendientes.encolar(inicio);
    distancia[inicio.fila][inicio.columna] = 0;

    const int df[] = {-1, 1, 0, 0};   // arriba, abajo, izquierda, derecha
    const int dc[] = {0, 0, -1, 1};

    bool encontrado = false;
    while (!pendientes.estaVacia() && !encontrado) {
        Posicion actual;
        pendientes.desencolar(actual);

        if (actual.fila == salida.fila && actual.columna == salida.columna) {
            encontrado = true;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nf = actual.fila + df[d];
            int nc = actual.columna + dc[d];

            if (nf < 0 || nf >= FILAS || nc < 0 || nc >= COLS) continue;
            if (mapa[nf][nc] == '#') continue;
            if (distancia[nf][nc] != -1) continue;

            distancia[nf][nc]   = distancia[actual.fila][actual.columna] + 1;
            prevFila[nf][nc]    = actual.fila;
            prevColumna[nf][nc] = actual.columna;
            pendientes.encolar({nf, nc});
        }
    }

    if (!encontrado) return false;

    distanciaFinal = distancia[salida.fila][salida.columna];

    // Reconstruccion: se va de la salida al inicio siguiendo los predecesores
    // y se apila; al desapilar queda en el orden correcto (inicio -> salida).
    Pila<Posicion> inverso;
    int f = salida.fila;
    int c = salida.columna;
    while (!(f == inicio.fila && c == inicio.columna)) {
        inverso.apilar({f, c});
        int pf = prevFila[f][c];
        int pc = prevColumna[f][c];
        if (pf == -1) return false;
        f = pf;
        c = pc;
    }
    inverso.apilar(inicio);

    largoCamino = 0;
    Posicion paso;
    while (inverso.desapilar(paso)) camino[largoCamino++] = paso;

    return true;
}

// ---------------------------------------------------------------------------
void mostrarMapa(char mapa[FILAS][COLS]) {
    for (int f = 0; f < FILAS; f++) {
        std::cout << "   ";
        for (int c = 0; c < COLS; c++) std::cout << mapa[f][c];
        std::cout << "\n";
    }
}

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 18: BFS en un laberinto con cola ===\n\n";

    // --- 1) Laberinto con camino --------------------------------------------
    char mapa[FILAS][COLS] = {
        {'S', '.', '.', '#', '.', '.', '.'},
        {'.', '#', '.', '#', '.', '#', '.'},
        {'.', '#', '.', '.', '.', '#', '.'},
        {'.', '#', '#', '#', '#', '#', '.'},
        {'.', '.', '.', '.', '.', '.', 'E'}
    };

    std::cout << "Laberinto:\n";
    mostrarMapa(mapa);

    int distancia = -1;
    Posicion camino[FILAS * COLS];
    int largo = 0;

    bool hayCamino = buscarSalida(mapa, distancia, camino, largo);
    assert(hayCamino);
    assert(distancia == 10);
    assert(largo == distancia + 1);

    std::cout << "\ndistancia minima = " << distancia << " pasos\n";
    std::cout << "camino (" << largo << " casillas):\n   ";
    for (int i = 0; i < largo; i++) {
        std::cout << "(" << camino[i].fila << "," << camino[i].columna << ")";
        if (i + 1 < largo) std::cout << " -> ";
    }
    std::cout << "\n";

    assert(camino[0].fila == 0 && camino[0].columna == 0);            // empieza en S
    assert(camino[largo - 1].fila == 4 && camino[largo - 1].columna == 6);  // termina en E

    // --- 2) Laberinto sin camino --------------------------------------------
    std::cout << "\nLaberinto sin salida:\n";
    char sinCamino[FILAS][COLS] = {
        {'S', '.', '#', '.', '.', '.', '.'},
        {'.', '#', '.', '.', '.', '.', '.'},
        {'.', '#', '.', '.', '.', '.', '.'},
        {'.', '#', '.', '.', '.', '.', '.'},
        {'.', '#', '.', '.', '.', '.', 'E'}
    };
    mostrarMapa(sinCamino);

    int distancia2 = -1;
    int largo2 = 0;
    bool resultado = buscarSalida(sinCamino, distancia2, camino, largo2);
    assert(resultado == false);
    std::cout << "\nhayCamino = false (la columna de pared bloquea el paso)\n";

    std::cout << "\nTodos los assert pasaron.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 18: BFS en un laberinto con cola ===

    Laberinto:
       S..#...
       .#.#.#.
       .#...#.
       .#####.
       ......E

    distancia minima = 10 pasos
    camino (11 casillas):
       (0,0) -> (1,0) -> (2,0) -> (3,0) -> (4,0) -> (4,1) -> (4,2) -> (4,3) -> (4,4) -> (4,5) -> (4,6)

    Laberinto sin salida:
       S.#....
       .#.....
       .#.....
       .#.....
       .#....E

    hayCamino = false (la columna de pared bloquea el paso)

    Todos los assert pasaron.
*/
