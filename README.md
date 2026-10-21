# Semana 5 — Manejo de Excepciones y Diseño por Contrato

**Curso:** Programación Orientada a Objetos II (SIS206) · **UNAP Puno** · 2026-II
**Estudiante:** Kevin Manuel Vilca Paco
**Docente:** Dr. Aldo Hernán Zanabria Gálvez
**Lenguajes:** C++17 y Python 3.12

Jerarquías de excepciones, Diseño por Contrato (precondiciones, poscondiciones, invariantes) y patrón `Result<T>`,
implementados en C++ y Python.

## Estructura

```
cpp/
  include/            errores_academicos.hpp · result.hpp · curso.hpp · matricula.hpp
  marco_teorico/      demo_excepciones · demo_curso · demo_result
  actividad1/         jerarquía + ErrorMatriculaDuplicada
  actividad2/         Matricula con DbC
  actividad3/         pipeline con Result<T>
  actividad4/         tests_matricula.cpp
  actividad5/         Proyecto Integrador (errores_dominio.hpp, modelos.hpp, tests_dominio.cpp)
python/
  excepciones.py · result.py · curso.py · matricula.py
  marco_01..03_*.py · actividad1..3_*.py
  proyecto_integrador/   (Actividad 5)
  tests/                 test_matricula.py · test_dominio.py
informe/                 informe en Word y PDF, diagramas UML y salidas de cada programa
DIAGNOSTICO.md           errores encontrados en el código del marco teórico y su corrección
```

## Ejecutar

**C++** (g++ 12+ o MinGW-w64):

```bash
make run            # compila con -Wall -Wextra y ejecuta todo
```

**Python** (3.12+):

```bash
cd python
pip install -r requirements.txt
python actividad1_jerarquia.py
python actividad2_matricula.py
python actividad3_pipeline.py
python -m proyecto_integrador.demo
python -m pytest tests -v
```

> No compilar con `-DNDEBUG` (ni ejecutar Python con `-O`): desactiva los `assert` de poscondiciones e invariantes.
