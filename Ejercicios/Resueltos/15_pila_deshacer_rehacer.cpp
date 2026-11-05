/* ============================================================================
   EJERCICIO 15 - Pila: editor con Deshacer / Rehacer
   Tema   : Pila (LIFO) usada como historial
   Archivo: 15_pila_deshacer_rehacer.cpp
   ============================================================================

   ENUNCIADO
   --------
   Simular un editor de texto con dos pilas:

     1) escribir(texto)  : agrega texto al final y guarda el estado anterior
                           en la pila de DESHACER. Ademas vacia la de REHACER.

     2) deshacer()       : vuelve al estado anterior. Devuelve false si no hay
                           nada que deshacer.

     3) rehacer()        : recupera el estado que se habia deshecho. Devuelve
                           false si no hay nada que rehacer.

     4) ver()            : devuelve el texto actual.

   Regla: al escribir algo nuevo despues de deshacer, el historial de rehacer
   se descarta (ya no tiene sentido volver a una rama abandonada). Es el mismo
   comportamiento de Ctrl+Z / Ctrl+Y en cualquier editor.

   Casos limite a comprobar:
     - deshacer sobre un editor sin cambios -> false
     - rehacer sin haber deshecho -> false
     - deshacer todo hasta dejarlo vacio
   ========================================================================== */

#include <iostream>
#include <string>
#include <cassert>

// ---------------------------------------------------------------------------
// Pila de textos (solo lo necesario para el ejercicio)
// ---------------------------------------------------------------------------
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T>
class Pila {
private:
    Nodo<T>* top;

public:
    Pila() : top(nullptr) {}

    ~Pila() {
        while (top != nullptr) {
            Nodo<T>* base = top->siguiente;
            delete top;
            top = base;
        }
    }

    void apilar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = top;
        top = nuevo;
    }

    bool desapilar(T& salida) {
        if (top == nullptr) return false;
        Nodo<T>* aBorrar = top;
        salida = aBorrar->dato;
        top = aBorrar->siguiente;
        delete aBorrar;
        return true;
    }

    bool estaVacia() const { return top == nullptr; }
    int  tamano() const {
        int n = 0;
        for (Nodo<T>* a = top; a != nullptr; a = a->siguiente) n++;
        return n;
    }
};

// ---------------------------------------------------------------------------
// Editor con historial
// ---------------------------------------------------------------------------
class Editor {
private:
    std::string texto;
    Pila<std::string> deshacer;
    Pila<std::string> rehacer;

public:
    const std::string& ver() const { return texto; }

    void escribir(const std::string& nuevoTexto) {
        deshacer.apilar(texto);        // se guarda el estado previo
        texto += nuevoTexto;

        // Si se estaba en medio de un "deshacer", escribir crea otra rama:
        // el futuro que se habia deshecho ya no es valido.
        std::string basura;
        while (rehacer.desapilar(basura)) { /* se descarta */ }
    }

    bool deshacerUltimo() {
        std::string anterior;
        if (!deshacer.desapilar(anterior)) return false;
        rehacer.apilar(texto);         // lo que dejo de estar visible
        texto = anterior;
        return true;
    }

    bool rehacerUltimo() {
        std::string siguiente;
        if (!rehacer.desapilar(siguiente)) return false;
        deshacer.apilar(texto);
        texto = siguiente;
        return true;
    }

    int estadosDeshacer() const { return deshacer.tamano(); }
    int estadosRehacer() const { return rehacer.tamano(); }

    void mostrar(const char* etiqueta = "") const {
        std::cout << etiqueta << "\"" << texto << "\""
                  << "   [deshacer = " << deshacer.tamano()
                  << ", rehacer = " << rehacer.tamano() << "]\n";
    }
};

// ---------------------------------------------------------------------------
int main() {
    std::cout << "=== EJERCICIO 15: pila como historial (deshacer / rehacer) ===\n\n";

    Editor editor;
    editor.mostrar("inicio:      ");

    editor.escribir("Hola");
    editor.mostrar("escribir:    ");

    editor.escribir(" mundo");
    editor.mostrar("escribir:    ");

    assert(editor.ver() == "Hola mundo");
    assert(editor.estadosDeshacer() == 2);

    std::cout << "\n-- deshacer dos veces --\n";
    assert(editor.deshacerUltimo() == true);
    editor.mostrar("deshacer:    ");
    assert(editor.ver() == "Hola");

    assert(editor.deshacerUltimo() == true);
    editor.mostrar("deshacer:    ");
    assert(editor.ver() == "");

    // Ya no hay nada mas que deshacer
    assert(editor.deshacerUltimo() == false);
    std::cout << "deshacer en vacio -> false\n";

    std::cout << "\n-- rehacer dos veces --\n";
    assert(editor.rehacerUltimo() == true);
    editor.mostrar("rehacer:     ");
    assert(editor.ver() == "Hola");

    assert(editor.rehacerUltimo() == true);
    editor.mostrar("rehacer:     ");
    assert(editor.ver() == "Hola mundo");

    assert(editor.rehacerUltimo() == false);
    std::cout << "rehacer sin deshacer -> false\n";

    std::cout << "\n-- escribir tras deshacer descarta el rehacer --\n";
    editor.deshacerUltimo();                 // vuelve a "Hola"
    editor.mostrar("deshacer:    ");
    assert(editor.estadosRehacer() == 1);

    editor.escribir("!");
    editor.mostrar("escribir:    ");
    assert(editor.ver() == "Hola!");
    assert(editor.estadosRehacer() == 0);    // rama abandonada: se descarto
    std::cout << "rehacer quedo en 0 (la rama anterior se descarto)\n";

    std::cout << "\nTodos los assert pasaron.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 15: pila como historial (deshacer / rehacer) ===

    inicio:      ""   [deshacer = 0, rehacer = 0]
    escribir:    "Hola"   [deshacer = 1, rehacer = 0]
    escribir:    "Hola mundo"   [deshacer = 2, rehacer = 0]

    -- deshacer dos veces --
    deshacer:    "Hola"   [deshacer = 1, rehacer = 1]
    deshacer:    ""   [deshacer = 0, rehacer = 2]
    deshacer en vacio -> false

    -- rehacer dos veces --
    rehacer:     "Hola"   [deshacer = 1, rehacer = 1]
    rehacer:     "Hola mundo"   [deshacer = 2, rehacer = 0]
    rehacer sin deshacer -> false

    -- escribir tras deshacer descarta el rehacer --
    deshacer:    "Hola"   [deshacer = 1, rehacer = 1]
    escribir:    "Hola!"   [deshacer = 2, rehacer = 0]
    rehacer quedo en 0 (la rama anterior se descarto)

    Todos los assert pasaron.
*/
