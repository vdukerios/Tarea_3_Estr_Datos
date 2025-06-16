# Nombre del ejecutable
EXECUTABLE = tarea3

# Archivos fuente y encabezados
SRC = main.c modules/processor.c modules/validator.c
HEADERS = modules/processor.h modules/validator.h

# Compilador
CC = gcc

# Regla principal: compilar el programa
$(EXECUTABLE): $(SRC) $(HEADERS)
	$(CC) -o $(EXECUTABLE) $(SRC)

# Regla para ejecutar el programa con la variable ARGS
run: $(EXECUTABLE)
	@./$(EXECUTABLE) $(ARGS)

# Regla para mostrar el formato de uso
run_manual:
	@echo "Ejecuta el programa manualmente con:"
	@echo "./$(EXECUTABLE) rutas.txt <ORIGEN> <DESTINO>"

# Regla para limpiar archivos intermedios y el ejecutable
clean:
	@del $(EXECUTABLE) *.o 2>nul || true

.PHONY: run run_manual clean
