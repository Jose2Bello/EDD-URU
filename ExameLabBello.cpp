/*Examen de laboratorio de EDD
Nombre: Jose Bello
1. Crear un programa que permita revertir una lista doblemente enlazada en un solo recorrido.
/*
Primer ejercicio
*/


#include <iostream>

template <typename T>

class Nodo {
    private:
        T dato;
        Nodo<T>* siguiente;
        Nodo<T>* anterior;
    public:
        Nodo(T dato) : dato(dato), siguiente(nullptr), anterior(nullptr) {}
    
    T getDato() {
            return dato;
        }   
        void setDato(T dato) {
            this->dato = dato;
        }

        Nodo<T>* getSiguiente() {
            return siguiente;
        }
        Nodo<T>* getAnterior() {    
        return anterior;
        }   

        Nodo<T>* setSiguiente(Nodo<T>* siguiente) {
            this->siguiente = siguiente;
            return this->siguiente;
        }
        Nodo<T>* setAnterior(Nodo<T>* anterior) {
            this->anterior = anterior;
            return this->anterior;
        }
        

    };

template <typename T>
class ListaDoble {
    private:
        Nodo<T>* cabeza;
        Nodo<T>* cola;  
    public:
        ListaDoble() : cabeza(nullptr), cola(nullptr) {}

    void agregarNodo(T dato) {
            Nodo<T>* nuevoNodo = new Nodo<T>(dato);
            if (cabeza == nullptr) {
                cabeza = nuevoNodo;
                cola = nuevoNodo;
            } else {
                cola->setSiguiente(nuevoNodo);
                nuevoNodo->setAnterior(cola);
                cola = nuevoNodo;
            }
        }

    Nodo<T>* getCabeza() {
            return cabeza;
        }

    Nodo<T>* getCola() {
            return cola;
        }

   void revertir() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* siguienteOriginal = actual->getSiguiente(); //creamos un puntero temporal para guardar el siguiente nodo
            actual->setSiguiente(actual->getAnterior());         //luego apuntamos el siguiente del nodo actual al anterior
            actual->setAnterior(siguienteOriginal);              //despues de eso cambiamos el anterior por el siguiente original
            actual = siguienteOriginal;                          //y finalmente avanzamos al siguiente nodo original
        }

        Nodo<T>* nodoTemporal = cabeza;
        cabeza = cola; //cambiamos la cabeza por la cola
        cola = nodoTemporal;//y la cola por la cabeza 
    }
    
    void mostrar() {
            Nodo<T>* actual = cabeza;
            while (actual != nullptr) {
                std::cout << actual->getDato() << " ";
                actual = actual->getSiguiente();
            }
            std::cout << std::endl;
        }
};


int main() {


    std::cout<< "Ejercicio 1: Revertir una lista doblemente enlazada en un solo recorrido" << std::endl;
    ListaDoble<int> lista;
    lista.agregarNodo(1);
    lista.agregarNodo(2);
    lista.agregarNodo(3);
    lista.agregarNodo(4);
    lista.agregarNodo(5);
    lista.agregarNodo(6);


    std::cout << "head: " << lista.getCabeza()->getDato() << std::endl;

    std::cout << "tail: " << lista.getCola()->getDato() << std::endl;


    std::cout << "Lista original: ";

    lista.mostrar();

    lista.revertir();

    std::cout << "head: " << lista.getCabeza()->getDato() << std::endl;

    std::cout << "tail: " << lista.getCola()->getDato() << std::endl;

    std::cout << "Lista revertida: ";
  
    lista.mostrar();
    
    return 0;
}
