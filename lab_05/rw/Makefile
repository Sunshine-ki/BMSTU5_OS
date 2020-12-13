CFLAGS := -std=c99 -lpthread -lrt
FILE_O := $(subst .c,.o,$(wildcard src/*.c))
HEADERS_DIR := headers/

app.exe:  main.o $(FILE_O)
	gcc -o app.exe $^  $(CFLAGS)
	rm *.o src/*.o

%.o: %.c
	gcc -c -I $(HEADERS_DIR) $^ $(CFLAGS)

src/%.o: src/%.c
	gcc -o $@ -c -I $(HEADERS_DIR) $^ $(CFLAGS)


clean:
	rm -f *.o *.exe src/*.o

run:
	./app.exe
