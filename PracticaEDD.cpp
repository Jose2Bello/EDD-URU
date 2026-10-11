#include <iostream>
using namespace::std;

template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;


    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};


template <typename T>
class ListaSimple {
private:
    Nodo<T>* head;

public:
    
    ListaSimple() : head(nullptr) {}

    
    ~ListaSimple() {
        Nodo<T>* actual = head;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }

   
    void agregarAlFinal(T valor) {
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);
        if (head == nullptr) {
            head = nuevoNodo;
            return;
        }

        Nodo<T>* actual = head;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }

  
    bool eliminar(T valor) {
        if (head == nullptr) return false;

      
        if (head->dato == valor) {
            Nodo<T>* aEliminar = head;
            head = head->siguiente;
            delete aEliminar;
            return true;
        }

        Nodo<T>* actual = head;
        while (actual->siguiente != nullptr && actual->siguiente->dato != valor) {
            actual = actual->siguiente;
        }

       
        if (actual->siguiente == nullptr) return false;

   
        Nodo<T>* aEliminar = actual->siguiente;
        actual->siguiente = actual->siguiente->siguiente;
        delete aEliminar;
        return true;
    }

  
    void mostrar() const {
        Nodo<T>* actual = head;
        while (actual != nullptr) {
            cout << "(" << actual->dato << ") -> ";
            actual = actual->siguiente;
        }
        cout << "nullptr";
    }
};

int main() {

    ListaSimple<int> miLista;

    cout << "Insertando 4 nodos \n";
    miLista.agregarAlFinal(1);
    miLista.agregarAlFinal(2);
    miLista.agregarAlFinal(3);
    miLista.agregarAlFinal(4);
    

    miLista.mostrar();

    cout << "\nEliminando el nodo con valor 2 \n";
    if (miLista.eliminar(2)) {
    } else {
        std::cout << "Valor no encontrado.\n";
    }

    cout<<"lista con valor 2 eliminado\n"; 
	
	miLista.mostrar();
	
    
    
    
    cout<<"\nagregando nodo al final\n";
    
      miLista.agregarAlFinal(8);
      
      miLista.mostrar();
      
      
    for (head!=nullptr){
    	miLista.eliminar(i)

	}
      
   
    return 0;
}