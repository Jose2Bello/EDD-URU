# 11. Resumen para el examen: pilas y colas

Chuleta de una página. Si mañana tienes el parcial y solo puedes releer algo,
relee esto.

---

## 1. LIFO vs FIFO, en una línea

| Estructura | Entra | Sale | Analogía |
|---|---|---|---|
| **Pila** | por el tope | por el tope | Pila de platos |
| **Cola** | por el final | por el frente | Fila del supermercado |
| **Deque** | por ambos | por ambos | Fila con dos puertas |
| **Cola de prioridad** | donde toque | el más urgente | Urgencias del hospital |

---

## 2. Esqueletos listos para escribir

### Pila con nodos

```cpp
template <typename T>
class Nodo { public: T dato; Nodo<T>* sig; Nodo(T v): dato(v), sig(nullptr) {} };

template <typename T>
class Pila {
    Nodo<T>* top;
public:
    Pila() : top(nullptr) {}
    ~Pila() { while (top) { Nodo<T>* b = top->sig; delete top; top = b; } }

    void apilar(T v) {                          // O(1)
        Nodo<T>* n = new Nodo<T>(v);
        n->sig = top;
        top = n;
    }
    bool desapilar(T& out) {                    // O(1)
        if (!top) return false;
        Nodo<T>* b = top;
        out = b->dato;                          // copiar ANTES de borrar
        top = b->sig;
        delete b;
        return true;
    }
    bool cima(T& out) const { if (!top) return false; out = top->dato; return true; }
    bool estaVacia() const { return top == nullptr; }
};
```

### Cola con dos punteros

```cpp
template <typename T>
class Cola {
    Nodo<T>* frente; Nodo<T>* cola;
public:
    Cola() : frente(nullptr), cola(nullptr) {}
    ~Cola() { while (frente) { Nodo<T>* s = frente->sig; delete frente; frente = s; } }

    void encolar(T v) {                         // O(1)
        Nodo<T>* n = new Nodo<T>(v);
        if (!cola) frente = cola = n;
        else { cola->sig = n; cola = n; }
    }
    bool desencolar(T& out) {                   // O(1)
        if (!frente) return false;
        Nodo<T>* b = frente;
        out = b->dato;
        frente = b->sig;
        delete b;
        if (!frente) cola = nullptr;            // ¡no olvidar!
        return true;
    }
    bool estaVacia() const { return frente == nullptr; }
};
```

### Cola circular (arreglo)

```cpp
class ColaCircular {
    int* datos; int cap, frente, final, cantidad;
public:
    explicit ColaCircular(int c): cap(c), frente(0), final(0), cantidad(0) {
        datos = new int[cap];
    }
    ~ColaCircular() { delete[] datos; }

    bool encolar(int v) {
        if (llena()) return false;
        datos[final] = v;
        final = (final + 1) % cap;              // el índice da la vuelta
        cantidad++;
        return true;
    }
    bool desencolar(int& out) {
        if (vacia()) return false;
        out = datos[frente];
        frente = (frente + 1) % cap;
        cantidad--;
        return true;
    }
    bool vacia() const { return cantidad == 0; }
    bool llena() const { return cantidad == cap; }
};
```

---

## 3. Los cuatro punteros que debes saber dibujar

```
PILA:   top -> [30] -> [20] -> [10] -> nullptr
         ^ entra y sale por aquí

COLA:   frente -> [10] -> [20] -> [30] -> nullptr
         ^sale                    ^entra (cola/back)
```

---

## 4. Reglas de oro

1. **Comprobar vacío** antes de sacar o mirar.
2. **Copiar el dato antes** de `delete`.
3. **Cada `new` con su `delete`**; los arreglos con `delete[]`.
4. En la cola, al quedar vacía, poner **`cola = nullptr`** además de `frente`.
5. En la cola circular, el **módulo** hace la vuelta; `cantidad` distingue vacía
   de llena.
6. En una pila solo se toca el **tope**; en una cola, los **dos extremos**.

---

## 5. Qué estructura usar

| El enunciado dice… | Usa |
|---|---|
| "el último que entró" / deshacer / historial | **Pila** |
| "por orden de llegada" / turnos / impresión | **Cola** |
| "por ambos extremos" / ventana deslizante | **Deque** |
| "el más urgente primero" | **Cola de prioridad** |
| "nivel por nivel" / camino más corto sin pesos | **Cola (BFS)** |
| "paréntesis balanceados" / expresión postfija | **Pila** |

---

## 6. Tabla de complejidades

| Operación | Pila | Cola | Cola circular | Deque | Cola prioridad (heap) |
|---|---|---|---|---|---|
| insertar | O(1) | O(1) | O(1) | O(1) | O(log n) |
| sacar | O(1) | O(1) | O(1) | O(1) | O(log n) |
| ver extremo | O(1) | O(1) | O(1) | O(1) | O(1) |
| buscar | O(n) | O(n) | O(n) | O(n) | O(n) |

---

## 7. Diez preguntas de examen con respuesta corta

1. **¿Por qué la pila es O(1) en `pop`?** → El tope está en el objeto; no se recorre.
2. **¿Por qué la cola necesita `back`?** → Para encolar en O(1) sin recorrer.
3. **¿Cuándo se queda `cola` desactualizada?** → Al desencolar el último; hay que ponerla en `nullptr`.
4. **¿Cómo se detecta "llena" en una cola circular?** → Con un contador, porque `frente == final` también ocurre cuando está vacía.
5. **¿Qué hace `(frente + 1) % capacidad`?** → Avanzar el índice y volver al inicio al llegar al final.
6. **¿Cómo se invierte una pila?** → Recursión con `insertarEnFondo`, o dos pilas auxiliares.
7. **¿Cómo se hace una cola con dos pilas?** → Apilar en una; para sacar, pasar todo a la otra.
8. **¿Cómo se hace una pila con dos colas?** → Encolar en una; para sacar, dejar el último fuera y devolver el resto.
9. **¿Qué estructura usa BFS?** → Una cola, porque visita por niveles y garantiza el camino más corto.
10. **¿Qué desempata en una cola de prioridad?** → El orden de llegada (FIFO) o el inverso (LIFO), según se defina.

---

## 8. Checklist de entrega

- [ ] Constructor inicializa todos los punteros a `nullptr`.
- [ ] Destructor libera todos los nodos (o `delete[]` del arreglo).
- [ ] `desapilar` / `desencolar` devuelven `false` si está vacía.
- [ ] Se probaron los casos: vacía, un elemento, vaciar del todo, sacar de vacía.
- [ ] No se lee `->siguiente` después de `delete`.
- [ ] La cola pone `cola = nullptr` al vaciarse.
- [ ] La cola circular usa módulo y contador.
- [ ] Compila con `-Wall -Wextra` sin advertencias.

---

Volver al [README principal](../README.md) · [06. Pilas](06_pilas.md) ·
[07. Colas](07_colas.md) · [10. Cola de prioridad](10_cola_de_prioridad.md)
