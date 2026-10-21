"""Actividad 5 — Proyecto Integrador: robustez en todas las capas (Python 3.12).

Ejecutar desde la carpeta python/:  python -m proyecto_integrador.demo
"""
from .excepciones import (ErrorCupoAgotado, ErrorDatoInvalido, ErrorDominio,
                          ErrorEntidadDuplicada, ErrorEntidadNoEncontrada)
from .modelos import Estudiante, RepositorioEstudiantes, Seccion


def main() -> None:
    repo = RepositorioEstudiantes()
    repo.agregar(Estudiante(1001, "Juan Mamani", 15.0))
    repo.agregar(Estudiante(1002, "Rosa Quispe", 13.5))
    print(f"Repositorio con {repo.cantidad} estudiantes\n")

    print("--- Result[T] en la búsqueda (fallo esperado) ---")
    for codigo in (1001, 9999):
        r = repo.buscar(codigo)
        print(f"buscar({codigo}) -> " + (r.valor.nombre if r.es_exito else f"FALLO: {r.error}"))

    print("\n--- Excepciones en las precondiciones (error de uso) ---")
    try:
        Estudiante(1003, "Ana", 25.0)
    except ErrorDatoInvalido as e:
        print(f"ErrorDatoInvalido(campo={e.campo}): {e}")
    try:
        repo.agregar(Estudiante(1001, "Otro", 10.0))
    except ErrorEntidadDuplicada as e:
        print(f"ErrorEntidadDuplicada: {e}")
    try:
        repo.obtener(4242)
    except ErrorEntidadNoEncontrada as e:
        print(f"ErrorEntidadNoEncontrada: {e}")

    print("\n--- Seccion con DbC ---")
    sec = Seccion("POO II - A", 2)
    sec.inscribir(repo.obtener(1001))
    sec.inscribir(repo.obtener(1002))
    print(f"Inscritos {sec.num_inscritos}, cupos libres {sec.cupos_libres}")
    try:
        sec.inscribir(Estudiante(1005, "Pedro", 12.0))
    except ErrorCupoAgotado as e:
        print(f"ErrorCupoAgotado: {e}")
    try:
        raise ErrorCupoAgotado(2, 2, "X")
    except ErrorDominio as e:
        print(f"Captura por familia (ErrorDominio): {e}")


if __name__ == "__main__":
    main()
