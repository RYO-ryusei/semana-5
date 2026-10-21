// Marco teórico 2.2 — captura selectiva en la jerarquía de excepciones (C++17)
// Autor: Kevin Manuel Vilca Paco — SIS206 POO II — UNAP Puno — 2026-II
#include <iostream>
#include "errores_academicos.hpp"

void registrar_nota(float cal) {
    if (cal < 0.0f || cal > 20.0f) throw ErrorCalificacion(cal);
}

// Captura de más específico a más general; devuelve qué rama atrapó el error.
void probar(const char* titulo, void (*accion)()) {
    std::cout << "--- " << titulo << " ---\n";
    try {
        accion();
        std::cout << "Sin errores\n";
    } catch (const ErrorCalificacion& e) {  // 1.° más específico
        std::cout << "[catch ErrorCalificacion] valor=" << e.valor() << "\n  " << e.what() << "\n";
    } catch (const ErrorValidacion& e) {  // 2.° rama validación
        std::cout << "[catch ErrorValidacion] " << e.what() << "\n";
    } catch (const ErrorAcademico& e) {  // 3.° cualquier error de dominio
        std::cout << "[catch ErrorAcademico] " << e.what() << "\n";
    } catch (const std::exception& e) {  // 4.° cualquier error estándar
        std::cout << "[catch std::exception] " << e.what() << "\n";
    }
    // No hay catch (...): es mejor un fallo visible que un silencio.
}

int main() {
    probar("Nota 25 (fuera de rango)", [] { registrar_nota(25.0f); });
    probar("Nota 15 (válida)", [] { registrar_nota(15.0f); });
    probar("Capacidad agotada", [] { throw ErrorCapacidad(30, 30, "SIS206"); });
    probar("Recurso inexistente", [] { throw ErrorRecursoNoEncontrado("Estudiante", 9999); });
    probar("Error estándar", [] { throw std::out_of_range("índice fuera de rango"); });
    return 0;
}
