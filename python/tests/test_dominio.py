"""Actividad 5 — Pruebas de contratos del Proyecto Integrador (pytest)."""
import pytest

from proyecto_integrador.excepciones import (ErrorCupoAgotado, ErrorDatoInvalido, ErrorDominio,
                                             ErrorEntidadDuplicada)
from proyecto_integrador.modelos import Estudiante, RepositorioEstudiantes, Seccion


# ── 3 pruebas VÁLIDAS ────────────────────────────────────────────────────
def test_estudiante_valido():
    e = Estudiante(1001, "Juan Mamani", 15.0)
    assert e.codigo == 1001 and e.ppa == 15.0
    e.actualizar_ppa(16.5)
    assert e.ppa == 16.5


def test_seccion_inscribe():
    s = Seccion("POO II - A", 2)
    s.inscribir(Estudiante(1, "A", 10))
    assert s.num_inscritos == 1 and s.cupos_libres == 1


def test_buscar_result():
    r = RepositorioEstudiantes()
    r.agregar(Estudiante(1001, "Juan Mamani", 15.0))
    assert r.buscar(1001).es_exito
    assert r.buscar(1001).valor.nombre == "Juan Mamani"
    assert not r.buscar(9999).es_exito


# ── 3 pruebas que DEBEN LANZAR ───────────────────────────────────────────
def test_ppa_fuera_de_rango():
    with pytest.raises(ErrorDatoInvalido) as info:
        Estudiante(1, "A", 25.0)
    assert info.value.campo == "ppa"


def test_cupo_agotado():
    s = Seccion("S", 1)
    s.inscribir(Estudiante(1, "A", 10))
    with pytest.raises(ErrorCupoAgotado):
        s.inscribir(Estudiante(2, "B", 10))


def test_repositorio_duplicado():
    r = RepositorioEstudiantes()
    r.agregar(Estudiante(1, "A", 10))
    with pytest.raises(ErrorEntidadDuplicada):
        r.agregar(Estudiante(1, "B", 11))


# ── Extras ───────────────────────────────────────────────────────────────
def test_obtener_no_encontrado_por_familia():
    with pytest.raises(ErrorDominio):
        RepositorioEstudiantes().obtener(77)


def test_inscripcion_repetida():
    s = Seccion("S", 3)
    e = Estudiante(1, "A", 10)
    s.inscribir(e)
    with pytest.raises(ErrorEntidadDuplicada):
        s.inscribir(e)
