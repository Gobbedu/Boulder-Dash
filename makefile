# =======================================================
#   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves
#   GRR20203892
#   Makefile do programa BOULDER DASH
#  ======================================================= 


# compilacao de acordo com a documentacao do allegro 5
CFLAGS = -Wall -ansi -g -std=c99 
LDLIBS = $$(pkg-config allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)
# nome do arquivo compilado
NAME = boulder


# codigo
CODED = main.c utils.c player.c grid.c states.c
HEADERS = structure.h 
#objetos gerados
objects = main.o utils.o player.o grid.o states.o


# REGRAS DE COMPILACAO
all: main

main: $(objects) 
	gcc -o $(NAME) $(objects) $(LDLIBS)

main.o: $(CODED)
	gcc $(CFLAGS) -c $(CODED) 

clean:
	rm $(objects) 

purge: clean
	rm $(NAME)
