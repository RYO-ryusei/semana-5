"""excepciones.py — Jerarquía de errores del Proyecto Integrador (Python 3.12).

Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
"""


class ErrorDominio(Exception):
    """Raíz del sistema. Guarda un diccionario de contexto."""

    def __init__(self, mensaje: str, contexto: dict | None = None):
        super().__init__(mensaje)
        self.contexto: dict = contexto or {}

    def __str__(self) -> str:
        base = super().__str__()
        if self.contexto:
            ctx = ", ".join(f"{k}={v!r}" for k, v in self.contexto.items())
            return f"{base}  <- contexto: {{{ctx}}}"
        return base


class ErrorDatoInvalido(ErrorDominio):
    def __init__(self, campo: str, detalle: str):
        super().__init__(f"Dato inválido en '{campo}': {detalle}", {"campo": campo})
        self.campo = campo


class ErrorEntidadDuplicada(ErrorDominio):
    def __init__(self, tipo: str, identificador):
        super().__init__(f"{tipo} id={identificador} ya existe", {"tipo": tipo, "id": identificador})


class ErrorEntidadNoEncontrada(ErrorDominio):
    def __init__(self, tipo: str, identificador):
        super().__init__(f"{tipo} id={identificador} no encontrado", {"tipo": tipo, "id": identificador})


class ErrorCupoAgotado(ErrorDominio):
    def __init__(self, actual: int, maximo: int, seccion: str):
        super().__init__(
            f"Sección {seccion} sin cupos ({actual}/{maximo})",
            {"actual": actual, "maximo": maximo, "seccion": seccion},
        )
        self.actual, self.maximo = actual, maximo
