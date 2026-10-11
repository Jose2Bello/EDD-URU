/* ============================================================================
   EJERCICIO 08 - Rotar una lista k posiciones
   Tema   : Listas - aritmetica de punteros y algoritmo en tres pasos
   ============================================================================

   ENUNCIADO
   --------
   Rotar una lista k posiciones hacia la DERECHA significa que los ultimos k
   elementos pasan al principio.

   Ejemplo:
       head -> [1] -> [2] -> [3] -> [4] -> [5] -> nullptr
       rotar(2)  ->  [4] -> [5] -> [1] -> [2] -> [3] -> nullptr
       rotar(0)  ->  la lista no cambia
       rotar(-2) ->  equivale a rotar(3)  (rotacion a la izquierda de 2)

   Casos limite obligatorios:
       - k mayor que el largo de la lista (se reduce con k % largo)
       - k = 0
       - k negativa
       - lista vacia
       - lista de un solo nodo

   METODO (tres pasos, todo O(n))
   1) Calcular el largo n y reducir k con k % n.
   2) Avanzar hasta el nodo que queda en la posicion n-k (el ultimo del bloque
      que se queda atras). Ese nodo sera la nueva cola.
   3) Puentear: la nueva cabeza es su siguiente, y su siguiente es nullptr.

   ========================================================================== */

#include <iostream>
#include <initializer_list>
#include <string>

template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T>
class ListaSimple {
private:
    Nodo<T>* head;

public:
    ListaSimple() : head(nullptr) {}

    ~ListaSimple() {
        Nodo<T>* a = head;
        while (a != nullptr) {
            Nodo<T>* sig = a->siguiente;
            delete a;
            a = sig;
        }
        head = nullptr;
    }

    void agregarAlFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (head == nullptr) { head = nuevo; return; }
        Nodo<T>* a = head;
        while (a->siguiente != nullptr) a = a->siguiente;
        a->siguiente = nuevo;
    }

    Nodo<T>* cabeza() const { return head; }

    // ------------------------------------------------------------------
    // ROTAR A LA DERECHA
    // ------------------------------------------------------------------
    void rotarDerecha(int k) {
        if (head == nullptr || head->siguiente == nullptr) return;   // 0 o 1 nodo

        int n = largo();
        int desplazamiento = k % n;              // reduce el numero de vueltas
        if (desplazamiento < 0) desplazamiento += n;   // k negativa -> izquierda
        if (desplazamiento == 0) return;          // no hay nada que hacer

        // Paso 1: llegar al nodo que ocupara la posicion n - desplazamiento
        Nodo<T>* nuevaCola = head;
        for (int i = 0; i < n - desplazamiento - 1; i++) {
            nuevaCola = nuevaCola->siguiente;
        }

        // Paso 2: nueva cabeza = siguiente de la nueva cola
        Nodo<T>* nuevaCabeza = nuevaCola->siguiente;

        // Paso 3: puentear -> cerrar la lista y mover head
        nuevaCola->siguiente = nullptr;     // el que era el ultimo deja de serlo
        Nodo<T>* ultimo = nuevaCabeza;
        while (ultimo->siguiente != nullptr) {
            ultimo = ultimo->siguiente;
        }
        ultimo->siguiente = head;           // se cierra el circulo
        head = nuevaCabeza;                 // se actualiza la cabeza
    }

    // ------------------------------------------------------------------
    // VERSION ALTERNATIVA cerrando el circulo:
    //   1) se enlaza el ultimo nodo con la cabeza
    //   2) se avanza hasta el nodo que sera la NUEVA COLA (n - k - 1 pasos)
    //   3) el siguiente de esa cola es la nueva cabeza, y se corta el enlace
    // ------------------------------------------------------------------
    void rotarDerechaConRapidoLento(int k) {
        if (head == nullptr || head->siguiente == nullptr) return;

        int n = largo();
        k %= n;
        if (k < 0) k += n;
        if (k == 0) return;

        // Paso 1: cerrar el circulo (el ultimo apunta a la cabeza)
        Nodo<T>* ultimo = head;
        while (ultimo->siguiente != nullptr) ultimo = ultimo->siguiente;
        ultimo->siguiente = head;

        // Paso 2: avanzar hasta el nodo que sera la nueva cola.
        //         La nueva cola es el nodo situado en la posicion n - k - 1
        //         (contando desde 0), porque el siguiente sera la nueva cabeza.
        Nodo<T>* nuevaCola = head;
        for (int i = 0; i < n - k - 1; i++) {
            nuevaCola = nuevaCola->siguiente;
        }

        // Paso 3: cortar el enlace y actualizar la cabeza
        Nodo<T>* nuevaCabeza = nuevaCola->siguiente;   // dentro del circulo
        nuevaCola->siguiente = nullptr;                // se cierra la lista
        head = nuevaCabeza;
    }

    // ------------------------------------------------------------------
    // ROTAR A LA IZQUIERDA (equivale a rotar derecha n - k veces)
    // ------------------------------------------------------------------
    void rotarIzquierda(int k) {
        if (head == nullptr || head->siguiente == nullptr) return;
        int n = largo();
        rotarDerecha((n - (k % n)) % n);
    }

    int largo() const {
        int n = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    // La lista como texto: comoda para imprimir resultados en comparaciones
    std::string aTexto() const {
        std::string s;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) {
            s += std::to_string(a->dato) + " -> ";
        }
        s += "nullptr";
        return s;
    }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) {
            std::cout << a->dato << " -> ";
        }
        std::cout << "nullptr\n";
    }
};

