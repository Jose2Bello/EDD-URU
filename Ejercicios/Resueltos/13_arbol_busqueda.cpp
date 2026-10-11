/* ============================================================================
   EJERCICIO 13 - Arbol binario de busqueda (ABB)
   Tema   : Arboles - insercion, recorridos, busqueda y borrado
   Archivo: 13_arbol_busqueda.cpp
   ============================================================================

   ENUNCIADO
   --------
   Implementar un arbol binario de busqueda (ABB) con:

     1) insertar(v)              : mantiene la propiedad de orden
     2) buscar(v)                : devuelve bool
     3) preOrden / inOrden / postOrden
     4) preOrdenIterativo()      : usando std::stack (evita el desbordamiento)
     5) altura()                 : numero de niveles
     6) esLleno()                : todos los niveles completos
     7) esBalanceado()           : la diferencia de alturas <= 1
     8) contarNodos() / sumaValores() / minimo() / maximo()
     9) sonIdenticos(a, b)       : compara dos arboles
    10) mezclarLista(lista)      : insercion masiva y_optimo (n log n)
    11) nivelOrden()             : recorrido por niveles (como el BFS)
    12) borrar(v)                : los tres casos del borrado

   PROPIEDAD DEL ABB
   -----------------
   Para todo nodo: todo lo del subarbol izquierdo es MENOR que el,
   y todo lo del subarbol derecho es MAYOR. Como en cada paso se descarta
   la mitad del arbol, buscar es O(log n).

   ========================================================================== */

#include <iostream>
#include <stack>
#include <queue>
#include <vector>

// ---------------------------------------------------------------------------
// Nodo del arbol
// ---------------------------------------------------------------------------
template <typename T>
class NodoArbol {
public:
    T dato;
    NodoArbol<T>* izquierda;
    NodoArbol<T>* derecha;

    NodoArbol(T valor) : dato(valor), izquierda(nullptr), derecha(nullptr) {}
};

// ---------------------------------------------------------------------------
// ABB
// ---------------------------------------------------------------------------
template <typename T>
class ABB {
private:
    NodoArbol<T>* raiz;

public:
    ABB() : raiz(nullptr) {}

    // Liberar un arbol = POST-ORDEN recursivo: primero los hijos, despues el padre
    ~ABB() { destruir(raiz); }

    void destruir(NodoArbol<T>* nodo) {
        if (nodo == nullptr) return;
        destruir(nodo->izquierda);   // subarbol izquierdo
        destruir(nodo->derecha);     // subarbol derecho
        delete nodo;                 // el padre se borra al final
    }

    // ------------------------------------------------------------------
    // 1) INSERTAR
    //    Version recursiva: el metodo recibe el nodo y devuelve el nodo
    //    (posiblemente uno nuevo). El metodo publico solo pasa `raiz`.
    // ------------------------------------------------------------------
    void insertar(T valor) { raiz = insertarEn(raiz, valor); }

    NodoArbol<T>* insertarEn(NodoArbol<T>* nodo, T valor) {
        if (nodo == nullptr) return new NodoArbol<T>(valor);   // se encontro el hueco

        if (valor < nodo->dato) {
            nodo->izquierda = insertarEn(nodo->izquierda, valor);
        } else if (valor > nodo->dato) {
            nodo->derecha = insertarEn(nodo->derecha, valor);
        }
        // si valor == nodo->dato no se inserta: el ABB no guarda duplicados

        return nodo;
    }

    // ------------------------------------------------------------------
    // 2) BUSCAR (version iterativa, sin recursion)
    // ------------------------------------------------------------------
    bool buscar(T valor) const {
        NodoArbol<T>* actual = raiz;
        while (actual != nullptr) {
            if      (valor == actual->dato) return true;
            else if (valor <  actual->dato) actual = actual->izquierda;
            else                            actual = actual->derecha;
        }
        return false;
    }

    // ------------------------------------------------------------------
    // 3) LOS TRES RECORRIDOS
    //    El nombre indica DONDE aparece la raiz respecto a sus hijos.
    // ------------------------------------------------------------------
    void preOrden(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return;
        std::cout << nodo->dato << " ";
        preOrden(nodo->izquierda);
        preOrden(nodo->derecha);
    }

