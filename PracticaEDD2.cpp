/*
 
   Realizar un programa que utilice una lista circular doble para imitar una
   lista de reproduccion. Los nodos deben tener un puntero al nodo anterior.

   Cada nodo guarda: nombre de la cancion, nombre del artista,
   duracion de la cancion y genero de la cancion */

#include <iostream>

const int TAM = 30;   

void descartarResto() { std::cin.ignore(1000, '\n'); }
 
 
struct Nodo {
    char nombre[TAM];
    char artista[TAM];
    char genero[TAM];
    int duracion;      // en segundos
    Nodo* anterior;    // puntero al nodo anterior
    Nodo* siguiente;   // puntero al nodo siguiente

    Nodo() : duracion(0), anterior(nullptr), siguiente(nullptr) {
        nombre[0] = '\0';
        artista[0] = '\0';
        genero[0] = '\0';
    }
};

class ListaReproduccion {
    Nodo* primero;
    Nodo* ultimo;
    Nodo* actual;

    void mostrar(const Nodo* cancion) const {
        std::cout << "   Cancion:  " << cancion->nombre << '\n'
                  << "   Artista:  " << cancion->artista << '\n'
                  << "   Duracion: " << cancion->duracion / 60 << ':';
        std::cout << cancion->duracion % 60 << '\n'
                  << "   Genero:   " << cancion->genero << '\n';
    }

public:
    ListaReproduccion() : primero(nullptr), ultimo(nullptr), actual(nullptr) {}

    ~ListaReproduccion() {
        if (primero == nullptr) return;
        Nodo* cancion = primero;
        do {
            Nodo* siguiente = cancion->siguiente;  
            delete cancion;
            cancion = siguiente;
        } while (cancion != primero);
    }

    bool estaVacia() const { return primero == nullptr; }

    void agregar(Nodo* nuevo) {
        if (estaVacia()) {                
            primero = ultimo = actual = nuevo;
            nuevo->anterior = nuevo;
            nuevo->siguiente = nuevo;
            return;
        }
        
        nuevo->anterior = ultimo;          
        nuevo->siguiente = primero;       
        ultimo->siguiente = nuevo;
        primero->anterior = nuevo;
        ultimo = nuevo;
    }

    void verLista() const {
        if (estaVacia()) {
            std::cout << "La lista esta vacia.\n";
            return;
        }
        std::cout << "\n Lista de reproduccion \n";
        Nodo* cancion = primero;
        int numero = 1;
        do {
            std::cout << (cancion == actual ? " > " : "   ") << numero++ << "\n";
            mostrar(cancion);
            cancion = cancion->siguiente;
        } while (cancion != primero);
    }
    

    void verActual() const {
        if (estaVacia()) {
            std::cout << "No hay canciones.\n";
            return;
        }
        std::cout << "\nReproduciendo\n";
        mostrar(actual);
    }

    void siguiente() {
        if (estaVacia()) {
            std::cout << "No hay canciones.\n";
            return;
        }
        actual = actual->siguiente;        
        verActual();
    }

    void anterior() {
        if (estaVacia()) {
            std::cout << "No hay canciones.\n";
            return;
        }
        actual = actual->anterior;
        verActual();
    }

    void eliminarActual() {
        if (estaVacia()) {
            std::cout << " No hay canciones que eliminar.\n";
            return;
        }

        Nodo* borrado = actual;

        if (primero == ultimo) {          
            primero = ultimo = actual = nullptr;
        } else {
            borrado->anterior->siguiente = borrado->siguiente;   
            borrado->siguiente->anterior = borrado->anterior;   
            if (borrado == primero) primero = borrado->siguiente;
            if (borrado == ultimo)  ultimo  = borrado->anterior;
            actual = borrado->siguiente;  
        }
        std::cout << "Se elimino: " << borrado->nombre << '\n'; 
        delete borrado;
    }
};


int main() {
    ListaReproduccion lista;
    char opcion;

    while (true) {
        std::cout << "\n1) Agregar  2) Ver lista  3) Siguiente  4) Anterior"
                  << "  5) Actual  6) Eliminar  0) Salir\nOpcion: ";

        if (!(std::cin >> opcion)) break;  
        descartarResto();                    
        if (opcion == '0') break;           

        if (opcion == '1') {
            Nodo* nuevo = new Nodo();   

            std::cout << "  Cancion:         "; 
			std::cin.getline(nuevo->nombre,  TAM, '\n');
            std::cout << "  Artista:         "; 
			std::cin.getline(nuevo->artista, TAM, '\n');
            std::cout << "  Duracion (seg):  "; 
			std::cin >> nuevo->duracion; descartarResto();
            std::cout << "  Genero:          "; 
			std::cin.getline(nuevo->genero,  TAM, '\n');

            if (nuevo->duracion <= 0) {
                std::cout << "  La duracion debe ser mayor que cero.\n";
                delete nuevo;
                continue;
            }

            lista.agregar(nuevo);
            std::cout << "  Cancion agregada.\n";
        }
        else if (opcion == '2') lista.verLista();
        else if(opcion == '3') lista.siguiente();
        else  if (opcion == '4') lista.anterior();
        else if (opcion == '5') lista.verActual();
        else if (opcion == '6') lista.eliminarActual();
        else std::cout << "  Opcion no valida.\n";
    }

    std::cout << "Reproductor cerrado.\n";
    return 0;
}