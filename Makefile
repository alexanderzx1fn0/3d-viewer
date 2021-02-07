all:
	g++ -Iglad/include/ main.cpp ./glad/src/glad.c -o main -lglfw -lGL -ldl
