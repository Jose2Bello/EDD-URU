/* ============================================================================
   PLANTILLA 10 - Cola de prioridad (y pila de prioridad) - version ligera
   Archivo : 10_cola_prioridad.cpp

   Librerias: SOLO <iostream>.

   En una cola normal sale el MAS VIEJO (FIFO). En una cola de prioridad sale
   el MAS URGENTE, sin importar cuando llego:

        llegan:  Rosa(4)  Carlos(1)  Miguel(3)  Lucia(1)  Andres(5)
        salen :  Carlos   Lucia      Miguel     Rosa      Andres

   Convenio usado aqui: prioridad 1 = MAS urgente (sale primero).

   Se incluyen tres variantes:
     1) ColaPrioridad<T>      arreglo con insercion ordenada, FIFO entre iguales
     2) PilaPrioridad<T>      igual, pero LIFO entre prioridades iguales
     3) ColaPrioridadHeap<T>  monticulo binario (heap), O(log n) por operacion

   La 1 y la 2 son las que se piden a mano en un examen. La 3 se incluye para
   que sepas que existe y por que es mas rapida.
   ========================================================================== */

#include <iostream>

// ---------------------------------------------------------------------------
// Una entrada guarda el dato, su prioridad y el orden de llegada.
// El "orden" es lo que hace que, entre dos iguales, gane el que llego antes.
// ---------------------------------------------------------------------------
template <typename T>
struct Entrada {
    T dato;
    int prioridad;
    long orden;

    Entrada() : dato(), prioridad(0), orden(0) {}
    Entrada(T d, int p, long o) : dato(d), prioridad(p), orden(o) {}
};

// ---------------------------------------------------------------------------
// COLA DE PRIORIDAD con insercion ordenada.
//   encolar   : O(n)   (se corre el bloque)
//   desencolar: O(n)   (se corre el bloque)
//   frente    : O(1)
// ---------------------------------------------------------------------------
template <typename T>
class ColaPrioridad {
private:
    Entrada<T>* datos;
    int cantidad;
    int capacidad;
    long contador;

    // ¿a sale antes que b?  Menor prioridad primero; empate -> llego antes.
    bool saleAntes(const Entrada<T>& a, const Entrada<T>& b) const {
        if (a.prioridad != b.prioridad) return a.prioridad < b.prioridad;
        return a.orden < b.orden;          // FIFO entre iguales
    }

public:
    explicit ColaPrioridad(int capacidadInicial = 50)
        : cantidad(0), capacidad(capacidadInicial), contador(0) {
        datos = new Entrada<T>[capacidad];
    }

    ~ColaPrioridad() { delete[] datos; }

    void encolar(T dato, int prioridad) {   // O(n)
        if (cantidad >= capacidad) return;

        Entrada<T> nueva(dato, prioridad, contador++);
        int i = cantidad - 1;
        while (i >= 0 && saleAntes(nueva, datos[i])) {
            datos[i + 1] = datos[i];
            i--;
        }
        datos[i + 1] = nueva;
        cantidad++;
    }

    bool desencolar(T& salida) {            // O(n)
        if (cantidad == 0) return false;
        salida = datos[0].dato;
        cantidad--;
        for (int i = 0; i < cantidad; i++) datos[i] = datos[i + 1];
        return true;
    }

    bool frente(T& salida, int& prioridad) const {
        if (cantidad == 0) return false;
        salida = datos[0].dato;
        prioridad = datos[0].prioridad;
        return true;
    }

    bool estaVacia() const { return cantidad == 0; }
    int  tamano() const { return cantidad; }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (int i = 0; i < cantidad; i++)
            std::cout << datos[i].dato << "(p" << datos[i].prioridad << ") ";
        if (cantidad == 0) std::cout << "(vacia)";
        std::cout << "     <- sale el de la izquierda\n";
    }
};

// ---------------------------------------------------------------------------
// PILA DE PRIORIDAD: sale el mas urgente; si hay empate, el ULTIMO que llego
// (LIFO). Misma idea, solo cambia el desempate.
// ---------------------------------------------------------------------------
template <typename T>
class PilaPrioridad {
private:
    Entrada<T>* datos;
    int cantidad;
    int capacidad;
    long contador;

    bool saleAntes(const Entrada<T>& a, const Entrada<T>& b) const {
        if (a.prioridad != b.prioridad) return a.prioridad < b.prioridad;
        return a.orden > b.orden;          // LIFO entre iguales
    }

public:
    explicit PilaPrioridad(int capacidadInicial = 50)
        : cantidad(0), capacidad(capacidadInicial), contador(0) {
        datos = new Entrada<T>[capacidad];
    }

    ~PilaPrioridad() { delete[] datos; }

    void apilar(T dato, int prioridad) {
        if (cantidad >= capacidad) return;

        Entrada<T> nueva(dato, prioridad, contador++);
        int i = cantidad - 1;
        while (i >= 0 && saleAntes(nueva, datos[i])) {
            datos[i + 1] = datos[i];
            i--;
        }
        datos[i + 1] = nueva;
        cantidad++;
    }

    bool desapilar(T& salida) {
        if (cantidad == 0) return false;
        salida = datos[0].dato;
        cantidad--;
        for (int i = 0; i < cantidad; i++) datos[i] = datos[i + 1];
        return true;
    }

