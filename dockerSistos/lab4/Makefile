CC      = gcc
CFLAGS  = -Wall -O0

TARGETS = ej2 ej3 ej4

all: $(TARGETS)

ej2: ej2_memoria_virtual.c
	$(CC) $(CFLAGS) -o $@ $<

ej3: ej3_paginacion.c
	$(CC) $(CFLAGS) -o $@ $<

ej4: ej4_page_faults.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
