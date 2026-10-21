"""Actividad 3 — Patrón Result: pipeline buscar -> validar -> inscribir (Python 3.12)."""
from dataclasses import dataclass, field

from result import Result

PPA_MINIMO = 12.0


@dataclass
class Estudiante:
    codigo: int
    nombre: str
    ppa: float
    cursos: list[str] = field(default_factory=list)


BD = {
    1001: Estudiante(1001, "Juan Mamani", 15.0),
    1002: Estudiante(1002, "Rosa Quispe", 10.5),
    1003: Estudiante(1003, "Luis Condori", 14.0, ["SIS206"]),
}


def buscar_estudiante(codigo: int) -> Result[Estudiante]:
    est = BD.get(codigo)
    return Result.ok(est) if est else Result.fallo(f"Código {codigo} no existe")


def validar_ppa_minimo(est: Estudiante) -> Result[Estudiante]:
    if est.ppa < PPA_MINIMO:
        return Result.fallo(f"PPA {est.ppa} < {PPA_MINIMO} (mínimo para inscribirse)")
    return Result.ok(est)


def inscribir(est: Estudiante, curso: str) -> Result[str]:
    if curso in est.cursos:
        return Result.fallo(f"{est.nombre} ya está inscrito en {curso}")
    return Result.ok(f"{est.nombre} inscrito en {curso}")


if __name__ == "__main__":
    print("Pipeline: buscar_estudiante -> validar_ppa_minimo -> inscribir(SIS206)\n")
    for codigo in (1001, 9999, 1002, 1003):
        resultado = (buscar_estudiante(codigo)
                     .and_then(validar_ppa_minimo)
                     .and_then(lambda e: inscribir(e, "SIS206")))
        etiqueta = f"EXITO: {resultado.valor}" if resultado.es_exito else f"FALLO: {resultado.error}"
        print(f"codigo={codigo} -> {etiqueta}")

    print("\nmap() sobre un resultado exitoso:")
    print("  nombre en mayúsculas =", buscar_estudiante(1001).map(lambda e: e.nombre).map(str.upper).valor)

    print("map() sobre un fallo (la función NO se ejecuta):")
    llamadas = []
    fallido = buscar_estudiante(9999).map(lambda e: llamadas.append(1) or e.nombre)
    print(f"  llamadas a la función = {len(llamadas)}, error propagado = {fallido.error}")

    # Resultado es inmutable (frozen=True)
    try:
        fallido._error = "otro"  # type: ignore[misc]
    except Exception as e:
        print(f"\nResult es inmutable: {type(e).__name__}")
