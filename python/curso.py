"""curso.py — Diseño por Contrato con la clase Curso (Python 3.12).

Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
"""
from excepciones import ErrorCapacidad, ErrorValidacion


class Curso:
    """
    INVARIANTE:
        0 <= _inscritos <= _cupos_max
        _creditos in range(1, 7)
        len(_codigo) > 0
    """

    def __init__(self, codigo: str, creditos: int, cupos_max: int):
        # ── PRECONDICIONES ────────────────────────────────────────────────
        if not codigo.strip():
            raise ErrorValidacion("Código del curso no puede ser vacío")
        if creditos not in range(1, 7):
            raise ErrorValidacion(f"Créditos {creditos} fuera de [1, 6]")
        if cupos_max < 1:
            raise ErrorValidacion("cupos_max debe ser >= 1")

        self._codigo = codigo.strip()
        self._creditos = creditos
        self._cupos_max = cupos_max
        self._inscritos = 0

        self._verificar_invariante()  # POST-constructor

    def inscribir(self) -> None:
        # PRECONDICIÓN
        if self._inscritos >= self._cupos_max:
            raise ErrorCapacidad(self._inscritos, self._cupos_max, self._codigo)

        self._inscritos += 1

        # POSCONDICIÓN
        assert self._inscritos <= self._cupos_max, "POST: invariante rota"
        self._verificar_invariante()

    def _verificar_invariante(self) -> None:
        """Se llama al final de toda mutación."""
        assert len(self._codigo) > 0, "INV: código vacío"
        assert 1 <= self._creditos <= 6, "INV: créditos"
        assert 0 <= self._inscritos <= self._cupos_max, "INV: inscritos"

    @property
    def codigo(self) -> str:
        return self._codigo

    @property
    def cupos_libres(self) -> int:
        return self._cupos_max - self._inscritos

    @property
    def inscritos(self) -> int:
        return self._inscritos

    def diagnostico(self) -> str:
        """Informe de estado útil para depuración."""
        try:
            self._verificar_invariante()
            return f"{self._codigo}: OK — {self._inscritos}/{self._cupos_max}"
        except AssertionError as e:
            return f"{self._codigo}: INVÁLIDO — {e}"
