# Enunciados de los 18 ejercicios (sin resolver)

Este archivo contiene **solo los enunciados**, para que puedas intentar resolverlos
antes de abrir la carpeta `Resueltos/`. Cada enunciado va seguido de un enlace a su
solución comentada.

**Reglas para todos los ejercicios**

1. No se permite usar la biblioteca estándar de contenedores (`std::vector`, `std::list`,
   `std::stack`, `std::queue`) **salvo donde el enunciado lo pida explícitamente**.
2. Se puede usar `std::cout`, `std::cin`, `std::string`, `assert`, `std::stack` (en los
   ejercicios 11 y 13, como está indicado) y los headers de C.
3. Cada nodo creado con `new` debe liberarse con `delete` (los arreglos con `delete[]`).
4. El `main` debe probar los casos límite: estructura vacía, un solo elemento,
   valor inexistente y elemento que no existe al principio o al final.
5. Usar `assert` para verificar los resultados.

---

## Bloque 1 — Listas simplemente enlazadas

### Ejercicio 01 — Recorrer y contar nodos
**Tema:** recorridos.

Se tiene una lista simplemente enlazada de enteros. Implementar:
`contar()`, `contarPares()`, `sumar()`, `maximo()`, `minimo()`, `indiceDe(v)`,
`contiene(v)` y `mostrar()`.

> Caso límite: `maximo()` y `minimo()` deben inicializarse con el **primer dato**, no
> con cero, porque la lista puede tener solo números negativos.
> 🔗 [Solución](../Resueltos/01_recorrer_contar.cpp)

---

### Ejercicio 02 — Invertir la lista
**Tema:** reasignación de punteros.

Invertir una lista de N nodos **sin crear nodos nuevos** y en O(n). Se piden dos
versiones:
1. `invertirIterativa()` con tres punteros (`prev`, `actual`, `siguiente`).
2. `invertirRecursiva()` con recursion.

> `1 -> 2 -> 3 -> 4` debe quedar `4 -> 3 -> 2 -> 1`.
> 🔗 [Solución](../Resueltos/02_invertir_lista.cpp)

---

### Ejercicio 03 — Insertar en cualquier posición y buscar
**Tema:** inserciones.

Sobre una lista de enteros implementar:
`agregarAlInicio(v)`, `agregarAlFinal(v)`, `agregarAntesDe(ref, v)`,
`agregarEnPosicion(v, p)`, `agregarOrdenado(v)`, `buscar(v)` y `estaOrdenada()`.

> Si `ref` no existe, `agregarAntesDe` agrega al final.
> Si `p` es mayor que el largo, `agregarEnPosicion` agrega al final.
> 🔗 [Solución](../Resueltos/03_insertar_y_buscar.cpp)

---

### Ejercicio 04 — Detectar ciclos
**Tema:** algoritmo de Floyd.

1. `tieneCiclo()` con el algoritmo de la tortuga y el liebre — O(n), O(1).
2. `tieneCicloPorVisitados()` con tabla de visitados — O(n) memoria.
3. `inicioDelCiclo()`: el nodo donde empieza el ciclo.
4. `longitudDelCiclo()`: cuántos nodos tiene el ciclo.
5. `romperCiclo()`: dejar la lista como una lista simple válida, sin perder nodos.
6. `mostrar()` protegido: nunca debe imprimir infinitamente aunque haya ciclo.

> Ejemplo de ciclo: `1 -> 2 -> 3 -> 4 -> (vuelve a 2)`. El inicio del ciclo es el nodo 2
> y la longitud del ciclo es 3.
> 🔗 [Solución](../Resueltos/04_detectar_ciclo.cpp)

---

### Ejercicio 05 — Eliminar duplicados
**Tema:** doble puntero.

1. `eliminarDuplicadosOrdenada()`: lista ordenada con repetidos juntos.
   `1 1 2 3 3 3 4` → `1 2 3 4`
2. `eliminarDuplicadosNoOrdenada()`: conserva el orden del primer ejemplar de cada valor.
   `3 1 3 1 2 3 5` → `3 1 2 5`
3. `vecesQueAparece(v)`.

> **La trampa:** al borrar un nodo, `actual` **no** debe avanzar.
> 🔗 [Solución](../Resueltos/05_eliminar_duplicados.cpp)

---

### Ejercicio 06 — Palíndromo
**Tema:** dos punteros, pila y recursion.

Determinar si una lista es palíndromo usando:
1. `esPalindromo()` con dos punteros (rápido/lento) e inversión de la segunda mitad.
2. `esPalindromoPila()` con `std::stack`.
3. `esPalindromoRecursivo()` por recursion.
4. `palindromoMasLargo()`: longitud del tramo palíndromo más largo.

