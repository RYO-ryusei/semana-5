// Marco teórico 2.5 — Patrón Result<T> con std::variant (C++17)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#include <iostream>
#include <string>
#include <unordered_map>
#include "result.hpp"

static const std::unordered_map<int, std::string> db = {{1001, "Juan Mamani"}};

Result<std::string> buscarEstudiante(int codigo) {
    auto it = db.find(codigo);
    if (it == db.end())
        return Result<std::string>::fallo("Código " + std::to_string(codigo) + " no encontrado");
    return Result<std::string>::ok(it->second);
}

int main() {
    for (int codigo : {1001, 9999}) {
        auto res = buscarEstudiante(codigo);
        if (res.esExito())
            std::cout << "Encontrado: " << res.valor() << "\n";
        else
            std::cout << "Error: " << res.error() << "\n";
    }

    // map(): transforma el valor solo si hubo éxito
    auto longitud = buscarEstudiante(1001).map([](const std::string& s) { return s.size(); });
    std::cout << "Longitud del nombre (map): " << longitud.valor() << "\n";
    auto sinNombre = buscarEstudiante(9999).map([](const std::string& s) { return s.size(); });
    std::cout << "map sobre fallo se propaga: " << sinNombre.error() << "\n";

    // valor() sobre un fallo es un bug del llamador -> logic_error
    try {
        buscarEstudiante(9999).valor();
    } catch (const std::logic_error& e) {
        std::cout << "valor() sobre fallo: " << e.what() << "\n";
    }
    return 0;
}
