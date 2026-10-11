/* ============================================================================
   EJERCICIO 11 - Pila: validacion de parentesis y evaluacion de expresiones
   Tema   : Pila (stack) - LIFO
   Archivo: 11_pila_expresiones.cpp
   ============================================================================

   ENUNCIADO
   --------
   Implementar una pila (stack) y usarla en tres problemas clasicos:

   1) Implementar la clase Pila<T> con apilar, desapilar, cima, estaVacia,
      tamano, estaLlena y limpiar.

   2) Validar parentesis y llaves balanceados en una cadena.
      debe devolver false si:
        - hay un cierre sin su apertura
        - el tipo no corresponde:  ( ) [ ] { }
        - al final sobran aperturas

   3) Convertir una expresion infija a POSTFIJA (notacion polaca inversa).
      Reglas de precedencia:  + -   <   * /   <   ^
      Ejemplo:  3 + 4 * 5   ->  "3 4 5 * +"

   4) Evaluar una expresion POSTFIJA directamente.
      Ejemplo:  "3 4 5 * +"  ->  23

   5) Evaluar una expresion INFIJA directamente usando dos pilas
      (una de valores y otra de operadores).

   ========================================================================== */

#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <sstream>

// ---------------------------------------------------------------------------
// Nodo (se declara primero porque la Pila lo usa)
// ---------------------------------------------------------------------------
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

// ---------------------------------------------------------------------------
// PILA implementada con lista enlazada (se ve el "crecimiento hacia arriba")
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

    void apilar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = top;   // el nuevo se apoya sobre el anterior
        top = nuevo;
    }

    bool desapilar(T& salida) {
        if (top == nullptr) return false;
        Nodo<T>* aBorrar = top;
        salida = top->dato;              // se copia ANTES de borrar
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
    bool estaLlena()  const { return false; }   // con nodos nunca se llena

    int tamano() const {
        int n = 0;
        for (Nodo<T>* a = top; a != nullptr; a = a->siguiente) n++;
        return n;
    }

    void limpiar() {
        T nada;
        while (desapilar(nada)) { /* se repite hasta vaciar la pila */ }
    }
};

// ---------------------------------------------------------------------------
// PILA con capacidad fija (arreglo). Se usa para mostrar la version "tope = -1"
// ---------------------------------------------------------------------------
template <typename T>
class PilaArreglo {
private:
    T* datos;
    int tope;        // -1 = vacia, 0..capacidad-1 = cantidad de elementos
    int capacidad;

public:
    explicit PilaArreglo(int cap) : tope(-1), capacidad(cap) {
        datos = new T[cap];
    }
    ~PilaArreglo() { delete[] datos; }   // los arreglos usan delete[]

    void apilar(T valor) {
        if (tope + 1 >= capacidad) return;   // pila llena: no se apila
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
};

// ---------------------------------------------------------------------------
// 2) VALIDAR PARENTESIS
// ---------------------------------------------------------------------------
bool parentesisBalanceados(const std::string& cadena) {
    Pila<char> pila;

    for (char c : cadena) {
        if (c == '(' || c == '[' || c == '{') {
            pila.apilar(c);                       // abre -> se apila
        } else if (c == ')' || c == ']' || c == '}') {
            char abierto;
            if (!pila.desapilar(abierto)) return false;   // cierre sin apertura
            bool correcto = (c == ')' && abierto == '(') ||
                            (c == ']' && abierto == '[') ||
                            (c == '}' && abierto == '{');
            if (!correcto) return false;                   // tipo equivocado
        }
    }

    return pila.estaVacia();   // si sobran aperturas, NO esta balanceada
}

// ---------------------------------------------------------------------------
// 3) INFIZA -> POSTFIJA
// ---------------------------------------------------------------------------
int precedencia(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;   // no es operador
}

bool esOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

std::string aPostfija(const std::string& infija) {
    std::string salida;
    std::stack<char> operadores;

    // Cada token se anade separado por un espacio. Sin los espacios, al evaluar
    // despues, "345*+" se leeria como el numero 345 y no como 3, 4, 5, *, +.
    auto agregar = [&salida](char c) { salida += c; salida += ' '; };

    for (char c : infija) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            agregar(c);                               // operando -> directo a la salida
        } else if (c == ' ') {
            continue;                                  // se ignoran los espacios
        } else if (c == '(') {
            operadores.push(c);                        // parentesis de apertura
        } else if (c == ')') {
            while (!operadores.empty() && operadores.top() != '(') {
                agregar(operadores.top());             // se vacia hasta el '('
                operadores.pop();
            }
            operadores.pop();                          // se descarta el '('
        } else if (esOperador(c)) {
            while (!operadores.empty() &&
                   operadores.top() != '(' &&
                   precedencia(operadores.top()) >= precedencia(c)) {
                agregar(operadores.top());             // el de mayor precedencia sale antes
                operadores.pop();
            }
            operadores.push(c);
        }
    }

    while (!operadores.empty()) {                      // se vacia lo que quede
        agregar(operadores.top());
        operadores.pop();
    }

    if (!salida.empty()) salida.pop_back();            // quitar el espacio final
    return salida;
}

