"""Actividad 4 — Pruebas de contratos de Matricula (pytest)."""
from datetime import date, timedelta

import pytest

from excepciones import ErrorAcademico, ErrorCapacidad, ErrorValidacion
from matricula import Matricula

HOY = date.today()


def test_constructor_valido():
    m = Matricula("20210500", ["SIS206", "SIS207"], HOY)
    assert m._cursos == ["SIS206", "SIS207"]
    assert not m._anulada


def test_constructor_sin_cursos():
    with pytest.raises(ErrorCapacidad):
        Matricula("20210500", [], HOY)


def test_constructor_demasiados_cursos():
    with pytest.raises(ErrorCapacidad):
        Matricula("20210500", [f"SIS{i}" for i in range(7)], HOY)


def test_constructor_duplicados():
    with pytest.raises(ErrorValidacion):
        Matricula("20210500", ["SIS206", "SIS206"], HOY)


def test_constructor_fecha_futura():
    with pytest.raises(ErrorValidacion):
        Matricula("20210500", ["SIS206"], HOY + timedelta(days=1))


def test_agregar_curso_exitoso():
    m = Matricula("20210500", ["SIS206"], HOY)
    m.agregar_curso("SIS207")
    assert "SIS207" in m._cursos
    assert len(m._cursos) == 2


def test_agregar_curso_duplicado():
    m = Matricula("20210500", ["SIS206"], HOY)
    with pytest.raises(ErrorValidacion):
        m.agregar_curso("SIS206")


def test_eliminar_ultimo_curso_no_permitido():
    m = Matricula("20210500", ["SIS206"], HOY)
    with pytest.raises(ErrorValidacion):
        m.eliminar_curso("SIS206")


def test_anular_y_modificar():
    m = Matricula("20210500", ["SIS206"], HOY)
    m.anular()
    with pytest.raises(ErrorAcademico):
        m.agregar_curso("SIS207")


def test_invariante_directo():
    m = Matricula("20210500", ["SIS206"], HOY)
    m._verificar_invariante()  # no debe lanzar


def test_diagnostico_detecta_invariante_rota():
    m = Matricula("20210500", ["SIS206"], HOY)
    assert m.diagnostico() == "Matrícula [ACTIVA] — 1/6 cursos"
    m._cursos.append("SIS206")  # bug simulado
    assert m.diagnostico().startswith("INVÁLIDA")
