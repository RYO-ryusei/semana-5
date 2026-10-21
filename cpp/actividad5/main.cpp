// Actividad 5 — Proyecto Integrador: robustez en todas las capas (C++17)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#include <iostream>
#include "modelos.hpp"

int main() {
    RepositorioEstudiantes repo;
    repo.agregar(Estudiante(1001, "Juan Mamani", 15.0));
    repo.agregar(Estudiante(1002, "Rosa Quispe", 13.5));
    std::cout << "Repositorio con " << repo.cantidad() << " estudiantes\n\n";

    std::cout << "--- Result<T> en la búsqueda (fallo esperado) ---\n";
    for (int codigo : {1001, 9999}) {
        auto r = repo.buscar(codigo);
        if (r.esExito()) std::cout << "buscar(" << codigo << ") -> " << r.valor().nombre() << "\n";
        else             std::cout << "buscar(" << codigo << ") -> FALLO: " << r.error() << "\n";
    }

    std::cout << "\n--- Excepciones en las precondiciones (error de uso) ---\n";
    try { Estudiante(1003, "Ana", 25.0); }
    catch (const ErrorDatoInvalido& e) { std::cout << "ErrorDatoInvalido(campo=" << e.campo() << "): " << e.what() << "\n"; }
    try { repo.agregar(Estudiante(1001, "Otro", 10.0)); }
    catch (const ErrorEntidadDuplicada& e) { std::cout << "ErrorEntidadDuplicada: " << e.what() << "\n"; }
    try { repo.obtener(4242); }
    catch (const ErrorEntidadNoEncontrada& e) { std::cout << "ErrorEntidadNoEncontrada: " << e.what() << "\n"; }

    std::cout << "\n--- Seccion con DbC ---\n";
    Seccion sec("POO II - A", 2);
    sec.inscribir(repo.obtener(1001));
    sec.inscribir(repo.obtener(1002));
    std::cout << "Inscritos " << sec.numInscritos() << ", cupos libres " << sec.cuposLibres() << "\n";
    try { sec.inscribir(Estudiante(1005, "Pedro", 12.0)); }
    catch (const ErrorCupoAgotado& e) { std::cout << "ErrorCupoAgotado: " << e.what() << "\n"; }
    try { throw ErrorCupoAgotado(2, 2, "X"); }
    catch (const ErrorDominio& e) { std::cout << "Captura por familia (ErrorDominio&): " << e.what() << "\n"; }
    return 0;
}
