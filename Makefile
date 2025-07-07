CC = gcc
CFLAGS = -Wall -g
SOURCES = ABB.c analisis_lexico.c analisis_sintactico.c errores.c sistema_entrada.c TS.c main.c
HEADERS = ABB.h analisis_lexico.h analisis_sintactico.h errores.h sistema_entrada.h TS.h
OBJECTS = $(SOURCES:.c=.o)

# Regra para compilar o programa
all: programa

programa: $(OBJECTS)
	$(CC) $(OBJECTS) -o programa

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJECTS) programa