    void inOrden(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return;
        inOrden(nodo->izquierda);
        std::cout << nodo->dato << " ";
        inOrden(nodo->derecha);
    }

    void postOrden(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return;
        postOrden(nodo->izquierda);
        postOrden(nodo->derecha);
        std::cout << nodo->dato << " ";
    }

    void mostrarPreOrden(const char* et = "") const  { std::cout << et; preOrden(raiz);  std::cout << "\n"; }
    void mostrarInOrden(const char* et = "") const   { std::cout << et; inOrden(raiz);   std::cout << "\n"; }
    void mostrarPostOrden(const char* et = "") const { std::cout << et; postOrden(raiz); std::cout << "\n"; }

    // ------------------------------------------------------------------
    // 4) PRE-ORDEN ITERATIVO con pila
    //    Se apila el derecho y luego el izquierdo: como la pila es LIFO,
    //    el izquierdo sale primero y se respeta el orden pre-orden.
    // ------------------------------------------------------------------
    void preOrdenIterativo() const {
        if (raiz == nullptr) return;

        std::stack<NodoArbol<T>*> pila;
        pila.push(raiz);

        while (!pila.empty()) {
            NodoArbol<T>* nodo = pila.top();
            pila.pop();
            std::cout << nodo->dato << " ";

            if (nodo->derecha  != nullptr) pila.push(nodo->derecha);
            if (nodo->izquierda != nullptr) pila.push(nodo->izquierda);
        }
    }

    // ------------------------------------------------------------------
    // 11) RECORRIDO POR NIVELES (BFS) usando una COLA
    // ------------------------------------------------------------------
    void nivelOrden() const {
        if (raiz == nullptr) return;

        std::queue<NodoArbol<T>*> pendientes;
        pendientes.push(raiz);

        while (!pendientes.empty()) {
            NodoArbol<T>* nodo = pendientes.front();
            pendientes.pop();
            std::cout << nodo->dato << " ";

            if (nodo->izquierda != nullptr) pendientes.push(nodo->izquierda);
            if (nodo->derecha  != nullptr) pendientes.push(nodo->derecha);
        }
    }

    // ------------------------------------------------------------------
    // 5) ALTURA: 1 + max(altura izquierda, altura derecha)
    //    Una hoja tiene altura 1; un arbol vacio tiene altura 0.
    // ------------------------------------------------------------------
    int altura(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return 0;

        int izq = altura(nodo->izquierda);
        int der = altura(nodo->derecha);
        return 1 + (izq > der ? izq : der);
    }

    int altura() const { return altura(raiz); }

    // ------------------------------------------------------------------
    // 6) ES LLENO: todos los niveles completos (el ultimo tambien)
    // ------------------------------------------------------------------
    bool esLleno(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return true;                    // subarbol vacio: OK

        // Si es HOJA (ningun hijo), su nivel esta completo.
        if (nodo->izquierda == nullptr && nodo->derecha == nullptr) return true;

        // Si tiene UN SOLO hijo, el nivel esta incompleto.
        if (nodo->izquierda == nullptr || nodo->derecha == nullptr) return false;

        // Tiene los dos: ambos subarboles deben estar llenos.
        return esLleno(nodo->izquierda) && esLleno(nodo->derecha);
    }

    bool esLleno() const { return esLleno(raiz); }

    // ------------------------------------------------------------------
    // 7) ES BALANCEADO: la diferencia de alturas de los hijos es <= 1
    //    en todos los nodos.
    // ------------------------------------------------------------------
    bool esBalanceado(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return true;

        int izq = altura(nodo->izquierda);
        int der = altura(nodo->derecha);
        if (izq - der > 1 || der - izq > 1) return false;

        return esBalanceado(nodo->izquierda) && esBalanceado(nodo->derecha);
    }

    bool esBalanceado() const { return esBalanceado(raiz); }

