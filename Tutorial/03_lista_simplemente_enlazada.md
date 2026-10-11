# 03. Lista simplemente enlazada — análisis de tu `PracticaEDD.cpp`

La lista simplemente enlazada es una cadena de nodos donde **cada nodo conoce al
siguiente y solo al siguiente**. No hay acceso directo: para llegar al nodo 500 hay que
recorrer los 499 anteriores.

---

## 1. Anatomía de la estructura

```
        objeto ListaSimple
        +----------------------+
        |  head: Nodo<T>*      |  <-- la lista ES un solo puntero
        +----------------------+
                 |
                 v
        +---------------+        +---------------+        +---------------+
        | dato: 10     | ----> | dato: 20     | ----> | dato: 30     |
        | siguiente: * |        | siguiente: * |        | siguiente:null|
        +---------------+        +---------------+        +---------------+
```

Puntos clave:
- La lista **no guarda los datos**, solo la dirección del primer nodo.
- No hay `tail`, por eso insertar al final es O(n).
- No hay `prev`, por eso no se puede ir hacia atrás.

---

## 2. Análisis línea por línea de tu código

### 2.1 La clase `Nodo<T>` (líneas 4-12)

```cpp
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;

    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};
```

✅ Correcto. El constructor con lista de inicialización `: dato(valor), siguiente(nullptr)`
es la forma moderna y segura. Nota que `siguiente` **nunca** se pasa por parámetro: siempre
empieza en `nullptr`, y eso evita punteros basura.

### 2.2 El atributo `head` (línea 18)

```cpp
private:
    Nodo<T>* head;
```

✅ `private` es correcto: nadie externo debe mover la cabeza de la lista a su antojo.
Por eso en `main` **no puedes** usar `head`, y eso explica el error de la línea 120.

### 2.3 El constructor (línea 22)

```cpp
ListaSimple() : head(nullptr) {}
```

✅ Perfecto: una lista recién creada está vacía.

### 2.4 El destructor (líneas 25-32)

```cpp
~ListaSimple() {
    Nodo<T>* actual = head;
    while (actual != nullptr) {
        Nodo<T>* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}
```

✅ Correcto y con el orden adecuado: salva el `siguiente` **antes** de borrar.
 gracias a esto no hay fuga de memoria aunque nunca llames a un `eliminar`.

⚠️ Detalle: el destructor no pone `head = nullptr` al final. No es obligatorio (el objeto
está muriendo), pero si el destructor se llamara dos veces sería un doble `delete`. Por
seguridad, algunos cursos agregan `head = nullptr;` al final.

### 2.5 `agregarAlFinal` (líneas 35-47)

```cpp
void agregarAlFinal(T valor) {
    Nodo<T>* nuevoNodo = new Nodo<T>(valor);
    if (head == nullptr) {
        head = nuevoNodo;
        return;
    }

    Nodo<T>* actual = head;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevoNodo;
}
```

✅ Correcto. Estructura clásica en tres pasos:

1. **Caso especial:** lista vacía → el nuevo nodo es la cabeza y se sale temprano (`return`).
2. **Recorrido:** avanzar hasta el último nodo.
3. **Enganche:** el último nodo apunta al nuevo.

Complejidad: **O(n)** porque recorre toda la lista.

💡 Mejora (opcional, nivel bonus): agregar un puntero `tail` que apunte siempre al último
nodo. Con `tail`, insertar al final pasa a ser **O(1)**.

### 2.6 `eliminar` (líneas 50-74)

```cpp
bool eliminar(T valor) {
    if (head == nullptr) return false;          // A: lista vacía

    if (head->dato == valor) {                  // B: es la cabeza
        Nodo<T>* aEliminar = head;
        head = head->siguiente;
        delete aEliminar;
        return true;
    }

    Nodo<T>* actual = head;
    while (actual->siguiente != nullptr && actual->siguiente->dato != valor) {  // C
        actual = actual->siguiente;
    }

    if (actual->siguiente == nullptr) return false;   // D: no existe

    Nodo<T>* aEliminar = actual->siguiente;
    actual->siguiente = actual->siguiente->siguiente;
    delete aEliminar;
    return true;
}
```

✅ **Correcto y bien estructurado.** Es la solución estándar y cubre todos los casos:

