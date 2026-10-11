/* ============================================================================
   EJERCICIO 14 - PROYECTO FINAL: agenda de estudiantes
   Tema   : Integracion de TODAS las estructuras del curso
   Archivo: 14_agenda_estudiantes.cpp
   ============================================================================

   ENUNCIADO
   --------
   Se pide un sistema de gestion de una lista de estudiantes que use, cada una
   en la parte donde rinde mejor, las estructuras vistas en el curso:

     - LISTA DOBLEMENTE ENLAZADA  : el orden de llegada a la lista de espera.
                                     Se necesita poder recorrer hacia atras y
                                     quitar del medio (quien deja la materia).
     - ABB                        : busqueda rapida por matricula (O(log n)).
     - COLA                       : el orden en que se atienden las consultas
                                     deertificate (FIFO).
     - PILA                       : historial de cambios para poder deshacer
                                     la ultima matriculacion (LIFO).
     - LISTA SIMPLEMENTE ENLAZADA : la lista de espera de otra seccion, en la
                                     que solo se encola y se desencola.

   Requisitos
   ----------
   1) Registrar un estudiante (matricula, nombre, carrera, promedio).
   2) Registrar una consulta en la cola y atenderla en orden FIFO.
   3) Matricular a un estudiante en una materia, con un maximo de cupos.
   4) Listar los matriculados ORDENADOS por promedio (de mayor a menor).
   5) Consultar una matricula: mostrar el estudiante y sus materias.
   6) Dar de baja un estudiante (se quita de todas las materias).
   7) Deshacer la ultima matriculacion (usando la pila).
   8) Reporte final: total de estudiantes, total de matriculas, estudiante con
      mejor promedio, y cuanto cupo libre queda en cada materia.

   ========================================================================== */

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <iomanip>

// ===========================================================================
// 1) LISTA DOBLEMENTE ENLAZADA: la lista de espera de la carrera
// ===========================================================================
class NodoEspera {
public:
    std::string nombre;
    std::string carrera;
    int turno;                 // numero de llegada (1, 2, 3...)
    NodoEspera* anterior;
    NodoEspera* siguiente;

    NodoEspera(const std::string& n, const std::string& c, int t)
        : nombre(n), carrera(c), turno(t), anterior(nullptr), siguiente(nullptr) {}
};

class ListaEspera {
private:
    NodoEspera* primero;
    NodoEspera* ultimo;
    int contadorTurnos;

public:
    ListaEspera() : primero(nullptr), ultimo(nullptr), contadorTurnos(0) {}

    ~ListaEspera() {
        while (primero != nullptr) {
            NodoEspera* sig = primero->siguiente;
            delete primero;
            primero = sig;
        }
        primero = ultimo = nullptr;
    }

    // Agregar al final: O(1) gracias al puntero `ultimo`.
    void agregar(const std::string& nombre, const std::string& carrera) {
        NodoEspera* nuevo = new NodoEspera(nombre, carrera, ++contadorTurnos);

        nuevo->anterior = ultimo;
        if (ultimo != nullptr) ultimo->siguiente = nuevo;
        else                 primero = nuevo;

        ultimo = nuevo;
    }

    // Quitar por nombre: se recorre y se puentEAN los dos vecinos.
    bool quitar(const std::string& nombre) {
        NodoEspera* actual = primero;

        while (actual != nullptr && actual->nombre != nombre) {
            actual = actual->siguiente;
        }
        if (actual == nullptr) return false;   // no estaba en la lista

        if (actual->anterior != nullptr) actual->anterior->siguiente = actual->siguiente;
        else                             primero = actual->siguiente;

        if (actual->siguiente != nullptr) actual->siguiente->anterior = actual->anterior;
        else                             ultimo = actual->anterior;

        delete actual;
        return true;
    }

    void mostrar() const {
        if (primero == nullptr) { std::cout << "    (lista de espera vacia)\n"; return; }
        for (NodoEspera* a = primero; a != nullptr; a = a->siguiente) {
            std::cout << "    " << a->turno << ". " << a->nombre << " (" << a->carrera << ")\n";
        }
    }