    // ------------------------------------------------------------------
    // 8) OTRAS MEDIDAS
    // ------------------------------------------------------------------
    int contarNodos(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return 0;
        return 1 + contarNodos(nodo->izquierda) + contarNodos(nodo->derecha);
    }

    int contarNodos() const { return contarNodos(raiz); }

    int sumaValores(NodoArbol<T>* nodo) const {
        if (nodo == nullptr) return 0;
        return nodo->dato + sumaValores(nodo->izquierda) + sumaValores(nodo->derecha);
    }

    int sumaValores() const { return sumaValores(raiz); }

    T minimo() const {
        NodoArbol<T>* a = raiz;
        while (a != nullptr && a->izquierda != nullptr) a = a->izquierda;  // siempre a la izquierda
        return a != nullptr ? a->dato : T();
    }

    T maximo() const {
        NodoArbol<T>* a = raiz;
        while (a != nullptr && a->derecha != nullptr) a = a->derecha;    // siempre a la derecha
        return a != nullptr ? a->dato : T();
    }

    // ------------------------------------------------------------------
    // 9) SON IDENTICOS: misma forma y mismos datos
    // ------------------------------------------------------------------
    static bool sonIdenticos(NodoArbol<T>* a, NodoArbol<T>* b) {
        if (a == nullptr && b == nullptr) return true;    // ambos vacios
        if (a == nullptr || b == nullptr) return false;   // solo uno vacio

        return a->dato == b->dato &&
               sonIdenticos(a->izquierda, b->izquierda) &&
               sonIdenticos(a->derecha,  b->derecha);
    }

    bool esIgual(const ABB& otra) const { return sonIdenticos(raiz, otra.raiz); }

    NodoArbol<T>* obtenerRaiz() const { return raiz; }

    // ------------------------------------------------------------------
    // 10) MEZCLAR UNA LISTA
    // ------------------------------------------------------------------
    void mezclarLista(const std::vector<T>& datos) {
        for (T v : datos) insertar(v);
    }

    // ------------------------------------------------------------------
    // 12) BORRAR: los tres casos
    //
    //    caso 1: nodo sin hijos      -> se borra directamente
    //    caso 2: nodo con un hijo    -> el hijo ocupa su lugar
    //    caso 3: nodo con dos hijos  -> se sube el minimo del subarbol derecho
    // ------------------------------------------------------------------
    NodoArbol<T>* minimoDe(NodoArbol<T>* nodo) {
        while (nodo->izquierda != nullptr) nodo = nodo->izquierda;
        return nodo;
    }

    void borrar(T valor) { raiz = borrarEn(raiz, valor); }

