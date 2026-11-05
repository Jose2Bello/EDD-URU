/* ============================================================================
   PLANTILLA 08 - Pila (stack, LIFO) - version completa de examen
   Archivo : 08_pila_completa.cpp

   Librerias: SOLO <iostream>. Nada de stack, vector, string ni assert.

   La PLANTILLA 05 trae lo basico (apilar, desapilar, cima) y las expresiones.
   Esta 08 trae lo que suele caer en el examen y no esta en la 05:

       Pila<T>      : todas las operaciones basicas +
                      copiar (constructor copia y operator=),
                      contar, buscar, contiene, duplicar,
                      invertir (recursivo), ordenarMayorArriba,
                      desapilarN, mostrar
       PilaMin<T>   : el minimo en O(1) con una pila auxiliar
       Aplicaciones : invertir una cadena, decimal -> binario,
                      palindromo con pila

   Idea central: el tope (top) es la cabeza de la lista, por eso apilar y
   desapilar son O(1) y SOLO se toca ese extremo.

        top -> [30] -> [20] -> [10] -> nullptr
               entra            sale
               y sale          despues
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
// Pila completa
// ---------------------------------------------------------------------------
template <typename T>
class Pila {
private:
    Nodo<T>* top;

    // Copia recursiva: baja hasta el fondo y luego apila de vuelta.
    // Asi se conserva el MISMO orden que la pila original.
    void copiarDesde(Nodo<T>* nodo) {
        if (nodo == nullptr) return;
        copiarDesde(nodo->siguiente);
        apilar(nodo->dato);
    }

    // Inserta un valor manteniendo el mayor en el tope (orden descendente
    // de abajo hacia arriba). Se usa solo desde ordenarMayorArriba().
    void insertarOrdenado(T valor) {
        if (estaVacia() || valor > cima()) {
            apilar(valor);
            return;
        }
        T temporal = T();
        desapilar(temporal);
        insertarOrdenado(valor);
        apilar(temporal);
    }

public:
    Pila() : top(nullptr) {}

    // ---- Constructor copia: hace una copia PROFUNDA, nodo por nodo --------
    Pila(const Pila& otra) : top(nullptr) {
        copiarDesde(otra.top);
    }

    // ---- operator=: primero se libera lo propio y luego se copia ----------
    Pila& operator=(const Pila& otra) {
        if (this != &otra) {
            limpiar();
            copiarDesde(otra.top);
        }
        return *this;
    }

    ~Pila() { limpiar(); }

    // ===================== OPERACIONES BASICAS ============================

    void apilar(T valor) {                   // O(1)
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = top;              // se apoya sobre el anterior
        top = nuevo;
    }

    bool desapilar(T& salida) {              // O(1)
        if (top == nullptr) return false;
        Nodo<T>* aBorrar = top;
        salida = aBorrar->dato;              // se copia ANTES de borrar
        top = aBorrar->siguiente;
        delete aBorrar;
        return true;
    }

    bool cima(T& salida) const {             // O(1), no saca el elemento
        if (top == nullptr) return false;
        salida = top->dato;
        return true;
    }

    T cima() const {                         // version que devuelve el valor
        if (top == nullptr) return T();
        return top->dato;
    }

    bool estaVacia() const { return top == nullptr; }
    bool estaLlena() const { return false; }  // con nodos nunca se llena

    int tamano() const {
        int n = 0;
        for (Nodo<T>* a = top; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    int contar() const { return tamano(); }

    void limpiar() {
        while (top != nullptr) {
            Nodo<T>* base = top->siguiente;
            delete top;
            top = base;
        }
    }

    // ===================== CONSULTAR ======================================

    bool busca(T valor) const {
        for (Nodo<T>* a = top; a != nullptr; a = a->siguiente)
            if (a->dato == valor) return true;
        return false;
    }

    bool contiene(T valor) const { return busca(valor); }

    int vecesQueAparece(T valor) const {
        int n = 0;
        for (Nodo<T>* a = top; a != nullptr; a = a->siguiente)
            if (a->dato == valor) n++;
        return n;
    }

    // ===================== TRANSFORMAR ===================================

    // Saca los n elementos del tope. Devuelve cuantos pudo sacar.
    int desapilarN(int n) {
        int sacados = 0;
        T temporal = T();
        while (sacados < n && desapilar(temporal)) sacados++;
        return sacados;
    }

    // Invierte la pila SIN crear una pila auxiliar: recursion.
    //   3 2 1  ->  1 2 3   (el tope cambia de 3 a 1)
    void invertir() {
        if (estaVacia()) return;
        T fondo = T();
        desapilar(fondo);      // se guarda el tope
        invertir();            // se invierte el resto
        insertarEnFondo(fondo);
    }

    void insertarEnFondo(T valor) {
        if (estaVacia()) { apilar(valor); return; }
        T temporal = T();
        desapilar(temporal);
        insertarEnFondo(valor);
        apilar(temporal);
    }

    // Duplica cada elemento conservando el orden:
    //   top 3 2 1  ->  top 3 3 2 2 1 1
    void duplicar() {
        Pila<T> auxiliar;
        while (!estaVacia()) {
            T valor = T();
            desapilar(valor);
            auxiliar.apilar(valor);
        }
        while (!auxiliar.estaVacia()) {
            T valor = T();
            auxiliar.desapilar(valor);
            apilar(valor);
            apilar(valor);
        }
    }

    // Deja el MAYOR en el tope. Al desapilar salen de mayor a menor.
    void ordenarMayorArriba() {
        if (estaVacia()) return;
        T temporal = T();
        desapilar(temporal);
        ordenarMayorArriba();
        insertarOrdenado(temporal);
    }

    // ===================== MOSTRAR ========================================

    // Se imprime del tope hacia el fondo: el primero es el que sale antes.
    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        for (Nodo<T>* a = top; a != nullptr; a = a->siguiente)
            std::cout << a->dato << " ";
        if (top == nullptr) std::cout << "(vacia)";
        std::cout << "     <- tope a la izquierda\n";
    }
};

// ---------------------------------------------------------------------------
// PilaMin: mantiene el minimo en O(1) usando una segunda pila.
//
// Se apila en `minimos` cuando el valor nuevo es menor o igual que el minimo
// actual. Asi, en todo momento, minimos.cima() es el minimo de toda la pila.
// ---------------------------------------------------------------------------
template <typename T>
class PilaMin {
private:
    Pila<T> datos;
    Pila<T> minimos;

public:
    void apilar(T valor) {
        datos.apilar(valor);
        if (minimos.estaVacia() || valor <= minimos.cima())
            minimos.apilar(valor);
    }

    bool desapilar(T& salida) {
        if (!datos.desapilar(salida)) return false;
        if (!minimos.estaVacia() && salida == minimos.cima())
            minimos.desapilar(salida);     // se descarta el minimo viejo
        return true;
    }

    bool cima(T& salida) const { return datos.cima(salida); }

    bool minimo(T& salida) const { return minimos.cima(salida); }

    bool estaVacia() const { return datos.estaVacia(); }
    int  tamano() const { return datos.tamano(); }

    void mostrar(const char* etiqueta = "") const {
        datos.mostrar(etiqueta);
        T m;
        if (minimos.cima(m))
            std::cout << "        (minimo actual = " << m << ")\n";
    }
};

// ===========================================================================
//  APLICACION 1: invertir una cadena con una pila
// ===========================================================================
void invertirCadena(const char* cadena) {
    Pila<char> pila;
    for (int i = 0; cadena[i] != '\0'; i++) pila.apilar(cadena[i]);

    std::cout << "   \"" << cadena << "\" -> \"";
    char c = '\0';
    while (pila.desapilar(c)) std::cout << c;
    std::cout << "\"\n";
}

// ===========================================================================
//  APLICACION 2: convertir un decimal a binario
//  Se divide entre 2 y el resto se apila; al final se desapila (queda al reves).
// ===========================================================================
void decimalABinario(int numero) {
    std::cout << "   " << numero << " -> ";

    if (numero == 0) { std::cout << "0\n"; return; }

    Pila<int> bits;
    int n = numero;
    while (n > 0) {
        bits.apilar(n % 2);
        n /= 2;
    }

    int bit;
    while (bits.desapilar(bit)) std::cout << bit;
    std::cout << "\n";
}

// ===========================================================================
//  APLICACION 3: palindromo con una pila
//  Se apila toda la cadena; al desapilar debe coincidir con la original.
// ===========================================================================
bool esPalindromo(const char* cadena) {
    Pila<char> pila;
    int longitud = 0;
    for (int i = 0; cadena[i] != '\0'; i++) {
        pila.apilar(cadena[i]);
        longitud++;
    }

    char c = '\0';
    for (int i = 0; i < longitud; i++) {
        pila.desapilar(c);
        if (c != cadena[i]) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== PLANTILLA 08: pila completa ===\n\n";

    // --- 1) Operaciones basicas ---------------------------------------------
    std::cout << "1) Pila basica\n";
    Pila<int> pila;
    pila.mostrar("   ");
    for (int v : {10, 20, 30}) pila.apilar(v);
    pila.mostrar("   apilar 10, 20, 30 -> ");

    int valor;
    pila.cima(valor);
    std::cout << "   cima = " << valor
              << "   tamano = " << pila.tamano()
              << "   estaVacia = " << std::boolalpha << pila.estaVacia() << "\n";
    std::cout << "   contiene(20) = " << pila.contiene(20)
              << "   contiene(99) = " << pila.contiene(99)
              << "   vecesQueAparece(20) = " << pila.vecesQueAparece(20) << "\n\n";

    // --- 2) Copia profunda --------------------------------------------------
    std::cout << "2) Copia profunda (constructor copia)\n";
    Pila<int> copia = pila;
    copia.apilar(99);
    std::cout << "   original: "; pila.mostrar("");
    std::cout << "   copia   : "; copia.mostrar("");
    std::cout << "   (apilar en la copia NO modifica la original)\n\n";

    // --- 3) Invertir y duplicar ----------------------------------------------
    std::cout << "3) Invertir y duplicar\n";
    Pila<int> invertida;
    for (int v : {1, 2, 3}) invertida.apilar(v);
    std::cout << "   antes:     "; invertida.mostrar("");
    invertida.invertir();
    std::cout << "   invertir(): "; invertida.mostrar("");

    Pila<int> duplicada;
    for (int v : {1, 2, 3}) duplicada.apilar(v);
    std::cout << "   antes:      "; duplicada.mostrar("");
    duplicada.duplicar();
    std::cout << "   duplicar(): "; duplicada.mostrar("");
    std::cout << "\n";

    // --- 4) Ordenar ----------------------------------------------------------
    std::cout << "4) Ordenar (el mayor queda en el tope)\n";
    Pila<int> desordenada;
    for (int v : {5, 1, 4, 2, 3}) desordenada.apilar(v);
    std::cout << "   antes:                 "; desordenada.mostrar("");
    desordenada.ordenarMayorArriba();
    std::cout << "   ordenarMayorArriba():  "; desordenada.mostrar("");
    std::cout << "   desapilando: ";
    while (desordenada.desapilar(valor)) std::cout << valor << " ";
    std::cout << "\n\n";

    // --- 5) desapilarN -------------------------------------------------------
    std::cout << "5) desapilarN(2)\n";
    Pila<int> recorte;
    for (int v : {10, 20, 30, 40}) recorte.apilar(v);
    std::cout << "   antes: ";
    recorte.mostrar("");
    std::cout << "   desapilarN(2) saco " << recorte.desapilarN(2) << " elementos -> ";
    recorte.mostrar("");
    std::cout << "   desapilarN(50) saco " << recorte.desapilarN(50) << " (no hay tantos)\n\n";

    // --- 6) PilaMin ----------------------------------------------------------
    std::cout << "6) PilaMin (minimo en O(1))\n";
    PilaMin<int> pmin;
    for (int v : {5, 3, 7, 2, 8}) {
        pmin.apilar(v);
        std::cout << "   apilar(" << v << ") -> ";
        pmin.mostrar("");
    }
    std::cout << "   desapilar -> "; pmin.desapilar(valor);
    std::cout << valor << " -> "; pmin.mostrar("");
    std::cout << "\n";

    // --- 7) Aplicaciones -----------------------------------------------------
    std::cout << "7) Aplicaciones\n";
    invertirCadena("estructuras");
    decimalABinario(10);
    decimalABinario(37);
    decimalABinario(0);

    const char* pruebas[] = {"anilina", "reconocer", "abb", "casa", "radar"};
    for (int i = 0; i < 5; i++)
        std::cout << "   esPalindromo(\"" << pruebas[i] << "\") = "
                  << std::boolalpha << esPalindromo(pruebas[i]) << "\n";

    std::cout << "\nFin de la plantilla.\n";
    return 0;
}