    void mostrarInvertida() const {
        if (ultimo == nullptr) { std::cout << "    (lista de espera vacia)\n"; return; }
        for (NodoEspera* a = ultimo; a != nullptr; a = a->anterior) {
            std::cout << "    " << a->turno << ". " << a->nombre << " (" << a->carrera << ")\n";
        }
    }

    int tamano() const {
        int n = 0;
        for (NodoEspera* a = primero; a != nullptr; a = a->siguiente) n++;
        return n;
    }
};

// ===========================================================================
// 2) ABB: busqueda rapida de estudiantes por matricula
// ===========================================================================
struct Estudiante {
    std::string matricula;   // es la clave del ABB
    std::string nombre;
    std::string carrera;
    double promedio;
    std::vector<std::string> materias;   // materias en las que esta matriculado

    Estudiante(const std::string& m, const std::string& n,
               const std::string& c, double p)
        : matricula(m), nombre(n), carrera(c), promedio(p) {}
};

class NodoABB {
public:
    Estudiante* dato;          // puntero a los datos del estudiante
    NodoABB* izquierda;
    NodoABB* derecha;

    NodoABB(Estudiante* e) : dato(e), izquierda(nullptr), derecha(nullptr) {}
};

class ArbolEstudiantes {
private:
    NodoABB* raiz;

public:
    ArbolEstudiantes() : raiz(nullptr) {}
    ~ArbolEstudiantes() { destruir(raiz); }

    void destruir(NodoABB* nodo) {
        if (nodo == nullptr) return;
        destruir(nodo->izquierda);
        destruir(nodo->derecha);
        delete nodo->dato;   // el ABB es dueno de los datos
        delete nodo;
    }

    void insertar(Estudiante* e) { raiz = insertarEn(raiz, e); }

    NodoABB* insertarEn(NodoABB* nodo, Estudiante* e) {
        if (nodo == nullptr) return new NodoABB(e);

        if (e->matricula < nodo->dato->matricula) {
            nodo->izquierda = insertarEn(nodo->izquierda, e);
        } else if (e->matricula > nodo->dato->matricula) {
            nodo->derecha = insertarEn(nodo->derecha, e);
        }
        return nodo;
    }

    // Busqueda iterativa: descarta la mitad del arbol en cada paso.
    Estudiante* buscar(const std::string& matricula) const {
        NodoABB* actual = raiz;
        while (actual != nullptr) {
            if      (matricula == actual->dato->matricula) return actual->dato;
            else if (matricula <  actual->dato->matricula) actual = actual->izquierda;
            else                                             actual = actual->derecha;
        }
        return nullptr;
    }

    // Recorrido en orden: como la clave es la matricula, sale ORDENADO.
    void inOrden(NodoABB* nodo, std::vector<Estudiante*>& salida) const {
        if (nodo == nullptr) return;
        inOrden(nodo->izquierda, salida);
        salida.push_back(nodo->dato);
        inOrden(nodo->derecha, salida);
    }

    std::vector<Estudiante*> listar() const {
        std::vector<Estudiante*> salida;
        inOrden(raiz, salida);
        return salida;
    }

    int contar() const { return static_cast<int>(listar().size()); }

    // ------------------------------------------------------------------
    // BORRADO en el ABB: los tres casos, igual que en un ABB de enteros
    //
    //   caso 1 (hoja)       : se borra el nodo
    //   caso 2 (un hijo)    : el hijo ocupa su lugar
    //   caso 3 (dos hijos)  : se copia el puntero del sucesor in-orden
    //                         y se borra el nodo del sucesor
    //
    //   OJO: al copiar `dato` (un puntero a Estudiante) NO se borra el
    //   Estudiante, porque la propiedad sigue siendo del arbol. Solo se
    //   libera el NodoABB.
    // ------------------------------------------------------------------
    bool borrar(const std::string& matricula) {
        bool existia = (buscar(matricula) != nullptr);
        raiz = borrarEn(raiz, matricula);
        return existia;
    }

