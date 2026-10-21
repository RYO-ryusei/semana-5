// matricula.hpp — Clase Matricula con Diseño por Contrato completo (Actividad 2)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#pragma once
#include <algorithm>
#include <cassert>
#include <chrono>
#include <string>
#include <vector>
#include "errores_academicos.hpp"

class Matricula {
    /*
     * INVARIANTE de clase:
     *   !estudiante_codigo_.empty()
     *   1 <= cursos_.size() <= MAX_CURSOS
     *   No hay duplicados en cursos_
     *   fecha_ <= ahora            (la fecha no puede ser futura)
     *   anulada_ == true  =>  cursos_ ya no se modifica
     */
public:
    using Fecha = std::chrono::system_clock::time_point;
    static constexpr int MAX_CURSOS = 6;

private:
    std::string estudiante_codigo_;
    std::vector<std::string> cursos_;
    Fecha fecha_;
    bool anulada_ = false;

    static bool hayDuplicados(const std::vector<std::string>& v) {
        for (std::size_t i = 0; i < v.size(); ++i)
            for (std::size_t j = i + 1; j < v.size(); ++j)
                if (v[i] == v[j]) return true;
        return false;  // O(n²) aceptable para n <= 6
    }
    bool contiene(const std::string& c) const {
        return std::find(cursos_.begin(), cursos_.end(), c) != cursos_.end();
    }

    void verificarInvariante() const {
        assert(!estudiante_codigo_.empty() && "INV: código vacío");
        assert(cursos_.size() >= 1 && "INV: sin cursos");
        assert(static_cast<int>(cursos_.size()) <= MAX_CURSOS && "INV: excede máximo");
        assert(!hayDuplicados(cursos_) && "INV: cursos duplicados");
        assert(fecha_ <= std::chrono::system_clock::now() && "INV: fecha futura");
    }

public:
    Matricula(std::string codigoEst, std::vector<std::string> cursos,
              Fecha fecha = std::chrono::system_clock::now()) {
        // ── PRECONDICIONES ───────────────────────────────────────────────
        if (codigoEst.empty()) throw ErrorValidacion("Código de estudiante vacío");
        if (cursos.empty() || static_cast<int>(cursos.size()) > MAX_CURSOS)
            throw ErrorCapacidad(static_cast<int>(cursos.size()), MAX_CURSOS, "matricula");
        if (hayDuplicados(cursos)) throw ErrorValidacion("Cursos duplicados en la matrícula");
        if (fecha > std::chrono::system_clock::now())
            throw ErrorValidacion("Fecha de matrícula futura no permitida");

        estudiante_codigo_ = std::move(codigoEst);
        cursos_ = std::move(cursos);
        fecha_ = fecha;
        verificarInvariante();  // POST-constructor
    }

    void agregarCurso(const std::string& curso) {
        // ── PRECONDICIONES ───────────────────────────────────────────────
        if (anulada_) throw ErrorAcademico("Matrícula anulada — no se puede modificar");
        if (curso.empty()) throw ErrorValidacion("Código de curso vacío");
        if (static_cast<int>(cursos_.size()) >= MAX_CURSOS)
            throw ErrorCapacidad(static_cast<int>(cursos_.size()), MAX_CURSOS, "matricula");
        if (contiene(curso)) throw ErrorValidacion("Curso '" + curso + "' ya está en la matrícula");

        [[maybe_unused]] const auto antes = cursos_.size();
        cursos_.push_back(curso);

        // ── POSCONDICIONES ───────────────────────────────────────────────
        assert(contiene(curso) && "POST: curso no agregado");
        assert(cursos_.size() == antes + 1 && "POST: el tamaño debe crecer en 1");
        verificarInvariante();
    }

    void eliminarCurso(const std::string& curso) {
        // ── PRECONDICIONES ───────────────────────────────────────────────
        if (anulada_) throw ErrorAcademico("Matrícula anulada — no se puede modificar");
        if (!contiene(curso)) throw ErrorValidacion("Curso '" + curso + "' no está en la matrícula");
        if (cursos_.size() == 1)
            throw ErrorValidacion("La matrícula no puede quedar sin cursos (mínimo 1)");

        [[maybe_unused]] const auto antes = cursos_.size();
        cursos_.erase(std::remove(cursos_.begin(), cursos_.end(), curso), cursos_.end());

        // ── POSCONDICIONES ───────────────────────────────────────────────
        assert(!contiene(curso) && "POST: curso no eliminado");
        assert(cursos_.size() == antes - 1 && "POST: el tamaño debe bajar en 1");
        verificarInvariante();
    }

    void anular() {
        if (anulada_) throw ErrorAcademico("Matrícula ya está anulada");
        anulada_ = true;  // anular no rompe la invariante
    }

    std::string diagnostico() const {
        // Corrección: "texto" + (cond ? "a" : "b") sumaba punteros const char*.
        const std::string estado = anulada_ ? "ANULADA" : "ACTIVA";
        return "Matrícula [" + estado + "] — " + std::to_string(cursos_.size()) + "/" +
               std::to_string(MAX_CURSOS) + " cursos";
    }

    // Getters (noexcept: solo leen)
    bool anulada() const noexcept { return anulada_; }
    int numCursos() const noexcept { return static_cast<int>(cursos_.size()); }
    const std::vector<std::string>& cursos() const noexcept { return cursos_; }
    const std::string& estudiante() const noexcept { return estudiante_codigo_; }
};
