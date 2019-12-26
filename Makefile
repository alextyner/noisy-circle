noise.html: noise.c canvas.c
	emcc -Wall noise.c canvas.c -o dist-html/noise.html -lm