    NodoABB* borrarEn(NodoABB* nodo, const std::string& matricula) {
        if (nodo == nullptr) return nullptr;   // no existia: no hay nada que hacer

        if (matricula < nodo->dato->matricula) {
            nodo->izquierda = borrarEn(nodo->izquierda, matricula);
        } else if (matricula > nodo->dato->matricula) {
            nodo->derecha = borrarEn(nodo->derecha, matricula);
        } else {
            NodoABB* reemplazo = nullptr;

            if (nodo->izquierda == nullptr) {
                reemplazo = nodo->derecha;                    // casos 1 y 2
            } else if (nodo->derecha == nullptr) {
                reemplazo = nodo->izquierda;                   // caso 2
            } else {
                // caso 3: buscar el sucesor in-orden (el menor del subarbol derecho)
                NodoABB* sucesor = nodo->derecha;
                while (sucesor->izquierda != nullptr) sucesor = sucesor->izquierda;

                nodo->dato = sucesor->dato;                    // se copia el puntero
                reemplazo = borrarMinimo(nodo->derecha);      // y se borra el nodo sucesor
            }

            delete nodo;      // nunca se borra nodo->dato: es del arbol
            return reemplazo;
        }
        return nodo;
    }

    NodoABB* borrarMinimo(NodoABB* nodo) {
        if (nodo->izquierda == nullptr) return nodo->derecha;
        nodo->izquierda = borrarMinimo(nodo->izquierda);
        return nodo;
    }
};

// ===========================================================================
// 3) Materias con cupos + LISTA SIMPLEMENTE ENLAZADA de matriculados
// ===========================================================================
struct NodoMateria {
    std::string matricula;
    NodoMateria* siguiente;
    NodoMateria(const std::string& m) : matricula(m), siguiente(nullptr) {}
};

class Materia {
public:
    std::string nombre;
    int capacidad;
    int inscritos;                 // cantidad de cupos ocupados
    NodoMateria* lista;            // lista simplemente enlazada de matriculas

    Materia(const std::string& n, int cap)
        : nombre(n), capacidad(cap), inscritos(0), lista(nullptr) {}

    ~Materia() {
        while (lista != nullptr) {
            NodoMateria* sig = lista->siguiente;
            delete lista;
            lista = sig;
        }
    }

    bool estaLlena() const { return inscritos >= capacidad; }
    int  lugaresLibres() const { return capacidad - inscritos; }

    void inscribir(const std::string& matricula) {
        NodoMateria* nuevo = new NodoMateria(matricula);
        nuevo->siguiente = lista;   // se inserta al inicio de la lista
        lista = nuevo;
        inscritos++;
    }

