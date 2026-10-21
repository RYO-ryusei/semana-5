"""Actividad 1 (Parte B) — Jerarquía de excepciones en Python 3.12 (equivalente a C++)."""
from excepciones import (ErrorAcademico, ErrorCalificacion, ErrorCapacidad,
                         ErrorMatriculaDuplicada, ErrorRecursoNoEncontrado, ErrorValidacion)


def validar_matricula(codigo: int, matriculados: list[int]) -> None:
    """Lanza ErrorMatriculaDuplicada si el código ya está; si no, lo registra."""
    if codigo in matriculados:
        raise ErrorMatriculaDuplicada(str(codigo))
    matriculados.append(codigo)


def intentar(codigo: int, matriculados: list[int]) -> None:
    print(f"Matriculando código {codigo} ... ", end="")
    try:
        validar_matricula(codigo, matriculados)
        print("OK")
    except ErrorMatriculaDuplicada as e:  # 1.° el más específico
        print("DUPLICADA")
        print(f"  except ErrorMatriculaDuplicada -> codigo_estudiante={e.codigo_estudiante}")
        print(f"  {e}")
    except ErrorValidacion as e:
        print(f"\n  except ErrorValidacion: {e}")
    except ErrorAcademico as e:
        print(f"\n  except ErrorAcademico: {e}")


def buscar_nota(codigo: int, notas: dict[int, float]) -> float:
    """Encadena la causa original (KeyError) en una excepción de dominio."""
    try:
        return notas[codigo]
    except KeyError as causa:
        raise ErrorRecursoNoEncontrado("Nota", codigo) from causa


if __name__ == "__main__":
    print("=== Parte B: ErrorMatriculaDuplicada ===")
    matriculados = [20210500, 20210501]
    intentar(20210502, matriculados)
    intentar(20210500, matriculados)

    print("\n=== Captura por familia ===")
    try:
        raise ErrorCalificacion(-3.5, "POO II")
    except ErrorValidacion as e:  # ErrorCalificacion ES un ErrorValidacion
        print(f"ErrorValidacion atrapó una ErrorCalificacion: {e}")

    print("\n=== Contexto rico del error (diccionario) ===")
    try:
        raise ErrorCapacidad(6, 6, "matrícula")
    except ErrorAcademico as e:
        print("contexto =", e.contexto)

    print("\n=== raise ... from ...: preserva la causa original ===")
    try:
        buscar_nota(9999, {1001: 15.0})
    except ErrorRecursoNoEncontrado as e:
        print("Error de dominio:", e)
        print("__cause__       :", repr(e.__cause__))