template <typename T>
void cargar(ListaSimple<T>& lista, std::initializer_list<T> datos) {
    for (T d : datos) lista.agregarAlFinal(d);
}

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 08: rotar la lista ===\n\n";

    // --- 1) Rotacion a la derecha, metodo de tres pasos -----------------------
    std::cout << "-- 1) rotarDerecha(k) --\n";
    struct Caso { int k; int largo; const char* nota; };
    const Caso casos[] = {
        { 0,  5, "k = 0: no cambia"},
        { 1,  5, "un elemento al frente"},
        { 2,  5, "dos elementos al frente"},
        { 4,  5, "rotacion casi completa"},
        { 5,  5, "k = largo: equivale a 0"},
        { 7,  5, "k mayor que largo (7 % 5 = 2)"},
        {12,  5, "k muy mayor (12 % 5 = 2)"},
        {-1,  5, "k negativa (-1): una rotacion a la izquierda"},
        {-2,  5, "k negativa (-2): dos rotaciones a la izquierda"},
    };

    for (const Caso& c : casos) {
        ListaSimple<int> lista;
        cargar(lista, {1, 2, 3, 4, 5});
        std::string antes = lista.aTexto();
        lista.rotarDerecha(c.k);
        std::cout << "  " << antes << "\n";
        std::cout << "     rotarDerecha(" << c.k << ") = " << lista.aTexto() << "\n";
        std::cout << "     (" << c.nota << ")\n";
    }

    // --- 2) Comprobacion de los casos limite ---------------------------------
    std::cout << "\n-- 2) Casos limite --\n";
    {
        ListaSimple<int> vacia;
        vacia.rotarDerecha(3);
        std::cout << "  Lista vacia:       " << vacia.aTexto()
                  << "   (protegida contra rotar)\n";
    }
    {
        ListaSimple<int> uno;
        uno.agregarAlFinal(42);
        uno.rotarDerecha(5);
        uno.mostrar("  Un solo elemento: ");
    }

    // --- 3) Version con rapido/lento -----------------------------------------
    std::cout << "\n-- 3) rotarDerechaConRapidoLento(k) --\n";
    for (int k = 0; k <= 7; k++) {
        ListaSimple<int> a, b;
        cargar(a, {1, 2, 3, 4, 5});
        cargar(b, {1, 2, 3, 4, 5});
        a.rotarDerecha(k);
        b.rotarDerechaConRapidoLento(k);

        std::cout << "  k = " << k << "  metodo de 3 pasos: " << a.aTexto() << "\n";
        std::cout << "            metodo con circulo: " << b.aTexto()
                  << (a.aTexto() == b.aTexto() ? "   [iguales]" : "   [DIFIEREN]") << "\n";
    }

    // --- 4) Rotacion a la izquierda ------------------------------------------
    std::cout << "\n-- 4) rotarIzquierda(k) --\n";
    {
        ListaSimple<int> l;
        cargar(l, {1, 2, 3, 4, 5});
        std::cout << "  " << l.aTexto() << "\n";
        l.rotarIzquierda(2);
        std::cout << "     rotarIzquierda(2) = " << l.aTexto() << "\n";
    }

    // --- 5) Rotaciones sucesivas equivalen a una sola -------------------------
    std::cout << "\n-- 5) Tres rotaciones de 1 equivalen a una de 3 --\n";
    {
        ListaSimple<int> a, b;
        cargar(a, {1, 2, 3, 4, 5});
        cargar(b, {1, 2, 3, 4, 5});
        a.rotarDerecha(1);
        a.rotarDerecha(1);
        a.rotarDerecha(1);
        b.rotarDerecha(3);
        std::cout << "  Tres de 1: " << a.aTexto() << "\n";
        std::cout << "  Una de 3:  " << b.aTexto() << "\n";
    }

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 08: rotar la lista ===
    -- 1) rotarDerecha(k) --
      1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         rotarDerecha(0) = 1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         (k = 0: no cambia)
      1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         rotarDerecha(1) = 5 -> 1 -> 2 -> 3 -> 4 -> nullptr
         (un elemento al frente)
      1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         rotarDerecha(2) = 4 -> 5 -> 1 -> 2 -> 3 -> nullptr
         (dos elementos al frente)
      1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         rotarDerecha(4) = 2 -> 3 -> 4 -> 5 -> 1 -> nullptr
         (rotacion casi completa)
      1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         rotarDerecha(5) = 1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         (k = largo: equivale a 0)
      1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         rotarDerecha(7) = 4 -> 5 -> 1 -> 2 -> 3 -> nullptr
         (k mayor que largo (7 % 5 = 2))
      1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         rotarDerecha(12) = 4 -> 5 -> 1 -> 2 -> 3 -> nullptr
         (k muy mayor (12 % 5 = 2))
      1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         rotarDerecha(-1) = 2 -> 3 -> 4 -> 5 -> 1 -> nullptr
         (k negativa (-1): una rotacion a la izquierda)
      1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         rotarDerecha(-2) = 3 -> 4 -> 5 -> 1 -> 2 -> nullptr
         (k negativa (-2): dos rotaciones a la izquierda)
    -- 2) Casos limite --
      Lista vacia:       nullptr   (protegida contra rotar)
      Un solo elemento: 42 -> nullptr
    -- 3) rotarDerechaConRapidoLento(k) --
      k = 0  metodo de 3 pasos: 1 -> 2 -> 3 -> 4 -> 5 -> nullptr
                metodo con circulo: 1 -> 2 -> 3 -> 4 -> 5 -> nullptr   [iguales]
      k = 1  metodo de 3 pasos: 5 -> 1 -> 2 -> 3 -> 4 -> nullptr
                metodo con circulo: 5 -> 1 -> 2 -> 3 -> 4 -> nullptr   [iguales]
      k = 2  metodo de 3 pasos: 4 -> 5 -> 1 -> 2 -> 3 -> nullptr
                metodo con circulo: 4 -> 5 -> 1 -> 2 -> 3 -> nullptr   [iguales]
      k = 3  metodo de 3 pasos: 3 -> 4 -> 5 -> 1 -> 2 -> nullptr
                metodo con circulo: 3 -> 4 -> 5 -> 1 -> 2 -> nullptr   [iguales]
      k = 4  metodo de 3 pasos: 2 -> 3 -> 4 -> 5 -> 1 -> nullptr
                metodo con circulo: 2 -> 3 -> 4 -> 5 -> 1 -> nullptr   [iguales]
      k = 5  metodo de 3 pasos: 1 -> 2 -> 3 -> 4 -> 5 -> nullptr
                metodo con circulo: 1 -> 2 -> 3 -> 4 -> 5 -> nullptr   [iguales]
      k = 6  metodo de 3 pasos: 5 -> 1 -> 2 -> 3 -> 4 -> nullptr
                metodo con circulo: 5 -> 1 -> 2 -> 3 -> 4 -> nullptr   [iguales]
      k = 7  metodo de 3 pasos: 4 -> 5 -> 1 -> 2 -> 3 -> nullptr
                metodo con circulo: 4 -> 5 -> 1 -> 2 -> 3 -> nullptr   [iguales]
    -- 4) rotarIzquierda(k) --
      1 -> 2 -> 3 -> 4 -> 5 -> nullptr
         rotarIzquierda(2) = 3 -> 4 -> 5 -> 1 -> 2 -> nullptr
    -- 5) Tres rotaciones de 1 equivalen a una de 3 --
      Tres de 1: 3 -> 4 -> 5 -> 1 -> 2 -> nullptr
      Una de 3:  3 -> 4 -> 5 -> 1 -> 2 -> nullptr
    Demostracion completada.
*/
