# 10. Cola de prioridad (y pila de prioridad)

Una cola normal atiende por **orden de llegada** (FIFO). Una **cola de
prioridad** atiende por **urgencia**: el elemento más urgente sale primero, sin
importar cuándo llegó. Es la estructura de las salas de emergencia, de los
planificadores de procesos y del algoritmo de Dijkstra.

> Esta guía es deliberadamente breve: en el curso el peso está en pilas y colas.

---

## 1. Concepto

Cada elemento entra con una **clave de prioridad**. Convenio habitual:

```
prioridad 1 = MÁS urgente      (menor número = se atiende antes)
```

```
llegan:   Rosa(4)   Carlos(1)   Miguel(3)   Lucia(1)   Andres(5)
salen :   Carlos    Lucia       Miguel      Rosa       Andres
```

Observa que **el orden de llegada no importa** salvo para desempatar.

| Operación | Qué hace | Coste (arreglo ordenado) | Coste (heap) |
|---|---|---|---|
| `encolar(x, p)` | inserta con prioridad `p` | O(n) | **O(log n)** |
| `desencolar()` | saca el más urgente | O(n) | **O(log n)** |
| `frente()` | mira el más urgente | **O(1)** | **O(1)** |
| `estaVacia()` | ¿vacía? | O(1) | O(1) |
| `tamano()` | cuántos hay | O(1) | O(1) |

---

## 2. Versión simple: arreglo con inserción ordenada

Lo que se pide a mano en un examen. Se mantiene el arreglo ordenado por
prioridad en el momento de insertar; el más urgente queda siempre en `datos[0]`.

```cpp
template <typename T>
struct Entrada {
    T dato;
    int prioridad;
    long orden;      // para desempatar: quien llegó antes
};

template <typename T>
class ColaPrioridad {
private:
    Entrada<T>* datos;
    int cantidad;
    int capacidad;
    long contador;

    bool saleAntes(const Entrada<T>& a, const Entrada<T>& b) const {
        if (a.prioridad != b.prioridad) return a.prioridad < b.prioridad;
        return a.orden < b.orden;         // FIFO entre iguales
    }

public:
    void encolar(T dato, int prioridad) {
        Entrada<T> nueva(dato, prioridad, contador++);
        int i = cantidad - 1;
        while (i >= 0 && saleAntes(nueva, datos[i])) {
            datos[i + 1] = datos[i];      // se corre un lugar a la derecha
            i--;
        }
        datos[i + 1] = nueva;
        cantidad++;
    }

    bool desencolar(T& salida) {
        if (cantidad == 0) return false;
        salida = datos[0].dato;           // el más urgente está al frente
        cantidad--;
        for (int i = 0; i < cantidad; i++) datos[i] = datos[i + 1];
        return true;
    }
};
```

💡 La **inserción ordenada** es la misma idea que la lista ordenada del
[tutorial 05](05_listas_ordenadas.md), pero sobre un arreglo.

---

## 3. El desempate: FIFO o LIFO

¿Qué pasa si dos elementos tienen la **misma** prioridad? Hay que decidir:

| Estructura | Empate entre iguales | Para qué |
|---|---|---|
| **Cola** de prioridad | sale el que **llegó antes** (FIFO) | turnos justos |
| **Pila** de prioridad | sale el **más reciente** (LIFO) | deshacer lo urgente |

Solo cambia una línea:

```cpp
// Cola: desempata por orden ascendente
return a.orden < b.orden;

// Pila: desempata por orden descendente
return a.orden > b.orden;
```

Por eso conviene guardar el campo `orden`: sin él, dos elementos con la misma
prioridad quedan en un orden arbitrario y el resultado no es reproducible.

---

## 4. Versión eficiente: montículo binario (heap)

Un **heap** es un árbol binario completo guardado en un arreglo donde cada padre
es más urgente que sus hijos. No hay que ordenar todo: basta con mantener esa
propiedad.

```
padre de i       = (i - 1) / 2
hijo izquierdo   = 2 * i + 1
hijo derecho     = 2 * i + 2
```

```
        [Carlos p1]           datos[0]
        /          \
  [Lucia p1]    [Miguel p3]
     /
[Andres p5]
```

- `encolar`: se pone al final y **sube** mientras sea más urgente que su padre.
- `desencolar`: se copia la raíz, se mueve el último a la raíz y **baja**
  eligiendo el hijo más urgente.

```cpp
void subir(int i) {
    while (i > 0) {
        int padre = (i - 1) / 2;
        if (!saleAntes(datos[i], datos[padre])) break;
        intercambiar(datos[i], datos[padre]);
        i = padre;
    }
}

void bajar(int i) {
    while (true) {
        int izq = 2 * i + 1, der = 2 * i + 2, menor = i;
        if (izq < cantidad && saleAntes(datos[izq], datos[menor])) menor = izq;
        if (der < cantidad && saleAntes(datos[der], datos[menor])) menor = der;
        if (menor == i) break;
        intercambiar(datos[i], datos[menor]);
        i = menor;
    }
}
```

Resultado: **O(log n)** por operación en vez de O(n).

📄 Las tres versiones (cola, pila y heap) implementadas y verificadas:
[`10_cola_prioridad.cpp`](../Plantillas/10_cola_prioridad.cpp)

---

## 5. Aplicaciones

| Aplicación | Por qué una cola de prioridad |
|---|---|
| **Sala de emergencias** | Primero el paciente más grave, no el que llegó antes |
| **Planificador de CPU** | Primero los procesos de mayor prioridad |
| **Dijkstra** | Se extrae siempre el nodo de menor distancia provisional |
| **Huffman** | Se combinan los dos árboles de menor frecuencia |
| **Tareas de impresión** | Los documentos urgentes saltan la fila |
| **Eventos simulados** | Se procesan por instante de tiempo |

---

## 6. Errores típicos

1. **Invertir el convenio de prioridad**: decidir si 1 es el más o el menos
   urgente y escribirlo en un comentario. La mitad de los fallos viene de aquí.
2. **No guardar el orden de llegada** y perder el desempate FIFO.
3. Implementar el heap con índices mal calculados (`2*i` en vez de `2*i + 1`).
4. En el heap, olvidar el `if (cantidad > 0) bajar(0);` tras extraer la última.
5. Confundir una cola de prioridad con una cola ordenada por valor del dato: la
   prioridad es un campo **independiente** del dato.

---

## 7. Autoevaluación

1. ¿Qué complejidad tiene `desencolar` en un arreglo ordenado y en un heap?
2. ¿Por qué se guarda el campo `orden`?
3. En un heap, ¿dónde está el elemento más urgente?
4. ¿Qué cambia entre una cola y una pila de prioridad con prioridades iguales?

<details>
<summary>Respuestas</summary>

1. O(n) en el arreglo (hay que correr los elementos); O(log n) en el heap.
2. Para desempatar de forma determinista entre prioridades iguales (FIFO o LIFO).
3. En la **raíz**, es decir, en `datos[0]`.
4. Solo el desempate: la cola usa el orden de llegada ascendente; la pila, el
   descendente.

</details>

---

Volver al [README principal](../README.md) · [06. Pilas](06_pilas.md) ·
[07. Colas](07_colas.md)
