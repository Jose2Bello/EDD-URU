/* ============================================================================
   PLANTILLA 05 - Pila (stack, LIFO)
   Archivo : 05_pila.cpp

   Librerias: SOLO <iostream>. Nada de stack, string, sstream ni assert.
   Las cadenas se manejan como arreglos de char terminados en '\0'.

   Estructura: el tope (top) es la cabeza de la lista enlazada, por eso
   apilar y desapilar son O(1).

       top -> [30] -> [20] -> [10] -> nullptr
              sale     entra
              primero  ultimo

   Metodos de Pila:      apilar, desapilar, cima, estaVacia, estaLlena,
                         tamano, limpiar, mostrar
   Metodos de PilaArreglo: apilar, desapilar, cima, estaVacia, estaLlena,
                         tamano, indiceDelTope

   Incluye 3 aplicaciones clasicas: parentesis balanceados, infija -> postfija
   y evaluacion de la postfija.
   ========================================================================== */

#include <iostream>

// ---------------------------------------------------------------------------
// Nodo
// ---------------------------------------------------------------------------
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

// ---------------------------------------------------------------------------
// Pila con nodos (no tiene limite de capacidad)
// ---------------------------------------------------------------------------
template <typename T>
class Pila {
private:
    Nodo<T>* top;

public:
    Pila() : top(nullptr) {}

    ~Pila() {
        while (top != nullptr) {
            Nodo<T>* base = top->siguiente;   // lo que estaba debajo
            delete top;
            top = base;
        }
    }

    void apilar(T valor) {                   // O(1)
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = top;               // el nuevo se apoya sobre el anterior
        top = nuevo;
    }

    bool desapilar(T& salida) {              // O(1)
        if (top == nullptr) return false;
        Nodo<T>* aBorrar = top;
        salida = aBorrar->dato;               // se copia ANTES de borrar
        top = aBorrar->siguiente;
        delete aBorrar;
        return true;
    }

    bool cima(T& salida) const {             // O(1), no saca el elemento
        if (top == nullptr) return false;
        salida = top->dato;
        return true;
    }

    bool estaVacia() const { return top == nullptr; }
    bool estaLlena()  const { return false; }   // con nodos nunca se llena

    int tamano() const {
        int n = 0;
        for (Nodo<T>* a = top; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    void limpiar() {
        T nada;
        while (desapilar(nada)) { /* se repite hasta vaciar */ }
    }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta;
        Nodo<T>* a = top;
        while (a != nullptr) {
            std::cout << a->dato << " ";
            a = a->siguiente;
        }
        if (top == nullptr) std::cout << "(vacia)";
        std::cout << "     <- el de la izquierda es el tope\n";
    }
};

// ---------------------------------------------------------------------------
// Pila con arreglo y capacidad fija.
// "tope" es el indice del ultimo elemento: -1 = vacia, 0..capacidad-1 = llena.
// ---------------------------------------------------------------------------
template <typename T>
class PilaArreglo {
private:
    T* datos;
    int tope;
    int capacidad;

public:
    PilaArreglo(int capacidadInicial)
        : tope(-1), capacidad(capacidadInicial) {
        datos = new T[capacidad];
    }

    ~PilaArreglo() { delete[] datos; }        // los arreglos usan delete[]

    void apilar(T valor) {
        if (estaLlena()) return;               // si esta llena, no se apila
        datos[++tope] = valor;
    }

    bool desapilar(T& salida) {
        if (tope < 0) return false;
        salida = datos[tope--];
        return true;
    }

    bool cima(T& salida) const {
        if (tope < 0) return false;
        salida = datos[tope];
        return true;
    }

