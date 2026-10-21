// Actividad 5 — Pruebas de contratos del Proyecto Integrador (C++17)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
// Compilar SIN -DNDEBUG (assert activo).
#include <cassert>
#include <iostream>
#include "modelos.hpp"

static int pasadas = 0;
#define PASS(msg) do { ++pasadas; std::cout << "[PASS] " << msg << "\n"; } while (0)

// ── 3 pruebas VÁLIDAS ────────────────────────────────────────────────────
void test_estudiante_valido() {
    Estudiante e(1001, "Juan Mamani", 15.0);
    assert(e.codigo() == 1001 && e.ppa() == 15.0);
    e.actualizarPPA(16.5);
    assert(e.ppa() == 16.5);
    PASS("estudiante válido y actualizarPPA");
}
void test_seccion_inscribe() {
    Seccion s("POO II - A", 2);
    s.inscribir(Estudiante(1, "A", 10));
    assert(s.numInscritos() == 1 && s.cuposLibres() == 1);
    PASS("sección inscribe dentro del cupo");
}
void test_buscar_result() {
    RepositorioEstudiantes r;
    r.agregar(Estudiante(1001, "Juan Mamani", 15.0));
    assert(r.buscar(1001).esExito());
    assert(r.buscar(1001).valor().nombre() == "Juan Mamani");
    assert(!r.buscar(9999).esExito());
    PASS("buscar() devuelve Result ok / fallo sin lanzar");
}

// ── 3 pruebas que DEBEN LANZAR ───────────────────────────────────────────
void test_ppa_fuera_de_rango() {
    try { Estudiante e(1, "A", 25.0); assert(false && "No lanzó"); }
    catch (const ErrorDatoInvalido& e) { assert(e.campo() == "ppa"); PASS(std::string("PPA inválido: ") + e.what()); }
}
void test_cupo_agotado() {
    Seccion s("S", 1);
    s.inscribir(Estudiante(1, "A", 10));
    try { s.inscribir(Estudiante(2, "B", 10)); assert(false && "No lanzó"); }
    catch (const ErrorCupoAgotado& e) { assert(e.actual() == 1 && e.maximo() == 1); PASS(std::string("cupo agotado: ") + e.what()); }
}
void test_repositorio_duplicado() {
    RepositorioEstudiantes r;
    r.agregar(Estudiante(1, "A", 10));
    try { r.agregar(Estudiante(1, "B", 11)); assert(false && "No lanzó"); }
    catch (const ErrorEntidadDuplicada& e) { PASS(std::string("duplicado: ") + e.what()); }
}

// ── Extras ───────────────────────────────────────────────────────────────
void test_obtener_no_encontrado() {
    RepositorioEstudiantes r;
    try { r.obtener(77); assert(false && "No lanzó"); }
    catch (const ErrorDominio& e) { PASS(std::string("obtener inexistente (por familia): ") + e.what()); }
}
void test_inscripcion_repetida() {
    Seccion s("S", 3);
    Estudiante e(1, "A", 10);
    s.inscribir(e);
    try { s.inscribir(e); assert(false && "No lanzó"); }
    catch (const ErrorEntidadDuplicada& ex) { PASS(std::string("inscripción repetida: ") + ex.what()); }
}

int main() {
    test_estudiante_valido();
    test_seccion_inscribe();
    test_buscar_result();
    test_ppa_fuera_de_rango();
    test_cupo_agotado();
    test_repositorio_duplicado();
    test_obtener_no_encontrado();
    test_inscripcion_repetida();
    std::cout << "\nTodas las pruebas pasaron (" << pasadas << ").\n";
    return 0;
}
