/* 1. crear un programa que ordene una pila de nodos desordenados de mayor (fondo) a menor (cima)
usando solo pilas y que en ningun momento del algoritmo exista un nodo de mayor valor encima a un nodo de menor valor */

#include <iostream>
using namespace std;

template <typename T>
class Pila {
private:
    struct Nodo {
        T valor;
        Nodo* siguiente;

        Nodo(const T& valor, Nodo* siguiente) : valor(valor), siguiente(siguiente) {}
    };

    Nodo* cima;

    void liberar() {
        while (cima != nullptr) {
            Nodo* borrar = cima;
            cima = cima->siguiente;
            delete borrar;
        }
    }

public:
    Pila() : cima(nullptr) {}

    ~Pila() {
        liberar();
    }

    Pila(const Pila&) = delete;
    Pila& operator=(const Pila&) = delete;

    void apilar(const T& valor) {
        cima = new Nodo(valor, cima);
    }

    bool desapilar(T& valor) {
        if (cima == nullptr)
            return false;

        Nodo* borrar = cima;
        valor = borrar->valor;
        cima = cima->siguiente;
        delete borrar;
        return true;
    }

    bool verCima(T& valor) const {
        if (cima == nullptr)
            return false;

        valor = cima->valor;
        return true;
    }

    void intercambiar(Pila& otra) {
        Nodo* nodos = cima;
        cima = otra.cima;
        otra.cima = nodos;
    }

    void mostrar() const {
        for (Nodo* actual = cima; actual != nullptr; actual = actual->siguiente)
            cout << actual->valor << " ";
        cout << "\n";
    }
};

template <typename T>
void ordenar(Pila<T>& pila) {
    Pila<T> auxiliar;
    T valor;
    T valorAuxiliar;

    while (pila.desapilar(valor)) {
        while (auxiliar.verCima(valorAuxiliar) && valorAuxiliar < valor) {
            auxiliar.desapilar(valorAuxiliar);
            pila.apilar(valorAuxiliar);
        }
        auxiliar.apilar(valor);
    }

    pila.intercambiar(auxiliar);
}

int main() {
    Pila<int> pila;
    pila.apilar(5);
    pila.apilar(3);
    pila.apilar(7);
    pila.apilar(2);
    pila.apilar(8);
    pila.apilar(3);

    cout << "Pila inicial (cima a fondo): ";
    pila.mostrar();

    ordenar(pila);

    cout << "Pila ordenada (cima a fondo): ";
    pila.mostrar();
    cout << "La cima queda con el menor valor y el fondo con el mayor.\n";

    return 0;
}
