/* ============================================================================
   EJERCICIO 16 - Cola con dos pilas y pila con dos colas
   Tema   : convertir una estructura LIFO en FIFO y viceversa
   Archivo: 16_cola_con_dos_pilas.cpp
   ============================================================================

   ENUNCIADO
   --------
   1) Implementar una COLA usando SOLO dos pilas.
      - encolar(x) es O(1)
      - desencolar() es O(1) amortizado
      La cola debe respetar el orden FIFO: sale el primero que entro.

   2) Implementar una PILA usando SOLO dos colas.
      - apilar(x) es O(1)
      - desapilar() es O(n)
      La pila debe respetar el orden LIFO: sale el ultimo que entro.

   3) Comprobar con assert:
      - vaciar del todo y que luego desencolar / desapilar devuelva false
      - operaciones intercaladas (encolar, desencolar, encolar...)

   Idea clave de la cola con dos pilas:
      `entrada` recibe lo nuevo. Cuando hace falta sacar, se vuelca `entrada`
      en `salida`: al invertirse dos veces el orden, el mas viejo queda arriba.
   ========================================================================== */

#include <iostream>
#include <cassert>

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
// Pila (LIFO)
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
    int  tamano() const {
        int n = 0;
        for (Nodo<T>* a = top; a != nullptr; a = a->siguiente) n++;
        return n;
    }
};

// ---------------------------------------------------------------------------
// Cola (FIFO)
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
            Nodo<T>* siguiente = frente->siguiente;
            delete frente;
            frente = siguiente;
        }
        cola = nullptr;
    }
    void encolar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (cola == nullptr) {
            frente = cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            cola = nuevo;
        }
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
    bool estaVacia() const { return frente == nullptr; }
    int  tamano() const {
        int n = 0;
        for (Nodo<T>* a = frente; a != nullptr; a = a->siguiente) n++;
        return n;
    }
};

// ===========================================================================
//  PARTE 1: Cola con dos pilas
// ===========================================================================
template <typename T>
class ColaConDosPilas {
private:
    Pila<T> entrada;
    Pila<T> salida;

    // Vuelca entrada -> salida. Al invertirse el orden, el mas viejo queda
    // arriba de `salida` y por eso sale primero.
    void volcar() {
        T valor = T();
        while (entrada.desapilar(valor)) salida.apilar(valor);
    }

public:
    void encolar(T valor) {                // O(1)
        entrada.apilar(valor);
    }

    bool desencolar(T& salidaValor) {      // O(1) amortizado
        if (salida.estaVacia()) volcar();  // solo cuando hace falta
        return salida.desapilar(salidaValor);
    }

    bool estaVacia() const {
        return entrada.estaVacia() && salida.estaVacia();
    }

    int tamano() const {
        return entrada.tamano() + salida.tamano();
    }
};

// ===========================================================================
//  PARTE 2: Pila con dos colas
// ===========================================================================
template <typename T>
class PilaConDosColas {
private:
    Cola<T> principal;   // donde viven los elementos
    Cola<T> auxiliar;    // aparcadero temporal

public:
    void apilar(T valor) {                 // O(1)
        principal.encolar(valor);
    }

    bool desapilar(T& salida) {            // O(n)
        if (principal.estaVacia()) return false;

        // Se pasan todos MENOS el ultimo a la cola auxiliar.
        T valor = T();
        while (principal.tamano() > 1) {
            principal.desencolar(valor);
            auxiliar.encolar(valor);
        }

        bool ok = principal.desencolar(salida);   // este es el ultimo que entro

        // Se devuelven los demas a la cola principal.
        while (auxiliar.desencolar(valor)) principal.encolar(valor);
        return ok;
    }

    bool estaVacia() const { return principal.estaVacia(); }
    int  tamano() const { return principal.tamano(); }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 16: cola con dos pilas, pila con dos colas ===\n\n";

    // --- 1) Cola con dos pilas: debe salir en orden FIFO ---------------------
    std::cout << "1) ColaConDosPilas -> FIFO\n";
    ColaConDosPilas<int> cola;
    assert(cola.estaVacia());

    for (int v = 1; v <= 5; v++) cola.encolar(v);
    assert(cola.tamano() == 5);

    std::cout << "   encolado 1, 2, 3, 4, 5 -> desencolando: ";
    int valor = 0;
    int esperado = 1;
    while (cola.desencolar(valor)) {
        std::cout << valor << " ";
        assert(valor == esperado);         // 1, 2, 3, 4, 5
        esperado++;
    }
    std::cout << "(FIFO correcto)\n";
    assert(cola.estaVacia());
    assert(cola.desencolar(valor) == false);
    std::cout << "   desencolar en vacia -> false\n\n";

    // --- 2) Operaciones intercaladas ----------------------------------------
    std::cout << "2) Operaciones intercaladas\n";
    ColaConDosPilas<int> mixta;
    mixta.encolar(10);
    mixta.encolar(20);
    mixta.desencolar(valor);
    assert(valor == 10);
    std::cout << "   encolar 10, 20; desencolar -> " << valor << "\n";

    mixta.encolar(30);
    mixta.desencolar(valor);
    assert(valor == 20);
    std::cout << "   encolar 30; desencolar -> " << valor << "\n";

    mixta.desencolar(valor);
    assert(valor == 30);
    std::cout << "   desencolar -> " << valor << "\n";
    assert(mixta.estaVacia());
    std::cout << "   (el volcado solo se hace cuando salida esta vacia)\n\n";

    // --- 3) Pila con dos colas: debe salir en orden LIFO ---------------------
    std::cout << "3) PilaConDosColas -> LIFO\n";
    PilaConDosColas<int> pila;
    assert(pila.estaVacia());

    for (int v = 1; v <= 5; v++) pila.apilar(v);
    assert(pila.tamano() == 5);

    std::cout << "   apilado 1, 2, 3, 4, 5 -> desapilando: ";
    esperado = 5;
    while (pila.desapilar(valor)) {
        std::cout << valor << " ";
        assert(valor == esperado);         // 5, 4, 3, 2, 1
        esperado--;
    }
    std::cout << "(LIFO correcto)\n";
    assert(pila.estaVacia());
    assert(pila.desapilar(valor) == false);
    std::cout << "   desapilar en vacia -> false\n\n";

    // --- 4) Complejidades ----------------------------------------------------
    std::cout << "4) Complejidades\n";
    std::cout << "   ColaConDosPilas: encolar O(1), desencolar O(1) amortizado\n";
    std::cout << "   PilaConDosColas: apilar  O(1), desapilar  O(n)\n";

    std::cout << "\nTodos los assert pasaron.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 16: cola con dos pilas, pila con dos colas ===

    1) ColaConDosPilas -> FIFO
       encolado 1, 2, 3, 4, 5 -> desencolando: 1 2 3 4 5 (FIFO correcto)
       desencolar en vacia -> false

    2) Operaciones intercaladas
       encolar 10, 20; desencolar -> 10
       encolar 30; desencolar -> 20
       desencolar -> 30
       (el volcado solo se hace cuando salida esta vacia)

    3) PilaConDosColas -> LIFO
       apilado 1, 2, 3, 4, 5 -> desapilando: 5 4 3 2 1 (LIFO correcto)
       desapilar en vacia -> false

    4) Complejidades
       ColaConDosPilas: encolar O(1), desencolar O(1) amortizado
       PilaConDosColas: apilar  O(1), desapilar  O(n)

    Todos los assert pasaron.
*/
