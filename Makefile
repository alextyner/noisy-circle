noise.html: noise.c vendor/wasm-canvas/canvas.c
	emcc -Wall noise.c vendor/wasm-canvas/canvas.c -o dist-html/noise.html -lm