    bool estaVacia() const { return cantidad == 0; }
    int  tamano() const { return cantidad; }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (int i = 0; i < cantidad; i++)
            std::cout << datos[i].dato << "(p" << datos[i].prioridad << ") ";
        if (cantidad == 0) std::cout << "(vacia)";
        std::cout << "\n";
    }
};

// ---------------------------------------------------------------------------
// COLA DE PRIORIDAD con MONTICULO BINARIO (heap).
//   encolar   : O(log n)
//   desencolar: O(log n)
// La raiz (indice 0) siempre es el que sale antes.
// ---------------------------------------------------------------------------
template <typename T>
class ColaPrioridadHeap {
private:
    Entrada<T>* datos;
    int cantidad;
    int capacidad;
    long contador;

    bool saleAntes(const Entrada<T>& a, const Entrada<T>& b) const {
        if (a.prioridad != b.prioridad) return a.prioridad < b.prioridad;
        return a.orden < b.orden;
    }

    // Sube el nodo hasta que su padre sea mas urgente que el.
    void subir(int i) {
        while (i > 0) {
            int padre = (i - 1) / 2;
            if (!saleAntes(datos[i], datos[padre])) break;
            Entrada<T> temporal = datos[i];
            datos[i] = datos[padre];
            datos[padre] = temporal;
            i = padre;
        }
    }

    // Baja el nodo eligiendo el hijo mas urgente.
    void bajar(int i) {
        while (true) {
            int izquierdo = 2 * i + 1;
            int derecho   = 2 * i + 2;
            int menor = i;

            if (izquierdo < cantidad && saleAntes(datos[izquierdo], datos[menor]))
                menor = izquierdo;
            if (derecho < cantidad && saleAntes(datos[derecho], datos[menor]))
                menor = derecho;
            if (menor == i) break;

            Entrada<T> temporal = datos[i];
            datos[i] = datos[menor];
            datos[menor] = temporal;
            i = menor;
        }
    }

public:
    explicit ColaPrioridadHeap(int capacidadInicial = 50)
        : cantidad(0), capacidad(capacidadInicial), contador(0) {
        datos = new Entrada<T>[capacidad];
    }

    ~ColaPrioridadHeap() { delete[] datos; }

    void encolar(T dato, int prioridad) {   // O(log n)
        if (cantidad >= capacidad) return;
        datos[cantidad] = Entrada<T>(dato, prioridad, contador++);
        subir(cantidad);
        cantidad++;
    }

    bool desencolar(T& salida) {            // O(log n)
        if (cantidad == 0) return false;
        salida = datos[0].dato;
        datos[0] = datos[cantidad - 1];
        cantidad--;
        if (cantidad > 0) bajar(0);
        return true;
    }

    bool estaVacia() const { return cantidad == 0; }
    int  tamano() const { return cantidad; }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (int i = 0; i < cantidad; i++)
            std::cout << datos[i].dato << "(p" << datos[i].prioridad << ") ";
        if (cantidad == 0) std::cout << "(vacia)";
        std::cout << "     (orden interno del heap)\n";
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== PLANTILLA 10: cola y pila de prioridad ===\n\n";

    // --- 1) Cola de prioridad: FIFO entre prioridades iguales ----------------
    std::cout << "1) ColaPrioridad (1 = mas urgente, empate = llega antes)\n";
    ColaPrioridad<const char*> clinica;
    clinica.encolar("Rosa",    4);
    clinica.encolar("Carlos",  1);
    clinica.encolar("Miguel",  3);
    clinica.encolar("Lucia",   1);
    clinica.encolar("Andres",  5);
    clinica.mostrar("   llegada: ");

    std::cout << "   atencion: ";
    const char* nombre = "";
    while (clinica.desencolar(nombre)) std::cout << nombre << " ";
    std::cout << "\n   (Carlos salio antes que Lucia: llegaron con la misma prioridad)\n\n";

    // --- 2) Pila de prioridad: LIFO entre prioridades iguales -----------------
    std::cout << "2) PilaPrioridad (empate = sale el ultimo que llego)\n";
    PilaPrioridad<int> pila;
    pila.apilar(10, 2);
    pila.apilar(20, 1);
    pila.apilar(30, 2);
    pila.apilar(40, 1);
    pila.mostrar("   apilado:  ");
    std::cout << "   salida:   ";
    int valor;
    while (pila.desapilar(valor)) std::cout << valor << " ";
    std::cout << "\n   (40 antes que 20 por ser mas reciente; 30 antes que 10)\n\n";

    // --- 3) Monticulo (heap) --------------------------------------------------
    std::cout << "3) ColaPrioridadHeap (O(log n) por operacion)\n";
    ColaPrioridadHeap<const char*> heap(20);
    heap.encolar("tareaC", 3);
    heap.encolar("tareaA", 1);
    heap.encolar("tareaD", 5);
    heap.encolar("tareaB", 1);
    heap.mostrar("   heap interno: ");
    std::cout << "   salida:       ";
    while (heap.desencolar(nombre)) std::cout << nombre << " ";
    std::cout << "  (mismo orden de atencion que la cola)\n\n";

    // --- 4) Cual elegir -------------------------------------------------------
    std::cout << "4) Resumen\n";
    std::cout << "   ColaPrioridad / PilaPrioridad : simple, O(n), ideal para el examen\n";
    std::cout << "   ColaPrioridadHeap             : O(log n), ideal si hay muchos datos\n";

    std::cout << "\nFin de la plantilla.\n";
    return 0;
}
