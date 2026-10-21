// Actividad 4 — Pruebas unitarias de contratos para Matricula (C++17, assert en main)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
// IMPORTANTE: compilar SIN -DNDEBUG para que assert() siga activo.
#include <cassert>
#include <chrono>
#include <iostream>
#include "matricula.hpp"

static int pasadas = 0;
#define PASS(msg) do { ++pasadas; std::cout << "[PASS] " << msg << "\n"; } while (0)

void test_constructor_valido() {
    Matricula m("20210500", {"SIS206", "SIS207", "SIS208"});
    assert(m.numCursos() == 3);
    assert(!m.anulada());
    PASS("constructor válido");
}

void test_constructor_sin_cursos() {
    try {
        Matricula m("20210500", {});  // debe lanzar
        assert(false && "No lanzó excepción");
    } catch (const ErrorCapacidad& e) {
        PASS(std::string("constructor sin cursos: ") + e.what());
    }
}

void test_constructor_demasiados_cursos() {
    try {
        Matricula m("20210500", {"A", "B", "C", "D", "E", "F", "G"});
        assert(false && "No lanzó excepción");
    } catch (const ErrorCapacidad& e) {
        assert(e.actual() == 7 && e.maximo() == 6);
        PASS(std::string("constructor con 7 cursos: ") + e.what());
    }
}

void test_constructor_duplicados() {
    try {
        Matricula m("20210500", {"SIS206", "SIS206"});
        assert(false && "No lanzó excepción");
    } catch (const ErrorValidacion& e) {
        PASS(std::string("constructor con duplicados: ") + e.what());
    }
}

void test_constructor_fecha_futura() {
    try {
        auto futura = std::chrono::system_clock::now() + std::chrono::hours(24);
        Matricula m("20210500", {"SIS206"}, futura);
        assert(false && "No lanzó excepción");
    } catch (const ErrorValidacion& e) {
        PASS(std::string("fecha futura: ") + e.what());
    }
}

void test_agregar_curso_exitoso() {
    Matricula m("20210500", {"SIS206"});
    m.agregarCurso("SIS207");
    assert(m.numCursos() == 2);
    assert(m.cursos().back() == "SIS207");
    PASS("agregar curso exitoso");
}

void test_agregar_curso_duplicado() {
    Matricula m("20210500", {"SIS206"});
    try {
        m.agregarCurso("SIS206");  // duplicado — debe lanzar
        assert(false && "No lanzó excepción");
    } catch (const ErrorValidacion& e) {
        PASS(std::string("duplicado: ") + e.what());
    }
}

void test_eliminar_curso() {
    Matricula m("20210500", {"SIS206", "SIS207"});
    m.eliminarCurso("SIS207");
    assert(m.numCursos() == 1);
    try {
        m.eliminarCurso("SIS206");  // quedaría vacía
        assert(false && "No lanzó excepción");
    } catch (const ErrorValidacion& e) {
        PASS(std::string("eliminar último curso: ") + e.what());
    }
}

void test_anular_y_modificar() {
    Matricula m("20210500", {"SIS206"});
    m.anular();
    try {
        m.agregarCurso("SIS207");  // anulada — debe lanzar
        assert(false && "No lanzó excepción");
    } catch (const ErrorAcademico& e) {
        PASS(std::string("anulada: ") + e.what());
    }
}

void test_diagnostico() {
    Matricula m("20210500", {"SIS206", "SIS207"});
    assert(m.diagnostico() == "Matrícula [ACTIVA] — 2/6 cursos");
    m.anular();
    assert(m.diagnostico() == "Matrícula [ANULADA] — 2/6 cursos");
    PASS("diagnóstico ACTIVA/ANULADA");
}

int main() {
    test_constructor_valido();
    test_constructor_sin_cursos();
    test_constructor_demasiados_cursos();
    test_constructor_duplicados();
    test_constructor_fecha_futura();
    test_agregar_curso_exitoso();
    test_agregar_curso_duplicado();
    test_eliminar_curso();
    test_anular_y_modificar();
    test_diagnostico();
    std::cout << "\nTodas las pruebas pasaron (" << pasadas << ").\n";
    return 0;
}
