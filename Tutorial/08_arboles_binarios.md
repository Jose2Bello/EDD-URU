# 08. Árboles binarios y árboles de búsqueda

Un **árbol** es una estructura jerárquica: tiene una raíz, cada nodo tiene un padre y puede
tener hijos. Un **árbol binario** tiene como máximo 2 hijos por nodo (izquierdo y derecho).
El **ABB (Árbol Binario de Búsqueda)** además cumple una propiedad de orden.

---

## 1. Anatomía del ABB

```
                    [ 50 ]
                   /      \
               [ 30 ]      [ 70 ]
               /    \        /    \
           [ 20 ]  [ 40 ]  [ 60 ]  [ 80 ]
```

**Propiedad del ABB:** para todo nodo, todo lo del subárbol izquierdo es **menor** que él,
y todo lo del subárbol derecho es **mayor**.

| Ventaja | Coste |
|---|---|
| Buscar un valor: en cada paso se descarta la mitad del árbol | **O(log n)** |
| Inserción ordenada: mismo recorrido | O(log n) |
| Recorrer en orden: `izq → raíz → der` sale ordenado de menor a mayor | O(n) |

🌲 Con **1000 elementos** el ABB tiene altura ~10 (2¹⁰ = 1024). Con 1 000 000, altura ~20.
Por eso "dividir y vencerás" es tan poderoso.

⚠️ El ABB **no siempre** es balanceado. Si insertas 1, 2, 3, 4, 5, se convierte en una
lista y todo pasa a ser O(n). La solución son los **AVL** o **rojo-negro**, que los
implementa la librería estándar (`std::set`, `std::map`).

---

## 2. El nodo

```cpp
template <typename T>
class NodoArbol {
public:
    T dato;
    NodoArbol<T>* izquierda;
    NodoArbol<T>* derecha;

    NodoArbol(T valor)
        : dato(valor), izquierda(nullptr), derecha(nullptr) {}
};
```

En un árbol **no hay lista**: no existe destructor en la clase árbol, se hace recursivo:

```cpp
void eliminarTodo(NodoArbol<T>* nodo) {
    if (nodo == nullptr) return;
    eliminarTodo(nodo->izquierda);
    eliminarTodo(nodo->derecha);
    delete nodo;
}
```

El orden `izq → der → delete` se llama **post-orden** y es el correcto: si borras antes a
los hijos, pierdes la dirección para llegar a ellos.

---

## 3. Inserción en el ABB

```cpp
NodoArbol<T>* raiz;

void insertar(T valor) {
    raiz = insertarEn(raiz, valor);
}

NodoArbol<T>* insertarEn(NodoArbol<T>* nodo, T valor) {
    if (nodo == nullptr) return new NodoArbol<T>(valor);   // encontré el hueco

    if (valor < nodo->dato)       nodo->izquierda = insertarEn(nodo->izquierda, valor);
    else if (valor > nodo->dato)  nodo->derecha  = insertarEn(nodo->derecha, valor);
    // si es igual, no se inserta (sin duplicados)

    return nodo;
}
```

**Patrón universal de árboles:** si un método puede recibir `nullptr`, escríbelo para que
acepte el nodo y devuelva el nodo (posiblemente nuevo). El llamador público solo pasa
`raiz`. Así no duplicas la lógica.

Diagrama de inserción de 50, 30, 70, 20:

```
   insertar(50):            [50]
   insertar(30):   30 < 50  [50]
                             /  \
                          [30]
   insertar(70):   70 > 50        [70]  (a la derecha del 50)
                            /
   insertar(20):   20 < 30   [30]
                          /
                       [20]
```

---

## 4. Los tres recorridos

```cpp
void preOrden(NodoArbol<T>* n) {        // RAÍZ - IZQ - DER
    if (n == nullptr) return;
    std::cout << n->dato << " ";
    preOrden(n->izquierda);
    preOrden(n->derecha);
}

void inOrden(NodoArbol<T>* n) {         // IZQ - RAÍZ - DER  (orden natural)
    if (n == nullptr) return;
    inOrden(n->izquierda);
    std::cout << n->dato << " ";
    inOrden(n->derecha);
}

void postOrden(NodoArbol<T>* n) {       // IZQ - DER - RAÍZ (para borrar)
    if (n == nullptr) return;
    postOrden(n->izquierda);
    postOrden(n->derecha);
    std::cout << n->dato << " ";
}
```

Con el árbol `[50], [30,70], [20,40,60,80]`:

| Recorrido | Secuencia |
|---|---|
| Pre-orden | 50 30 20 40 70 60 80 |
| In-orden | **20 30 40 50 60 70 80** (¡ordenado!) |
| Post-orden | 20 40 30 60 80 70 50 |

Mnemotecnia: el nombre dice **dónde va la raíz** respecto a sus hijos.

---

## 5. Búsqueda

```cpp
bool buscar(NodoArbol<T>* nodo, T valor) {
    while (nodo != nullptr) {          // versión iterativa: sin recursión
        if      (valor == nodo->dato) return true;
        else if (valor <  nodo->dato) nodo = nodo->izquierda;
        else                         nodo = nodo->derecha;
    }
    return false;
}
```

