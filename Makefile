# Compilar y ejecutar todo el proyecto C++ y Python (Linux / MinGW con make)
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Icpp/include -Icpp/actividad5
BIN      := build

PROGS := marco_excepciones marco_curso marco_result act1 act2 act3 act4_tests act5 act5_tests

all: $(addprefix $(BIN)/,$(PROGS))

$(BIN):
	mkdir -p $(BIN)

$(BIN)/marco_excepciones: cpp/marco_teorico/demo_excepciones.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BIN)/marco_curso: cpp/marco_teorico/demo_curso.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BIN)/marco_result: cpp/marco_teorico/demo_result.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BIN)/act1: cpp/actividad1/main.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BIN)/act2: cpp/actividad2/main.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BIN)/act3: cpp/actividad3/main.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BIN)/act4_tests: cpp/actividad4/tests_matricula.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BIN)/act5: cpp/actividad5/main.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BIN)/act5_tests: cpp/actividad5/tests_dominio.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	@for p in $(PROGS); do echo "\n===== $$p ====="; ./$(BIN)/$$p; done

test-py:
	cd python && python -m pytest tests -v

clean:
	rm -rf $(BIN)
