"""result.py — Patrón Result[T]: el error viaja como valor (Python 3.12).

Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
"""
from collections.abc import Callable
from dataclasses import dataclass
from typing import Generic, TypeVar

T = TypeVar("T")
U = TypeVar("U")


@dataclass(frozen=True)
class Result(Generic[T]):
    _valor: T | None = None
    _error: str | None = None

    # ── Fábricas ──────────────────────────────────────────────────────────
    @staticmethod
    def ok(valor: T) -> "Result[T]":
        return Result(_valor=valor)

    @staticmethod
    def fallo(e: str) -> "Result":
        return Result(_error=e)

    # ── Consulta ──────────────────────────────────────────────────────────
    @property
    def es_exito(self) -> bool:
        return self._error is None

    @property
    def valor(self) -> T:
        if not self.es_exito:
            raise RuntimeError(f"Result fallido: {self._error}")
        return self._valor  # type: ignore[return-value]

    @property
    def error(self) -> str:
        return self._error or ""

    def valor_or(self, alternativo: T) -> T:
        return self._valor if self.es_exito else alternativo  # type: ignore[return-value]

    # ── map: transforma el valor (f devuelve un valor normal) ─────────────
    def map(self, f: Callable[[T], U]) -> "Result[U]":
        return Result.ok(f(self._valor)) if self.es_exito else Result.fallo(self._error)  # type: ignore[arg-type]

    # ── and_then: encadena pasos que ya devuelven Result (flatMap) ────────
    def and_then(self, f: Callable[[T], "Result[U]"]) -> "Result[U]":
        return f(self._valor) if self.es_exito else Result.fallo(self._error)  # type: ignore[arg-type]
