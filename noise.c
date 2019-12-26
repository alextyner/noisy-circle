#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define use_em 1

#if use_em
#include "canvas.h"
#endif

#define AVG(x, y) ((x + y) / 2)

static void fail(char const *msg) {
    printf("ERROR: %s\n", msg);
    exit(EXIT_FAILURE);
}

void print_array(int rows, int cols, float arr[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", arr[i][j]);
        }
        puts("");
    }
    puts("");
    // Go around the edge.
    for (int i = 0; i < rows; i++) {
        printf("%f ", arr[i][0]);
    }
    for (int j = 1; j < cols; j++) {
        printf("%f ", arr[rows - 1][j]);
    }
    for (int i = rows - 2; i >= 0; i--) {
        printf("%f ", arr[i][cols - 1]);
    }
    for (int j = cols - 2; j >= 0; j--) {
        printf("%f ", arr[0][j]);
    }
    puts("");
}

void generate_noise(float initial, float delta, int rows, int cols, float arr[rows][cols]) {
    // Randomize the grid.
    // Seed the first row using the provided value.
    arr[0][0] = initial;
    for (int j = 1; j < cols; j++) {
        // Generate subsequent values based on the cell directly left.
        arr[0][j] = arr[0][j - 1] + (((float)rand()) / RAND_MAX * (2 * delta) - delta);
    }
    // Randomize the rest of the rows.
    for (int i = 1; i < rows; i++) {
        // Seed the row using the cell directly above.
        arr[i][0] = arr[i - 1][0] + (((float)rand()) / RAND_MAX * (2 * delta) - delta);
        // Generate subsequent values based on the average of the two cells left and above.
        for (int j = 1; j < cols; j++) {
            arr[i][j] = AVG(arr[i - 1][j], arr[i][j - 1]);
        }
    }
}

void display_noise_circle(int rows, int cols, float arr[rows][cols]) {
#if use_em

    HTMLCanvasElement *canvas = createCanvas("noise");
    CanvasRenderingContext2D *ctx = canvas->getContext(canvas, "2d");

    canvas->setHeight(canvas, 720);
    canvas->setWidth(canvas, 1280);

    ctx->setFillStyle(ctx, "white");
    ctx->fillRect(ctx, 0, 0, canvas->getWidth(canvas), canvas->getHeight(canvas));
    ctx->setFillStyle(ctx, "black");

    // Display a perfect circle.
    ctx->setStrokeStyle(ctx, "black");
    ctx->beginPath(ctx);
    ctx->ellipse(ctx, canvas->getWidth(canvas) / 2, canvas->getHeight(canvas) / 2, 100, 100, 0, 0, 2 * M_PI);
    ctx->stroke(ctx);
    // --

    float delta = (2 * M_PI) / ((rows - 1) * 4);  // 10x10 = 36 values to show
    float theta = 0.0f;
    for (int i = 0; i < rows; i++) {  // LEFT
        float r = arr[i][0];
        float x = canvas->getWidth(canvas) / 2 + r * cosf(theta);
        float y = canvas->getHeight(canvas) / 2 + r * sinf(theta);
        ctx->beginPath(ctx);
        ctx->ellipse(ctx, x, y, 5, 5, 0, 0, 2 * M_PI);
        ctx->fill(ctx);
        theta += delta;
    }
    for (int j = 1; j < cols; j++) {  // BOTTOM
        float r = arr[rows - 1][j];
        float x = canvas->getWidth(canvas) / 2 + r * cosf(theta);
        float y = canvas->getHeight(canvas) / 2 + r * sinf(theta);
        ctx->beginPath(ctx);
        ctx->ellipse(ctx, x, y, 5, 5, 0, 0, 2 * M_PI);
        ctx->fill(ctx);
        theta += delta;
    }
    for (int i = rows - 2; i >= 0; i--) {  // RIGHT
        float r = arr[i][cols - 1];
        float x = canvas->getWidth(canvas) / 2 + r * cosf(theta);
        float y = canvas->getHeight(canvas) / 2 + r * sinf(theta);
        ctx->beginPath(ctx);
        ctx->ellipse(ctx, x, y, 5, 5, 0, 0, 2 * M_PI);
        ctx->fill(ctx);
        theta += delta;
    }
    for (int j = cols - 2; j > 0; j--) {  // TOP
        float r = arr[0][j];
        float x = canvas->getWidth(canvas) / 2 + r * cosf(theta);
        float y = canvas->getHeight(canvas) / 2 + r * sinf(theta);
        ctx->beginPath(ctx);
        ctx->ellipse(ctx, x, y, 5, 5, 0, 0, 2 * M_PI);
        ctx->fill(ctx);
        theta += delta;
    }

    freeCanvas(canvas);
#endif
}

int main(int argc, char **argv) {
    int rows = 10, cols = 10;

    // Allocate a 2D float array.
    float(*arr)[cols] = NULL;
    if (!(arr = malloc(rows * cols * sizeof(float))))
        fail("Couldn't allocate space for the 2D array.");

    // Set the rand() seed.
    srand(time(NULL));

    // Pick a seed value.
    float initial = 100.0f;
    // Pick a randomization range (in either direction).
    float delta = 15.00f;

    // Populate the array with randomized values.
    generate_noise(initial, delta, rows, cols, arr);

    print_array(rows, cols, arr);

    display_noise_circle(rows, cols, arr);

    free(arr);
    return EXIT_SUCCESS;
}