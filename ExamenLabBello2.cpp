/*Eaxmen laboratorio EDD (parte 2)
nombre: Jose Bello
2. Crear un programa que vaya eliminando los nodos de una lista doble enlazada circular en base a sus posiciones, ej: borrar de dos en dos 
Se borra el primer nodo se saltan 2 posiciones se borra el nodo correspondiente
Se debe ingresar la cantidad de nodos que se quieren saltar antes de empezar la secuencia de eliminación. mostrar el dato del nodo que se vaya
a borrar cada vez */

#include <iostream>

template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* anterior;
    Nodo<T>* siguiente;

    Nodo(T valor) : dato(valor), anterior(nullptr), siguiente(nullptr) {}

    void setSiguiente(Nodo<T>* siguiente) {
        this->siguiente = siguiente;
    }

    void setAnterior(Nodo<T>* anterior) {
        this->anterior = anterior;
    }
    T getDato() {
        return dato;
    }
    void setDato(T valor) {
        dato = valor;
    }

};

template <typename T>
class ListaCircularDoble {
private:
    Nodo<T>* head;
    Nodo<T>* tail;
public:
    ListaCircularDoble() : head(nullptr), tail(nullptr) {}

void agregarAlFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (head == nullptr) {
            head = tail = nuevo;
            nuevo->anterior = nuevo;
            nuevo->siguiente = nuevo;
            return;
        }
        nuevo->anterior = tail;
        nuevo->siguiente = head;
        tail->siguiente = nuevo;
        head->anterior = nuevo;
        tail = nuevo;
    }

void mostrarlista() {
        if (head == nullptr) {
            std::cout << "Lista vacia" << std::endl;
            return;
        }
        Nodo<T>* actual = head;
        do {
            std::cout << actual->dato << " ";
            actual = actual->siguiente;
        } while (actual != head);
        std::cout << std::endl;
    }

void eliminarNodos(int saltos) {
        if (head == nullptr) return;

        Nodo<T>* actual = head;
        while (actual->siguiente != actual) { // Mientras haya más de un nodo
            for (int i = 0; i < saltos; ++i) {
                actual = actual->siguiente; //saltar nodos
            }
            std::cout << "Eliminando nodo con dato: " << actual->dato << std::endl;
            Nodo<T>* nodoAEliminar = actual;
            actual->anterior->siguiente = actual->siguiente;
            actual->siguiente->anterior = actual->anterior;
            if (nodoAEliminar == head) {
                head = nodoAEliminar->siguiente; 
            }
            if (nodoAEliminar == tail) {
                tail = nodoAEliminar->anterior; // Actualizar cola si es necesario
            }
            actual = actual->siguiente; // Mover al siguiente nodo
            delete nodoAEliminar; // Eliminar el nodo
        }
        std::cout << "Eliminando ultimo nodo con dato: " << actual->dato << std::endl;
        delete actual; 
        head = tail = nullptr; // Lista vacia
    }
};


int main() {
    ListaCircularDoble<int> lista;
    int cantidadNodos, saltos;

    std::cout << "Ingrese la cantidad de nodos a agregar: ";
    std::cin >> cantidadNodos;


    for (int i = 1; i <= cantidadNodos; ++i) {
        lista.agregarAlFinal(i);
    }

    std::cout << "Lista inicial: ";
    lista.mostrarlista();

    std::cout << "Ingrese la cantidad de nodos a saltar antes de eliminar: ";
    std::cin >> saltos;

    lista.eliminarNodos(saltos);

    std::cout << "Lista final despues de eliminar nodos: ";
    lista.mostrarlista();

    return 0;
}

