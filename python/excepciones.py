"""excepciones.py — Jerarquía de excepciones del dominio académico (Python 3.12).

Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
"""


class ErrorAcademico(Exception):
    """Raíz de todas las excepciones del dominio académico."""

    def __init__(self, mensaje: str, contexto: dict | None = None):
        super().__init__(mensaje)
        # contexto: datos adicionales del error (campo, valor, entidad…)
        self.contexto: dict = contexto or {}

    def __str__(self) -> str:
        base = super().__str__()
        if self.contexto:
            ctx = ", ".join(f"{k}={v!r}" for k, v in self.contexto.items())
            return f"{base}  <- contexto: {{{ctx}}}"
        return base


class ErrorValidacion(ErrorAcademico):
    """Datos de entrada que violan las reglas del dominio."""


class ErrorCalificacion(ErrorValidacion):
    """Nota fuera del rango [0, 20] del sistema peruano vigesimal."""

    def __init__(self, valor: float, campo: str = "nota"):
        super().__init__(
            f"Calificación {valor} fuera de [0, 20]",
            contexto={"campo": campo, "valor": valor, "rango": "[0, 20]"},
        )
        self.valor_invalido = valor


class ErrorMatriculaDuplicada(ErrorValidacion):
    """Actividad 1 — el estudiante ya está matriculado."""

    def __init__(self, codigo_estudiante: str):
        super().__init__(
            f"Matrícula duplicada para el estudiante {codigo_estudiante}",
            contexto={"codigo_estudiante": codigo_estudiante},
        )
        self.codigo_estudiante = codigo_estudiante


class ErrorCapacidad(ErrorAcademico):
    """Operación que excede el límite permitido."""

    def __init__(self, actual: int, maximo: int, entidad: str):
        super().__init__(
            f"{entidad}: capacidad agotada ({actual}/{maximo})",
            contexto={"actual": actual, "maximo": maximo, "entidad": entidad},
        )


class ErrorRecursoNoEncontrado(ErrorAcademico):
    """Entidad buscada que no existe en el repositorio."""

    def __init__(self, tipo: str, identificador):
        super().__init__(
            f"{tipo} con id={identificador!r} no encontrado",
            contexto={"tipo": tipo, "id": identificador},
        )
