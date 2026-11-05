
/*
1. Realizar la plantilla de la clase de pilas y colas
2. Utilizar la plantilla en un programa donde se ingrese valores dentro de la pila y que se pasen posteriormente
a una cola
*/
#include <iostream>
using namespace std;

template <class T>
class Pila {
    private:
        T* arr;
        int top;
        int size;

    public:
        Pila(int size) {
            this->size = size;
            top = -1;
            arr = new T[size];
        }

        ~Pila() {
            delete[] arr;
        }

        void push(T value) {
            if (top >= size - 1) {
                cout << "Pila llena" << endl;
                return;
            }
            arr[++top] = value;
        }

        T pop() {
            if (top < 0) {
                cout << "Pila vacía" << endl;
                return T(); // Retorna un valor por defecto
            }
            return arr[top--];
        }

        bool isEmpty() {
            return top < 0;
        }

};


template <class T>
class Cola {
    private:
        T* arr;
        int front;
        int rear;
        int size;
    public:
        Cola(int size) {
            this->size = size;
            front = -1;
            rear = -1;
            arr = new T[size];
        }

        ~Cola() {
            delete[] arr;
        }

        void enqueue(T value) {
            if (rear >= size - 1) {
                cout << "Cola llena" << endl;
                return;
            }
            if (front == -1) {
                front = 0;
            }
            arr[++rear] = value;
        }

        T dequeue() {
            if (front == -1 || front > rear) {
                cout << "Cola vacía" << endl;
                return T(); 
            }
            T value = arr[front++];
            if (front > rear) {
                front = -1;
                rear = -1;
            }
            return value;
        }

        bool isEmpty() {
            return front == -1;
        }
};

int main(){
    cout<< "Transferencia de valores de una pila a una cola" << endl;
    Pila<int> pila(5);
    Cola<int> cola(5);

    int value;
    cout<< "Ingrese valores a la pila (3 valores):" << endl;
    cin >> value;
    pila.push(value);
    cout << "Ingrese otro valor a la pila:" << endl;
    cin >> value;
    pila.push(value);
    cout << "Ingrese otro valor a la pila:" << endl;
    cin >> value;
    pila.push(value);
    cout << "Ingrese otro valor a la pila:" << endl;
    cin >> value;
    pila.push(value);

    while (!pila.isEmpty()) {
        value = pila.pop();
        cola.enqueue(value);
    }

    cout << "Valores transferidos a la cola:" << endl;
    while (!cola.isEmpty()) {
        value = cola.dequeue();
        cout << value << " ";
    }

    return 0;
}