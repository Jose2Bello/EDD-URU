# 09. Complejidad, errores típicos y checklist final

Este tutorial es el "control de calidad" del curso: cómo razonar sobre el costo de un
algoritmo y qué revisar antes de entregar un trabajo.

---

## 1. Cómo se analiza la complejidad (Big-O)

El Big-O ignora las constantes y los términos menores: describe **cómo crece** el tiempo
cuando `n` crece.

| Tipo | Significado | Ejemplo: `n = 1000` |
|---|---|---|
| **O(1)** | Constante | Acceso a `arreglo[i]`, `push` de pila |
| **O(log n)** | Logarítmica (divide y vencerás) | Búsqueda en ABB |
| **O(n)** | Lineal | Recorrer una lista |
| **O(n log n)** | Casi lineal | `sort` de la librería, heapsort |
| **O(n²)** | Cuadrática (malo) | Doble bucles anidados: 1000² = 1 000 000 de operaciones |
| **O(2ⁿ)** | Exponencial (pésimo) | Recursión sin poda: 2¹⁰⁰0 |

### El método para analizar un algoritmo

1. Cuenta cuántas veces se ejecuta **cada línea**.
2. Suma los conteos.
3. ¿De qué depende el conteo? Si depende de `n`, saca el factor `n`.
4. Si hay varios bucles: **multiplica** las potencias (anidados), **suma** si están en
   paralelo (`if/else`, `a || b`).
5. Descarta lo que no dependa de `n`.

**Ejemplo 1 — dos bucles anidados:**
```cpp
for (int i = 0; i < n; i++)              // n vueltas
    for (int j = 0; j < n; j++)          // n vueltas por cada una
        cout << i;                       // n * n = n² veces
// => O(n²)
```

**Ejemplo 2 — bucles en paralelo:**
```cpp
if (a) for (int i = 0; i < n; i++) { /* n */ }
else   for (int i = 0; i < n; i++) { /* n */ }
// Solo se ejecuta UNO de los dos: => O(n), NO O(n²)
```

**Ejemplo 3 — condición con cortocircuito:**
```cpp
bool existe(vector<int> v, int x) {
    for (int i = 0; i < n; i++)             // n
        if (v[i] == x) return true;          // + 1 (n en el peor caso)
}
// => O(n)
```

**Ejemplo 4 — la trampa clásica del parcial:**
```cpp
for (int i = 0; i < n; i++)
    while (lista != nullptr) { lista = lista->siguiente; }
// La lista tiene n elementos: el while avanza n-1 veces EN CADA vuelta del for
// => n * (n-1) = O(n²)
```

### Coste de espacio

También se mide: cuántas variables/auxiliares usamos.

```cpp
// recursión pura: en el peor caso la pila de llamadas guarda n elementos => O(n) espacio
// versión iterativa con punteros: 3 variables => O(1) espacio
```

---

## 2. Tabla resumen de todo el curso

### Listas

| Operación | Simple | Doble | Circular |
|---|---|---|---|
| Agregar al inicio | O(1) | O(1) | O(1) |
| Agregar al final | O(n) / O(1) con tail | O(1) | O(1) |
| Agregar en el medio | O(n) | O(n) | O(n) |
| Eliminar por valor | O(n) | O(n) | O(n) |
| Eliminar con puntero dado | O(n) | **O(1)** | O(n) |
| Buscar | O(n) | O(n) | O(n) |
| Invertir | O(n) | O(n) | O(n) |
| Acceder al último | O(n) | **O(1)** | O(1) |

### Pila y cola

| Pila | | Cola | |
|---|---|---|---|
| push | O(1) | encolar | O(1) |
| pop | O(1) | desencolar | O(1) |
| top | O(1) | front | O(1) |
| buscar | O(n) | buscar | O(n) |
| invertir | O(n) | invertir | O(n) |

### Árbol ABB

| Operación | Coste |
|---|---|
| Buscar / insertar / borrar | O(log n) |
| Recorrer entero | O(n) |
| Minimo / máximo | O(log n) |
| Obtener altura | O(n) |

---

## 3. Los 10 errores que más se repiten

| # | Error | Cómo se detecta | Cómo se evita |
|---|---|---|---|
| 1 | Recorrido infinito por usar `while (actual)` en vez de `!= nullptr` | El programa se cuelga | Siempre `!= nullptr` |
| 2 | Fuga de memoria: `new` sin `delete` | Valgrind / memoria creciente | Destructor siempre |
| 3 | Doble `delete` (o borrar dos veces el mismo nodo) | El programa revienta al final | Puentear **antes** de borrar |
| 4 | Perder la lista al reasignar `head` | Lista vacía o con datos basura | Guardar → puentear → borrar |
| 5 | No comprobar lista vacía antes de `head->dato` | Segfault | `if (head != nullptr)` |
| 6 | Liberar un nodo y luego leer `->siguiente` | Comportamiento indefinido | Guardar el siguiente en variable |
| 7 | Recursión sin caso base | Stack overflow | `if (nodo == nullptr) return;` primero |
| 8 | Confundir pila con cola (LIFO vs FIFO) | Resultados inesperados | Decidir qué entra y qué sale |
| 9 | Orden incorrecto en `eliminar` (borrar antes de puentear) | Lista con nodos saltados | Orden: guardar, puentear, borrar |
| 10 | Omitir el `postOrden` al destruir un árbol | Fuga de memoria | `izq`, `der`, `delete` |

