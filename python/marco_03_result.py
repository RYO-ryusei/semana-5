"""Marco teórico 2.5 — patrón Result con pipeline encadenado."""
from result import Result

_db: dict[int, dict] = {1001: {"nombre": "Juan Mamani", "ppa": 15.0},
                        1002: {"nombre": "Rosa Quispe", "ppa": 12.0}}


def buscar_estudiante(codigo: int) -> Result[dict]:
    est = _db.get(codigo)
    return Result.ok(est) if est else Result.fallo(f"Código {codigo} no existe")


def validar_ppa_beca(est: dict) -> Result[dict]:
    if est["ppa"] < 14.0:
        return Result.fallo(f"PPA {est['ppa']} < 14.0 (mínimo para beca)")
    return Result.ok(est)


def asignar_beca(est: dict) -> Result[str]:
    return Result.ok(f"Beca asignada: {est['nombre']}")


if __name__ == "__main__":
    for codigo in (1001, 1002, 9999):
        resultado = (buscar_estudiante(codigo)
                     .and_then(validar_ppa_beca)
                     .and_then(asignar_beca))
        if resultado.es_exito:
            print(f"codigo={codigo} -> {resultado.valor}")
        else:
            print(f"codigo={codigo} -> No se pudo asignar beca: {resultado.error}")

    print("\nmap() transforma un valor; and_then() encadena pasos que devuelven Result:")
    print("  map      ->", buscar_estudiante(1001).map(lambda e: e["nombre"].upper()).valor)
    print("  map fallo->", buscar_estudiante(9999).map(lambda e: e["nombre"]).error)
    try:
        buscar_estudiante(9999).valor
    except RuntimeError as e:
        print("  valor sobre fallo ->", e)
