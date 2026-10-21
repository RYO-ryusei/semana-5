// modelos.hpp — Clases del Proyecto Integrador con DbC y Result (C++17)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#pragma once
#include <algorithm>
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>
#include "errores_dominio.hpp"
#include "result.hpp"

// ── Estudiante ───────────────────────────────────────────────────────────
class Estudiante {
    /* INVARIANTE: codigo_ > 0, !nombre_.empty(), 0 <= ppa_ <= 20 */
    int codigo_;
    std::string nombre_;
    double ppa_;

    void verificarInvariante() const {
        assert(codigo_ > 0 && "INV: código no positivo");
        assert(!nombre_.empty() && "INV: nombre vacío");
        assert(ppa_ >= 0.0 && ppa_ <= 20.0 && "INV: PPA fuera de [0,20]");
    }

public:
    Estudiante(int codigo, std::string nombre, double ppa)
        : codigo_(codigo), nombre_(std::move(nombre)), ppa_(ppa) {
        if (codigo <= 0) throw ErrorDatoInvalido("codigo", "debe ser positivo");
        if (nombre_.empty()) throw ErrorDatoInvalido("nombre", "no puede ser vacío");
        if (ppa < 0.0 || ppa > 20.0) throw ErrorDatoInvalido("ppa", "debe estar en [0, 20]");
        verificarInvariante();
    }

    void actualizarPPA(double nuevo) {
        if (nuevo < 0.0 || nuevo > 20.0) throw ErrorDatoInvalido("ppa", "debe estar en [0, 20]");
        ppa_ = nuevo;
        assert(ppa_ == nuevo && "POST: PPA no actualizado");
        verificarInvariante();
    }

    int codigo() const noexcept { return codigo_; }
    const std::string& nombre() const noexcept { return nombre_; }
    double ppa() const noexcept { return ppa_; }
};

// ── Seccion (grupo de un curso con cupos) ────────────────────────────────
class Seccion {
    /* INVARIANTE: !nombre_.empty(), cupos_ >= 1, 0 <= inscritos <= cupos_, sin repetidos */
    std::string nombre_;
    int cupos_;
    std::vector<int> inscritos_;

    void verificarInvariante() const {
        assert(!nombre_.empty() && "INV: nombre vacío");
        assert(cupos_ >= 1 && "INV: cupos < 1");
        assert(static_cast<int>(inscritos_.size()) <= cupos_ && "INV: inscritos > cupos");
        for (std::size_t i = 0; i < inscritos_.size(); ++i)
            for (std::size_t j = i + 1; j < inscritos_.size(); ++j)
                assert(inscritos_[i] != inscritos_[j] && "INV: estudiante repetido");
    }

public:
    Seccion(std::string nombre, int cupos) : nombre_(std::move(nombre)), cupos_(cupos) {
        if (nombre_.empty()) throw ErrorDatoInvalido("nombre", "no puede ser vacío");
        if (cupos < 1) throw ErrorDatoInvalido("cupos", "debe ser >= 1");
        verificarInvariante();
    }

    void inscribir(const Estudiante& e) {
        if (static_cast<int>(inscritos_.size()) >= cupos_)
            throw ErrorCupoAgotado(static_cast<int>(inscritos_.size()), cupos_, nombre_);
        if (std::find(inscritos_.begin(), inscritos_.end(), e.codigo()) != inscritos_.end())
            throw ErrorEntidadDuplicada("Inscripción", e.codigo());
        [[maybe_unused]] const auto antes = inscritos_.size();
        inscritos_.push_back(e.codigo());
        assert(inscritos_.size() == antes + 1 && "POST: no creció la lista");
        verificarInvariante();
    }

    int numInscritos() const noexcept { return static_cast<int>(inscritos_.size()); }
    int cuposLibres() const noexcept { return cupos_ - numInscritos(); }
    const std::string& nombre() const noexcept { return nombre_; }
};

// ── Repositorio ──────────────────────────────────────────────────────────
class RepositorioEstudiantes {
    std::unordered_map<int, Estudiante> datos_;

public:
    void agregar(const Estudiante& e) {
        if (datos_.count(e.codigo())) throw ErrorEntidadDuplicada("Estudiante", e.codigo());
        datos_.emplace(e.codigo(), e);
        assert(datos_.count(e.codigo()) == 1 && "POST: no se guardó");
    }

    // Versión con excepción (la búsqueda fallida se trata como error inesperado)
    const Estudiante& obtener(int codigo) const {
        auto it = datos_.find(codigo);
        if (it == datos_.end()) throw ErrorEntidadNoEncontrada("Estudiante", codigo);
        return it->second;
    }

    // Versión refactorizada: "no encontrado" es un fallo ESPERADO -> Result<T>
    Result<Estudiante> buscar(int codigo) const {
        auto it = datos_.find(codigo);
        if (it == datos_.end())
            return Result<Estudiante>::fallo("Estudiante id=" + std::to_string(codigo) + " no encontrado");
        return Result<Estudiante>::ok(it->second);
    }

    int cantidad() const noexcept { return static_cast<int>(datos_.size()); }
};
