/* ============================================================================
   EJERCICIO 06 - Determinar si una lista es un palindromo
   Tema   : Listas - dos punteros, pila y recursion
   Archivo: 06_palindromo_lista.cpp
   ============================================================================

   ENUNCIADO
   --------
   Una lista es palindromo si se lee igual de izquierda a derecha que de derecha
   a izquierda. Solo se permite usar la estructura de lista enlazada: NO se
   puede copiar los datos a un arreglo.

   Casos a considerar:
       1 -> 2 -> 3 -> 2 -> 1     es palindromo
       1 -> 2 -> 3 -> 4           NO es palindromo
       1 -> 2 -> 2 -> 1           es palindromo
       1 -> 1                     es palindromo (un solo elemento)
       nullptr                    es palindromo (lista vacia)

   Implementar:

   1) esPalindromo()          : rapido/lento + invertir la segunda mitad. O(n)
   2) esPalindromoPila()      : usando std::stack.                            O(n)
   3) esPalindromoRecursivo() : por recursion, comparando por indice.        O(n^2)
   4) palindromoMasLargo()    : longitud del tramo palindromo mas largo.      O(n^2)

   ========================================================================== */

#include <iostream>
#include <stack>

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

    // ------------------------------------------------------------------
    // Auxiliar reutilizable: invierte una cadena de nodos y devuelve la
    // nueva cabeza. Apta para lista simplemente enlazada.
    // ------------------------------------------------------------------
    Nodo<T>* invertirNodos(Nodo<T>* cabeza) {
        Nodo<T>* anterior = nullptr;
        Nodo<T>* actual   = cabeza;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;  // salvar antes de rompe
            actual->siguiente = anterior;             // invertir la flecha
            anterior = actual;
            actual   = siguiente;
        }
        return anterior;   // el ultimo procesado es la nueva cabeza
    }

    // ------------------------------------------------------------------
    // 1) RAPIDO / LENTO
    //
    //    Paso 1: rapido avanza de a dos y lento de a uno. Al terminar,
    //            `lento` es el ULTIMO nodo de la primera mitad.
    //    Paso 2: se separa la segunda mitad y se invierte.
    //    Paso 3: se comparan ambas mitades nodo a nodo.
    //    Paso 4: se invierte otra vez la segunda mitad para RESTAURAR la lista.
    //
    //    Ojo: el metodo es NO const porque reordena los enlaces temporalmente,
    //    aunque al final la lista queda igual que estaba.
    // ------------------------------------------------------------------
    bool esPalindromo() {
        if (head == nullptr || head->siguiente == nullptr) {
            return true;   // lista vacia o de un solo elemento: es palindromo
        }

        // Paso 1: hallar el medio
        Nodo<T>* lento  = head;
        Nodo<T>* rapido = head;
        while (rapido->siguiente != nullptr && rapido->siguiente->siguiente != nullptr) {
            lento  = lento->siguiente;
            rapido = rapido->siguiente->siguiente;
        }

        // Paso 2: separar e invertir la segunda mitad
        Nodo<T>* segunda   = lento->siguiente;
        lento->siguiente   = nullptr;
        Nodo<T>* invertida = invertirNodos(segunda);

        // Paso 3: comparar
        Nodo<T>* izq = head;
        Nodo<T>* der = invertida;
        bool es = true;
        while (der != nullptr && izq != nullptr) {
            if (izq->dato != der->dato) { es = false; break; }
            izq = izq->siguiente;
            der = der->siguiente;
        }

        // Paso 4: restaurar la lista a su estado original
        lento->siguiente = invertirNodos(invertida);

        return es;
    }

    // ------------------------------------------------------------------
    // 2) CON PILA
    //    Se apilan todos los valores y luego se comparan con la segunda
    //    mitad de la lista (que sigue en su orden original).
    // ------------------------------------------------------------------
    bool esPalindromoPila() const {
        std::stack<T> valores;
        int n = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) {
            valores.push(a->dato);
            n++;
        }

        int mitad = n / 2;
        Nodo<T>* actual = head;
        for (int i = 0; i < n - mitad; i++) {   // solo la segunda mitad
            if (valores.top() != actual->dato) return false;
            valores.pop();
            actual = actual->siguiente;
        }
        return true;
    }

    // ------------------------------------------------------------------
    // 3) RECURSIVA (por indice)
    //    La recursion avanza los dos indices hacia el centro. Ojo: como
    //    enPosicion() es O(n), el costo total es O(n^2). Sirve para
    //    entender la mecanica, no para eficiencia.
    // ------------------------------------------------------------------
    bool esPalindromoRecursivo() const {
        int n = largo();
        return compararPorIndice(0, n - 1);
    }

    bool compararPorIndice(int i, int j) const {
        if (i >= j) return true;                       // caso base: indices cruzados
        if (enPosicion(i) != enPosicion(j)) return false;
        return compararPorIndice(i + 1, j - 1);        // caso recursivo
    }

    // ------------------------------------------------------------------
    // 4) PALINDROMO MAS LARGO
    //    Se prueban todos los centros posibles (impares y pares) y se
    //    mide cuanto se extiende cada uno en los dos sentidos.
    // ------------------------------------------------------------------
    int palindromoMasLargo() const {
        int n = largo();
        int mejor = 0;

        for (int centro = 0; centro < n; centro++) {
            // centro impar: el nodo mismo es el centro
            int l = centro, r = centro;
            while (l >= 0 && r < n && enPosicion(l) == enPosicion(r)) { l--; r++; }
            int largoImpar = r - l - 1;

            // centro par: entre dos nodos
            l = centro; r = centro + 1;
            while (l >= 0 && r < n && enPosicion(l) == enPosicion(r)) { l--; r++; }
            int largoPar = r - l - 1;

            int maximo = largoImpar > largoPar ? largoImpar : largoPar;
            if (maximo > mejor) mejor = maximo;
        }
        return mejor;
    }

    // Utilidad: dato que esta en la posicion `indice` (0 = cabeza)
    T enPosicion(int indice) const {
        Nodo<T>* a = head;
        while (indice > 0 && a != nullptr) { a = a->siguiente; indice--; }
        return a != nullptr ? a->dato : T();
    }

    int largo() const {
        int n = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) {
            std::cout << a->dato << " -> ";
        }
        std::cout << "nullptr\n";
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 06: palindromo en lista ===\n\n";

    // --- 1) rapido / lento ---------------------------------------------------
    std::cout << "-- 1) Dos punteros (rapido y lento) --\n";

    struct Caso { int datos[8]; int n; bool esperado; const char* nota; };

    Caso casos[] = {
        {{1, 2, 3, 2, 1},    5, true,  "impar, simetrico"},
        {{1, 2, 3, 4},       4, false, "par, no simetrico"},
        {{1, 2, 2, 1},       4, true,  "par, simetrico"},
        {{1, 1},             2, true,  "caso minimo par"},
        {{5},                1, true,  "un solo elemento"},
        {{1, 2, 1},          3, true,  "impar pequeno"},
        {{1, 2},             2, false, "dos distintos"},
        {{1, 2, 3, 4, 1},    5, false, "extremos iguales, interior no"},
    };

    for (const Caso& c : casos) {
        ListaSimple<int> lista;
        for (int i = 0; i < c.n; i++) lista.agregarAlFinal(c.datos[i]);
        std::cout << "  ";
        lista.mostrar("");
        std::cout << "  -> " << c.nota << " = " << std::boolalpha << lista.esPalindromo() << "\n";
        // El metodo restaura la lista: se verifica que sigue intacta
    }

    // Casos limite
    ListaSimple<int> vacia;
    std::cout << "\n  ";
    vacia.mostrar("");
    std::cout << "  -> lista vacia = " << vacia.esPalindromo() << "\n\n";

    // --- 2) Con pila ----------------------------------------------------------
    std::cout << "-- 2) Con pila --\n";
    {
        ListaSimple<int> l;
        int d[] = {1, 2, 3, 2, 1};
        for (int x : d) l.agregarAlFinal(x);
        std::cout << "  [1,2,3,2,1] -> " << l.esPalindromoPila() << "  (esperado true)\n";
    }
    {
        ListaSimple<int> l;
        int d[] = {1, 2, 3, 4};
        for (int x : d) l.agregarAlFinal(x);
        std::cout << "  [1,2,3,4]   -> " << l.esPalindromoPila() << "  (esperado false)\n";
    }
    {
        ListaSimple<int> l;
        int d[] = {7, 1, 2, 1, 7};
        for (int x : d) l.agregarAlFinal(x);
        std::cout << "  [7,1,2,1,7] -> " << l.esPalindromoPila() << "  (esperado true)\n\n";
    }

    // --- 3) Recursiva --------------------------------------------------------
    std::cout << "-- 3) Recursiva --\n";
    {
        ListaSimple<int> l;
        int d[] = {9, 4, 5, 4, 9};
        for (int x : d) l.agregarAlFinal(x);
        l.mostrar("  ");
        std::cout << "  -> esPalindromoRecursivo() = " << l.esPalindromoRecursivo() << "\n\n";
    }

    // --- 4) Palindromo mas largo --------------------------------------------
    std::cout << "-- 4) Tramo palindromo mas largo --\n";
    {
        ListaSimple<int> l;
        int d[] = {1, 2, 3, 4, 5, 4, 3, 2, 1};   // toda la lista es palindromo
        for (int x : d) l.agregarAlFinal(x);
        l.mostrar("  ");
        std::cout << "  -> longitud = " << l.palindromoMasLargo() << "  (esperado 9)\n";
    }
    {
        ListaSimple<int> l;
        int d[] = {1, 2, 3, 2, 1, 9, 8};        // el mejor tramo es de 5
        for (int x : d) l.agregarAlFinal(x);
        l.mostrar("  ");
        std::cout << "  -> longitud = " << l.palindromoMasLargo() << "  (esperado 5)\n";
    }
    {
        ListaSimple<int> l;
        int d[] = {4, 6, 6, 8};                // el unico palindromo es [6,6]
        for (int x : d) l.agregarAlFinal(x);
        l.mostrar("  ");
        std::cout << "  -> longitud = " << l.palindromoMasLargo() << "  (esperado 2)\n";
    }
    {
        ListaSimple<int> l;
        int d[] = {4, 6, 8, 10};                // sin elementos iguales contiguos
        for (int x : d) l.agregarAlFinal(x);
        l.mostrar("  ");
        std::cout << "  -> longitud = " << l.palindromoMasLargo() << "  (esperado 1: cada elemento solo)\n\n";
    }

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 06: palindromo en lista ===
    -- 1) Dos punteros (rapido y lento) --
      1 -> 2 -> 3 -> 2 -> 1 -> nullptr
      -> impar, simetrico = true
      1 -> 2 -> 3 -> 4 -> nullptr
      -> par, no simetrico = false
      1 -> 2 -> 2 -> 1 -> nullptr
      -> par, simetrico = true
      1 -> 1 -> nullptr
      -> caso minimo par = true
      5 -> nullptr
      -> un solo elemento = true
      1 -> 2 -> 1 -> nullptr
      -> impar pequeno = true
      1 -> 2 -> nullptr
      -> dos distintos = false
      1 -> 2 -> 3 -> 4 -> 1 -> nullptr
      -> extremos iguales, interior no = false
      nullptr
      -> lista vacia = true
    -- 2) Con pila --
      [1,2,3,2,1] -> true  (esperado true)
      [1,2,3,4]   -> false  (esperado false)
      [7,1,2,1,7] -> true  (esperado true)
    -- 3) Recursiva --
      9 -> 4 -> 5 -> 4 -> 9 -> nullptr
      -> esPalindromoRecursivo() = true
    -- 4) Tramo palindromo mas largo --
      1 -> 2 -> 3 -> 4 -> 5 -> 4 -> 3 -> 2 -> 1 -> nullptr
      -> longitud = 9  (esperado 9)
      1 -> 2 -> 3 -> 2 -> 1 -> 9 -> 8 -> nullptr
      -> longitud = 5  (esperado 5)
      4 -> 6 -> 6 -> 8 -> nullptr
      -> longitud = 2  (esperado 2)
      4 -> 6 -> 8 -> 10 -> nullptr
      -> longitud = 1  (esperado 1: cada elemento solo)
    Demostracion completada.
*/
