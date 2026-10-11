# 02. Nodos, punteros y memoria

Todo lo que harás en EDD se apoya en una sola idea: **un objeto que guarda un dato y
la dirección del siguiente objeto**. Ese objeto es un nodo.

---

## 1. El nodo

```cpp
template <typename T>
class Nodo {
public:
    T dato;                 // la información
    Nodo<T>* siguiente;      // la dirección del nodo que sigue

    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};
```

Lectura de la declaración:

- `template <typename T>` → **plantilla**: `Nodo` sirve para `int`, `double`, `string`, `Estudiante`…
  Cada vez que usas `Nodo<int>`, el compilador genera internamente una versión de la clase
  con `T = int`. Por eso `ListaSimple<Estudiante>` funciona sin tocar nada.
- `T dato;` → guarda el valor.
- `Nodo<T>* siguiente;` → puntero a otro nodo del **mismo tipo**.
- El constructor **inicializa `siguiente` en `nullptr`**: es fundamental. Un puntero sin
  inicializar es basura y el programa se cuelga o rompe.

### Instanciación

```cpp
Nodo<int>* p = new Nodo<int>(7);
p->dato;      // 7
p->siguiente; // nullptr
delete p;
```

`new` reserva memoria en el **montículo (heap)** y devuelve la dirección. `p->` es
"acceder al miembro mediante el puntero" (`p->dato` equivale a `(*p).dato`).

---

## 2. Dibujo mental obligatorio

Cuando el profesor dice "dibuja la lista", dibuja el **heap**:

```
head
 |
 v
 +------+------+      +------+------+      +------+------+
 | dato | next | ---> | dato | next | ---> | dato | next | ---> nullptr
 |  10  |      |      |  20  |      |      |  30  |      |
 +------+------+      +------+------+      +------+------+
```

`head` **no es un nodo**, es solo una dirección guardada dentro del objeto lista.
Ese detalle es la causa número uno de errores en exámenes.

---

## 3. Los tres casos límite de cualquier recorrido

```cpp
// 1) Lista vacía: head == nullptr
// 2) Un solo nodo:  head->siguiente == nullptr
// 3) Lista normal:  head->siguiente != nullptr
```

El bucle universal que sirve para los tres:

```cpp
Nodo<int>* actual = head;
while (actual != nullptr) {
    // hacer algo con actual->dato
    actual = actual->siguiente;   // avanza (en el caso 2 avanza a nullptr y para)
}
```

Nunca escribas `while (actual)` (eso compara con el entero 1, no con `nullptr`) ni
`while (actual->siguiente)` (eso omite el último nodo y, si la lista tiene un nodo,
se sale de la memoria).

---

## 4. Fugas de memoria: la regla de oro

> Cada `new` debe tener exactamente un `delete` que lo ejecute.

Tres formas de garantizarlo:

### 4.1 Destructor de la lista (lo que hace tu `PracticaEDD.cpp`)

```cpp
~ListaSimple() {
    Nodo<T>* actual = head;
    while (actual != nullptr) {
        Nodo<T>* siguiente = actual->siguiente;  // 1) salva al siguiente ANTES de borrar
        delete actual;                           // 2) borra este nodo
        actual = siguiente;                      // 3) avanza al que salvamos
    }
}
```

El orden importa: si borraras primero y luego leyeras `actual->siguiente`, estarías
**leyendo memoria liberada** (comportamiento indefinido: lo normal es que el programa reviente).

### 4.2 Copia profunda: si la clase se copia, hay que duplicar los nodos

```cpp
ListaSimple(const ListaSimple& otra) : head(nullptr) {
    Nodo<T>* actual = otra.head;
    while (actual != nullptr) {
        agregarAlFinal(actual->dato);   // crea nodos nuevos, no comparte los viejos
        actual = actual->siguiente;
    }
}
```

Si no lo haces, dos listas comparten los mismos nodos: al destruir una, la otra queda
con punteros colgantes (*dangling pointers*). Es un bug clásico de parcial.

### 4.3 Evitar la copia (recomendado en clase)

```cpp
ListaSimple(const ListaSimple&) = delete;
ListaSimple& operator=(const ListaSimple&) = delete;
```

---

## 5. Reglas de oro para no perder punteros

Cuando reasignes `head` o un `siguiente`, guarda la dirección vieja **primero**:

```cpp
// MAL: se pierde la referencia al resto de la lista
Nodo<int>* nuevo = new Nodo<int>(5);
nuevo->siguiente = head;
head = nuevo;              // en realidad aquí SÍ está bien, porque head es una variable

// MAL: sí se pierde
actual->siguiente = nuevo;      // el nodo nuevo no apunta a nada
actual = nuevo;                 // se "!pierde" la dirección del resto

// BIEN:
Nodo<int>* aEliminar = actual->siguiente;
actual->siguiente = actual->siguiente->siguiente;   // puentea al vecino
delete aEliminar;                                  // libera el huérfano
```

**Truco para memorizarlo**: `guardar` → `puentear` → `borrar`.

---

## 6. `using namespace std;` — por qué conviene evitarlo

Tu código actual tiene `using namespace std;` en la línea 2. Funciona, pero es mala
práctica: si declaras una clase `Lista` y existe `std::list`, el compilador se queja con
ambigüedad. En los ejercicios de este curso **no se usa**, y en su lugar:

```cpp
#include <iostream>
#include <string>

int main() {
    std::cout << "hola\n";     // calificar con std::
}
```

Úsalo solo si tu profesor lo exige y solo si el código compila limpio.

---

## 7. Cómo declarar una plantilla correctamente

Si divides el código en `.h` y `.cpp`:

```cpp
// Nodo.h
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};
```

```cpp
// main.cpp  (una sola unidad de traducción, así funciona directo)
#include "Nodo.h"
int main() { Nodo<double> n(3.5); std::cout << n.dato; }
```

Si quieres separar de verdad, toda la definición de la plantilla (no solo la declaración)
debe estar en el `.h`, o usar `template class Nodo<int>;` explícitamente. Para el coursework
lo más seguro es: **un solo archivo `.cpp`**.

---

## 8. Checklist antes de entregar

- [ ] ¿Todas las variables están inicializadas?
- [ ] ¿Cada `new` tiene su `delete`?
- [ ] ¿El bucle `while` usa `!= nullptr`?
- [ ] ¿Probé lista vacía, un elemento y varios?
- [ ] ¿El método `mostrar()` imprime `nullptr` al final?
- [ ] ¿La clase tiene destructor?
- [ ] Compila con `-Wall` sin warnings.

---

## Siguiente tutorial → [03. Lista simplemente enlazada](03_lista_simplemente_enlazada.md)