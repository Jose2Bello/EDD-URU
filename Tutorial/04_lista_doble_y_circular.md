# 04. Lista doblemente enlazada y lista circular

Las dos mejoras naturales de la lista simplemente enlazada: **poder ir hacia atrás**
(lista doble) y **no tener un final (nullptr)** (lista circular).

---

## 1. Lista doblemente enlazada

Cada nodo guarda **dos** punteros: `anterior` y `siguiente`. Además la lista guarda
`head` y `tail` para poder alcanzar ambos extremos en O(1).

```
        head                                      tail
         |                                         |
         v                                         v
 nullptr <-+---+------+      +------+------+      +------+---+-> nullptr
            | 10 |      |      | 20  |      |      | 30  |   |
            +---+------+ <-- +-----+------+ <--- +------+---+
              prev/next       prev/next              prev/next
```

### Nodo

```cpp
template <typename T>
class NodoD {
public:
    T dato;
    NodoD<T>* anterior;
    NodoD<T>* siguiente;

    NodoD(T valor) : dato(valor), anterior(nullptr), siguiente(nullptr) {}
};
```

### Las cuatro operaciones clave

**Insertar al inicio:**
```cpp
void agregarAlInicio(T valor) {
    NodoD<T>* nuevo = new NodoD<T>(valor);
    nuevo->siguiente = head;                 // 1) el nuevo apunta a la vieja cabeza
    if (head != nullptr)
        head->anterior = nuevo;              // 2) la vieja cabeza mira hacia atrás
    head = nuevo;                            // 3) nueva cabeza
    if (tail == nullptr) tail = nuevo;       // 4) si estaba vacía, tail también
}
```

**Insertar al final:**
```cpp
void agregarAlFinal(T valor) {
    NodoD<T>* nuevo = new NodoD<T>(valor);
    nuevo->anterior = tail;
    if (tail != nullptr)
        tail->siguiente = nuevo;
    tail = nuevo;
    if (head == nullptr) head = nuevo;
}
```

**Eliminar un nodo cualquiera (tenemos su puntero, así que es O(1)):**
```cpp
bool eliminarNodo(NodoD<T>* nodo) {
    if (nodo == nullptr) return false;

    if (nodo->anterior != nullptr) nodo->anterior->siguiente = nodo->siguiente;
    else                          head = nodo->siguiente;      // era la cabeza

    if (nodo->siguiente != nullptr) nodo->siguiente->anterior = nodo->anterior;
    else                           tail = nodo->anterior;       // era la cola

    delete nodo;
    return true;
}
```

**Invertir la lista (reutilizando nodos, no creando nuevos):**
```cpp
void invertir() {
    NodoD<T>* actual = head;
    std::swap(head, tail);          // las puntas se intercambian
    while (actual != nullptr) {
        NodoD<T>* sig = actual->siguiente;
        std::swap(actual->anterior, actual->siguiente);  // se voltea el nodo
        actual = sig;
    }
}
```

### ¿Cuándo usar lista doble?

| Situación | Lista doble |
|---|---|
| El cursor se mueve mucho hacia adelante y hacia atrás (editor de texto, historial) | Sí |
| Necesitas obtener el último elemento seguido | Sí (con `tail`) |
| Solo agregas al final y recorres una vez (registro de alumnos) | No, la simple basta |

Coste: cada nodo duplica la memoria de punteros (16 bytes en vez de 8 en 64 bits).

📄 Implementación completa: [`09_lista_doble.cpp`](../Ejercicios/Resueltos/09_lista_doble.cpp)

---

## 2. Lista circular

El último nodo **no apunta a `nullptr`, sino al primer nodo**. Se usa para
estructuras donde "no hay fin": buffers de audio, colas de prioridad, turnos rotativos,
y el famoso problema de **Josephus**.

```
   head
    |
    v
  +------+------+      +------+------+      +------+------+
  | 10   | next | ---> | 20   | next | ---> | 30   | next | ---+
  +------+------+      +------+------+      +------+------+   |
    ^                                                         |
    +---------------------------------------------------------+
```

### Implementación mínima

```cpp
template <typename T>
class ListaCircular {
private:
    Nodo<T>* head;
    Nodo<T>* tail;

public:
    ListaCircular() : head(nullptr), tail(nullptr) {}

    ~ListaCircular() {
        if (head == nullptr) return;
        Nodo<T>* actual = head;
        do {                       // do-while: al menos una vuelta
            Nodo<T>* sig = actual->siguiente;
            delete actual;
            actual = sig;
        } while (actual != head);
    }

    void agregarAlFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (head == nullptr) {          // lista vacía: el nodo se apunta a sí mismo
            head = tail = nuevo;
            nuevo->siguiente = nuevo;
            return;
        }
        tail->siguiente = nuevo;
        nuevo->siguiente = head;        // cerrar el círculo
        tail = nuevo;
    }

    void mostrar() const {
        if (head == nullptr) { std::cout << "vacia"; return; }
        Nodo<T>* actual = head;
        do {
            std::cout << "(" << actual->dato << ") -> ";
            actual = actual->siguiente;
        } while (actual != head);
        std::cout << "(vuelve a head)";
    }
};
```

### ⚠️ La trampa mortal: el recorrido infinito

```cpp
// MAL: en una lista circular esto NUNCA termina
while (actual != nullptr) { ... }

// BIEN: hay que comparar contra head, no contra nullptr
while (actual != head) { ... }

// BIEN: do-while (necesita al menos una vuelta para mostrar un nodo)
do { ... } while (actual != head);
```

### Aplicación clásica: Josephus

*"N personas en círculo se numeran de 1 a N. Se empieza contando en 1 y cada K personas
se elimina una persona del círculo, empezando de nuevo en el siguiente. ¿A quién le
tocó el último turno?"*

Se resuelve **O(n·k)** con lista circular: se avanza K-1 posiciones y se elimina el nodo.

📄 Implementación completa: [`10_josephus_circular.cpp`](../Ejercicios/Resueltos/10_josephus_circular.cpp)

---

## 3. Tabla comparativa de las tres variantes

| Característica | Simple | Doble | Circular |
|---|---|---|---|
| Punteros por nodo | 1 (`siguiente`) | 2 (`anterior`, `siguiente`) | 1 (`siguiente`) |
| Termina en | `nullptr` | `nullptr` en ambos extremos | la cabeza |
| Recorrer hacia atrás | No | **Sí** | No |
| Insertar al final | O(n) | O(1) con `tail` | O(1) con `tail` |
| Insertar/eliminar junto a un nodo conocido | O(n) | **O(1)** | O(n) |
| Ahorro de memoria | **Máximo** | Mínimo | Alto |

---

## 4. Ejercicios ípicos que te van a pedir

1. Implementa la lista doble con `insertar`, `eliminar`, `buscar`, `mostrar` y `largo`.
2. Dada una lista simple, crea su versión doble sin perder datos.
3. Dada una lista doble, crea su versión simple.
4. Convierte una lista circular en simplemente enlazada.
5. Invierte una lista circular manteniendo la circularidad.
6. Elimina los elementos en posiciones pares de una circular.
7. Josephus con K variable (imprime quién sale en cada ronda).

📄 Las soluciones 1, 4, 5 y 7 están en los ejercicios 09 y 10.

---

## Siguiente tutorial → [05. Listas ordenadas y operaciones clásicas](05_listas_ordenadas.md)
