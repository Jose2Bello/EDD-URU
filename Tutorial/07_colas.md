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

## 8. Anatomía: por qué se guardan dos punteros

```
        objeto Cola<int>
        +----------------------+         +----------------------+
        |  frente: Nodo<T>*    |         |  cola: Nodo<T>*      |
        +----------------------+         +----------------------+
                 |                                  |
                 v                                  v
        +---------------+     +---------------+   +---------------+
        | dato: 10      | --> | dato: 20      |-> | dato: 30      |
        | siguiente: *  |     | siguiente: *  |   | siguiente:null|
        +---------------+     +---------------+   +---------------+
             frente                                  cola (back)
             sale                                     entra
```

Con un único puntero al frente, `encolar` tendría que recorrer toda la cola
hasta el final → **O(n)**. Con `cola`/`back` guardado, todo es **O(1)**. Esa es
la frase que el profesor quiere leer en el examen.

---

## 9. Operaciones que suelen pedir además

### 9.1 Copia profunda

Igual que en la pila: se copia **nodo por nodo**, nunca el puntero.

```cpp
Cola(const Cola& otra) : frente(nullptr), cola(nullptr) {
    copiarDesde(otra.frente);
}

void copiarDesde(Nodo<T>* nodo) {
    if (nodo == nullptr) return;
    encolar(nodo->dato);        // encolar va al final: conserva el orden
    copiarDesde(nodo->siguiente);
}
```

### 9.2 Invertir

Se invierte la cadena de enlaces y se **intercambian** `frente` y `cola`.

```cpp
void invertir() {
    Nodo<T>* anterior = nullptr;
    Nodo<T>* actual = frente;
    while (actual != nullptr) {
        Nodo<T>* siguiente = actual->siguiente;
        actual->siguiente = anterior;
        anterior = actual;
        actual = siguiente;
    }
    Nodo<T>* temporal = frente;
    frente = cola;
    cola = temporal;
}
```

### 9.3 Rotar

Rotar a la izquierda `k` posiciones es desencolar y volver a encolar `k` veces.
Se usa `k %= tamano()` para que `rotar(7)` sobre 5 elementos sea `rotar(2)`.

```cpp
void rotar(int k) {
    int n = tamano();
    if (n == 0) return;
    k %= n;
    for (int i = 0; i < k; i++) {
        T valor;
        desencolar(valor);
        encolar(valor);
    }
}
```

📄 Todo esto implementado: [`09_cola_completa.cpp`](../Plantillas/09_cola_completa.cpp)

---

## 10. Cola implementada con dos pilas

Pregunta clásica. Una cola saca el **más viejo**; una pila saca el **más nuevo**.
Con dos pilas, `salida` actúa de bandeja: al pasar todo `entrada` → `salida`, el
orden se invierte y el más viejo queda arriba.

```cpp
template <typename T>
class ColaConDosPilas {
private:
    Pila<T> entrada;
    Pila<T> salida;

    void pasar() {
        T v;
        while (entrada.desapilar(v)) salida.apilar(v);
    }

public:
    void encolar(T v) { entrada.apilar(v); }

    bool desencolar(T& salidaValor) {
        if (salida.estaVacia()) pasar();   // solo se pasa cuando hace falta
        return salida.desapilar(salidaValor);
    }
};
```

`encolar` es **O(1)**; `desencolar` es **O(1) amortizado**, porque cada elemento
se mueve entre pilas como máximo una vez. Si se pasara en cada `desencolar`, sería
O(n) por operación.

---

## 11. Deque: aplicaciones

| Aplicación | Por qué un deque |
|---|---|
| **Palíndromo** | Se compara sacando por los dos extremos |
| **Ventana deslizante** | Se quitan viejos por delante y se añaden nuevos por detrás |
| **Historial de navegación** | Se avanza y retrocede por ambos lados |
| **Búfer circular** | Se descarta lo viejo por un lado y entra lo nuevo por el otro |

### Palíndromo con deque

```cpp
bool esPalindromoDeque(const char* cadena) {
    Deque<char> d;
    for (int i = 0; cadena[i] != '\0'; i++) d.empujarFinal(cadena[i]);

    char primero, ultimo;
    while (d.tamano() > 1) {
        d.sacarFrente(primero);
        d.sacarFinal(ultimo);
        if (primero != ultimo) return false;
    }
    return true;
}
```

