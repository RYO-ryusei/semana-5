// Actividad 1 (Parte A) — Jerarquía de excepciones en C++17 + ErrorMatriculaDuplicada
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#include <algorithm>
#include <iostream>
#include <vector>
#include "errores_academicos.hpp"

// Lanza ErrorMatriculaDuplicada si el código ya está matriculado; si no, lo registra.
void validar_matricula(int codigo, std::vector<int>& matriculados) {
    if (std::find(matriculados.begin(), matriculados.end(), codigo) != matriculados.end())
        throw ErrorMatriculaDuplicada(std::to_string(codigo));
    matriculados.push_back(codigo);
}

void intentar(int codigo, std::vector<int>& matriculados) {
    std::cout << "Matriculando código " << codigo << " ... ";
    try {
        validar_matricula(codigo, matriculados);
        std::cout << "OK\n";
    } catch (const ErrorMatriculaDuplicada& e) {  // 1.° el más específico
        std::cout << "DUPLICADA\n  catch ErrorMatriculaDuplicada -> codigo_estudiante="
                  << e.codigo_estudiante() << "\n  " << e.what() << "\n";
    } catch (const ErrorValidacion& e) {  // 2.° rama de validación
        std::cout << "\n  catch ErrorValidacion: " << e.what() << "\n";
    } catch (const ErrorAcademico& e) {  // 3.° cualquier error del dominio
        std::cout << "\n  catch ErrorAcademico: " << e.what() << "\n";
    }
}

int main() {
    std::cout << "=== Parte A: ErrorMatriculaDuplicada ===\n";
    std::vector<int> matriculados = {20210500, 20210501};
    intentar(20210502, matriculados);  // nuevo
    intentar(20210500, matriculados);  // duplicado

    std::cout << "\n=== Captura selectiva: una sola rama atrapa a sus hijos ===\n";
    try {
        throw ErrorCalificacion(-3.5f);
    } catch (const ErrorValidacion& e) {  // ErrorCalificacion ES un ErrorValidacion
        std::cout << "ErrorValidacion atrapó una ErrorCalificacion: " << e.what() << "\n";
    }

    std::cout << "\n=== Polimorfismo: captura por referencia a la base ===\n";
    try {
        throw ErrorCapacidad(6, 6, "matricula");
    } catch (const ErrorAcademico& e) {
        std::cout << "what() via ErrorAcademico&: " << e.what() << "\n";
    }

    std::cout << "\n=== noexcept en los getters ===\n";
    ErrorCalificacion ec(25.0f);
    std::cout << "noexcept(ec.valor()) = " << std::boolalpha << noexcept(ec.valor()) << "\n";
    return 0;
}
