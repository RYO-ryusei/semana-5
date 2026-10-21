// result.hpp — Patrón Result<T>: el error viaja como valor (C++17, std::variant)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#pragma once
#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

// Se accede al variant por ÍNDICE (0 = éxito, 1 = fallo) y no por tipo.
// Así Result<std::string> funciona: con holds_alternative<T> el variant
// <std::string, std::string> sería ambiguo (error del código original).
template <typename T>
class [[nodiscard]] Result {
    std::variant<T, std::string> dato_;

    Result(std::in_place_index_t<0> i, T v) : dato_(i, std::move(v)) {}
    Result(std::in_place_index_t<1> i, std::string e) : dato_(i, std::move(e)) {}

public:
    // ── Fábricas ──────────────────────────────────────────────────────────
    static Result ok(T valor) { return Result(std::in_place_index<0>, std::move(valor)); }
    static Result fallo(std::string e) { return Result(std::in_place_index<1>, std::move(e)); }

    // ── Consulta ──────────────────────────────────────────────────────────
    bool esExito() const noexcept { return dato_.index() == 0; }

    const T& valor() const {
        if (!esExito()) throw std::logic_error("Result fallido: " + std::get<1>(dato_));
        return std::get<0>(dato_);
    }
    const std::string& error() const {
        if (esExito()) throw std::logic_error("Result exitoso: no tiene error");
        return std::get<1>(dato_);
    }
    T valorOr(T alternativo) const { return esExito() ? std::get<0>(dato_) : alternativo; }

    // ── map: transforma el valor (f devuelve un valor normal U) ───────────
    template <typename F>
    auto map(F f) const -> Result<std::invoke_result_t<F, const T&>> {
        using U = std::invoke_result_t<F, const T&>;
        if (esExito()) return Result<U>::ok(f(std::get<0>(dato_)));
        return Result<U>::fallo(std::get<1>(dato_));
    }

    // ── andThen: encadena pasos que ya devuelven Result (flatMap) ─────────
    template <typename F>
    auto andThen(F f) const -> std::invoke_result_t<F, const T&> {
        using R = std::invoke_result_t<F, const T&>;
        if (esExito()) return f(std::get<0>(dato_));
        return R::fallo(std::get<1>(dato_));
    }
};
