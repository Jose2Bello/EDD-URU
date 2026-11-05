# Estructuras de Datos (EDD) — Curso completo con ejercicios resueltos

Todo el material está en español y **verificado**: los 18 ejercicios compilan sin errores
ni advertencias (`g++ -std=c++17 -Wall -Wextra`) y todos pasan sus `assert`. La salida
real de cada programa está pegada al final de su archivo, en la sección
`=== SALIDA ESPERADA ===`.

```
edd/
├── README.md                    <- estás aquí (índice general)
├── compilar_todo.bat            <- compila y ejecuta todos los ejercicios
├── compilar_plantillas.bat      <- compila y ejecuta todas las plantillas
├── PracticaEDD.cpp              <- tu código original (lista simple)
├── Tutorial/                    <- teoría en Markdown (léelo en orden)
├── Plantillas/                  <- código base listo para copiar y pegar
└── Ejercicios/
    ├── Enunciados/              <- los 14 enunciados, sin resolver
    └── Resueltos/               <- los 14 ejercicios con solución comentada
```

---

## ⚡ Empieza por aquí: las plantillas

Si solo quieres **copiar una estructura y usarla en tu práctica**, ve directo a
[`Plantillas/`](Plantillas/README.md). Los 7 archivos están listos, compilan sin
advertencias y traen un `main` de prueba:

| # | Plantilla |
|---|---|
| 01 | [Lista simplemente enlazada](Plantillas/01_lista_simplemente_enlazada.cpp) |
| 02 | [Lista doblemente enlazada](Plantillas/02_lista_doblemente_enlazada.cpp) |
| 03 | [Lista circular](Plantillas/03_lista_circular.cpp) |
| 04 | [Lista circular doble](Plantillas/04_lista_circular_doble.cpp) |
| 05 | [Pila + expresiones](Plantillas/05_pila.cpp) |
| 06 | [Cola, cola circular y deque](Plantillas/06_cola.cpp) |
| 07 | [Árbol binario de búsqueda](Plantillas/07_arbol_binario_busqueda.cpp) |
| 08 | [Pila completa (de examen)](Plantillas/08_pila_completa.cpp) |
| 09 | [Cola completa + deque + BFS](Plantillas/09_cola_completa.cpp) |
| 10 | [Cola y pila de prioridad](Plantillas/10_cola_prioridad.cpp) |

---

## 📘 Ruta de aprendizaje

### Fase 1 — Fundamentos
| Tutorial | Contenido |
|---|---|
| [01. Introducción a las EDD](Tutorial/01_introduccion_edd.md) | Qué es una EDD, clasificación, comparativa de costos, cuándo usar cada una |
| [02. Nodos, punteros y memoria](Tutorial/02_nodos_y_punteros.md) | `new`/`delete`, punteros nulos, fugas de memoria, plantillas en C++ |
| [03. Lista simplemente enlazada](Tutorial/03_lista_simplemente_enlazada.md) | Análisis **línea por línea** de `PracticaEDD.cpp`, diagramas, código corregido |

### Fase 2 — Variantes de listas
| Tutorial | Contenido |
|---|---|
| [04. Lista doblemente enlazada y circular](Tutorial/04_lista_doble_y_circular.md) | `anterior`, puntero a la cola, circularidad, evitar el recorrido infinito |
| [05. Listas ordenadas y operaciones clásicas](Tutorial/05_listas_ordenadas.md) | Inserción ordenada, duplicados, partición, fusión, unión, intersección |

### Fase 3 — Estructuras lineales
| Tutorial | Contenido |
|---|---|
| [06. Pilas (Stack)](Tutorial/06_pilas.md) | LIFO, aplicaciones, dos implementaciones, infija → postfija |
| [07. Colas y Deque](Tutorial/07_colas.md) | FIFO, cola circular, cola con dos pilas, deque, BFS, ventana deslizante |
| [10. Cola de prioridad](Tutorial/10_cola_de_prioridad.md) | inserción ordenada, heap O(log n), desempate FIFO/LIFO, pila de prioridad |
| [11. Resumen para el examen](Tutorial/11_resumen_examen_pilas_colas.md) | chuleta de pilas y colas: esqueletos, complejidades y 10 preguntas |

### Fase 4 — Estructuras no lineales
| Tutorial | Contenido |
|---|---|
| [08. Árboles binarios y ABB](Tutorial/08_arboles_binarios.md) | ABB, los tres recorridos, altura, borrado, BFS/DFS |
| [09. Complejidad y buenas prácticas](Tutorial/09_complejidad_y_practicas.md) | Big-O de cada operación, 10 errores típicos, checklist de entrega |

### Fase 5 — Práctica: 18 ejercicios resueltos

Primero lee los [enunciados completos](Ejercicios/Enunciados/00_enunciados.md) e inténtalos
tú; después compara con la solución.

