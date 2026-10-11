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

## Siguiente tutorial → [07. Colas y Deque](07_colas.md)