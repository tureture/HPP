#include "graphics/graphics.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

/*
Assignment 4 - OpenMP version of the code
High Performance Computing
By Ture Hassler & Jacob Malmenstedt
*/

// Define a struct for the particle
struct Particle
{
    double x;
    double y;
    double mass;
    double vx;
    double vy;
    double brightness;
};

int main(int argc, char *argv[])
{

    // Parse command line arguments and initialize input variables
    if (argc != 7)
    {
        printf("Usage: ./galsim N filename nsteps delta_t graphics num_threads\n");
        return 1;
    }
    int N = atoi(argv[1]);
    char *filename = argv[2];
    int nsteps = atoi(argv[3]);
    double delta_t = atof(argv[4]);
    int graphics = atoi(argv[5]);
    int nr_threads = atoi(argv[6]);

    omp_set_num_threads(nr_threads);


    // Allocate memory for N particles
    struct Particle *particles = (struct Particle *)malloc(N * sizeof(struct Particle));

    // initialize variables inside for loops
    double acc_x = 0, acc_y = 0, acc_k; // acceleration
    double rij;                         // distance between particles
    const double e0 = 0.001;            // weird constant
    const double G = 100.0 / N;         // gravitational constant
    int k;
    struct Particle p1, p2; // particles

    // Read data from file and initialize particles with data
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    int sum = 0;
    for (int i = 0; i < N; i++)
    {
        sum += fread(&particles[i].x, sizeof(double), 1, file);
        sum += fread(&particles[i].y, sizeof(double), 1, file);
        sum += fread(&particles[i].mass, sizeof(double), 1, file);
        sum += fread(&particles[i].vx, sizeof(double), 1, file);
        sum += fread(&particles[i].vy, sizeof(double), 1, file);
        sum += fread(&particles[i].brightness, sizeof(double), 1, file);
    }
    fclose(file);
    if (sum != 6 * N) // check if all data was read
    {
        printf("Error reading file\n");
        return 1;
    }

    // *********************** Simulation with graphics ***************************************

    // Initialize graphics, separate if else statement to avoid unnecessary calculations
    if (graphics)
    {
        InitializeGraphics("Galaxy Simulation", 800, 800);
        SetCAxes(0, 1);
        printf("Hit q to quit.\n");
        while (!CheckForQuit()) // note this will run until you quit. Working as intended.
        {
            for (int j = 0; j < N; j++) // for all particles update acc and vel
            {

                p1 = particles[j]; // current particle

                
                for (int k = 0; k < N; k++) // calculations of acc for all particles before current particle
                {
                    p2 = particles[k];
                    rij = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                    acc_k = p2.mass / ((rij + e0) * (rij + e0) * (rij + e0));
                    acc_x += acc_k * (p1.x - p2.x);
                    acc_y += acc_k * (p1.y - p2.y);
                }
                acc_x *= -G;
                acc_y *= -G;

                // update velocity
                particles[j].vx += acc_x * delta_t;
                particles[j].vy += acc_y * delta_t;

                // reset acceleration
                acc_x = 0;
                acc_y = 0;
            }

            for (int j = 0; j < N; j++) // for all particles update pos
            {
                // update position
                particles[j].x += particles[j].vx * delta_t;
                particles[j].y += particles[j].vy * delta_t;
            }
        }
        FlushDisplay();
        CloseDisplay();
    }
    //********************** No Graphics Simulation ************************************************************
    else
    {
        for (int i = 0; i < nsteps; i++) // for all timesteps
        {
            //#pragma omp target data map(tofrom: particles[0:N])
            //#pragma omp target teams distribute parallel for private(p1, p2, rij, acc_k, acc_x, acc_y, k)
            #pragma omp parallel for private(p1, p2, rij, acc_k, acc_x, acc_y, k) schedule(dynamic)
            for (int j = 0; j < N; j++) // for all particles update acc and vel
            {
                p1 = particles[j]; // current particle

                for (k = 0; k < N; k++) // calculations of acc for all particles
                {
                    p2 = particles[k];
                    rij = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                    acc_k = p2.mass / ((rij + e0) * (rij + e0) * (rij + e0));
                    acc_x += acc_k * (p1.x - p2.x);
                    acc_y += acc_k * (p1.y - p2.y);
                }
                acc_x *= -G;
                acc_y *= -G;

                // update velocity
                particles[j].vx += acc_x * delta_t;
                particles[j].vy += acc_y * delta_t;

                // reset acceleration
                acc_x = 0;
                acc_y = 0;
            }
            #pragma omp barrier // wait for all threads to finish (maybe not necessary)
            // the implied barrier should work but to force the same threads to continue I added this


            #pragma omp parallel for schedule(static)
            //#pragma omp target data map(tofrom: particles[0:N])
            //#pragma omp target teams distribute parallel for
            for (int j = 0; j < N; j++) // for all particles update pos
            {
                // update position
                particles[j].x += particles[j].vx * delta_t;
                particles[j].y += particles[j].vy * delta_t;
            }
        }
    }

    // Write to binary file
    FILE *file_out;
    file_out = fopen("result.gal", "w");
    for (int i = 0; i < N; i++)
    {
        fwrite(&particles[i].x, sizeof(double), 1, file_out);
        fwrite(&particles[i].y, sizeof(double), 1, file_out);
        fwrite(&particles[i].mass, sizeof(double), 1, file_out);
        fwrite(&particles[i].vx, sizeof(double), 1, file_out);
        fwrite(&particles[i].vy, sizeof(double), 1, file_out);
        fwrite(&particles[i].brightness, sizeof(double), 1, file_out);
    }
    fclose(file_out);

    // free memory
    free(particles);

    return 0;
}