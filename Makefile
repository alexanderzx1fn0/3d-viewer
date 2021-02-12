all:
	g++ -Iglad/include/ main.cpp AbstractCamera.cpp TargetCamera.cpp ./glad/src/glad.c -o main -lglfw -lGL -ldl