    bool desinscribir(const std::string& matricula) {
        NodoMateria* actual = lista;
        NodoMateria* anterior = nullptr;

        while (actual != nullptr) {
            if (actual->matricula == matricula) {
                if (anterior == nullptr) lista = actual->siguiente;
                else                    anterior->siguiente = actual->siguiente;
                delete actual;
                inscritos--;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return false;
    }

    void listar() const {
        if (lista == nullptr) { std::cout << "      (sin inscritos)\n"; return; }
        for (NodoMateria* a = lista; a != nullptr; a = a->siguiente) {
            std::cout << "      - " << a->matricula << "\n";
        }
    }
};

// ===========================================================================
// 4) SISTEMA COMPLEJO
// ===========================================================================
class SistemaUniversidad {
private:
    ArbolEstudiantes arbol;                  // ABB por matricula
    std::vector<Materia*> materias;          // lista de materias (arreglo dinamico)
    std::queue<std::string> consultas;       // COLA de consultas pendientes
    std::stack<std::pair<std::string, std::string>> historial;  // PILA de matriculaciones
    ListaEspera espera;                      // LISTA DOBLE de espera

public:
    SistemaUniversidad() = default;

    ~SistemaUniversidad() {
        for (Materia* m : materias) delete m;
    }

    // ------------------------------------------------------------------
    // 1) Registrar estudiante
    // ------------------------------------------------------------------
    Estudiante* registrar(const std::string& matricula, const std::string& nombre,
                          const std::string& carrera, double promedio) {
        if (arbol.buscar(matricula) != nullptr) return nullptr;   // ya existe

        Estudiante* e = new Estudiante(matricula, nombre, carrera, promedio);
        arbol.insertar(e);
        return e;
    }

    Materia* crearMateria(const std::string& nombre, int capacidad) {
        materias.push_back(new Materia(nombre, capacidad));
        return materias.back();
    }

    // ------------------------------------------------------------------
    // 2) Consultas: se encolan y se atienden en orden FIFO
    // ------------------------------------------------------------------
    void dejarConsulta(const std::string& matricula) {
        consultas.push(matricula);
    }

    void atenderConsultas() {
        if (consultas.empty()) {
            std::cout << "    No hay consultas pendientes.\n";
            return;
        }
        while (!consultas.empty()) {
            std::string matricula = consultas.front();   // sale el primero
            consultas.pop();

            Estudiante* e = arbol.buscar(matricula);
            std::cout << "    Atendiendo consulta de " << matricula << ": ";
            if (e == nullptr) {
                std::cout << "el estudiante no esta registrado.\n";
            } else {
                std::cout << e->nombre << " (" << e->carrera
                          << ") -> matriculado en " << e->materias.size() << " materia(s)\n";
            }
        }
    }

    // ------------------------------------------------------------------
    // 3) Matricular en una materia
    // ------------------------------------------------------------------
    bool matricular(const std::string& matricula, const std::string& materia) {
        Estudiante* e = arbol.buscar(matricula);
        if (e == nullptr) return false;                       // no existe el estudiante

        for (const std::string& m : e->materias) {
            if (m == materia) return false;                   // ya estaba matriculado
        }

        for (Materia* m : materias) {
            if (m->nombre != materia) continue;
            if (m->estaLlena()) return false;                 // sin cupos
            m->inscribir(matricula);
            e->materias.push_back(materia);
            historial.push(std::make_pair(matricula, materia));   // se apila el cambio
            return true;
        }
        return false;                                          // la materia no existe
    }

    // ------------------------------------------------------------------
    // 7) DESHACER la ultima matriculacion (pila)
    // ------------------------------------------------------------------
    bool deshacerMatriculacion() {
        if (historial.empty()) return false;

        std::pair<std::string, std::string> cambio = historial.top();
        historial.pop();

        Estudiante* e = arbol.buscar(cambio.first);
        for (Materia* m : materias) {
            if (m->nombre == cambio.second) m->desinscribir(cambio.first);
        }

        if (e != nullptr) {
            e->materias.pop_back();   // el ABB mantiene el orden de las materias
        }
        return true;
    }

    // ------------------------------------------------------------------
    // 5) Consultar una matricula
    // ------------------------------------------------------------------
    Estudiante* consultar(const std::string& matricula) const {
        return arbol.buscar(matricula);
    }

    // ------------------------------------------------------------------
    // 4) Listar estudiantes ordenados por promedio (de mayor a menor)
    //    Se usa el IN-ORDEN del ABB (sale ordenado por matricula) y despues
    //    se ordena por promedio con std::sort.
    // ------------------------------------------------------------------
    std::vector<Estudiante*> ordenarPorPromedio() const {
        std::vector<Estudiante*> lista = arbol.listar();
        std::sort(lista.begin(), lista.end(),
                  [](const Estudiante* a, const Estudiante* b) {
                      return a->promedio > b->promedio;   // de mayor a menor
                  });
        return lista;
    }

    // ------------------------------------------------------------------
    // 6) Dar de baja un estudiante: se quita de todas las materias
    // ------------------------------------------------------------------
    bool darDeBaja(const std::string& matricula) {
        Estudiante* e = arbol.buscar(matricula);
        if (e == nullptr) return false;      // el ABB no lo encuentra

        // 1) se le da de baja de todas sus materias (una lista por materia)
        for (const std::string& nombreMateria : e->materias) {
            for (Materia* m : materias) {
                if (m->nombre == nombreMateria) m->desinscribir(matricula);
            }
        }
        e->materias.clear();

        // 2) sale de la lista de espera, si estaba
        espera.quitar(e->nombre);

        // 3) se elimina del ABB
        return arbol.borrar(matricula);
    }

    int totalEstudiantes() const { return arbol.contar(); }

    void agregarAEspera(const std::string& nombre, const std::string& carrera) {
        espera.agregar(nombre, carrera);
    }

    // ------------------------------------------------------------------
    // 8) Reporte final
    // ------------------------------------------------------------------
    void reporte() const {
        std::cout << "\n  REPORTE FINAL\n";
        std::cout << "  -------------\n";
        std::cout << "  Estudiantes registrados : " << arbol.contar() << "\n";

        int totalMatriculas = 0;
        for (const Materia* m : materias) totalMatriculas += m->inscritos;
        std::cout << "  Total de matriculas     : " << totalMatriculas << "\n";

        std::vector<Estudiante*> porPromedio = ordenarPorPromedio();
        if (!porPromedio.empty()) {
            std::cout << "  Mejor promedio          : " << porPromedio[0]->nombre
                      << " (" << porPromedio[0]->promedio << ")\n";
        }

        std::cout << "  En lista de espera      : " << espera.tamano() << "\n\n";

        std::cout << "  Cupos por materia:\n";
        for (const Materia* m : materias) {
            std::cout << "    " << std::left << std::setw(22) << m->nombre
                      << std::setw(3) << m->inscritos << "/" << m->capacidad
                      << "  (" << m->lugaresLibres() << " libre(s))\n";
        }
    }

    const std::vector<Materia*>& getMaterias() const { return materias; }
    const ListaEspera& getEspera() const { return espera; }
};

// ===========================================================================
int main() {
    std::cout << "=== EJERCICIO 14: proyecto final (sistema universitario) ===\n\n";

    SistemaUniversidad uni;

    // --- 1) Registrar estudiantes (ABB por matricula) -----------------------
    std::cout << "-- 1) Registrar estudiantes --\n";
    uni.registrar("2023101", "Ana Torres",    "Ingenieria", 8.7);
    uni.registrar("2023102", "Luis Ramirez",  "Ingenieria", 7.9);
    uni.registrar("2023103", "Sofia Diaz",    "Matematicas", 9.4);
    uni.registrar("2023104", "Diego Morales", "Ingenieria", 6.8);
    uni.registrar("2023105", "Luisa Fernandez","Matematicas", 8.1);
    std::cout << "  5 estudiantes registrados (la clave del ABB es la matricula)\n";
    std::cout << "  Registrar de nuevo a 2023101 -> "
              << (uni.registrar("2023101", "Otro", "X", 1.0) == nullptr ? "rechazado (ya existe)" : "aceptado")
              << "\n\n";

    // --- Crear materias -------------------------------------------------------
    std::cout << "-- Materias con cupos --\n";
    uni.crearMateria("Estructuras de Datos", 3);
    uni.crearMateria("Calculo",              2);
    uni.crearMateria("Base de Datos",        2);
    std::cout << "  Estructuras de Datos (3 cupos), Calculo (2), Base de Datos (2)\n\n";

    // --- 3) Matricular --------------------------------------------------------
    std::cout << "-- 3) Matricular --\n";
    std::cout << "  2023101 en Estructuras de Datos: " << std::boolalpha << uni.matricular("2023101", "Estructuras de Datos") << "\n";
    std::cout << "  2023102 en Estructuras de Datos: " << uni.matricular("2023102", "Estructuras de Datos") << "\n";
    std::cout << "  2023103 en Estructuras de Datos: " << uni.matricular("2023103", "Estructuras de Datos") << "\n";
    std::cout << "  -> ahora la materia esta LLENA (3/3)\n";
    std::cout << "  2023104 en Estructuras de Datos: " << uni.matricular("2023104", "Estructuras de Datos") << "  (sin cupos)\n\n";

    std::cout << "  2023103 en Calculo:       " << uni.matricular("2023103", "Calculo") << "\n";
    std::cout << "  2023105 en Calculo:       " << uni.matricular("2023105", "Calculo") << "\n";
    std::cout << "  2023104 en Calculo:       " << uni.matricular("2023104", "Calculo") << "  (sin cupos)\n";
    std::cout << "  2023103 en Estructuras:   " << uni.matricular("2023103", "Estructuras de Datos") << "  (ya matriculado)\n";
    std::cout << "  9999999 en Calculo:       " << uni.matricular("9999999", "Calculo") << "  (no existe el estudiante)\n";
    std::cout << "  2023101 en Materia X:     " << uni.matricular("2023101", "Materia X") << "  (no existe la materia)\n\n";

    // --- 5) Consultar una matricula -----------------------------------------
    std::cout << "-- 5) Consultar una matricula --\n";
    Estudiante* e = uni.consultar("2023103");
    if (e != nullptr) {
        std::cout << "  " << e->matricula << " | " << e->nombre << " | " << e->carrera
                  << " | promedio " << e->promedio << "\n";
        std::cout << "    Materias: ";
        for (const std::string& m : e->materias) std::cout << m << ", ";
        std::cout << "\n";
    }
    std::cout << "  Consultar 7777777 -> "
              << (uni.consultar("7777777") == nullptr ? "no registrado" : "encontrado") << "\n\n";

    // --- 2) Consultas con COLA ----------------------------------------------
    std::cout << "-- 2) Consultas en la cola (se atienden en orden FIFO) --\n";
    uni.dejarConsulta("2023102");
    uni.dejarConsulta("2023101");
    uni.dejarConsulta("7777777");
    uni.dejarConsulta("2023105");
    uni.atenderConsultas();
    std::cout << "\n";

    // --- 4) Ordenar por promedio --------------------------------------------
    std::cout << "-- 4) Estudiantes ordenados por promedio --\n";
    std::vector<Estudiante*> orden = uni.ordenarPorPromedio();
    int lugar = 1;
    for (const Estudiante* est : orden) {
        std::cout << "  " << lugar++ << ". " << std::left << std::setw(20) << est->nombre
                  << std::setw(6) << est->promedio << "  (" << est->carrera << ")\n";
    }
    std::cout << "\n";

    // --- 7) Deshacer la ultima matriculacion (PILA) -------------------------
    std::cout << "-- 7) Deshacer la ultima matriculacion (pila) --\n";
    std::cout << "  Antes de deshacer, 2023105 esta en: ";
    Estudiante* luisa = uni.consultar("2023105");
    std::cout << luisa->materias.size() << " materia(s)\n";
    std::cout << "  deshacerMatriculacion() = " << std::boolalpha << uni.deshacerMatriculacion() << "\n";
    std::cout << "  Ahora 2023105 esta en:    " << uni.consultar("2023105")->materias.size() << " materia(s)\n";

    // --- Lista de espera con LISTA DOBLE ------------------------------------
    std::cout << "\n-- 6) Lista de espera (lista doblemente enlazada) --\n";
    uni.agregarAEspera("Pedro Gomez",   "Ingenieria");
    uni.agregarAEspera("Laura Sanchez", "Ingenieria");
    uni.agregarAEspera("Marco Ruiz",    "Matematicas");
    std::cout << "  Recorrido normal (del primero al ultimo):\n";
    uni.getEspera().mostrar();
    std::cout << "  Recorrido inverso (del ultimo al primero):\n";
    uni.getEspera().mostrarInvertida();
    std::cout << "  Total en espera: " << uni.getEspera().tamano() << "\n\n";

    // --- 6) Dar de baja ------------------------------------------------------
    std::cout << "-- 6) Dar de baja a un estudiante --\n";
    Estudiante* ana = uni.consultar("2023101");
    std::cout << "  Ana Torres antes:  " << ana->materias.size()
              << " materia(s), total de estudiantes = " << uni.totalEstudiantes() << "\n";
    std::cout << "  darDeBaja(\"2023101\") = " << std::boolalpha << uni.darDeBaja("2023101") << "\n";
    std::cout << "  Ana Torres despues: " << (uni.consultar("2023101") == nullptr
                                              ? "ya NO esta registrado"
                                              : "sigue registrado") << "\n";
    std::cout << "  Total de estudiantes ahora: " << uni.totalEstudiantes() << "\n";
    std::cout << "  darDeBaja(\"7777777\") = " << uni.darDeBaja("7777777")
              << "  (no estaba registrado)\n\n";

    // --- 8) Reporte final ---------------------------------------------------
    uni.reporte();

    std::cout << "\n  Listados de cada materia:\n";
    for (const Materia* m : uni.getMaterias()) {
        std::cout << "    " << m->nombre << ":\n";
        m->listar();
    }

    std::cout << "\nDemostracion completada.\n";
    return 0;
}

// === SALIDA ESPERADA ===
/*
    === EJERCICIO 14: proyecto final (sistema universitario) ===
    -- 1) Registrar estudiantes --
      5 estudiantes registrados (la clave del ABB es la matricula)
      Registrar de nuevo a 2023101 -> rechazado (ya existe)
    -- Materias con cupos --
      Estructuras de Datos (3 cupos), Calculo (2), Base de Datos (2)
    -- 3) Matricular --
      2023101 en Estructuras de Datos: true
      2023102 en Estructuras de Datos: true
      2023103 en Estructuras de Datos: true
      -> ahora la materia esta LLENA (3/3)
      2023104 en Estructuras de Datos: false  (sin cupos)
      2023103 en Calculo:       true
      2023105 en Calculo:       true
      2023104 en Calculo:       false  (sin cupos)
      2023103 en Estructuras:   false  (ya matriculado)
      9999999 en Calculo:       false  (no existe el estudiante)
      2023101 en Materia X:     false  (no existe la materia)
    -- 5) Consultar una matricula --
      2023103 | Sofia Diaz | Matematicas | promedio 9.4
        Materias: Estructuras de Datos, Calculo,
      Consultar 7777777 -> no registrado
    -- 2) Consultas en la cola (se atienden en orden FIFO) --
        Atendiendo consulta de 2023102: Luis Ramirez (Ingenieria) -> matriculado en 1 materia(s)
        Atendiendo consulta de 2023101: Ana Torres (Ingenieria) -> matriculado en 1 materia(s)
        Atendiendo consulta de 7777777: el estudiante no esta registrado.
        Atendiendo consulta de 2023105: Luisa Fernandez (Matematicas) -> matriculado en 1 materia(s)
    -- 4) Estudiantes ordenados por promedio --
      1. Sofia Diaz          9.4     (Matematicas)
      2. Ana Torres          8.7     (Ingenieria)
      3. Luisa Fernandez     8.1     (Matematicas)
      4. Luis Ramirez        7.9     (Ingenieria)
      5. Diego Morales       6.8     (Ingenieria)
    -- 7) Deshacer la ultima matriculacion (pila) --
      Antes de deshacer, 2023105 esta en: 1 materia(s)
      deshacerMatriculacion() = true
      Ahora 2023105 esta en:    0 materia(s)
    -- 6) Lista de espera (lista doblemente enlazada) --
      Recorrido normal (del primero al ultimo):
        1. Pedro Gomez (Ingenieria)
        2. Laura Sanchez (Ingenieria)
        3. Marco Ruiz (Matematicas)
      Recorrido inverso (del ultimo al primero):
        3. Marco Ruiz (Matematicas)
        2. Laura Sanchez (Ingenieria)
        1. Pedro Gomez (Ingenieria)
      Total en espera: 3
    -- 6) Dar de baja a un estudiante --
      Ana Torres antes:  1 materia(s), total de estudiantes = 5
      darDeBaja("2023101") = true
      Ana Torres despues: ya NO esta registrado
      Total de estudiantes ahora: 4
      darDeBaja("7777777") = false  (no estaba registrado)
      REPORTE FINAL
      -------------
      Estudiantes registrados : 4
      Total de matriculas     : 3
      Mejor promedio          : Sofia Diaz (9.4)
      En lista de espera      : 3
      Cupos por materia:
        Estructuras de Datos  2  /3  (1 libre(s))
        Calculo               1  /2  (1 libre(s))
        Base de Datos         0  /2  (2 libre(s))
      Listados de cada materia:
        Estructuras de Datos:
          - 2023103
          - 2023102
        Calculo:
          - 2023103
        Base de Datos:
          (sin inscritos)
    Demostracion completada.
*/