> `esPalindromo()` altera temporalmente los enlaces: **restaurar la lista al final**.
> 🔗 [Solución](../Resueltos/06_palindromo_lista.cpp)

---

### Ejercicio 07 — Fusionar listas ordenadas
**Tema:** fusión (merge).

Sobre dos listas ordenadas sin repetidos, implementar:
1. `fusionarIterativa()` con nodo centinela.
2. `fusionarRecursiva()`.
3. `fusionarEnPlace()`.
4. `interseccion()`.
5. `diferencia()` (elementos de A que no están en B).

> Las tres primeras **reutilizan** los nodos de entrada: hay que transferir la propiedad
> con `propiedadDeLosNodos() = nullptr` o el destructor borra dos veces.
> 🔗 [Solución](../Resueltos/07_fusionar_listas.cpp)

---

### Ejercicio 08 — Rotar la lista
**Tema:** aritmética de punteros.

Rotar la lista `k` posiciones a la derecha. Casos límite: `k = 0`, `k` mayor que el largo,
`k` negativa, lista vacía y lista de un solo elemento.

> Rotar 7 posiciones una lista de 5 equivale a rotar 2.
> Se piden dos versiones: una en tres pasos y otra cerrando el círculo.
> 🔗 [Solución](../Resueltos/08_rotar_lista.cpp)

---

## Bloque 2 — Listas doble, circular, pila y cola

### Ejercicio 09 — Lista doblemente enlazada completa
**Tema:** punteros `anterior`/`siguiente` y `head`/`tail`.

Implementar: `agregarAlInicio`, `agregarAlFinal`, `insertarAntesDe`, `eliminarValor`,
`eliminarNodo(puntero)`, `buscar`, `invertir`, `concatenar`, `estaOrdenada`,
`mostrarAdelante` y `mostrarAtras`.

> Demostrar que `eliminarNodo` es **O(1)** mientras que `eliminarValor` es O(n).
> `concatenar` deja la lista fuente vacía (le cede los nodos).
> 🔗 [Solución](../Resueltos/09_lista_doble.cpp)

---

### Ejercicio 10 — Lista circular y Josephus
**Tema:** recorrido sin `nullptr`.

Parte 1 — lista circular con `agregarAlFinal`, `agregarAlInicio`, `largo`,
`eliminarValor`, `invertir` (conservando la circularidad) y `mostrar` protegido.

Parte 2 — problema de Josephus: N personas en círculo, se elimina una cada K y se
imprime el orden de eliminación y el último sobreviviente.

> **Trampa mortal:** en una circular, el recorrido se detiene comparando con `head`,
> nunca con `nullptr`.
> Con N = 7 y K = 3 el sobreviviente es el **4**.
> 🔗 [Solución](../Resueltos/10_josephus_circular.cpp)

---

### Ejercicio 11 — Pila y expresiones
**Tema:** LIFO.

1. Implementar `Pila<T>` (nodos) y `PilaArreglo<T>` (capacidad fija).
2. `parentesisBalanceados(cadena)`: devuelve `false` si hay cierre sin apertura,
   si el tipo no corresponde `([)]`, o si sobran aperturas.
3. `aPostfija(infija)`: convertir `3 + 4 * 5` en `"3 4 5 * +"`.
4. `evaluarPostfija(postfija)`: `"3 4 5 * +"` → `23`.
5. `evaluarInfija(infija)`: evaluar con dos pilas.

> Los tokens de la postfija **deben** ir separados por espacios.
> 🔗 [Solución](../Resueltos/11_pila_expresiones.cpp)

---

### Ejercicio 12 — Cola, cola circular y deque
**Tema:** FIFO.

1. `Cola<T>` con dos punteros: `encolar`, `desencolar`, `primero`, `ultimo`, `invertir`.
2. `ColaCircular` de arreglo con capacidad fija, índices que dan la vuelta con el módulo.
3. `ColaPrioridad`: sale primero el de mayor urgencia (prioridad 1).
4. Simulación de una ventanilla con llegar / atender / ver la fila.
5. `Deque`: se puede empujar y sacar por **los dos extremos**.

> En la cola circular hay que distinguir "vacía" de "llena": hace falta un contador.
> 🔗 [Solución](../Resueltos/12_cola_ventanilla.cpp)

---

## Bloque 3 — Árboles e integración

### Ejercicio 13 — Árbol binario de búsqueda
**Tema:** ABB.

Implementar: `insertar`, `buscar`, `preOrden`, `inOrden`, `postOrden`,
`preOrdenIterativo` (con `std::stack`), `nivelOrden` (con `std::queue`), `altura`,
`esLleno`, `esBalanceado`, `contarNodos`, `sumaValores`, `minimo`, `maximo`,
`sonIdenticos`, `mezclarLista` y `borrar` (los tres casos).