    NodoArbol<T>* borrarEn(NodoArbol<T>* nodo, T valor) {
        if (nodo == nullptr) return nullptr;   // no existia: no hay nada que hacer

        if (valor < nodo->dato) {
            nodo->izquierda = borrarEn(nodo->izquierda, valor);
        } else if (valor > nodo->dato) {
            nodo->derecha = borrarEn(nodo->derecha, valor);
        } else {
            // === se encontro el nodo ===
            if (nodo->izquierda == nullptr) return nodo->derecha;   // casos 1 y 2
            if (nodo->derecha == nullptr)  return nodo->izquierda;  // caso 2

            // caso 3: se copia el VALOR del sucesor (minimo del subarbol derecho)
            NodoArbol<T>* sucesor = minimoDe(nodo->derecha);
            nodo->dato = sucesor->dato;                             // dato
            nodo->derecha = borrarEn(nodo->derecha, sucesor->dato); // y se borra el original
        }
        return nodo;
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 13: arbol binario de busqueda ===\n\n";

    // --- Construccion ---------------------------------------------------------
    std::cout << "-- Construccion del arbol --\n";
    std::cout << "  Insertando: 50, 30, 70, 20, 40, 60, 80\n\n";
    std::cout << "                   [50]\n";
    std::cout << "                  /    \\\n";
    std::cout << "             [30]      [70]\n";
    std::cout << "             /   \\      /   \\\n";
    std::cout << "        [20]  [40]  [60]  [80]\n\n";

    ABB<int> arbol;
    int datos[] = {50, 30, 70, 20, 40, 60, 80};
    for (int d : datos) arbol.insertar(d);

    // --- 3) Recorridos --------------------------------------------------------
    std::cout << "-- 3) Recorridos --\n";
    arbol.mostrarPreOrden("  Pre-orden  (raiz, izq, der): ");
    arbol.mostrarInOrden("  In-orden   (izq, raiz, der): ");
    arbol.mostrarPostOrden("  Post-orden (izq, der, raiz): ");
    std::cout << "  El in-orden sale ORDENADO porque es un ABB.\n\n";

    // --- 4) Recorrido iterativo ----------------------------------------------
    std::cout << "-- 4) Pre-orden iterativo (con pila) --\n";
    std::cout << "  ";
    arbol.preOrdenIterativo();
    std::cout << "  <- debe coincidir con el pre-orden recursivo\n\n";

    // --- 11) Recorrido por niveles (BFS con cola) ----------------------------
    std::cout << "-- 11) Recorrido por niveles (con cola) --\n";
    std::cout << "  ";
    arbol.nivelOrden();
    std::cout << "  <- sale de arriba hacia abajo\n\n";

    // --- 2) Buscar ------------------------------------------------------------
    std::cout << "-- 2) Buscar --\n";
    int busquedas[] = {50, 20, 80, 99};
    for (int v : busquedas) {
        std::cout << "  buscar(" << v << ") = " << std::boolalpha << arbol.buscar(v) << "\n";
    }
    std::cout << "\n";

    // --- 5) a 8) Medidas -------------------------------------------------------
    std::cout << "-- 5) a 8) Medidas del arbol --\n";
    std::cout << "  altura        = " << arbol.altura() << "\n";
    std::cout << "  contarNodos   = " << arbol.contarNodos() << "\n";
    std::cout << "  sumaValores   = " << arbol.sumaValores() << "\n";
    std::cout << "  minimo        = " << arbol.minimo() << "\n";
    std::cout << "  maximo        = " << arbol.maximo() << "\n";
    std::cout << "  esLleno       = " << arbol.esLleno() << "\n";
    std::cout << "  esBalanceado  = " << arbol.esBalanceado() << "\n\n";

    // --- 9) Comparar dos arboles ---------------------------------------------
    std::cout << "-- 9) sonIdenticos --\n";
    ABB<int> otro;
    for (int d : datos) otro.insertar(d);
    std::cout << "  Dos arboles con los mismos datos -> " << arbol.esIgual(otro) << "\n";

    ABB<int> distinto;
    for (int d : datos) distinto.insertar(d);
    distinto.borrar(20);
    std::cout << "  Uno sin el 20                    -> " << arbol.esIgual(distinto) << "\n\n";

    // --- 12) Borrado: los tres casos -----------------------------------------
    std::cout << "-- 12) Borrado (los tres casos) --\n";
    std::cout << "  Arbol inicial: ";
    arbol.mostrarInOrden("");

    // caso 1: hoja
    arbol.borrar(20);
    std::cout << "  Caso 1 (hoja 20):            ";
    arbol.mostrarInOrden("");

    // caso 2: un solo hijo (el 30 ya solo tiene al 40)
    arbol.borrar(30);
    std::cout << "  Caso 2 (un solo hijo, 30):   ";
    arbol.mostrarInOrden("");

    // caso 3: dos hijos (la raiz 50 tiene al 40 y al 70)
    arbol.borrar(50);
    std::cout << "  Caso 3 (dos hijos, 50):      ";
    arbol.mostrarInOrden("");

    // borrar algo inexistente
    arbol.borrar(999);
    std::cout << "  Borrar 999 (no existe):      ";
    arbol.mostrarInOrden("");

    std::cout << "  buscar(30) tras los borrados = " << std::boolalpha << arbol.buscar(30) << "\n\n";

    // --- Caso limite: arbol vacio --------------------------------------------
    std::cout << "-- Caso limite: ABB vacio --\n";
    ABB<int> vacio;
    std::cout << "  buscar(1)  = " << vacio.buscar(1) << "\n";
    std::cout << "  altura    = " << vacio.altura() << "\n";
    std::cout << "  esLleno   = " << vacio.esLleno() << "\n";
    std::cout << "  in-orden  = ";
    vacio.mostrarInOrden("");
    vacio.borrar(5);   // no debe romper nada
    std::cout << "  borrar(5) en arbol vacio: sin error\n";
    std::cout << "  esBalanceado = " << vacio.esBalanceado() << "\n\n";

    // --- 10) Mezclar una lista -----------------------------------------------
    std::cout << "-- 10) mezclarLista --\n";
    ABB<int> grande;
    std::vector<int> lista = {40, 20, 60, 10, 30, 50, 70, 80, 25};
    grande.mezclarLista(lista);
    grande.mostrarInOrden("  In-orden: ");
    std::cout << "  nodos = " << grande.contarNodos()
              << "  altura = " << grande.altura()
              << "  balanceado = " << std::boolalpha << grande.esBalanceado() << "\n\n";

    // --- ABB degenerado: el peor caso ----------------------------------------
    std::cout << "-- ABB degenerado (insertar en orden creciente) --\n";
    ABB<int> cadena;
    for (int i = 1; i <= 5; i++) cadena.insertar(i);   // 1,2,3,4,5 -> se vuelve una lista
    cadena.mostrarInOrden("  In-orden: ");
    std::cout << "  altura = " << cadena.altura() << "  (un ABB bien construido daria 3)\n";
    std::cout << "  balanceado = " << cadena.esBalanceado() << "  (por eso existen AVL y rojo-negro)\n";

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 13: arbol binario de busqueda ===
    -- Construccion del arbol --
      Insertando: 50, 30, 70, 20, 40, 60, 80
                       [50]
                      /    \
                 [30]      [70]
                 /   \      /   \
            [20]  [40]  [60]  [80]
    -- 3) Recorridos --
      Pre-orden  (raiz, izq, der): 50 30 20 40 70 60 80
      In-orden   (izq, raiz, der): 20 30 40 50 60 70 80
      Post-orden (izq, der, raiz): 20 40 30 60 80 70 50
      El in-orden sale ORDENADO porque es un ABB.
    -- 4) Pre-orden iterativo (con pila) --
      50 30 20 40 70 60 80   <- debe coincidir con el pre-orden recursivo
    -- 11) Recorrido por niveles (con cola) --
      50 30 70 20 40 60 80   <- sale de arriba hacia abajo
    -- 2) Buscar --
      buscar(50) = true
      buscar(20) = true
      buscar(80) = true
      buscar(99) = false
    -- 5) a 8) Medidas del arbol --
      altura        = 3
      contarNodos   = 7
      sumaValores   = 350
      minimo        = 20
      maximo        = 80
      esLleno       = true
      esBalanceado  = true
    -- 9) sonIdenticos --
      Dos arboles con los mismos datos -> true
      Uno sin el 20                    -> false
    -- 12) Borrado (los tres casos) --
      Arbol inicial: 20 30 40 50 60 70 80
      Caso 1 (hoja 20):            30 40 50 60 70 80
      Caso 2 (un solo hijo, 30):   40 50 60 70 80
      Caso 3 (dos hijos, 50):      40 60 70 80
      Borrar 999 (no existe):      40 60 70 80
      buscar(30) tras los borrados = false
    -- Caso limite: ABB vacio --
      buscar(1)  = false
      altura    = 0
      esLleno   = true
      in-orden  =
      borrar(5) en arbol vacio: sin error
      esBalanceado = true
    -- 10) mezclarLista --
      In-orden: 10 20 25 30 40 50 60 70 80
      nodos = 9  altura = 4  balanceado = true
    -- ABB degenerado (insertar en orden creciente) --
      In-orden: 1 2 3 4 5
      altura = 5  (un ABB bien construido daria 3)
      balanceado = false  (por eso existen AVL y rojo-negro)
    Demostracion completada.
*/
