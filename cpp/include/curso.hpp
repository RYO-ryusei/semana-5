// curso.hpp — Diseño por Contrato: precondiciones, poscondiciones e invariante
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#pragma once
#include <cassert>   // assert(): poscondiciones e invariantes
#include <string>
#include <utility>
#include "errores_academicos.hpp"

class Curso {
    /*
     * INVARIANTE de clase:
     *   0 <= inscritos_ <= cupos_max_
     *   creditos_ in [1, 6]
     *   !codigo_.empty()
     */
    std::string codigo_;
    int creditos_;
    int cupos_max_;
    int inscritos_ = 0;

    void verificarInvariante() const {
        assert(!codigo_.empty() && "INV: código vacío");
        assert(creditos_ >= 1 && creditos_ <= 6 && "INV: créditos fuera [1,6]");
        assert(inscritos_ >= 0 && "INV: inscritos negativo");
        assert(inscritos_ <= cupos_max_ && "INV: inscritos > cupos");
    }

public:
    Curso(std::string codigo, int creditos, int cuposMax)
        : codigo_(std::move(codigo)), creditos_(creditos), cupos_max_(cuposMax) {
        // PRECONDICIONES (error de uso -> throw)
        if (codigo_.empty()) throw ErrorValidacion("Código del curso no puede ser vacío");
        if (creditos < 1 || creditos > 6) throw ErrorValidacion("Créditos deben estar en [1, 6]");
        if (cuposMax < 1) throw ErrorValidacion("cuposMax debe ser >= 1");
        verificarInvariante();  // POST-constructor
    }

    void inscribir() {
        // PRECONDICIÓN
        if (inscritos_ >= cupos_max_) throw ErrorCapacidad(inscritos_, cupos_max_, codigo_);
        inscritos_++;
        // POSCONDICIÓN (assert: si falla es un bug interno)
        assert(inscritos_ <= cupos_max_ && "POST: invariante rota tras inscribir");
        verificarInvariante();
    }

    const std::string& codigo() const noexcept { return codigo_; }
    int inscritos() const noexcept { return inscritos_; }
    int cuposMax() const noexcept { return cupos_max_; }
    int cuposLibres() const noexcept { return cupos_max_ - inscritos_; }
};
