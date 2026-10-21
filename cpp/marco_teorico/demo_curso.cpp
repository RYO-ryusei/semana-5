// Marco teórico 2.4 — Diseño por Contrato con la clase Curso (C++17)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#include <iostream>
#include "curso.hpp"

int main() {
    Curso poo("SIS206", 4, 2);
    std::cout << "Curso " << poo.codigo() << " creado: cupos libres = " << poo.cuposLibres() << "\n";

    poo.inscribir();
    poo.inscribir();
    std::cout << "Inscritos: " << poo.inscritos() << "/" << poo.cuposMax() << "\n";

    try {
        poo.inscribir();  // precondición violada: no hay cupos
    } catch (const ErrorCapacidad& e) {
        std::cout << "Precondición: " << e.what() << "  (" << e.actual() << "/" << e.maximo() << ")\n";
    }

    try {
        Curso malo("SIS999", 9, 10);  // créditos fuera de [1, 6]
    } catch (const ErrorValidacion& e) {
        std::cout << "Constructor: " << e.what() << "\n";
    }
    try {
        Curso vacio("", 3, 10);
    } catch (const ErrorValidacion& e) {
        std::cout << "Constructor: " << e.what() << "\n";
    }
    return 0;
}