---

## 4. Cómo presentar el trabajo

### Estructura de un archivo `.cpp` limpio

```cpp
// 1) Directivas de preprocesado
#include <iostream>
#include <string>

// 2) using namespace std;   (solo si lo pide el profesor)

// 3) Clase Nodo
// 4) Clase de la estructura
// 5) main
int main() {
    // datos de prueba
    // operaciones
    // resultados
    return 0;
}
```

### Documentación que el profesor espera

- **Encabezado** con tu nombre, materia, fecha y número de práctica.
- **Análisis previo**: ¿qué hace el programa? ¿qué EDD usa y por qué?
- **Diagrama** del estado inicial y final (a mano o con texto).
- Código comentado en los puntos clave.
- **Pruebas**: casos normal y casos límite.
- **Conclusión**: costo de cada operación, ventajas y desventajas.

### Ejemplo de comentario útil

```cpp
// MAL: "aquí va algo"
actual->siguiente = aEliminar->siguiente;   // puentea: el anterior salta al siguiente

// BIEN: "aquí se guarda el siguiente antes de borrar para no perderlo"
Nodo<T>* siguiente = aEliminar->siguiente;
actual->siguiente = siguiente;
delete aEliminar;
```

---

## 5. Checklist final antes de entregar

**Código**
- [ ] Compila **sin errores ni warnings** (`g++ -std=c++17 -Wall archivo.cpp`)
- [ ] Todas las variables puntero están inicializadas en su declaración o constructor
- [ ] Cada `new` tiene su `delete`; los arreglos usan `delete[]`
- [ ] El destructor está implementado (en listas, colas, pilas)
- [ ] `main` devuelve `int` y termina con `return 0;`
- [ ] Los métodos de solo lectura son `const`

**Lógica**
- [ ] ¿Probé la estructura **vacía**?
- [ ] ¿Probé con **un solo** elemento?
- [ ] ¿Probé eliminar el **primer** elemento?
- [ ] ¿Probé eliminar el **último** elemento?
- [ ] ¿Probé eliminar un valor que **no existe**?
- [ ] ¿Probé insertar cuando ya **no hay memoria** (si usas `new` puede fallar)? (opcional)

**Documentación**
- [ ] Hay diagrama inicial y final
- [ ] Cada operación no obvia está comentada
- [ ] Incluí la tabla de complejidad
- [ ] Escribí la conclusión (qué estructura conviene y por qué)

---

## 6. Ejercicios de repaso final

Intenta resolver estos **sin mirar** las soluciones; están resueltos en los archivos de
`Ejercicios/Resueltos/`:

1. Dada una lista, ¿cuántos nodos tienen un número par como dato? → *ejercicio 01*
2. Invierte una lista de N nodos en O(n) con 3 punteros → *ejercicio 02*
3. ¿Tiene la lista un ciclo? Usa el algoritmo de Floyd → *ejercicio 04*
4. Dada una lista ordenada sin duplicados, quita los repetidos → *ejercicio 05*
5. ¿Es palíndromo la secuencia? Solo con lista → *ejercicio 06*
6. Une dos listas ordenadas en una sola → *ejercicio 07*
7. Rota la lista k posiciones a la derecha → *ejercicio 08*
8. Implementa una lista doble completa e inviertela -> *ejercicio 09*
9. Problema de Josephus con lista circular → *ejercicio 10*
10. Valida una expresión aritmética con pila → *ejercicio 11*
11. Simula una cola de atención con cola → *ejercicio 12*
12. ABB: inserta, recorre y borra → *ejercicio 13*
13. Agenda de estudiantes ordenada por promedio con ABB → *ejercicio 14*

---

## 7. Recursos para seguir practicando

- **Visualizador de algoritmos**: busca *visualgo.net* (tiene ABB, lista, pila, cola,
  ordenamiento, BFS/DFS animados). Excelente para entender y para dibujar en el examen.
- **Práctica**: LeetCode (fácil), HackerRank. Busca problemas etiquetados *Linked List*,
  *Stack*, *Queue*, *Tree*.
- **Documentación**: la referencia de `std::list`, `std::stack`, `std::queue`,
  `std::deque`, `std::map` en `cppreference.com`.

---

¡Éxito con la práctica! Ya tienes la teoría completa, 14 ejercicios resueltos y una
lista de verificación para cada entrega.
