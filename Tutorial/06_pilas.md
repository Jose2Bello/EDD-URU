# 06. Pilas (Stack)

La **pila** es la estructura de datos más simple y la más usada. Es **LIFO**
(*Last In, First Out*): el último que entra es el primero que sale. Igual que una
pila de platos: solo puedes sacar del cima.

---

## 1. Concepto y operaciones

```
        push (empujar)          pop (sacar)
              |                      ^
              v                      |
          +----------+          +----------+
          |    30    | <-- top  |    20    |
          +----------+          +----------+
          |    20    |          |    10    |
          +----------+          +----------+
              (top apunta al nodo 30)
```

| Operación | Qué hace | Coste |
|---|---|---|
| `push(x)` / `apilar(x)` | mete `x` arriba | **O(1)** |
| `pop()` / `desapilar()` | saca el de arriba | **O(1)** |
| `top()` / `cima()` | mira el de arriba sin sacarlo | **O(1)** |
| `isEmpty()` / `estaVacia()` | ¿no hay nada? | **O(1)** |
| `size()` / `tamano()` | cuántos hay | O(1) con contador |

Regla de oro: **solo se toca el extremo superior**. Nunca se inserta ni se busca en el medio.

---

## 2. Implementación con lista enlazada (lo que se pide en el coursework)

Se aprovecha que insertar y eliminar al inicio es O(1).

```cpp
template <typename T>
class Pila {
private:
    Nodo<T>* top;

public:
    Pila() : top(nullptr) {}

    ~Pila() {
        while (top != nullptr) {
            Nodo<T>* base = top->siguiente;  // lo que estaba debajo
            delete top;
            top = base;
        }
    }

    void apilar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = top;   // el nuevo se apoya sobre el anterior
        top = nuevo;
    }

    bool desapilar(T& salida) {
        if (top == nullptr) return false;   // pila vacía
        Nodo<T>* aBorrar = top;
        salida = top->dato;                // se guarda el valor ANTES de borrar
        top = top->siguiente;
        delete aBorrar;
        return true;
    }

    bool cima(T& salida) const {
        if (top == nullptr) return false;
        salida = top->dato;
        return true;
    }

    bool estaVacia() const { return top == nullptr; }

    int tamano() const {
        int n = 0;
        for (Nodo<T>* a = top; a != nullptr; a = a->siguiente) n++;
        return n;
    }
};
```

📌 Nota el patrón `bool desapilar(T& salida)`: en C++ no existen los parámetros `out`.
La alternativa más simple es que el método **devuelva el valor** y, si puede fallar,
que devuelva un valor centinela. Con el parámetro por referencia el método devuelve
`true/false` (¿tuvo éxito?) y el valor real viaja por `salida`.

### Implementación alternativa con arreglo

```cpp
template <typename T>
class PilaArreglo {
private:
    T* datos;
    int tope;      // cantidad de elementos ocupados (-1 = vacía)
    int capacidad;

public:
    PilaArreglo(int cap) : tope(-1), capacidad(cap) {
        datos = new T[cap];
    }
    ~PilaArreglo() { delete[] datos; }

    void apilar(T v) {
        if (tope + 1 >= capacidad) return;   // pila llena: no se apila
        datos[++tope] = v;
    }
    bool desapilar(T& salida) {
        if (tope < 0) return false;
        salida = datos[tope--];
        return true;
    }
};
```

⚠️ Recordatorio: los arreglos se liberan con `delete[]`, no con `delete`.

---

## 3. La biblioteca estándar

```cpp
#include <stack>
std::stack<int> p;
p.push(1);
p.push(2);
int x = p.top();
p.pop();
if (!p.empty()) { /*...*/ }
```

En un examen casi siempre te piden la **implementación a mano**, porque lo que se evalúa
es que entiendas los punteros. Usa la librería solo para comparar o para verificar.

---

## 4. Aplicaciones reales de la pila

| Aplicación | Cómo funciona la pila |
|---|---|
| **Deshacer / Rehacer** (Ctrl+Z) | Cada acción se apila; deshacer saca de la pila |
| **Historial del navegador** | Cada URL visitada se apila; "atrás" saca del tope |
| **Verificación de paréntesis** | Abre → apila, cierra → desapila y compara |
| **Evaluación de expresiones (postfija / infija)** | Lee de derecha a izquierda, operadores a la pila |
| **Conversión infija → postfija** | Precedencia de operadores mediante pila |
| **Compiladores (análisis sintáctico)** | Cada llaves/llave abierta se apila |
| **Resolución de laberintos / backtracking** | Guardar el camino recorrido para retroceder |
| **Llamadas a funciones anidadas** | La pila de llamadas del sistema |
| **Inversión de cadenas** | A apilar los caracteres y luego desapilar |

---

## 5. Algoritmo de balanceo de paréntesis