Cada paso descarta ~la mitad de los elementos: **O(log n)**.

---

## 6. Borrado: los tres casos

Es la operación más difícil. Solo hay que "matar" un nodo sin perder el subárbol.

```
Caso 1: hoja (sin hijos)  ->  se borra directamente
Caso 2: un solo hijo      ->  el hijo ocupa su lugar
Caso 3: dos hijos         ->  se copia el successor in-order
```

```cpp
NodoArbol<T>* minimo(NodoArbol<T>* n) {
    while (n->izquierda != nullptr) n = n->izquierda;   // siempre a la izquierda
    return n;
}

NodoArbol<T>* borrar(NodoArbol<T>* nodo, T valor) {
    if (nodo == nullptr) return nullptr;

    if (valor < nodo->dato) {
        nodo->izquierda = borrar(nodo->izquierda, valor);
    } else if (valor > nodo->dato) {
        nodo->derecha = borrar(nodo->derecha, valor);
    } else {
        // === lo encontramos ===
        if (nodo->izquierda == nullptr) return nodo->derecha;   // caso 1 y 2
        if (nodo->derecha == nullptr)  return nodo->izquierda;  // caso 2

        // caso 3: dos hijos. Se sube el mínimo del subárbol derecho
        NodoArbol<T>* sucesor = minimo(nodo->derecha);
        nodo->dato = sucesor->dato;                            // se copia el VALOR
        nodo->derecha = borrar(nodo->derecha, sucesor->dato);   // y se borra el original
    }
    return nodo;
}
```

Ejemplo de borrado con dos hijos:

```
Borrar 30 de:      [50]
                  /    \
               [30]     [70]
              /   \    /   \
           [20] [40][60] [80]

Paso 1: successor in-order de 30 = el mínimo de su subárbol derecho = 40
Paso 2: 30.dato = 40.dato
                  [50]
                  /    \
               [40]     [70]
              /   \    /   \
           [20]    [] [60] [80]
Paso 3: borrar el 40 original (ahora es una hoja)
                  [50]
                  /    \
               [40]     [70]
                  \    /   \
                   [60] [80]
```

---

## 7. Otras operaciones útiles

```cpp
int altura(NodoArbol<T>* n) {
    if (n == nullptr) return 0;
    int izq = altura(n->izquierda);
    int der = altura(n->derecha);
    return 1 + (izq > der ? izq : der);          // el mayor de los dos, más 1
}

bool esLleno(NodoArbol<T>* n) {                 // todos los niveles completos
    if (n == nullptr) return true;
    if (n->izquierda == nullptr || n->derecha == nullptr) return false;
    return esLleno(n->izquierda) && esLleno(n->derecha);
}

bool sonIdenticos(NodoArbol<T>* a, NodoArbol<T>* b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    return a->dato == b->dato &&
           sonIdenticos(a->izquierda, b->izquierda) &&
           sonIdenticos(a->derecha, b->derecha);
}

int contarNodos(NodoArbol<T>* n) {
    if (n == nullptr) return 0;
    return 1 + contarNodos(n->izquierda) + contarNodos(n->derecha);
}

int sumaValores(NodoArbol<T>* n) {
    if (n == nullptr) return 0;
    return n->dato + sumaValores(n->izquierda) + sumaValores(n->derecha);
}
```

---

## 8. Recorrido iterativo con pila (evita desbordamiento en árboles grandes)

La recursión es elegante, pero con 100 000 nodos en una rama la pila del sistema revienta.
La versión iterativa usa una pila explícita:

```cpp
void preOrdenIterativo(NodoArbol<T>* raiz) {
    if (raiz == nullptr) return;
    std::stack<NodoArbol<T>*> pila;
    pila.push(raiz);
    while (!pila.empty()) {
        NodoArbol<T>* n = pila.top();
        pila.pop();
        std::cout << n->dato << " ";
        if (n->derecha  != nullptr) pila.push(n->derecha);   // primero el derecho
        if (n->izquierda != nullptr) pila.push(n->izquierda); // se atiende antes
    }
}
```

💡 Ejercicio mental: **inversión de un árbol** (intercambiar izquierda y derecha de todos
los nodos) también se puede hacer con dos punteros como en una lista doble.

📄 Ejercicio completo: [`13_arbol_busqueda.cpp`](../Ejercicios/Resueltos/13_arbol_busqueda.cpp)

---

## 9. Tabla resumen

| Operación | ABB promedio | ABB desbalanceado |
|---|---|---|
| Buscar | O(log n) | O(n) |
| Insertar | O(log n) | O(n) |
| Borrar | O(log n) | O(n) |
| Recorrer (cualquiera) | O(n) | O(n) |
| Obtener mínimo / máximo | O(log n) | O(n) |

---

## Siguiente tutorial → [09. Complejidad y buenas prácticas](09_complejidad_y_practicas.md)