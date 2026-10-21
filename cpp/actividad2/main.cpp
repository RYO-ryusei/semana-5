// Actividad 2 — Diseño por Contrato: clase Matricula (C++17)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#include <chrono>
#include <iostream>
#include "matricula.hpp"

template <typename F>
void caso(const std::string& titulo, F accion) {
    std::cout << "- " << titulo << ": ";
    try {
        accion();
        std::cout << "OK\n";
    } catch (const ErrorCapacidad& e) {
        std::cout << "ErrorCapacidad -> " << e.what() << "\n";
    } catch (const ErrorValidacion& e) {
        std::cout << "ErrorValidacion -> " << e.what() << "\n";
    } catch (const ErrorAcademico& e) {
        std::cout << "ErrorAcademico -> " << e.what() << "\n";
    }
}

int main() {
    std::cout << "=== Constructor (precondiciones) ===\n";
    caso("matrícula válida", [] { Matricula m("20210500", {"SIS206", "SIS207"}); });
    caso("código vacío", [] { Matricula m("", {"SIS206"}); });
    caso("sin cursos", [] { Matricula m("20210500", {}); });
    caso("7 cursos", [] { Matricula m("20210500", {"A", "B", "C", "D", "E", "F", "G"}); });
    caso("cursos duplicados", [] { Matricula m("20210500", {"SIS206", "SIS206"}); });
    caso("fecha futura", [] {
        auto manana = std::chrono::system_clock::now() + std::chrono::hours(48);
        Matricula m("20210500", {"SIS206"}, manana);
    });

    std::cout << "\n=== Mutadores (pre + pos + invariante) ===\n";
    Matricula m("20210500", {"SIS206"});
    std::cout << m.diagnostico() << "\n";
    caso("agregar SIS207", [&] { m.agregarCurso("SIS207"); });
    caso("agregar SIS207 otra vez", [&] { m.agregarCurso("SIS207"); });
    caso("eliminar SIS207", [&] { m.eliminarCurso("SIS207"); });
    caso("eliminar el último curso", [&] { m.eliminarCurso("SIS206"); });
    for (const char* c : {"SIS208", "SIS209", "SIS210", "SIS211", "SIS212"})
        m.agregarCurso(c);
    std::cout << m.diagnostico() << "\n";
    caso("agregar un 7.° curso", [&] { m.agregarCurso("SIS213"); });

    std::cout << "\n=== Anulación ===\n";
    m.anular();
    std::cout << m.diagnostico() << "\n";
    caso("agregar con matrícula anulada", [&] { m.agregarCurso("SIS213"); });
    caso("anular por segunda vez", [&] { m.anular(); });
    return 0;
}
