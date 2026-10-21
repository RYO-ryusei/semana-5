"""Marco teórico 2.3 — jerarquía de excepciones, captura selectiva y encadenamiento."""
import os
import tempfile

from excepciones import (ErrorAcademico, ErrorCalificacion, ErrorCapacidad,
                         ErrorRecursoNoEncontrado, ErrorValidacion)


def registrar_nota(valor: float, campo: str = "nota") -> None:
    if not (0.0 <= valor <= 20.0):
        raise ErrorCalificacion(valor, campo)


def probar(titulo: str, accion) -> None:
    print(f"--- {titulo} ---")
    try:
        accion()
        print("Sin errores")
    except ErrorCalificacion as e:
        print(f"[except ErrorCalificacion] {e}")
    except ErrorValidacion as e:  # captura si no fue ErrorCalificacion
        print(f"[except ErrorValidacion] {e}")
    except ErrorAcademico as e:
        print(f"[except ErrorAcademico] {e}")


def cargar_nota_desde_archivo(ruta: str, codigo: int) -> float:
    try:
        with open(ruta) as f:
            return float(f.read())
    except (OSError, ValueError) as causa:
        raise ErrorRecursoNoEncontrado("Nota", codigo) from causa  # __cause__ = causa original


def _raise(exc):
    raise exc


if __name__ == "__main__":
    probar("Nota 25 (fuera de rango)", lambda: registrar_nota(25.0, "POO II"))
    probar("Nota 15 (válida)", lambda: registrar_nota(15.0))
    probar("Capacidad agotada", lambda: _raise(ErrorCapacidad(30, 30, "SIS206")))

    print("\n--- Encadenamiento: raise ... from ... ---")
    for ruta, contenido in (("/ruta/que/no/existe.txt", None), ("nota_mala.txt", "abc")):
        if contenido is not None:
            ruta = os.path.join(tempfile.gettempdir(), ruta)
            with open(ruta, "w") as f:
                f.write(contenido)
        try:
            cargar_nota_desde_archivo(ruta, 1001)
        except ErrorRecursoNoEncontrado as e:
            print(f"Error de dominio : {e}")
            print(f"Causa original   : {type(e.__cause__).__name__}: {e.__cause__}")
