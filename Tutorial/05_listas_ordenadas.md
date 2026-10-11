# 05. Listas ordenadas y operaciones clásicas

Una lista está **ordenada** si sus datos guardan la relación de orden de principio a fin
(de menor a mayor, o de mayor a menor). Mantener el orden cuesta O(n) por inserción, pero
convierte muchas operaciones en algo más simple y evita duplicados.

---

## 1. Inserción ordenada

La operación central de una lista ordenada. Se recorre hasta encontrar el lugar y se inserta
**antes** del primer nodo mayor, o al final si no hay ninguno.

```cpp
template <typename T>
class ListaOrdenada {
private:
    Nodo<T>* head;

public:
    // Inserta manteniendo el orden creciente. No admite duplicados.
    void insertar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);

        // Caso 1: lista vacía
        if (head == nullptr) { head = nuevo; return; }

        // Caso 2: el valor es el más pequeño -> va a la cabeza
        if (valor <= head->dato) {
            nuevo->siguiente = head;
            head = nuevo;
            return;
        }

        // Caso 3: buscar el nodo anterior
        Nodo<T>* actual = head;
        while (actual->siguiente != nullptr && actual->siguiente->dato < valor) {
            actual = actual->siguiente;
        }

        nuevo->siguiente = actual->siguiente;   // puentear hacia adelante
        actual->siguiente = nuevo;             // puentear hacia atrás
    }
};
```

Complejidad: **O(n)** en el peor caso, **O(1)** si el valor es el menor.

Diagrama insertando `7` en `[3, 5, 9, 12]`:

```
head -> [3] -> [5] -> [9] -> [12] -> nullptr
            |
            v  actual
insertando 7:  actual->siguiente->dato (9) < 7 ? NO -> paramos

head -> [3] -> [5] -> [7] -> [9] -> [12] -> nullptr
                     ^
                     nuevo
```

---

## 2. Eliminar duplicados

Si la lista está ordenada, los duplicados están **juntos**, así que basta comparar cada
nodo con su vecino.

```cpp
Nodo<T>* actual = head;
while (actual != nullptr && actual->siguiente != nullptr) {
    if (actual->dato == actual->siguiente->dato) {
        Nodo<T>* repetido = actual->siguiente;      // este es el que sobra
        actual->siguiente = repetido->siguiente;   // puentear
        delete repetido;                           // borrar
    } else {
        actual = actual->siguiente;                // avanzar SOLO si no hubo borrado
    }
}
```

⚠️ **El detalle que casi todos fallan**: si borraste un nodo, `actual` **no se mueve**.
Si avanzas, te saltas comparar el valor duplicado que quedó justo después.

📄 Ejercicio completo: [`05_eliminar_duplicados.cpp`](../Ejercicios/Resueltos/05_eliminar_duplicados.cpp)

---

## 3. Partición en dos listas

Problema clásico: *"dada una lista, crear dos: una con los pares y otra con los impares"*.

```cpp
//Se recorre una vez y se va colgando cada nodo de la lista que le toca
Nodo<int>* pares = nullptr;
Nodo<int>* impares = nullptr;
Nodo<int>* actual = head;

while (actual != nullptr) {
    Nodo<int>* sig = actual->siguiente;   // salva el siguiente
    actual->siguiente = nullptr;         // lo desconecta de la lista original

    if (actual->dato % 2 == 0) {
        actual->siguiente = pares; pares = actual;     // insertar al inicio: O(1)
    } else {
        actual->siguiente = impares; impares = actual;
    }
    actual = sig;
}
```

Ojo: al colgar al inicio, la lista resultante queda en **orden inverso** respecto al
recorrido. Si quieres conservar el orden, usa `tail` de cada lista auxiliar.

---

## 4. Fusión (merge) de dos listas ordenadas

Se usa **un solo puntero de comparación**: se avanza por la lista cuyo valor sea menor.

```cpp
Nodo<T>* fusionar(Nodo<T>* a, Nodo<T>* b) {
    Nodo<T>* dummy = new Nodo<T>(0);   // nodo centinela: simplifica los bordes
    Nodo<T>* cola = dummy;

    while (a != nullptr && b != nullptr) {
        if (a->dato <= b->dato) { cola->siguiente = a; a = a->siguiente; }
        else                   { cola->siguiente = b; b = b->siguiente; }
        cola = cola->siguiente;
    }
    cola->siguiente = (a != nullptr) ? a : b;   // cuelga lo que sobre

    Nodo<T>* resultado = dummy->siguiente;
    delete dummy;      // el centinela no se devuelve, solo se usó de "ancla"
    return resultado;
}
```

El **nodo centinela (dummy)** es un truco clásico: te ahorra el `if` para el caso
"lista vacía" y el `if` para el final. Se crea, se usa y se borra.

📄 Versión recursiva y con `std::vector`: [`07_fusionar_listas.cpp`](../Ejercicios/Resueltos/07_fusionar_listas.cpp)

---

## 5. Unión e intersección

Sobre **dos listas ordenadas sin duplicados**:

| Operación | Algoritmo | Coste |
|---|---|---|
| Unión | Fusionar ambas y quitar duplicados (o insertar todo en una lista ordenada) | O(n + m) |
| Intersección | Avanzar con dos punteros: si son iguales, guardo; si `a < b`, avanzo `a`; si `b < a`, avanzo `b` | O(n + m) |
| Unión orden = `O(n + m)` | Mucho mejor que insertar de a uno, que sería O(n·m) | |

---

## 6. Otros clásicos que debes saber hacer

| Problema | Idea | Coste |
|---|---|---|
| **Rotar la lista k** | Calcular `k % largo()`, cortar en dos y reencolar | O(n) |
| **Palíndromo** | Recorrer con dos punteros desde extremos, comparando | O(n) |
| **Detectar ciclo** | Algoritmo de Floyd (dos punteros a distinta velocidad) | O(n) |
| **Invertir en pares** | Intercambiar `dato` entre nodos contiguos | O(n) |
| **Sumatoria de sublistas** | Acumular al recorrer | O(n) |
| **Aplanar listas anidadas** | Cola: se meten todas y se extraen | O(n) |
| **Mover el n-ésimo al frente** | Recorrer hasta él y puentear | O(n) |
| **Revertir un subsegmento (leetcode 92)** | Tres punteros | O(n) |
| **Quitar duplicados sin ordenar** | Tabla hash auxiliar | O(n) |
| **Mayor suma de dos elementos contiguos** | Ventana deslizante de tamaño 2 | O(n) |

---

## 7. Rotar la lista: el cálculo clave

Si la lista tiene 5 nodos y quieres rotar 7 posiciones, el resultado es el mismo que
rotar `7 % 5 = 2` posiciones.

```
Lista:      [1] -> [2] -> [3] -> [4] -> [5] -> nullptr
k = 7, largo = 5, por lo tanto k % largo = 2
Resultado:  [4] -> [5] -> [1] -> [2] -> [3] -> nullptr
            (desplazamos a la derecha, el signo importa)
```

📄 Ejercicio completo: [`08_rotar_lista.cpp`](../Ejercicios/Resueltos/08_rotar_lista.cpp)

---

## Siguiente tutorial → [06. Pilas (Stack)](06_pilas.md)