/* Procesar sumas y restas usando pilas. */

#include <iostream>
using namespace std;

class PilaCaracteres {
private:
    struct Nodo {
        char caracter;
        Nodo* siguiente;

        Nodo(char caracter, Nodo* siguiente)
            : caracter(caracter), siguiente(siguiente) {}
    };

    Nodo* cima;

public:
    PilaCaracteres() : cima(nullptr) {}

    ~PilaCaracteres() {
        char caracter;
        while (desapilar(caracter)) {
        }
    }

    PilaCaracteres(const PilaCaracteres&) = delete;
    PilaCaracteres& operator=(const PilaCaracteres&) = delete;

    void apilar(char caracter) {
        cima = new Nodo(caracter, cima);
    }

    bool desapilar(char& caracter) {
        if (cima == nullptr)
            return false;

        Nodo* borrar = cima;
        caracter = borrar->caracter;
        cima = cima->siguiente;
        delete borrar;
        return true;
    }
};

int main() {
    PilaCaracteres invertida;
    PilaCaracteres expresion;
    char caracter;

    cout << "Ingrese una expresion (maximo 5 operaciones): ";
    while (cin.get(caracter) && caracter != '\n') {
        if (caracter != ' ' && caracter != '\t' && caracter != '\r')
            invertida.apilar(caracter);
    }

    while (invertida.desapilar(caracter))
        expresion.apilar(caracter);

    int resultado = 0;
    int numero = 0;
    int operaciones = 0;
    int digitos = 0;
    char operacion = '+';

    while (expresion.desapilar(caracter)) {
        if (caracter >= '0' && caracter <= '9') {
            numero = numero * 10 + (caracter - '0');
            ++digitos;
        } else if (caracter == '+' || caracter == '-') {
            if (digitos == 0) {
                cout << "Expresion invalida.\n";
                return 1;
            }

            if (operacion == '+')
                resultado += numero;
            else
                resultado -= numero;

            ++operaciones;
            if (operaciones > 5) {
                cout << "La expresion no puede tener mas de 5 operaciones.\n";
                return 1;
            }

            operacion = caracter;
            numero = 0;
            digitos = 0;
        } else {
            cout << "Expresion invalida.\n";
            return 1;
        }
    }

    if (digitos == 0) {
        cout << "Expresion invalida.\n";
        return 1;
    }

    if (operacion == '+')
        resultado += numero;
    else
        resultado -= numero;

    cout << "Resultado: " << resultado << "\n";
    return 0;
}