| # | Ejercicio | Tema y técnicas | Solución |
|---|---|---|---|
| 01 | Recorrer y contar nodos | recorridos, casos límite | [`01_recorrer_contar.cpp`](Ejercicios/Resueltos/01_recorrer_contar.cpp) |
| 02 | Invertir la lista | tres punteros y recursión | [`02_invertir_lista.cpp`](Ejercicios/Resueltos/02_invertir_lista.cpp) |
| 03 | Insertar en cualquier posición y buscar | 5 formas de insertar | [`03_insertar_y_buscar.cpp`](Ejercicios/Resueltos/03_insertar_y_buscar.cpp) |
| 04 | Detectar ciclos | algoritmo de Floyd (tortuga y liebre) | [`04_detectar_ciclo.cpp`](Ejercicios/Resueltos/04_detectar_ciclo.cpp) |
| 05 | Eliminar duplicados | doble puntero, el nodo que no avanza | [`05_eliminar_duplicados.cpp`](Ejercicios/Resueltos/05_eliminar_duplicados.cpp) |
| 06 | Palíndromo | dos punteros, pila y recursión | [`06_palindromo_lista.cpp`](Ejercicios/Resueltos/06_palindromo_lista.cpp) |
| 07 | Fusionar listas ordenadas | nodo centinela, recursión, intersección | [`07_fusionar_listas.cpp`](Ejercicios/Resueltos/07_fusionar_listas.cpp) |
| 08 | Rotar la lista *k* posiciones | `k % n`, dos algoritmos | [`08_rotar_lista.cpp`](Ejercicios/Resueltos/08_rotar_lista.cpp) |
| 09 | Lista doblemente enlazada completa | `anterior`, `head`/`tail`, invertir, concatenar | [`09_lista_doble.cpp`](Ejercicios/Resueltos/09_lista_doble.cpp) |
| 10 | Lista circular y Josephus | recorrido sin `nullptr` | [`10_josephus_circular.cpp`](Ejercicios/Resueltos/10_josephus_circular.cpp) |
| 11 | Pila: paréntesis y expresiones | balanceo, infija → postfija, evaluación | [`11_pila_expresiones.cpp`](Ejercicios/Resueltos/11_pila_expresiones.cpp) |
| 12 | Cola, cola circular y deque | FIFO, índices con módulo, prioridad | [`12_cola_ventanilla.cpp`](Ejercicios/Resueltos/12_cola_ventanilla.cpp) |
| 13 | Árbol binario de búsqueda | ABB completo con borrado en 3 casos | [`13_arbol_busqueda.cpp`](Ejercicios/Resueltos/13_arbol_busqueda.cpp) |
| 14 | **Proyecto final** | integra las 5 estructuras | [`14_agenda_estudiantes.cpp`](Ejercicios/Resueltos/14_agenda_estudiantes.cpp) |
| 15 | Pila como historial | deshacer / rehacer con dos pilas | [`15_pila_deshacer_rehacer.cpp`](Ejercicios/Resueltos/15_pila_deshacer_rehacer.cpp) |
| 16 | Cola con dos pilas y pila con dos colas | conversión LIFO ↔ FIFO | [`16_cola_con_dos_pilas.cpp`](Ejercicios/Resueltos/16_cola_con_dos_pilas.cpp) |
| 17 | Deque: ventana deslizante | máximo por ventana en O(n) | [`17_deque_ventana_maxima.cpp`](Ejercicios/Resueltos/17_deque_ventana_maxima.cpp) |
| 18 | BFS en un laberinto | cola, distancia mínima y ruta | [`18_bfs_ruta_laberinto.cpp`](Ejercicios/Resueltos/18_bfs_ruta_laberinto.cpp) |

Cada archivo empieza con el **enunciado completo** en un bloque de comentario, sigue con la
**solución comentada línea por línea** y termina con la **salida real verificada**.

---

## ⚙️ Compilar y ejecutar

### Opción 1 — Todo de un clic (Windows)
```powershell
.\compilar_todo.bat          # compila y ejecuta todos
.\compilar_todo.bat 07       # solo el ejercicio 07
```

### Opción 2 — Un solo archivo
```powershell
g++ -std=c++17 -Wall -Wextra -O2 -static Ejercicios\Resueltos\07_fusionar_listas.cpp -o 07.exe
.\07.exe
```

El flag `-static` evita depender de las DLL de MinGW al ejecutar. En otros equipos puedes
quitarlo. Con Code::Blocks, Dev-C++ o Visual Studio: crea un proyecto, pega el `.cpp` y
pulsa *Build and Run*.

---

## 🚨 Análisis de tu `PracticaEDD.cpp`

Tu código **funciona correctamente** hasta la línea 118. A partir de ahí hay tres errores
que impiden compilar:

| Línea | Problema | Corrección |
|---|---|---|
| 120 | `head` es **privado**: no se puede usar desde `main` | Añadir un método `estaVacia()` o `primero()` a la clase |
| 120 | Falta la condición del `for`: `for (head != nullptr)` no es un bucle | `while (!miLista.estaVacia())` |
| 121 | La variable `i` **no existe** y falta el `;` | Declararla con `int i = miLista.primero();` |

El bucle que necesitabas:

```cpp
while (!miLista.estaVacia()) {
    int i = miLista.primero();   // sacar el dato ANTES de borrar el nodo
    miLista.eliminar(i);
}
```

La versión completa y corregida de tu práctica, con los métodos `estaVacia()`, `primero()`,
`largo()` y `vaciar()` añadidos, está en el
[tutorial 03](Tutorial/03_lista_simplemente_enlazada.md).

---

## 📌 Resumen de lo que debes saber para el examen

1. **Dibujar antes de programar**: estado inicial, estado final, y qué punteros cambian.
2. **Los cuatro casos límite** de toda lista: vacía, un elemento, eliminar el primero,
   eliminar el último, y un valor que no existe.
3. **Regla de oro de memoria**: *guardar → puentear → borrar*.
4. **Cada `new` tiene su `delete`**; el destructor de la lista lo resuelve todo.
5. **Complejidad**: la lista simplemente enlazada es O(n) para casi todo; el ABB es O(log n);
   la pila y la cola son O(1) si guardas los dos extremos.
6. **En un ABB el recorrido in-orden sale ordenado**: es la propiedad que lo hace útil.
7. **Nunca** acceder al nodo que se acaba de borrar: copia su dato primero.