// errores_dominio.hpp — Jerarquía de errores del Proyecto Integrador (C++17)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#pragma once
#include <stdexcept>
#include <string>
#include <utility>

// Raíz del sistema
class ErrorDominio : public std::runtime_error {
public:
    explicit ErrorDominio(const std::string& msg) : std::runtime_error("[Dominio] " + msg) {}
    ~ErrorDominio() override = default;
};

// Dato de entrada inválido (guarda campo y valor)
class ErrorDatoInvalido : public ErrorDominio {
    std::string campo_;
public:
    ErrorDatoInvalido(std::string campo, const std::string& detalle)
        : ErrorDominio("Dato inválido en '" + campo + "': " + detalle), campo_(std::move(campo)) {}
    const std::string& campo() const noexcept { return campo_; }
};

// Entidad que ya existe
class ErrorEntidadDuplicada : public ErrorDominio {
    std::string tipo_;
    int id_;
public:
    ErrorEntidadDuplicada(std::string tipo, int id)
        : ErrorDominio(tipo + " id=" + std::to_string(id) + " ya existe"), tipo_(std::move(tipo)), id_(id) {}
    const std::string& tipo() const noexcept { return tipo_; }
    int id() const noexcept { return id_; }
};

// Entidad que no existe
class ErrorEntidadNoEncontrada : public ErrorDominio {
    std::string tipo_;
    int id_;
public:
    ErrorEntidadNoEncontrada(std::string tipo, int id)
        : ErrorDominio(tipo + " id=" + std::to_string(id) + " no encontrado"), tipo_(std::move(tipo)), id_(id) {}
    const std::string& tipo() const noexcept { return tipo_; }
    int id() const noexcept { return id_; }
};

// Sección sin cupos
class ErrorCupoAgotado : public ErrorDominio {
    int actual_, maximo_;
public:
    ErrorCupoAgotado(int actual, int maximo, const std::string& seccion)
        : ErrorDominio("Sección " + seccion + " sin cupos (" + std::to_string(actual) + "/" +
                       std::to_string(maximo) + ")"),
          actual_(actual), maximo_(maximo) {}
    int actual() const noexcept { return actual_; }
    int maximo() const noexcept { return maximo_; }
};
