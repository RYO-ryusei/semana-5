// errores_academicos.hpp — Jerarquía de excepciones del dominio académico (C++17)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#pragma once
#include <iomanip>
#include <sstream>
#include <stdexcept>   // std::runtime_error
#include <string>
#include <utility>     // std::move

namespace detalle {
// std::to_string(float) imprime "25.000000"; este helper deja "25.00".
inline std::string fmt(float v) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(2) << v;
    return os.str();
}
}  // namespace detalle

// ── Raíz del dominio: hereda de runtime_error ─────────────────────────────
class ErrorAcademico : public std::runtime_error {
public:
    explicit ErrorAcademico(const std::string& msg)
        : std::runtime_error("[Académico] " + msg) {}
    // Destructor virtual explícito: obligatorio para el polimorfismo de excepciones.
    ~ErrorAcademico() override = default;
};

// ── Primer nivel ──────────────────────────────────────────────────────────
class ErrorValidacion : public ErrorAcademico {
public:
    explicit ErrorValidacion(const std::string& msg)
        : ErrorAcademico("[Validación] " + msg) {}
};

// ── Segundo nivel: error con valor concreto ───────────────────────────────
class ErrorCalificacion : public ErrorValidacion {
    float valor_;
public:
    explicit ErrorCalificacion(float v)
        : ErrorValidacion("Calificación " + detalle::fmt(v) + " fuera de rango [0, 20]"),
          valor_(v) {}
    float valor() const noexcept { return valor_; }   // noexcept: nunca lanza
};

// Actividad 1 — matrícula duplicada (hereda de ErrorValidacion)
class ErrorMatriculaDuplicada : public ErrorValidacion {
    std::string codigo_estudiante_;
public:
    explicit ErrorMatriculaDuplicada(std::string codigo)
        : ErrorValidacion("Matrícula duplicada para el estudiante " + codigo),
          codigo_estudiante_(std::move(codigo)) {}
    const std::string& codigo_estudiante() const noexcept { return codigo_estudiante_; }
};

class ErrorCapacidad : public ErrorAcademico {
    int actual_, maximo_;
public:
    ErrorCapacidad(int actual, int maximo, const std::string& entidad)
        : ErrorAcademico(entidad + ": capacidad agotada (" + std::to_string(actual) +
                         "/" + std::to_string(maximo) + ")"),
          actual_(actual), maximo_(maximo) {}
    int actual() const noexcept { return actual_; }
    int maximo() const noexcept { return maximo_; }
};

class ErrorRecursoNoEncontrado : public ErrorAcademico {
    std::string tipo_;
    int id_;
public:
    ErrorRecursoNoEncontrado(std::string tipo, int id)
        : ErrorAcademico(tipo + " id=" + std::to_string(id) + " no encontrado"),
          tipo_(std::move(tipo)), id_(id) {}
    const std::string& tipo() const noexcept { return tipo_; }
    int id() const noexcept { return id_; }
};
