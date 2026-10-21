"""matricula.py — Clase Matricula con Diseño por Contrato completo (Actividad 2).

Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
"""
from datetime import date

from excepciones import ErrorAcademico, ErrorCapacidad, ErrorValidacion


class Matricula:
    """
    INVARIANTE:
        _codigo_estudiante no vacío
        1 <= len(_cursos) <= MAX_CURSOS
        len(set(_cursos)) == len(_cursos)   # sin duplicados
        _fecha <= date.today()              # no en el futuro
        _anulada  =>  _cursos ya no se modifica
    """

    MAX_CURSOS = 6

    def __init__(self, codigo_est: str, cursos: list[str], fecha: date | None = None):
        fecha = fecha or date.today()
        # ── PRECONDICIONES ────────────────────────────────────────────────
        if not codigo_est.strip():
            raise ErrorValidacion("Código de estudiante vacío")
        if not (1 <= len(cursos) <= self.MAX_CURSOS):
            raise ErrorCapacidad(len(cursos), self.MAX_CURSOS, "matrícula")
        if len(set(cursos)) != len(cursos):
            raise ErrorValidacion("Cursos duplicados en la matrícula")
        if fecha > date.today():
            raise ErrorValidacion(f"Fecha futura no permitida: {fecha}")

        self._codigo_estudiante = codigo_est.strip()
        self._cursos = list(cursos)
        self._fecha = fecha
        self._anulada = False
        self._verificar_invariante()  # POST-constructor

    def agregar_curso(self, curso: str) -> None:
        # ── PRECONDICIONES ────────────────────────────────────────────────
        if self._anulada:
            raise ErrorAcademico("Matrícula anulada — no se puede modificar")
        if not curso.strip():
            raise ErrorValidacion("Código de curso vacío")
        if len(self._cursos) >= self.MAX_CURSOS:
            raise ErrorCapacidad(len(self._cursos), self.MAX_CURSOS, "matrícula")
        if curso in self._cursos:
            raise ErrorValidacion(f"Curso {curso!r} ya está en la matrícula")

        antes = len(self._cursos)
        self._cursos.append(curso)

        # ── POSCONDICIONES ────────────────────────────────────────────────
        assert curso in self._cursos, "POST: curso no agregado"
        assert len(self._cursos) == antes + 1, "POST: el tamaño debe crecer en 1"
        assert len(self._cursos) <= self.MAX_CURSOS, "POST: excede máximo"
        self._verificar_invariante()

    def eliminar_curso(self, curso: str) -> None:
        # ── PRECONDICIONES ────────────────────────────────────────────────
        if self._anulada:
            raise ErrorAcademico("Matrícula anulada — no se puede modificar")
        if curso not in self._cursos:
            raise ErrorValidacion(f"Curso {curso!r} no está en la matrícula")
        if len(self._cursos) == 1:
            raise ErrorValidacion("La matrícula no puede quedar sin cursos (mínimo 1)")

        antes = len(self._cursos)
        self._cursos.remove(curso)

        # ── POSCONDICIONES ────────────────────────────────────────────────
        assert curso not in self._cursos, "POST: curso no eliminado"
        assert len(self._cursos) == antes - 1, "POST: el tamaño debe bajar en 1"
        self._verificar_invariante()

    def anular(self) -> None:
        if self._anulada:
            raise ErrorAcademico("Matrícula ya está anulada")
        self._anulada = True

    def _verificar_invariante(self) -> None:
        assert len(self._codigo_estudiante) > 0, "INV: código vacío"
        assert 1 <= len(self._cursos) <= self.MAX_CURSOS, "INV: cursos"
        assert len(set(self._cursos)) == len(self._cursos), "INV: duplicados"
        assert self._fecha <= date.today(), "INV: fecha futura"

    def diagnostico(self) -> str:
        try:
            self._verificar_invariante()
            estado = "ANULADA" if self._anulada else "ACTIVA"
            return f"Matrícula [{estado}] — {len(self._cursos)}/{self.MAX_CURSOS} cursos"
        except AssertionError as e:
            return f"INVÁLIDA: {e}"

    # Propiedades de solo lectura
    @property
    def anulada(self) -> bool:
        return self._anulada

    @property
    def cursos(self) -> tuple[str, ...]:
        return tuple(self._cursos)

    @property
    def num_cursos(self) -> int:
        return len(self._cursos)
