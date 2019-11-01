EXECUTABLES=ProdConsInit Productor Consumidor ProdConsFin
LDFLAGS=-pthread -lrt
CPPFLAGS=-g

.phony: clean


all: $(EXECUTABLES)

clean:
	rm -f *.*~ *~
	rm -f $(EXECUTABLES)
