"""Marco teórico 2.4 — Diseño por Contrato con la clase Curso."""
from curso import Curso
from excepciones import ErrorCapacidad, ErrorValidacion

if __name__ == "__main__":
    poo = Curso("SIS206", 4, 2)
    print(f"Curso {poo.codigo} creado: cupos libres = {poo.cupos_libres}")
    poo.inscribir()
    poo.inscribir()
    print(f"Inscritos: {poo.inscritos}")
    print("Diagnóstico:", poo.diagnostico())

    try:
        poo.inscribir()  # precondición violada
    except ErrorCapacidad as e:
        print(f"Precondición: {e}")

    for args in (("SIS999", 9, 10), ("", 3, 10), ("SIS500", 3, 0)):
        try:
            Curso(*args)
        except ErrorValidacion as e:
            print(f"Constructor{args}: {e}")

    # Invariante rota "a la fuerza" (simula un bug interno): diagnostico() lo detecta
    poo._inscritos = 99
    print("Tras corromper el estado:", poo.diagnostico())
