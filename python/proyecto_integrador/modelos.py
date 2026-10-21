"""modelos.py — Clases del Proyecto Integrador con DbC y Result (Python 3.12).

Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
"""
from result import Result

from .excepciones import (ErrorCupoAgotado, ErrorDatoInvalido, ErrorEntidadDuplicada,
                          ErrorEntidadNoEncontrada)


class Estudiante:
    """INVARIANTE: codigo > 0, nombre no vacío, 0 <= ppa <= 20."""

    def __init__(self, codigo: int, nombre: str, ppa: float):
        if codigo <= 0:
            raise ErrorDatoInvalido("codigo", "debe ser positivo")
        if not nombre.strip():
            raise ErrorDatoInvalido("nombre", "no puede ser vacío")
        if not (0.0 <= ppa <= 20.0):
            raise ErrorDatoInvalido("ppa", "debe estar en [0, 20]")
        self._codigo, self._nombre, self._ppa = codigo, nombre.strip(), float(ppa)
        self._verificar_invariante()

    def actualizar_ppa(self, nuevo: float) -> None:
        if not (0.0 <= nuevo <= 20.0):
            raise ErrorDatoInvalido("ppa", "debe estar en [0, 20]")
        self._ppa = float(nuevo)
        assert self._ppa == nuevo, "POST: PPA no actualizado"
        self._verificar_invariante()

    def _verificar_invariante(self) -> None:
        assert self._codigo > 0, "INV: código no positivo"
        assert self._nombre, "INV: nombre vacío"
        assert 0.0 <= self._ppa <= 20.0, "INV: PPA fuera de [0,20]"

    codigo = property(lambda self: self._codigo)
    nombre = property(lambda self: self._nombre)
    ppa = property(lambda self: self._ppa)


class Seccion:
    """INVARIANTE: nombre no vacío, cupos >= 1, 0 <= inscritos <= cupos, sin repetidos."""

    def __init__(self, nombre: str, cupos: int):
        if not nombre.strip():
            raise ErrorDatoInvalido("nombre", "no puede ser vacío")
        if cupos < 1:
            raise ErrorDatoInvalido("cupos", "debe ser >= 1")
        self._nombre, self._cupos, self._inscritos = nombre.strip(), cupos, []
        self._verificar_invariante()

    def inscribir(self, est: Estudiante) -> None:
        if len(self._inscritos) >= self._cupos:
            raise ErrorCupoAgotado(len(self._inscritos), self._cupos, self._nombre)
        if est.codigo in self._inscritos:
            raise ErrorEntidadDuplicada("Inscripción", est.codigo)
        antes = len(self._inscritos)
        self._inscritos.append(est.codigo)
        assert len(self._inscritos) == antes + 1, "POST: no creció la lista"
        self._verificar_invariante()

    def _verificar_invariante(self) -> None:
        assert self._nombre, "INV: nombre vacío"
        assert self._cupos >= 1, "INV: cupos < 1"
        assert len(self._inscritos) <= self._cupos, "INV: inscritos > cupos"
        assert len(set(self._inscritos)) == len(self._inscritos), "INV: repetidos"

    @property
    def num_inscritos(self) -> int:
        return len(self._inscritos)

    @property
    def cupos_libres(self) -> int:
        return self._cupos - len(self._inscritos)


class RepositorioEstudiantes:
    def __init__(self):
        self._datos: dict[int, Estudiante] = {}

    def agregar(self, est: Estudiante) -> None:
        if est.codigo in self._datos:
            raise ErrorEntidadDuplicada("Estudiante", est.codigo)
        self._datos[est.codigo] = est
        assert est.codigo in self._datos, "POST: no se guardó"

    def obtener(self, codigo: int) -> Estudiante:
        """Versión con excepción."""
        if codigo not in self._datos:
            raise ErrorEntidadNoEncontrada("Estudiante", codigo)
        return self._datos[codigo]

    def buscar(self, codigo: int) -> Result[Estudiante]:
        """Versión refactorizada: 'no encontrado' es un fallo ESPERADO -> Result."""
        est = self._datos.get(codigo)
        return Result.ok(est) if est else Result.fallo(f"Estudiante id={codigo} no encontrado")

    @property
    def cantidad(self) -> int:
        return len(self._datos)