    bool estaVacia() const { return tope < 0; }
    bool estaLlena()  const { return tope + 1 >= capacidad; }
    int  tamano() const { return tope + 1; }
    int  indiceDelTope() const { return tope; }
};

// ===========================================================================
//  APLICACION 1: parentesis, llaves y corchetes balanceados
// ===========================================================================
bool parentesisBalanceados(const char* cadena) {
    Pila<char> pila;

    for (int i = 0; cadena[i] != '\0'; i++) {
        char c = cadena[i];

        if (c == '(' || c == '[' || c == '{') {
            pila.apilar(c);                       // abre -> se apila
        } else if (c == ')' || c == ']' || c == '}') {
            char abierto;
            if (!pila.desapilar(abierto)) return false;    // cierre sin apertura
            bool correcto = (c == ')' && abierto == '(') ||
                            (c == ']' && abierto == '[') ||
                            (c == '}' && abierto == '{');
            if (!correcto) return false;                   // tipo equivocado
        }
    }

    return pila.estaVacia();   // si sobran aperturas, NO esta balanceada
}

// ===========================================================================
//  APLICACION 2: infija -> postfija (notacion polaca inversa)
//  Los operadores salen DESPUES de sus operandos.
//      3 + 4 * 5   ->   3 4 5 * +
// ===========================================================================

bool esDigito(char c) { return c >= '0' && c <= '9'; }

bool esOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int precedencia(char operador) {
    if (operador == '+' || operador == '-') return 1;
    if (operador == '*' || operador == '/') return 2;
    if (operador == '^') return 3;
    return 0;
}

// "salida" debe caber: se reserva un arreglo grande en el main
void aPostfija(const char* infija, char* salida) {
    Pila<char> operadores;
    int k = 0;                                   // posicion de escritura

    for (int i = 0; infija[i] != '\0'; i++) {
        char c = infija[i];

        if (esDigito(c)) {
            salida[k++] = c;
            salida[k++] = ' ';                   // separa los tokens
        } else if (c == ' ') {
            continue;                            // se ignoran los espacios
        } else if (c == '(') {
            operadores.apilar(c);
        } else if (c == ')') {
            char t;
            while (operadores.cima(t) && t != '(') {   // vacia hasta el '('
                operadores.desapilar(t);
                salida[k++] = t;
                salida[k++] = ' ';
            }
            operadores.desapilar(t);             // se descarta el '('
        } else if (esOperador(c)) {
            char t;
            while (operadores.cima(t) && t != '(' && precedencia(t) >= precedencia(c)) {
                operadores.desapilar(t);         // sale el de mayor precedencia
                salida[k++] = t;
                salida[k++] = ' ';
            }
            operadores.apilar(c);
        }
    }

    char t;
    while (operadores.desapilar(t)) {             // vacia lo que sobre
        salida[k++] = t;
        salida[k++] = ' ';
    }

    salida[k > 0 ? k - 1 : 0] = '\0';             // quita el espacio final
}

// ===========================================================================
//  APLICACION 3: evaluar una expresion en postfija
//  Al leer un operador se sacan DOS operandos de la pila.
// ===========================================================================
double aplicar(char operador, double a, double b) {
    switch (operador) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b == 0.0) ? 0.0 : a / b;
        default:  return 0.0;
    }
}

double evaluarPostfija(const char* postfija) {
    Pila<double> valores;
    int i = 0;

    while (postfija[i] != '\0') {
        char c = postfija[i];

        if (esDigito(c)) {
            double numero = 0;
            while (esDigito(postfija[i])) {      // lee un numero completo
                numero = numero * 10 + (postfija[i] - '0');
                i++;
            }
            valores.apilar(numero);
        } else if (c == ' ') {
            i++;
        } else {                               // es un operador
            double b = 0.0, a = 0.0;
            valores.desapilar(b);               // primero el operando derecho
            valores.desapilar(a);               // luego el izquierdo
            valores.apilar(aplicar(c, a, b));
            i++;
        }
    }

    double resultado = 0;
    valores.desapilar(resultado);
    return resultado;
}

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== PLANTILLA 05: pila ===\n\n";

    // 1) Pila de nodos
    std::cout << "1) Pila con nodos (sin limite de capacidad)\n";
    Pila<int> pila;
    pila.mostrar("   ");
    pila.apilar(10);
    pila.apilar(20);
    pila.apilar(30);
    pila.mostrar("   apilar 10, 20, 30 -> ");

    int v;
    pila.cima(v);
    std::cout << "   cima = " << v << "   tamano = " << pila.tamano() << "\n";

    std::cout << "   desapilando: ";
    while (pila.desapilar(v)) std::cout << v << " ";
    std::cout << "\n   desapilar en pila vacia = " << std::boolalpha << pila.desapilar(v) << "\n\n";

    // 2) Pila con capacidad fija
    std::cout << "2) Pila con arreglo (capacidad 3)\n";
    PilaArreglo<int> fija(3);
    for (int i = 1; i <= 5; i++) {
        if (fija.estaLlena()) std::cout << "   no se apila " << i * 10 << ": la pila esta llena\n";
        else                  fija.apilar(i * 10);
    }
    std::cout << "   tamano = " << fija.tamano()
              << "   indiceDelTope = " << fija.indiceDelTope() << "\n\n";

    // 3) Parentesis balanceados
    std::cout << "3) parentesisBalanceados\n";
    const char* pruebas[] = {"{[(())]}", "((", ")(", "([)]", "a + (b * (c - d))"};
    for (int i = 0; i < 5; i++) {
        std::cout << "   \"" << pruebas[i] << "\" -> " << std::boolalpha
                  << parentesisBalanceados(pruebas[i]) << "\n";
    }
    std::cout << "\n";

    // 4) Infija -> postfija -> resultado
    std::cout << "4) infija -> postfija -> resultado\n";
    const char* infijas[] = {"3 + 4 * 5", "1 + 2 * 3 - 4", "( 3 + 4 ) * 5", "8 / 2 + 3"};
    char postfija[64];

    for (int i = 0; i < 4; i++) {
        aPostfija(infijas[i], postfija);
        std::cout << "   " << infijas[i] << "   ->   \"" << postfija << "\"   = "
                  << evaluarPostfija(postfija) << "\n";
    }

    std::cout << "\nFin de la plantilla.\n";
    return 0;
}