> Un palíndromo también se resuelve apilando toda la cadena y comparando al
> desapilar (ver plantilla 08). El deque evita la copia y además detecta el fallo
> en cuanto aparece.

### Ventana deslizante (máximo de cada ventana de tamaño k)

Problema clásico O(n): mantener en un deque los **índices** de los candidatos en
orden decreciente de valor. El frente es siempre el máximo de la ventana.

```cpp
// Recorre el arreglo y mantiene en el deque solo lo que puede ser máximo.
//   quitar por delante los indices que ya salieron de la ventana
//   quitar por detras los valores menores que el nuevo
```

📄 Resuelto y verificado: [`17_deque_ventana_maxima.cpp`](../Ejercicios/Resueltos/17_deque_ventana_maxima.cpp)

---

## 12. BFS en un laberinto con cola

La cola es **la** estructura de BFS: obliga a explorar por niveles. La primera
vez que se alcanza una casilla, se hace por el camino más corto.

```cpp
int distanciaMinima(char mapa[FILAS][COLS]) {
    int distancia[FILAS][COLS];
    // inicializar todo a -1 (no visitado)
    Cola<Posicion> pendientes;
    pendientes.encolar(inicio);
    distancia[inicio.fila][inicio.columna] = 0;

    while (!pendientes.estaVacia()) {
        Posicion actual; pendientes.desencolar(actual);
        if (esSalida(actual)) return distancia[actual.fila][actual.columna];

        for (cada vecino valido y no visitado) {
            distancia[nf][nc] = distancia[actual.fila][actual.columna] + 1;
            pendientes.encolar({nf, nc});
        }
    }
    return -1;   // sin camino
}
```

📄 Implementado en [`09_cola_completa.cpp`](../Plantillas/09_cola_completa.cpp) y
con reconstrucción de la ruta en [`18_bfs_ruta_laberinto.cpp`](../Ejercicios/Resueltos/18_bfs_ruta_laberinto.cpp)

---

## 13. Cola de prioridad

La cola normal es FIFO; la de prioridad atiende primero al más urgente.
Es un tema aparte, con su propia guía:

➡️ [10. Cola de prioridad](10_cola_de_prioridad.md)

---

## 14. Tabla de complejidades

| Operación | Cola con nodos | Cola circular (arreglo) | Deque |
|---|---|---|---|
| `encolar` por el final | O(1) | O(1) | O(1) |
| `desencolar` por el frente | O(1) | O(1) | O(1) |
| `encolar` / `desencolar` por el frente | no | no | O(1) |
| `primero` / `ultimo` | O(1) | O(1) | O(1) |
| `tamano` | O(n) / O(1) con contador | O(1) | O(n) |
| `invertir` | O(n) | O(n) | O(n) |
| `buscar` | O(n) | O(n) | O(n) |

---

## 15. Autoevaluación (respuestas al final)

1. ¿Por qué una cola necesita dos punteros y una pila solo uno?
2. ¿Qué distingue una cola circular de una cola con arreglo normal?
3. ¿Por qué hace falta un contador en la cola circular?
4. ¿Qué complejidad tiene `desencolar` en una cola con dos pilas?
5. ¿Qué estructura usa BFS y por qué?

<details>
<summary>Respuestas</summary>

1. Porque se inserta por un extremo y se elimina por el otro. La pila hace ambas
   cosas por el mismo extremo (el tope).
2. Que los índices **dan la vuelta** con el módulo, reutilizando las posiciones
   que quedan libres al frente; no hace falta desplazar elementos.
3. Porque `frente == final` ocurre tanto cuando está **vacía** como cuando está
   **llena**. El contador `cantidad` desempata los dos casos.
4. **O(1) amortizado**: cada elemento se pasa de `entrada` a `salida` una sola
   vez en toda su vida.
5. Una **cola**, porque permite recordar los vecinos pendientes y explorar por
   niveles, garantizando así el camino más corto en grafos sin pesos.

</details>

---

## Siguiente tutorial → [08. Árboles binarios y ABB](08_arboles_binarios.md)