#include "graphics/graphics.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int N, nsteps, graphics;
    char *filename;
    double delta_t;


    // Parse command line arguments
    if (argc != 6) {
        printf("Usage: ./galsim N filename nsteps delta_t graphics\n");
        return 1;
    }
    N = atoi(argv[1]);
    filename = argv[2];
    nsteps = atoi(argv[3]);
    delta_t = atof(argv[4]);
    graphics = atoi(argv[5]);

    // Allocate memory for N particles
    double * pos_and_mass = (double *) malloc(3 * N * sizeof(double)); 
    double * vel = (double *) malloc(2 * N * sizeof(double));
    double * brightness = (double *) malloc(N * sizeof(double));

    // Read input data from file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    double *positions = (double *) malloc(2 * N * sizeof(double));
    double *masses = (double *) malloc(N * sizeof(double));
    double *velocities = (double *) malloc(2 * N * sizeof(double));
    double *brightnesses = (double *) malloc(N * sizeof(double));
    for (int i = 0; i < N; i++) {
        fscanf(file, "particle %d position %lf %lf\n", &i, &positions[2*i], &positions[2*i+1]);
        fscanf(file, "particle %d mass %lf\n", &i, &masses[i]);
        fscanf(file, "particle %d velocity %lf %lf\n", &i, &velocities[2*i], &velocities[2*i+1]);
        fscanf(file, "particle %d brightness %lf\n", &i, &brightnesses[i]);
    }
    fclose(file);

    // Do simulation
    // ...

    // Free memory
    free(positions);
    free(masses);
    free(velocities);
    free(brightnesses);

    return 0;
}