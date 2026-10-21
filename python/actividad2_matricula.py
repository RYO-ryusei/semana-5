"""Actividad 2 — Diseño por Contrato: clase Matricula (Python 3.12)."""
from datetime import date, timedelta

from excepciones import ErrorAcademico, ErrorCapacidad, ErrorValidacion
from matricula import Matricula


def caso(titulo: str, accion) -> None:
    print(f"- {titulo}: ", end="")
    try:
        accion()
        print("OK")
    except (ErrorCapacidad, ErrorValidacion, ErrorAcademico) as e:
        print(f"{type(e).__name__} -> {e}")


if __name__ == "__main__":
    print("=== Constructor (precondiciones) ===")
    caso("matrícula válida", lambda: Matricula("20210500", ["SIS206", "SIS207"]))
    caso("código vacío", lambda: Matricula("", ["SIS206"]))
    caso("sin cursos", lambda: Matricula("20210500", []))
    caso("7 cursos", lambda: Matricula("20210500", list("ABCDEFG")))
    caso("cursos duplicados", lambda: Matricula("20210500", ["SIS206", "SIS206"]))
    caso("fecha futura", lambda: Matricula("20210500", ["SIS206"], date.today() + timedelta(days=2)))

    print("\n=== Mutadores (pre + pos + invariante) ===")
    m = Matricula("20210500", ["SIS206"])
    print(m.diagnostico())
    caso("agregar SIS207", lambda: m.agregar_curso("SIS207"))
    caso("agregar SIS207 otra vez", lambda: m.agregar_curso("SIS207"))
    caso("eliminar SIS207", lambda: m.eliminar_curso("SIS207"))
    caso("eliminar el último curso", lambda: m.eliminar_curso("SIS206"))
    for c in ("SIS208", "SIS209", "SIS210", "SIS211", "SIS212"):
        m.agregar_curso(c)
    print(m.diagnostico())
    caso("agregar un 7.° curso", lambda: m.agregar_curso("SIS213"))

    print("\n=== Anulación ===")
    m.anular()
    print(m.diagnostico())
    caso("agregar con matrícula anulada", lambda: m.agregar_curso("SIS213"))
    caso("anular por segunda vez", m.anular)

    print("\n=== diagnostico() detecta una invariante rota ===")
    m2 = Matricula("20210501", ["SIS206"])
    m2._cursos.append("SIS206")  # bug simulado: duplicado saltándose la API
    print(m2.diagnostico())