// ---------------------------------------------------------------------------
// 4) EVALUAR POSTFIJA
// ---------------------------------------------------------------------------
double aplicar(char op, double a, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b == 0.0) ? 0.0 : a / b;
        case '^': return 1.0;
        default:  return 0.0;
    }
}

double evaluarPostfija(const std::string& postfija) {
    std::stack<double> valores;

    std::istringstream flujo(postfija);
    std::string token;

    while (flujo >> token) {
        if (token.size() == 1 && esOperador(token[0])) {
            double b = valores.top(); valores.pop();   // primero sale el operando de la derecha
            double a = valores.top(); valores.pop();   // luego el de la izquierda
            valores.push(aplicar(token[0], a, b));
        } else {
            valores.push(std::stod(token));
        }
    }

    return valores.top();
}

// ---------------------------------------------------------------------------
// 5) EVALUAR INFIZA con dos pilas
// ---------------------------------------------------------------------------
double evaluarInfija(const std::string& infija) {
    std::stack<double> valores;
    std::stack<char>   operadores;

    for (char c : infija) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            valores.push(c - '0');                  // solo un digito: suficiente aqui
        } else if (c == '(') {
            operadores.push(c);
        } else if (c == ')') {
            while (!operadores.empty() && operadores.top() != '(') {
                char op = operadores.top(); operadores.pop();
                double b = valores.top(); valores.pop();
                double a = valores.top(); valores.pop();
                valores.push(aplicar(op, a, b));
            }
            operadores.pop();                        // se descarta el '('
        } else if (esOperador(c)) {
            while (!operadores.empty() &&
                   operadores.top() != '(' &&
                   precedencia(operadores.top()) >= precedencia(c)) {
                char op = operadores.top(); operadores.pop();
                double b = valores.top(); valores.pop();
                double a = valores.top(); valores.pop();
                valores.push(aplicar(op, a, b));
            }
            operadores.push(c);
        }
    }

    while (!operadores.empty()) {
        char op = operadores.top(); operadores.pop();
        double b = valores.top(); valores.pop();
        double a = valores.top(); valores.pop();
        valores.push(aplicar(op, a, b));
    }

    return valores.top();
}

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 11: pila y expresiones ===\n\n";

    // --- 1) La pila basica ----------------------------------------------------
    std::cout << "-- 1) Pila basica --\n";
    Pila<int> pila;
    std::cout << "  estaVacia al inicio = " << std::boolalpha << pila.estaVacia() << "\n";
    pila.apilar(10);
    pila.apilar(20);
    pila.apilar(30);

    int valor;
    pila.cima(valor);
    std::cout << "  tras apilar 10, 20, 30 -> cima = " << valor
              << "  tamano = " << pila.tamano() << "\n";
    pila.desapilar(valor);
    std::cout << "  tras un desapilar    -> valor = " << valor
              << "  tamano = " << pila.tamano() << "\n";

    pila.limpiar();
    std::cout << "  tras limpiar         -> estaVacia = " << pila.estaVacia() << "\n";
    std::cout << "  desapilar en pila vacia -> " << pila.desapilar(valor) << "\n\n";

    // --- Pila con capacidad fija ---------------------------------------------
    std::cout << "  Pila de arreglo con capacidad 3:\n";
    PilaArreglo<int> fija(3);
    std::cout << "    vacia = " << fija.estaVacia() << "  llena = " << fija.estaLlena() << "\n";
    for (int i = 1; i <= 5; i++) {              // se intenta apilar 5 veces
        if (!fija.estaLlena()) {
            fija.apilar(i * 10);
        } else {
            std::cout << "    no se apila " << i * 10 << ": la pila esta llena\n";
        }
    }
    int cimaF;
    fija.cima(cimaF);
    std::cout <<    "    cima = " << cimaF << "  tamano = " << fija.tamano() << "\n\n";

    // --- 2) Parentesis balanceados --------------------------------------------
    std::cout << "-- 2) parentesisBalanceados --\n";
    struct Prueba { const char* texto; bool esperado; };
    const Prueba pruebas[] = {
        {"( )",                          true},
        {"()",                           true},
        {"{[()]}",                       true},
        {"(()(()))",                     true},
        {"",                             true},
        {"(",                            false},
        {")",                            false},
        {")(",                           false},
        {"([)]",                         false},   // tipo equivocado
        {"{[(]}",                        false},   // tipo equivocado
        {"(((",                          false},   // sobran aperturas
        {"a + (b * (c - d))",            true},
        {"a + (b * (c - d",              false},
    };

    for (const Prueba& p : pruebas) {
        bool resultado = parentesisBalanceados(p.texto);
        std::cout << "  \"" << p.texto << "\" -> " << resultado
                  << "   (esperado " << std::boolalpha << p.esperado << ")"
                  << (resultado == p.esperado ? "  OK" : "  FALLO") << "\n";
    }
    std::cout << "\n";

    // --- 3) Infija -> postfija -------------------------------------------------
    std::cout << "-- 3) infija -> postfija --\n";
    struct Conv { const char* infija; const char* postfija; };
    const Conv conversiones[] = {
        {"3 + 4 * 5",       "3 4 5 * +"},
        {"3 * 4 + 5",       "3 4 * 5 +"},
        {"( 3 + 4 ) * 5",   "3 4 + 5 *"},
        {"1 + 2 * 3 - 4",   "1 2 3 * + 4 -"},
        {"8 - 5 + 2",       "8 5 - 2 +"},
        {"2 * ( 3 + 4 ) * 5","2 3 4 + * 5 *"},
    };

    for (const Conv& c : conversiones) {
        std::string resultado = aPostfija(c.infija);
        std::cout << "  " << c.infija << "  ->  \"" << resultado << "\""
                  << "   (esperado \"" << c.postfija << "\")"
                  << (resultado == c.postfija ? "  OK" : "  FALLO") << "\n";
    }
    std::cout << "\n";

    // --- 4) Evaluar postfija ---------------------------------------------------
    std::cout << "-- 4) evaluarPostfija --\n";
    const double esperadas[] = {23.0, 17.0, 35.0, 3.0, 5.0, 70.0};
    for (int i = 0; i < 6; i++) {
        double r = evaluarPostfija(conversiones[i].postfija);
        std::cout << "  \"" << conversiones[i].postfija << "\" = " << r
                  << "   (esperado " << esperadas[i] << ")"
                  << (r == esperadas[i] ? "  OK" : "  FALLO") << "\n";
    }
    std::cout << "\n";

    // --- 5) Evaluar infija con dos pilas ---------------------------------------
    std::cout << "-- 5) evaluarInfija (dos pilas) --\n";
    for (int i = 0; i < 6; i++) {
        double r = evaluarInfija(conversiones[i].infija);
        std::cout << "  " << conversiones[i].infija << " = " << r
                  << "   (esperado " << esperadas[i] << ")"
                  << (r == esperadas[i] ? "  OK" : "  FALLO") << "\n";
    }

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 11: pila y expresiones ===
    -- 1) Pila basica --
      estaVacia al inicio = true
      tras apilar 10, 20, 30 -> cima = 30  tamano = 3
      tras un desapilar    -> valor = 30  tamano = 2
      tras limpiar         -> estaVacia = true
      desapilar en pila vacia -> false
      Pila de arreglo con capacidad 3:
        vacia = true  llena = false
        no se apila 40: la pila esta llena
        no se apila 50: la pila esta llena
        cima = 30  tamano = 3
    -- 2) parentesisBalanceados --
      "( )" -> true   (esperado true)  OK
      "()" -> true   (esperado true)  OK
      "{[()]}" -> true   (esperado true)  OK
      "(()(()))" -> true   (esperado true)  OK
      "" -> true   (esperado true)  OK
      "(" -> false   (esperado false)  OK
      ")" -> false   (esperado false)  OK
      ")(" -> false   (esperado false)  OK
      "([)]" -> false   (esperado false)  OK
      "{[(]}" -> false   (esperado false)  OK
      "(((" -> false   (esperado false)  OK
      "a + (b * (c - d))" -> true   (esperado true)  OK
      "a + (b * (c - d" -> false   (esperado false)  OK
    -- 3) infija -> postfija --
      3 + 4 * 5  ->  "3 4 5 * +"   (esperado "3 4 5 * +")  OK
      3 * 4 + 5  ->  "3 4 * 5 +"   (esperado "3 4 * 5 +")  OK
      ( 3 + 4 ) * 5  ->  "3 4 + 5 *"   (esperado "3 4 + 5 *")  OK
      1 + 2 * 3 - 4  ->  "1 2 3 * + 4 -"   (esperado "1 2 3 * + 4 -")  OK
      8 - 5 + 2  ->  "8 5 - 2 +"   (esperado "8 5 - 2 +")  OK
      2 * ( 3 + 4 ) * 5  ->  "2 3 4 + * 5 *"   (esperado "2 3 4 + * 5 *")  OK
    -- 4) evaluarPostfija --
      "3 4 5 * +" = 23   (esperado 23)  OK
      "3 4 * 5 +" = 17   (esperado 17)  OK
      "3 4 + 5 *" = 35   (esperado 35)  OK
      "1 2 3 * + 4 -" = 3   (esperado 3)  OK
      "8 5 - 2 +" = 5   (esperado 5)  OK
      "2 3 4 + * 5 *" = 70   (esperado 70)  OK
    -- 5) evaluarInfija (dos pilas) --
      3 + 4 * 5 = 23   (esperado 23)  OK
      3 * 4 + 5 = 17   (esperado 17)  OK
      ( 3 + 4 ) * 5 = 35   (esperado 35)  OK
      1 + 2 * 3 - 4 = 3   (esperado 3)  OK
      8 - 5 + 2 = 5   (esperado 5)  OK
      2 * ( 3 + 4 ) * 5 = 70   (esperado 70)  OK
    Demostracion completada.
*/
