exe:	main.cpp
	g++ *.cpp src/*.cpp src/*.c `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -I/usr/local/include -L/usr/local/lib -w

mysql:
	g++ *.cpp src/*.cpp src/*.c `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -I/usr/include/mysql /usr/lib/mysql/libmysqlclient.so -I/usr/local/include -L/usr/local/lib -w

newmysql:
	g++ *.cpp src/*.cpp src/*.c `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -Iincludes/mysql -Iincludes/mysql/cppconn -l:libmysqlcppconn.so.7 -w

static:
	g++ *.cpp src/*.cpp src/*.c -Wl,-Bstatic `sdl-config --cflags --static-libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -Iincludes/mysql -Iincludes/mysql/cppconn -l:libmysqlcppconn-static.a -L /usr/local/lib -L /usr/lib/x86_64-linux_gnu/  -w

debug:
	g++ *.cpp src/*.cpp src/*.c `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -w -g	

run:
	./a.out

r:
	./a.out

clean:
	rm a.out

c:
	rm a.out