El ejemplo canónico. Recorre la cadena y apila cada símbolo de apertura:

```cpp
bool balanceada(const std::string& s) {
    Pila<char> p;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            p.apilar(c);
        } else if (c == ')' || c == ']' || c == '}') {
            char abierto;
            if (!p.desapilar(abierto)) return false;   // cierre sin apertura
            bool correcto = (c == ')' && abierto == '(') ||
                            (c == ']' && abierto == '[') ||
                            (c == '}' && abierto == '{');
            if (!correcto) return false;               // tipo equivocado
        }
    }
    return p.estaVacia();     // si sobran aperturas, NO está balanceada
}
```

Ojo con la última línea: si termina con `false`, no está balanceada. Ese es el error típico.

📄 Con validación de expresiones aritméticas: [`11_pila_expresiones.cpp`](../Ejercicios/Resueltos/11_pila_expresiones.cpp)

---

## 6. Convertir infija a postfija (RPN)

Notación **postfija** (RPN): los operadores van **después** de los operandos.
`3 + 4 * 5` → `3 4 5 * +`

```
Entrada:    3 + 4 * 5
Pila:      [3]      (3 es operando, se imprime directo)
           [3]
        [3]         + es operador -> a la pila
        [3, +]
     [3, +]          4 es operando
     [3, +]
  [3, +, 4]          * tiene mayor precedencia que + -> también a la pila
  [3, +, 4, *]
     [3, +, 4, *]    5 es operando
     [3, +, 4, *, 5]
Salida:     3 4 5
Al terminar, se vacía la pila imprimiendo: * y luego +  =>  "3 4 5 * +"
```

---

## 7. Errores típicos en exámenes

1. Confundir `top` con `head`: la pila **crece hacia arriba**, se apila por el mismo lado
   del que se saca.
2. Intentar acceder a un elemento del medio. No se puede.
3. No comprobar la pila vacía antes de `pop` o `top`.
4. Olvidar `delete` de los nodos.
5. Devolver la referencia al dato en vez de copiarlo (`T&` en el retorno es una bomba de
   reloj).

---

## 8. Anatomía: qué guarda realmente una pila

```
        objeto Pila<int>
        +----------------------+
        |  top: Nodo<T>*       |  <-- la pila ES un solo puntero
        +----------------------+
                 |
                 v
        +---------------+     +---------------+     +---------------+
        | dato: 30      | --> | dato: 20      | --> | dato: 10      |
        | siguiente: *  |     | siguiente: *  |     | siguiente:null|
        +---------------+     +---------------+     +---------------+
             tope                                     fondo
```

Consecuencias directas:

- Apilar y desapilar tocan **un solo puntero** (`top`) → **O(1)**.
- `tamano()` es O(n) si se recorre; **O(1)** si se guarda un contador aparte.
- **No se puede** acceder al elemento del medio: no hay `siguiente` que salte.
- El **fondo** es el primero que se apiló y el último que saldrá.

---

## 9. Las cinco operaciones que casi siempre piden además

### 9.1 `tamano()` en O(1)

Si el profesor insiste en O(1), guarda un contador en la clase y actualízalo en
`apilar` y `desapilar`. Si no, recorrer es perfectamente válido:

```cpp
int tamano() const {
    int n = 0;
    for (Nodo<T>* a = top; a != nullptr; a = a->siguiente) n++;
    return n;
}
```

### 9.2 Copia profunda (constructor copia y `operator=`)

El error clásico es copiar solo el puntero: las dos pilas comparten nodos y el
destructor borra dos veces. La copia correcta se hace **nodo por nodo**:

```cpp
Pila(const Pila& otra) : top(nullptr) { copiarDesde(otra.top); }

Pila& operator=(const Pila& otra) {
    if (this != &otra) {          // evitar autoasignación p = p
        limpiar();
        copiarDesde(otra.top);
    }
    return *this;
}

// Baja hasta el fondo y vuelve apilando: conserva el orden original.
void copiarDesde(Nodo<T>* nodo) {
    if (nodo == nullptr) return;
    copiarDesde(nodo->siguiente);
    apilar(nodo->dato);
}
```

### 9.3 Invertir la pila (sin estructuras extra)

Con recursión: se saca el tope, se invierte el resto y el valor se mete al fondo.

```cpp
void invertir() {
    if (estaVacia()) return;
    T fondo;
    desapilar(fondo);
    invertir();
    insertarEnFondo(fondo);
}

void insertarEnFondo(T valor) {
    if (estaVacia()) { apilar(valor); return; }
    T temporal;
    desapilar(temporal);
    insertarEnFondo(valor);
    apilar(temporal);
}
```

`3 2 1` (tope 3) → `1 2 3` (tope 1).

> Ojo: usar **una sola** pila auxiliar no invierte nada; hay que usar dos o
> recursión. Es una pregunta trampa frecuente.

