# 07. Colas y Deque

La **cola** es **FIFO** (*First In, First Out*): el primero que entra es el primero que sale.
Como una fila de supermercado: se forma por detrás y se atiende por delante.

---

## 1. Operaciones de una cola

```
       encolar (entra por atrás)
             |
             v
  +------+------+------+------+        frente
  |  1   |  2   |  3   |  4   | <---- (sale por aquí)
  +------+------+------+------+
            final            ^
              |              |
        desencolar (sale por el frente)
```

| Operación | Qué hace | Coste |
|---|---|---|
| `encolar(x)` / `push` | mete al final | **O(1)** |
| `desencolar()` / `pop` | saca del frente | **O(1)** |
| `frente()` / `front` | mira el primero sin sacarlo | **O(1)** |
| `final()` / `back` | mira el último | **O(1)** con `tail` |
| `estaVacia()` | ¿vacía? | **O(1)** |
| `tamano()` | cuántos hay | O(1) con contador |

---

## 2. Implementación con dos punteros: `front` y `back`

La diferencia clave frente a la pila: aquí **guardamos los dos extremos**, porque se
inserta por un lado y se elimina por el otro.

```cpp
template <typename T>
class Cola {
private:
    Nodo<T>* front;   // sale por aquí
    Nodo<T>* back;    // entra por aquí

public:
    Cola() : front(nullptr), back(nullptr) {}

    ~Cola() {
        while (front != nullptr) {
            Nodo<T>* sig = front->siguiente;
            delete front;
            front = sig;
        }
        back = nullptr;
    }

    void encolar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = nullptr;      // siempre será el último
        if (back == nullptr) {
            front = back = nuevo;        // estaba vacía
        } else {
            back->siguiente = nuevo;     // el antiguo último apunta al nuevo
            back = nuevo;                // el nuevo es el último
        }
    }

    bool desencolar(T& salida) {
        if (front == nullptr) return false;
        Nodo<T>* aBorrar = front;
        salida = front->dato;
        front = front->siguiente;
        delete aBorrar;
        if (front == nullptr) back = nullptr;   // se vació: back también
        return true;
    }

    bool primero(T& salida) const {
        if (front == nullptr) return false;
        salida = front->dato;
        return true;
    }

    bool ultimo(T& salida) const {
        if (back == nullptr) return false;
        salida = back->dato;
        return true;
    }

    bool estaVacia() const { return front == nullptr; }
};
```

**Todo es O(1)** gracias a tener `front` y `back`. Si solo guardaras `front`, encolar sería
O(n) porque habría que recorrer hasta el final. Esa es la diferencia clave que el profesor
quiere ver en el examen.

---

## 3. Cola circular con arreglo (la clásica del coursework)

Un arreglo fijo de tamaño `N`, con dos índices que se mueven y dan la vuelta. Se evita el
`new`/`delete` de nodos y es muy eficiente.

```
Capacidad 5, elementos [10, 20, 30]:
 indice:    0    1    2    3    4
           [10] [20] [30] [ -] [ -]
           ^frente      ^final

Tras encolar 40 y 50 (dan la vuelta):
           [10] [20] [30] [40] [50]
                               ^frente      ^final

Tras desencolar 3 veces:
            [ -] [ -] [ -] [40] [50]
            ^frente              ^final
```

```cpp
class ColaCircular {
private:
    int* datos;
    int frente;      // posición del primer elemento
    int final;       // posición donde se insertará el siguiente
    int capacidad;
    int cantidad;

public:
    explicit ColaCircular(int cap)
        : frente(0), final(0), cantidad(0), capacidad(cap) {
        datos = new int[cap];
    }
    ~ColaCircular() { delete[] datos; }

    bool encolar(int v) {
        if (llena()) return false;
        datos[final] = v;
        final = (final + 1) % capacidad;   // el módulo hace el "envolvente"
        cantidad++;
        return true;
    }

    bool desencolar(int& salida) {
        if (vacia()) return false;
        salida = datos[frente];
        frente = (frente + 1) % capacidad;
        cantidad--;
        return true;
    }

    bool vacia()  const { return cantidad == 0; }
    bool llena()  const { return cantidad == capacidad; }
    int  tamano() const { return cantidad; }
};
```

Detalle importante: el módulo `(i + 1) % capacidad` es lo que hace que los índices "den la
vuelta". Y se necesita `cantidad` (o un índice extra) para diferenciar entre
*vacía* y *llena* cuando `frente == final`.

---

## 4. Cola de prioridad

Cada elemento tiene una prioridad; sale primero el de **mayor** (o menor) prioridad.

```cpp
struct Cliente {
    std::string nombre;
    int prioridad;      // 1 = más urgente
};

template <typename T>
class ColaPrioridad {
private:
    std::vector<T> datos;

public:
    void encolar(const T& nuevo) {
        int i = static_cast<int>(datos.size());
        datos.push_back(nuevo);
        while (i > 0 && datos[i].prioridad < datos[i - 1].prioridad) {
            std::swap(datos[i], datos[i - 1]);
            i--;
        }
    }

    bool desencolar(T& salida) {
        if (datos.empty()) return false;
        salida = datos.front();
        datos.erase(datos.begin());     // en la práctica se usa un heap
        return true;
    }
};
```

---

## 5. Deque (*double-ended queue*): cola por los dos extremos

Permite encolar y desencolar **por el frente y por el final**. En la librería estándar es
`std::deque<T>`.

| Operación | Coste |
|---|---|
| Push/pop por ambos extremos | O(1) |
| Acceder a un elemento | O(1) |
| Insertar/borrar en el medio | O(n) |
| Push/pop por el final | O(1) |

```cpp
#include <deque>
std::deque<int> d;
d.push_back(1);
d.push_front(0);      // 0 1
d.pop_front();        // 1
d.pop_back();         // vacía
```

📄 Implementación con lista doble (un deque es esencialmente una lista doble):
[`09_lista_doble.cpp`](../Ejercicios/Resueltos/09_lista_doble.cpp)

---

## 6. Aplicaciones de la cola

| Aplicación | Explicación |
|---|---|
| **Impresión de trabajos** | El primero que llega se imprime primero |
| **Turnos en un banco** | FIFO puro |
| **BFS (búsqueda en anchura)** | La cola marca el orden en que se exploran los nodos de un grafo o árbol |
| **Búfer de teclado / de red** | Los pulsos tecleados se procesan en orden |
| **Servicio de atención al cliente** | Cada llegada entra al final de la línea |
| **Nivel por nivel de un árbol** | Al vaciar la cola se pasa al siguiente nivel |
| **Simulación de procesos** | El planificador de la CPU (round-robin) |
| **Solicitudes HTTP** | El servidor las atiende en orden de llegada |

---

## 7. Cómo funciona un BFS con cola

```
Grafo:   A --> B --> D
         |     |
         v     v
         C --> E

Cola:   [A]
Sacamos A, metemos sus vecinos B y C:   [B, C]
Sacamos B, metemos D y E:              [C, D, E]
Sacamos C, su vecino E ya estaba:      [D, E]
Sacamos D:                             [E]
Sacamos E:                             []

Orden de visita: A, B, C, D, E   (por niveles, de izquierda a derecha)
```

💡 Por eso BFS necesita una cola: es la única forma de "recordar" a quién visitar después
mientras avanzas en horizontal.

---

## Siguiente tutorial → [08. Árboles binarios y ABB](08_arboles_binarios.md)