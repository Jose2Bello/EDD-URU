# Plantillas de estructuras de datos

Archivos **listos para copiar y pegar** en tus prácticas. Cada uno es autónomo:
solo usa la biblioteca estándar, compila sin errores ni advertencias
(`g++ -std=c++17 -Wall -Wextra`) y trae un `main` de prueba que muestra el
funcionamiento de cada método.

```powershell
.\compilar_plantillas.bat        # compila y ejecuta las 7
.\compilar_plantillas.bat 03     # solo la circular
```

| # | Archivo | Métodos incluidos |
|---|---|---|
| 01 | [`01_lista_simplemente_enlazada.cpp`](01_lista_simplemente_enlazada.cpp) | `agregarAlInicio`, `agregarAlFinal`, `agregarOrdenado`, `insertarAntesDe`, `insertarEnPosicion`, `eliminar`, `eliminarPrimero`, `buscar`, `contiene`, `indiceDe`, `largo`, `sumar`, `maximo`, `minimo`, `invertir`, `invertirRecursivo`, `concatenar`, `eliminarDuplicados`, `estaOrdenada`, `mostrar`, `limpiar` |
| 02 | [`02_lista_doblemente_enlazada.cpp`](02_lista_doblemente_enlazada.cpp) | `agregarAlInicio`, `agregarAlFinal`, `insertarAntesDe`, `insertarDespuesDe`, `eliminarValor`, `eliminarNodo` (O(1)), `buscar`, `contiene`, `largo`, `invertir`, `concatenar`, `estaOrdenada`, `mostrarAdelante`, `mostrarAtras`, `limpiar` |
| 03 | [`03_lista_circular.cpp`](03_lista_circular.cpp) | `agregarAlInicio`, `agregarAlFinal`, `eliminarValor`, `eliminarNodo`, `buscar`, `contiene`, `largo`, `estaOrdenada`, `invertir` (conserva el círculo), `concatenar`, `mostrar` + **Josephus** |
| 04 | [`04_lista_circular_doble.cpp`](04_lista_circular_doble.cpp) | Los de la 02, pero **circular**: `head->anterior` es el último y `tail->siguiente` es el primero, así se puede seguir andando en ambos sentidos indefinidamente |
| 05 | [`05_pila.cpp`](05_pila.cpp) | `Pila` con nodos + `PilaArreglo` con capacidad fija + `parentesisBalanceados`, `aPostfija`, `evaluarPostfija` |
| 06 | [`06_cola.cpp`](06_cola.cpp) | `Cola` (FIFO), `ColaCircular` con arreglo, `ColaPrioridad`, `Deque` |
| 07 | [`07_arbol_binario_busqueda.cpp`](07_arbol_binario_busqueda.cpp) | `insertar`, `buscar`, `borrar` (3 casos), `preOrden`, `inOrden`, `postOrden`, `preOrdenIterativo`, `nivelOrden`, `altura`, `esLleno`, `esBalanceado`, `contarNodos`, `sumaValores`, `minimo`, `maximo`, `sonIdenticos`, `listar` |
| 08 | [`08_pila_completa.cpp`](08_pila_completa.cpp) | **Pila de examen**: copia profunda (`operator=`), `contar`, `busca`, `vecesQueAparece`, `desapilarN`, `invertir` (recursivo), `duplicar`, `ordenarMayorArriba`; `PilaMin` (mínimo en O(1)); aplicaciones: invertir cadena, decimal → binario, palíndromo |
| 09 | [`09_cola_completa.cpp`](09_cola_completa.cpp) | **Cola de examen**: copia profunda, `busca`, `invertir`, `rotar`; `ColaCircular` genérica; `Deque` completo; aplicaciones: **BFS en laberinto**, palíndromo con deque, **cola con dos pilas** |
| 10 | [`10_cola_prioridad.cpp`](10_cola_prioridad.cpp) | `ColaPrioridad` (inserción ordenada, FIFO entre iguales), `PilaPrioridad` (LIFO entre iguales) y `ColaPrioridadHeap` (montículo, O(log n)) |

---

## Cómo usar una plantilla

1. Copia el archivo a tu práctica.
2. Borra el `main` de prueba (o deja el tuyo al final).
3. Quita los métodos que no necesites y renombra los tuyos.
4. Recuerda: si borras el `main` de prueba, **los `assert` también se van**;
   añade los tuyos.

---

## Las 4 diferencias clave entre las listas

| | Simple | Doble | Circular | Circular doble |
|---|---|---|---|---|
| Punteros por nodo | 1 (`siguiente`) | 2 (`anterior`, `siguiente`) | 1 (`siguiente`) | 2 (`anterior`, `siguiente`) |
| Termina en | `nullptr` | `nullptr` en ambos extremos | la cabeza | la cabeza por los dos lados |
| Recorrer hacia atrás | No | **Sí** | No | **Sí** |
| Insertar al final | O(n) | **O(1)** con `tail` | O(1) con `tail` | O(1) con `tail` |
| Insertar junto a un nodo conocido | O(n) | **O(1)** | O(n) | **O(1)** |
| Caso típico | registro de datos | lista de tareas, historial | Josephus, turnos | carrusel, anillo |

---

## Los tres errores que más se repiten en estas plantillas

1. **Recorrido infinito en una circular.** El bucle se detiene comparando con
   `head`, nunca con `nullptr`:
   ```cpp
   Nodo<T>* a = head;
   do { a = a->siguiente; } while (a != head);
   ```

2. **Destructor infinito en una circular doble.** Primero se abre el círculo
   (`tail->siguiente = nullptr`) y luego se borra en cadena; si no, se llega a
   un nodo ya liberado.

3. **`concatenar` y `fusionar` reutilizan nodos.** La lista de entrada queda con
   punteros colgando: hay que vaciarla (`otra.head = nullptr; otra.tail = nullptr;`)
   o el destructor borrará los mismos nodos dos veces.

---

Volver al [README principal](../README.md) · [Tutoriales](../Tutorial/) ·
[Ejercicios](../Ejercicios/Enunciados/00_enunciados.md)