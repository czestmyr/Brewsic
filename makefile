LDFLAGS  = `sdl-config --libs` -lSDL_ttf -lSDL_image
CXXFLAGS = `sdl-config --cflags` -g -include draw/SDL_draw.h
NAME     = brewsic

OBJECTS = main.o mixer.o generators/sine.o generators/saw.o generators/square.o filters/gate.o filters/adsr.o filters/delay.o synths/triosc.o filters/flanger.o gui/guimgr.o gui/window.o gui/fonts.o gui/background.o gui/button.o gui/style.o gui/keyboard.o
SDL_DRAW = draw/SDL_draw.o

${NAME}: ${OBJECTS} ${SDL_DRAW}
	g++ ${OBJECTS} ${SDL_DRAW} ${LDFLAGS} -o ${NAME}

clean:
	rm -f ${OBJECTS} ${NAME}

%.o: %.cpp %.h
	g++ -c $< -o $@ ${CXXFLAGS}

