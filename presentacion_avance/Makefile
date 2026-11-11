# Makefile - CB-CTT (primera entrega)
# Uso rapido:
#   make             compila el programa
#   make test        ejecuta la primera instancia de instancias/
#   make test-all    ejecuta todas las instancias y muestra una tabla resumen
#   make clean       borra lo compilado
# La semilla del azar se puede cambiar: make test-all SEMILLA=7

CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Wall -Wextra -pedantic
SRC_DIR  := src
BUILD    := build
TARGET   := cbctt
INST_DIR := instancias
SOL_DIR  := soluciones
SEMILLA  ?= 1

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# -MMD -MP generan archivos .d para que un cambio en un .h recompile lo necesario
$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Prueba rapida con la primera instancia encontrada en instancias/
test: $(TARGET)
	@mkdir -p $(SOL_DIR)
	@file=$$(ls $(INST_DIR)/*.ctt 2>/dev/null | head -n 1); \
	if [ -n "$$file" ]; then \
		name=$$(basename "$$file" .ctt); \
		echo "=== Ejecutando prueba con $$file (semilla $(SEMILLA)) ==="; \
		./$(TARGET) "$$file" "$(SOL_DIR)/$$name.txt" $(SEMILLA) || exit 1; \
		head -n 8 "$(SOL_DIR)/$$name.txt"; \
		echo "..."; \
		echo "Salida completa en $(SOL_DIR)/$$name.txt"; \
	else \
		echo "Error: no se encontraron archivos .ctt en la carpeta $(INST_DIR)/"; \
		exit 1; \
	fi

# Ejecuta todas las instancias, guarda cada salida en soluciones/ y resume los resultados
test-all: $(TARGET)
	@mkdir -p $(SOL_DIR)
	@echo "================================================================================"
	@printf "%-12s | %-9s | %-14s | %-12s | %-11s\n" "Instancia" "Factible" "Infactibilidad" "Penalizacion" "Tiempo (s)"
	@echo "--------------------------------------------------------------------------------"
	@total=0; feas=0; fallas=0; \
	for f in $(INST_DIR)/*.ctt; do \
		[ -f "$$f" ] || continue; \
		total=$$((total + 1)); \
		name=$$(basename "$$f" .ctt); \
		out="$(SOL_DIR)/$$name.txt"; \
		if ! ./$(TARGET) "$$f" "$$out" $(SEMILLA); then \
			echo "$$name: ERROR al ejecutar"; fallas=$$((fallas + 1)); continue; \
		fi; \
		t_s=$$(sed -n 1p "$$out" | tr -d '\r'); \
		inf=$$(sed -n 2p "$$out" | tr -d '\r' | awk '{print $$1}'); \
		pen=$$(sed -n 2p "$$out" | tr -d '\r' | awk '{print $$2}'); \
		if [ "$$inf" = "0" ]; then feas=$$((feas + 1)); fact="SI"; else fact="NO"; fi; \
		printf "%-12s | %-9s | %-14s | %-12s | %-11s\n" "$$name" "$$fact" "$$inf" "$$pen" "$$t_s"; \
	done; \
	echo "================================================================================"; \
	echo "Resumen: $$total instancias ejecutadas, $$feas factibles, $$fallas con error (semilla $(SEMILLA))."

clean:
	rm -rf $(BUILD) $(TARGET) $(TARGET).exe

.PHONY: all test test-all clean

-include $(OBJECTS:.o=.d)