> En un ABB el recorrido **in-orden sale ordenado**.
> Insertar 1, 2, 3, 4, 5 produce un árbol **degenerado**: altura 5 en vez de 3.
> 🔗 [Solución](../Resueltos/13_arbol_busqueda.cpp)

---

### Ejercicio 14 — Proyecto final: sistema universitario
**Tema:** integración de todas las EDD.

Un sistema de gestión de estudiantes donde **cada estructura se usa donde rinde mejor**:

| Estructura | Dónde se usa |
|---|---|
| ABB | búsqueda por matrícula (O(log n)) |
| Lista doblemente enlazada | lista de espera: recorrer hacia atrás |
| Lista simplemente enlazada | lista de inscritos de cada materia |
| Cola | consultas: se atienden en orden FIFO |
| Pila | historial: deshacer la última matriculación |

Requisitos: registrar estudiante, crear materias con cupos, matricular (rechazar si no hay
cupos / si ya está matriculado / si no existe la materia o el estudiante), encolar y atender
consultas, listar ordenados por promedio, deshacer la última matriculación, dar de baja a
un estudiante (quitándolo de todas las materias y de la lista de espera) y generar un
reporte final.

> `darDeBaja` debe quitar al estudiante de todas las materias, de la lista de espera y
> **del ABB** (borrado en un ABB = los tres casos de siempre).
> 🔗 [Solución](../Resueltos/14_agenda_estudiantes.cpp)

---

## Bloque 4 — Ampliación de pilas y colas

### Ejercicio 15 — Pila como historial (deshacer / rehacer)
**Tema:** dos pilas y estados.

Implementar un editor con `escribir(texto)`, `deshacer()`, `rehacer()` y `ver()`,
usando dos pilas: una de estados previos (deshacer) y otra de estados futuros
(rehacer). Al escribir algo nuevo después de deshacer, el historial de rehacer se
**descarta** (igual que Ctrl+Z / Ctrl+Y).

> Casos límite: deshacer sin cambios, rehacer sin haber deshecho y deshacer hasta
> dejar el texto vacío.
> 🔗 [Solución](../Resueltos/15_pila_deshacer_rehacer.cpp)

---

### Ejercicio 16 — Cola con dos pilas y pila con dos colas
**Tema:** conversión entre LIFO y FIFO.

1. `ColaConDosPilas<T>`: `encolar` O(1) y `desencolar` O(1) amortizado.
2. `PilaConDosColas<T>`: `apilar` O(1) y `desapilar` O(n).

> Comprobar con `assert` el orden FIFO y LIFO, las operaciones intercaladas y
> sacar de una estructura vacía.
> 🔗 [Solución](../Resueltos/16_cola_con_dos_pilas.cpp)

---

### Ejercicio 17 — Deque: máximo de cada ventana deslizante
**Tema:** deque y complejidad O(n).

Dado un arreglo y una ventana de tamaño `k`, devolver el máximo de cada ventana
consecutiva en **O(n)**, guardando índices en un deque con valores decrecientes.

> `[1, 3, -1, -3, 5, 3, 6, 7]` con `k = 3` da `[3, 3, 5, 5, 6, 7]`.
> Casos límite: `k = 1`, `k = n`, `k > n` y `k <= 0`.
> 🔗 [Solución](../Resueltos/17_deque_ventana_maxima.cpp)

---

### Ejercicio 18 — BFS en un laberinto y reconstrucción de la ruta
**Tema:** cola y búsqueda en anchura.

Con una matriz de `'S'`, `'E'`, `'.'` y `'#'`, calcular la distancia mínima de
`S` a `E` con BFS y reconstruir el camino usando predecesores.

> La primera vez que BFS llega a una casilla, llega por el camino más corto.
> Probar también un laberinto **sin** camino.
> 🔗 [Solución](../Resueltos/18_bfs_ruta_laberinto.cpp)

---

## Lista de errores que más se repiten

| # | Error | Cómo se corrige |
|---|---|---|
| 1 | Recorrido infinito (`while (actual)`) | siempre `!= nullptr` |
| 2 | Fuga de memoria | destructor en todas las clases con `new` |
| 3 | Doble `delete` | puentear **antes** de borrar |
| 4 | Perder la lista al reasignar la cabeza | guardar → puentear → borrar |
| 5 | `head->dato` sin comprobar `head != nullptr` | siempre comprobar antes |
| 6 | Recursión sin caso base | `if (nodo == nullptr) return;` primero |
| 7 | Confundir LIFO con FIFO | decidir qué entra y qué sale |
| 8 | Leer `->siguiente` después de `delete` | guardar antes en una variable |

---

Volver al [README principal](../../README.md) · Ver la [ruta de tutoriales](../../Tutorial/)