| Caso | Qué hace |
|---|---|
| A | Lista vacía → `false` |
| B | El valor está en la cabeza → se mueve `head` y se borra el nodo viejo |
| C | Avanza buscando, parándose **uno antes** del candidato |
| D | Se llegó al final sin encontrar → `false` |
| E | Se encuentra: "puentea" y borra |

El truco de la línea C es que `actual` siempre queda **apuntando al nodo anterior** del que
se va a borrar; por eso `actual->siguiente` es a la vez "el nodo con el dato" y "el nodo
que hay que puentear".

⚠️ Mejora posible: `while (actual->siguiente != nullptr && ...)` se puede escribir como
`while (actual->siguiente && ...)`, es lo mismo.

### 2.7 `mostrar` (líneas 77-84)

```cpp
void mostrar() const {
    Nodo<T>* actual = head;
    while (actual != nullptr) {
        std::cout << "(" << actual->dato << ") -> ";
        actual = actual->siguiente;
    }
    std::cout << "nullptr";
}
```

✅ Correcto. El `const` es buena práctica: `mostrar` no modifica la lista, y el compilador
te lo comprueba. Tus aplicaciones **no lo necesitan** (no heredan de nada), pero no molesta.

Imprime `(1) -> (2) -> (3) -> nullptr`, que es justo la representación en texto del
diagrama de arriba. Perfecto para verificar en un examen.

### 2.8 `main` (líneas 87-127) — los errores

```cpp
ListaSimple<int> miLista;      // ✅ uso correcto de la plantilla
miLista.agregarAlFinal(1);     // ✅
...
miLista.mostrar();             // ✅
if (miLista.eliminar(2)) {} else { ... }   // ✅
miLista.agregarAlFinal(8);     // ✅

for (head != nullptr) {        // ❌  ERROR 1 y 2
    miLista.eliminar(i)        // ❌  ERROR 3
}
```

| # | Línea | Error | Mensaje del compilador |
|---|---|---|---|
| 1 | 120 | `head` es **privado** y no está declarado en `main` | `'head' was not declared in this scope` |
| 2 | 120 | `for` sin condición: `for (head != nullptr)` no es un bucle válido | `expected primary-expression` |
| 3 | 121 | `i` **no existe** y falta el punto y coma | `'i' was not declared in this scope` / `expected ';'` |

Además, aunque existiera un `i`, no tendrías **de dónde** sacarlo: necesitas el dato del
primer nodo. Por eso hay que añadir métodos de consulta a la clase.

---

## 3. Versión corregida y ampliada de `PracticaEDD.cpp`

```cpp
#include <iostream>
using namespace std;

template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T>
class ListaSimple {
private:
    Nodo<T>* head;

public:
    ListaSimple() : head(nullptr) {}

    ~ListaSimple() {
        Nodo<T>* actual = head;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        head = nullptr;
    }

    bool estaVacia() const { return head == nullptr; }          // NUEVO

    T primero() const {                                        // NUEVO
        return head != nullptr ? head->dato : T();
    }

    int largo() const {                                         // NUEVO
        int n = 0;
        for (Nodo<T>* a = head; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    void agregarAlFinal(T valor) {
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);
        if (head == nullptr) { head = nuevoNodo; return; }

        Nodo<T>* actual = head;
        while (actual->siguiente != nullptr) actual = actual->siguiente;
        actual->siguiente = nuevoNodo;
    }

    bool eliminar(T valor) {
        if (head == nullptr) return false;

        if (head->dato == valor) {
            Nodo<T>* aEliminar = head;
            head = head->siguiente;
            delete aEliminar;
            return true;
        }

        Nodo<T>* actual = head;
        while (actual->siguiente != nullptr && actual->siguiente->dato != valor)
            actual = actual->siguiente;

        if (actual->siguiente == nullptr) return false;

        Nodo<T>* aEliminar = actual->siguiente;
        actual->siguiente = actual->siguiente->siguiente;
        delete aEliminar;
        return true;
    }

    void mostrar() const {
        Nodo<T>* actual = head;
        while (actual != nullptr) {
            cout << "(" << actual->dato << ") -> ";
            actual = actual->siguiente;
        }
        cout << "nullptr";
    }
};

int main() {
    ListaSimple<int> miLista;

    cout << "Insertando 4 nodos \n";
    miLista.agregarAlFinal(1);
    miLista.agregarAlFinal(2);
    miLista.agregarAlFinal(3);
    miLista.agregarAlFinal(4);
    miLista.mostrar();                       // (1) -> (2) -> (3) -> (4) -> nullptr

    cout << "\nEliminando el nodo con valor 2 \n";
    if (miLista.eliminar(2)) cout << "Valor eliminado.\n";
    else                     cout << "Valor no encontrado.\n";

    miLista.mostrar();                       // (1) -> (3) -> (4) -> nullptr

    cout << "\nAgregando nodo al final\n";
    miLista.agregarAlFinal(8);
    miLista.mostrar();                       // (1) -> (3) -> (4) -> (8) -> nullptr

    cout << "\nVaciando la lista\n";
    while (!miLista.estaVacia()) {
        int i = miLista.primero();           // saco el dato ANTES de borrar el nodo
        miLista.eliminar(i);
        miLista.mostrar();
        cout << "\n";
    }

    cout << "Lista vacia: ";
    miLista.mostrar();                       // nullptr
    cout << "\nLargo final: " << miLista.largo() << "\n";
    return 0;
}
```

