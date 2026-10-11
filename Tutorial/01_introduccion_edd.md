# 01. Introducción a las Estructuras de Datos

## 1. ¿Qué es una estructura de datos (EDD)?

Una **estructura de datos** es una forma de organizar los datos dentro de la memoria de
la computadora para poder **almacenarlos, buscarlos, insertarlos y borrarlos** de la manera
más eficiente posible.

Piensa en una biblioteca: los libros no se apilan tirados en el suelo, sino que se colocan
en estanterías *organizadas* (por tema, por autor, por fecha). Esa organización es una
estructura de datos. Cada organización tiene ventajas y desventajas:

- Buscar por autor (ordenado) → rápido.
- Insertar un libro nuevo al principio (ordenado) → hay que mover todos los demás.

**No existe la estructura perfecta.** La correcta depende de la operación que más hagas.

---

## 2. El criterio de elección: ¿qué operación es la más frecuente?

Esa es la pregunta clave de cualquier examen y de cualquier diseño de software.

| Si la operación más frecuente es... | Usa... | Ejemplo real |
|---|---|---|
| Insertar/eliminar en un extremo y acceder al último | **Pila (stack)** | Deshacer (Ctrl+Z), botón "atrás" del navegador, siftos de un compilador |
| Insertar por un lado y sacar por el otro | **Cola (queue)** | Turnos de una ventanilla, cola de impresión, BFS |
| Buscar un elemento por un identificador | **Tabla hash / ABB** | Diccionario: cuántas palabras hay en un texto |
| Buscar en un conjunto **ordenado** | **Árbol binario de búsqueda** | Índice de una base de datos, agenda de contactos |
| Acceder por posición (índice) | **Arreglo / vector** | Lista de reproducción, matriz |
| Insertar/eliminar en la mitad | **Lista doblemente enlazada** | Lista de tareas donde mueves elementos con las flechas del teclado |
| Nodos con jerarquía (padres e hijos) | **Árbol general** | Sistema de archivos, DOM, genealogía |

---

## 3. Clasificación de las EDD

### 3.1 Lineales
Los elementos están en secuencia, y cada uno tiene una relación directa con el anterior y el siguiente.

```
Lista simplemente:   [10] -> [20] -> [30] -> nullptr

Lista doblemente:   nullptr <- [10] <-> [20] <-> [30] -> nullptr
                           prev    next     prev  next

Lista circular:     [10] -> [20] -> [30] -+
                 ^                        |
                 +------------------------+

Pila (stack):          push  ->  [ 3 ]
                                 [ 2 ]   top
                                 [ 1 ]
                              pop  <-   (sale el 3)

Cola (queue):      frente -> [ 1 ] [ 2 ] [ 3 ] -> final
```

### 3.2 No lineales
- **Árboles**: un nodo raíz con hijos. Cada hijo tiene un solo padre.
- **Grafos**: nodos (vértices) unidos por aristas; un nodo puede tener varios caminos de entrada.

---

## 4. Comparativa de costos (complejidad) — lo que debes memorizar

`n` = cantidad de elementos.

| Operación | Arreglo | Lista simple | Lista doble | Pila | Cola | ABB |
|---|---|---|---|---|---|---|
| Acceso por índice | **O(1)** | O(n) | O(n) | O(n) | O(n) | O(n) |
| Búsqueda | O(n) | O(n) | O(n) | O(n) | O(n) | **O(log n)** |
| Insertar al inicio | O(n) | **O(1)** | **O(1)** | O(1) | O(n) | O(log n) |
| Insertar al final | O(1)¹ | O(n) | **O(1)**² | O(1) | O(1) | O(log n) |
| Eliminar del inicio | O(n) | **O(1)** | **O(1)** | O(1) | **O(1)** | O(log n) |
| Obtener el último | O(1) | O(n) | **O(1)** | **O(1)** | O(1) | O(log n) |

¹ Solo si se reserva el espacio de antemano; si hay que hacer `resize`, es O(n).
² Con puntero a `tail`.

**O(1)** = constante (no depende de n). **O(n)** = lineal (peor caso). **O(log n)** = muy rápido (divide y vencerás: 1000 elementos → ~10 pasos).

---

## 5. El criterio "memoria vs. velocidad"

| | Válido | Coste |
|---|---|---|
| Lista simplemente enlazada | 1 puntero por nodo | Menos memoria, recorridos más lentos |
| Lista doblemente enlazada | 2 punteros por nodo | El doble de punteros, pero insertar/eliminar junto a un nodo conocido es O(1) |
| ABB desbalanceado | — | Puede degenerar en una lista (O(n) para todo) |
| ABB balanceado | — | Siempre O(log n), pero más complejo de implementar |

En el coursework suele pedirse **implementar la versión simple** para entender el concepto, y
**mencionar** cuál sería la versión optimizada. No compliques de más: primero que funcione.

---

## 6. Cómo se evalúa una EDD en un examen

1. **Dibuja el estado inicial** (todos los nodos y sus flechas).
2. **Dibuja el estado final** esperado.
3. **Identifica los punteros que cambian** (marca en rojo los que se reasignan).
4. **Escribe el código**: primero comprueba los casos límite.
5. **Libera la memoria** con `delete`.

Caso límite clásicos de una lista:
- Lista vacía (`head == nullptr`).
- Insertar en lista vacía.
- Eliminar la **cabeza**.
- Eliminar el **último** (el penúltimo debe quedar con `siguiente = nullptr`).
- Eliminar un valor que **no existe**.
- Insertar en una lista de **un solo** elemento.

---

## 7. Ejemplo mental: insertar al inicio de una lista con 3 nodos

Estado inicial:

```
head
 |
 v
[10] -> [20] -> [30] -> nullptr
```

Insertar `5` al inicio. Los punteros que cambian son `head` y el `siguiente` del nodo nuevo:

```
  head (cambia)
   |
   v
  [5] -> [10] -> [20] -> [30] -> nullptr
  nuevo       (antes head)
```

```cpp
void insertarAlInicio(T valor) {
    Nodo<T>* nuevo = new Nodo<T>(valor);
    nuevo->siguiente = head;   // 1) engancha al viejo head
    head = nuevo;             // 2) head pasa a ser el nuevo
}
```

Cuando este mismo razonamiento lo automaticas, ya no necesitas pensar el orden: **nunca
pierdes el puntero viejo antes de haberlo guardado en algún lado**.

---

## Siguiente tutorial → [02. Nodos, punteros y memoria](02_nodos_y_punteros.md)