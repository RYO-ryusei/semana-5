// Actividad 3 — Patrón Result<T>: pipeline buscar -> validar -> inscribir (C++17)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "result.hpp"

struct Estudiante {
    int codigo;
    std::string nombre;
    double ppa;
    std::vector<std::string> cursos;
};

static constexpr double PPA_MINIMO = 12.0;

static const std::unordered_map<int, Estudiante> BD = {
    {1001, {1001, "Juan Mamani", 15.0, {}}},
    {1002, {1002, "Rosa Quispe", 10.5, {}}},
    {1003, {1003, "Luis Condori", 14.0, {"SIS206"}}},
};

// Paso 1 — fallo esperado: el código puede no existir
Result<Estudiante> buscar_estudiante(int codigo) {
    auto it = BD.find(codigo);
    if (it == BD.end())
        return Result<Estudiante>::fallo("Código " + std::to_string(codigo) + " no existe");
    return Result<Estudiante>::ok(it->second);
}

// Paso 2 — regla de negocio
Result<Estudiante> validar_ppa_minimo(const Estudiante& e) {
    if (e.ppa < PPA_MINIMO)
        return Result<Estudiante>::fallo("PPA " + std::to_string(e.ppa).substr(0, 4) + " < " +
                                         std::to_string(PPA_MINIMO).substr(0, 4) + " (mínimo para inscribirse)");
    return Result<Estudiante>::ok(e);
}

// Paso 3 — inscripción
Result<std::string> inscribir(const Estudiante& e, const std::string& curso) {
    if (std::find(e.cursos.begin(), e.cursos.end(), curso) != e.cursos.end())
        return Result<std::string>::fallo(e.nombre + " ya está inscrito en " + curso);
    return Result<std::string>::ok(e.nombre + " inscrito en " + curso);
}

int main() {
    std::cout << "Pipeline: buscar_estudiante -> validar_ppa_minimo -> inscribir(SIS206)\n\n";
    for (int codigo : {1001, 9999, 1002, 1003}) {
        auto resultado = buscar_estudiante(codigo)
                             .andThen(validar_ppa_minimo)
                             .andThen([](const Estudiante& e) { return inscribir(e, "SIS206"); });

        std::cout << "codigo=" << codigo << " -> ";
        if (resultado.esExito())
            std::cout << "EXITO: " << resultado.valor() << "\n";
        else
            std::cout << "FALLO: " << resultado.error() << "\n";
    }

    // map(): transforma el valor de un Result exitoso (sin crear otro Result a mano)
    std::cout << "\nmap() sobre un resultado exitoso:\n";
    auto nombre = buscar_estudiante(1001).map([](const Estudiante& e) { return e.nombre; });
    auto mayus = nombre.map([](const std::string& s) {
        std::string r = s;
        std::transform(r.begin(), r.end(), r.begin(), ::toupper);
        return r;
    });
    std::cout << "  nombre en mayúsculas = " << mayus.valor() << "\n";

    std::cout << "map() sobre un fallo (la función NO se ejecuta):\n";
    int llamadas = 0;
    auto fallido = buscar_estudiante(9999).map([&](const Estudiante& e) {
        ++llamadas;
        return e.nombre;
    });
    std::cout << "  llamadas a la función = " << llamadas << ", error propagado = " << fallido.error() << "\n";
    return 0;
}
