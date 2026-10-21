# Diagnóstico del código del marco teórico

Antes de pasar a las actividades se compiló/ejecutó el código del marco teórico tal como aparece en la guía.
Esto es lo que se encontró y cómo quedó corregido en este repositorio.

| # | Dónde | Problema detectado | Corrección |
|---|-------|--------------------|------------|
| 1 | `result.hpp` (2.5) | **No compila con `Result<std::string>`**: los constructores `Result(T)` y `Result(std::string)` quedan con la misma firma y `std::holds_alternative<T>` es ambiguo en `variant<string, string>`. | Se accede al `variant` por **índice** (`in_place_index<0/1>`, `index()`, `std::get<0/1>`). |
| 2 | Uso de `result.hpp` (2.5) | Faltan `#include <unordered_map>` y `#include <iostream>` (`cout`/`cerr` no declarados). | Includes agregados. |
| 3 | `Matricula::diagnostico()` (Act. 2) | `"texto" + (cond ? "A" : "B")` suma dos `const char*` → error de compilación. | Se guarda el estado en un `std::string` antes de concatenar. |
| 4 | `Matricula` C++ (Act. 2) | `agregarCurso` y la verificación de duplicados están como `TODO`; sin ellos la prueba `test_agregar_curso_duplicado` fallaría. Además la invariante pide *fecha no futura* y la clase C++ no tenía fecha. | Implementados `agregarCurso`, `eliminarCurso`, duplicados y `fecha_`. |
| 5 | `Result::map` | Semántica distinta entre lenguajes: en C++ `map` **envuelve** el valor; en Python `map` **encadena** (flatMap). Un `map` C++ con una función que devuelve `Result` produce `Result<Result<T>>`. | `map` (transforma) y `andThen` / `and_then` (encadena) en ambos lenguajes. |
| 6 | f-strings de `result.py` (2.5) | `f"PPA {est["ppa"]}"` reutiliza las mismas comillas: solo válido en Python ≥ 3.12 (en Colab con 3.10/3.11 da `SyntaxError`). | Comillas simples dentro del f-string. |
| 7 | Nombres de archivo | El texto habla de `errores_academicos.py`, pero `Matricula` importa `excepciones`; la Act. 1 usa `excepciones_dominio.hpp` y las instrucciones `errores_academicos.hpp`. | Un solo nombre por lenguaje: `errores_academicos.hpp` / `excepciones.py` (y `errores_dominio.hpp` para el Proyecto Integrador). |
| 8 | `std::to_string(float)` | Mensajes como `Calificación 25.000000 fuera de rango`. | Formateo con `ostringstream` → `25.00`. |
| 9 | Includes de `errores_academicos.hpp` | Usa `std::move` sin `<utility>`. | `<utility>` agregado. |
| 10 | `Result` (C++) | Un `Result` ignorado pierde el error en silencio. | `class [[nodiscard]] Result` → el compilador avisa si se descarta. |

Todo el código C++ compila sin advertencias con `-std=c++17 -Wall -Wextra -Wpedantic`, también con `-DNDEBUG`,
y las pruebas pasan con AddressSanitizer + UBSan.