### 9.4 Ordenar una pila

Solo con operaciones de pila: se saca el tope, se ordena el resto y se vuelve a
insertar en su sitio. Deja el **mayor en el tope**.

```cpp
void insertarOrdenado(T valor) {
    if (estaVacia() || valor > cima()) { apilar(valor); return; }
    T temporal;
    desapilar(temporal);
    insertarOrdenado(valor);
    apilar(temporal);
}

void ordenarMayorArriba() {
    if (estaVacia()) return;
    T temporal;
    desapilar(temporal);
    ordenarMayorArriba();
    insertarOrdenado(temporal);
}
```

### 9.5 El mínimo en O(1): `PilaMin`

Con una segunda pila `minimos` que solo guarda los valores que son nuevos
mínimos. Así `minimos.cima()` siempre es el mínimo actual.

```cpp
void apilar(T valor) {
    datos.apilar(valor);
    if (minimos.estaVacia() || valor <= minimos.cima())
        minimos.apilar(valor);        // <= para soportar duplicados del minimo
}

bool desapilar(T& salida) {
    if (!datos.desapilar(salida)) return false;
    if (!minimos.estaVacia() && salida == minimos.cima())
        minimos.desapilar(salida);
    return true;
}
```

📄 Todo esto implementado y probado: [`08_pila_completa.cpp`](../Plantillas/08_pila_completa.cpp)

---

## 10. Preguntas clásicas de examen

### "Implementa una pila usando dos colas"

La pila debe sacar el **último** que entró. Con colas, al desapilar se pasa todo
a la otra cola dejando el último fuera:

```cpp
template <typename T>
class PilaConDosColas {
    Cola<T> a, b;
public:
    void apilar(T v) { a.encolar(v); }

    bool desapilar(T& salida) {
        if (a.estaVacia()) return false;
        // pasar todo menos el ultimo a la otra cola
        while (a.tamano() > 1) { T x; a.desencolar(x); b.encolar(x); }
        bool ok = a.desencolar(salida);   // sale el ultimo
        while (!b.estaVacia()) { T x; b.desencolar(x); a.encolar(x); }
        return ok;
    }
};
```

`apilar` es O(1); `desapilar` es O(n). La operación inversa (cola con dos pilas)
está en el [tutorial 07](07_colas.md#10-cola-implementada-con-dos-pilas).

### "¿Por qué la pila permite `top` en O(1)?"

Porque el tope es la **cabeza** de la lista: está guardado directamente en el
objeto y no hay que recorrer nada.

### "¿Qué diferencia hay entre `pop` y `top`?"

`pop` **saca** el elemento (lo borra); `top` solo lo **mira** y deja la pila igual.

### "¿Se puede hacer una pila de tamaño fijo?"

Sí: con un arreglo y el índice `tope` (ver la `PilaArreglo` de la plantilla 05).
Ventaja: memoria contigua y sin `new` por elemento. Desventaja: hay que decidir la
capacidad de antemano y controlar el desbordamiento.

---

## 11. Tabla de complejidades

| Operación | Con nodos | Con arreglo |
|---|---|---|
| `apilar` | O(1) | O(1) |
| `desapilar` | O(1) | O(1) |
| `cima` | O(1) | O(1) |
| `estaVacia` | O(1) | O(1) |
| `tamano` | O(n) / O(1) con contador | O(1) (`tope + 1`) |
| `buscar` | O(n) | O(n) |
| `invertir` / `ordenar` | O(n) / O(n²) | O(n) / O(n²) |
| `minimo` con `PilaMin` | O(1) | O(1) |

---

## 12. Autoevaluación (respuestas al final)

1. ¿En qué se diferencia `top` de `head`?
2. ¿Qué pasa si se llama `desapilar` sobre una pila vacía?
3. ¿Cuántas pilas auxiliares se necesitan para invertir una pila?
4. ¿Por qué `PilaMin` compara con `<=` y no con `<`?
5. ¿Qué complejidad tiene ordenar una pila solo con operaciones de pila?

<details>
<summary>Respuestas</summary>

1. En la práctica son lo mismo (el primer nodo), pero el **significado** es
   distinto: `top` marca el extremo por el que entra y sale; `head` era solo el
   inicio de una lista.
2. Debe devolver `false` y no modificar nada. Nunca se debe leer `top->dato` sin
   comprobar antes `top != nullptr`.
3. Una sola **no** alcanza para invertir; se necesitan dos pilas auxiliares o
   recursión.
4. Porque puede haber **varios** elementos con el mismo mínimo. Si al desapilar
   uno quedara otro igual, el mínimo no debe cambiar.
5. O(n²): cada inserción ordenada recorre hasta el fondo de la pila auxiliar.

</details>

---

## Siguiente tutorial → [07. Colas y Deque](07_colas.md)