### El bucle que necesitabas

```cpp
while (!miLista.estaVacia()) {
    int i = miLista.primero();   // 1) obtener el dato
    miLista.eliminar(i);         // 2) borrar el nodo que lo contiene
}
```

Tres formas equivalentes y todas correctas:

```cpp
// A) con los métodos de la clase (la más legible)
while (!miLista.estaVacia()) { miLista.eliminar(miLista.primero()); }

// B) con un puntero público auxiliar (no recomendado: rompe el encapsulamiento)
while (miLista.head != nullptr) { miLista.eliminar(miLista.head->dato); }

// C) vaciando de golpe con un método dedicado (O(n) y sinbucles raros de lógica)
miLista.vaciar();
```

---

## 4. Las operaciones que te van a pedir agregar

Tu lista actual solo tiene `agregarAlFinal`, `eliminar` y `mostrar`. Estas son las
clásicas que debes saber hacer (todas aparecen resueltas en los ejercicios):

| Operación | Idea | Complejidad |
|---|---|---|
| `agregarAlInicio` | `nuevo->siguiente = head; head = nuevo;` | O(1) |
| `agregarAntesDe(valorRef, valor)` | recorrer hasta el anterior e insertar entre medias | O(n) |
| `buscar(valor)` | recorrer comparando, devolver `bool` o posición | O(n) |
| `invertir()` | `prev / actual / siguiente` (tres punteros) | O(n) |
| `invertirRecursivo()` | mismo resultado con recursión | O(n) |
| `largo()` | contar en un recorrido | O(n) |
| `eliminarDuplicados()` | comparar nodo actual con su siguiente | O(n) |
| `concatenar(otra)` | puentear el final de A con el head de B | O(n) |
| `invertirEnPar()` | reordenar de forma intercalada | O(n) |
| `sumar / máximo / mínimo` | acumular en el recorrido | O(n) |

La técnica de **tres punteros para invertir** (ejercicio 02):

```cpp
Nodo<T>* prev = nullptr;
Nodo<T>* actual = head;
while (actual != nullptr) {
    Nodo<T>* siguiente = actual->siguiente;  // salva al siguiente
    actual->siguiente = prev;                // invierte la flecha
    prev = actual;                           // avanza prev
    actual = siguiente;                      // avanza actual
}
head = prev;                                 // la nueva cabeza es el último
```

Diagrama de una inversión con 3 nodos:

```
Antes:   head -> [10] -> [20] -> [30] -> nullptr
                               |
      paso 1 (actual=10):      v
            prev=null  [10] -> null    [20] -> [30]
      paso 2 (actual=20):
            [10] <- [20] -> null        [30]
      paso 3 (actual=30):
            [10] <- [20] <- [30] -> null
                                |
                                v
                               head
```

---

## 5. Complejidad de tu implementación

| Método | Mejor caso | Caso promedio | Peor caso |
|---|---|---|---|
| `agregarAlFinal` | O(1) (lista vacía) | O(n) | O(n) |
| `eliminar` | O(1) (es la cabeza) | O(n) | O(n) |
| `mostrar` | O(n) | O(n) | O(n) |
| destructor | O(n) | O(n) | O(n) |

---

## Siguiente tutorial → [04. Lista doblemente enlazada y circular](04_lista_doble_y_circular.md)