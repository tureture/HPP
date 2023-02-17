#include "graphics/graphics.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


/*
Assignment 3
High Performance Computing
By Ture Hassler & Jacob Malmenstedt
*/


// Define a struct for the particle
struct Particle {
    double x;
    double y;
    double mass;
    double vx;
    double vy;
    double brightness;
};


int main(int argc, char *argv[])
{   

    int N, nsteps, graphics;
    char *filename;
    double delta_t;
    

    // Parse command line arguments
    if (argc != 6)
    {
        printf("Usage: ./galsim N filename nsteps delta_t graphics\n");
        return 1;
    }
    N = atoi(argv[1]);
    filename = argv[2];
    nsteps = atoi(argv[3]);
    delta_t = atof(argv[4]);
    graphics = atoi(argv[5]);

    double time2 = get_wall_seconds();

    // Allocate memory for N particles
    struct Particle *particles = (struct Particle *)malloc(N * sizeof(struct Particle));


    double acc_x = 0, acc_y = 0;                                     // acceleration
    double rij;
    const double e0 = 0.001;
    const double G = 100.0 / N; // gravitational constant
    int k;

    // Read input data from file
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    // Read data from file and initialize particles
for (int i = 0; i < N; i++)
{
    fread(&particles[i].x, sizeof(double), 1, file);
    fread(&particles[i].y, sizeof(double), 1, file);
    fread(&particles[i].mass, sizeof(double), 1, file);
    fread(&particles[i].vx, sizeof(double), 1, file);
    fread(&particles[i].vy, sizeof(double), 1, file);
    fread(&particles[i].brightness, sizeof(double), 1, file);
}
fclose(file);
    fclose(file);

    double time3 = get_wall_seconds();

    // ***********************Do the simulation ***************************************

    for (int i = 0; i < nsteps; i++) // for all timesteps
    {
        for (int j = 0; j < N; j++) // for all particles update acc and vel
        {

                // calc acceleration
                for (k = 0; k < j; k++) // all particles before current particle
                {
                    struct Particle p1 = particles[j];
                    struct Particle p2 = particles[k];
                    double rij = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                    double acc_k = p2.mass / ((rij + e0) * (rij + e0) * (rij + e0));
                    acc_x += acc_k * (p1.x - p2.x);
                    acc_y += acc_k * (p1.y - p2.y);
                }
                for (k = j + 1; k < N; k++) // all particles after current particle
                {
                    struct Particle p1 = particles[j];
                    struct Particle p2 = particles[k];
                    double rij = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                    double acc_k = p2.mass / ((rij + e0) * (rij + e0) * (rij + e0));
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

    double time4 = get_wall_seconds();

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
    



    double time5 = get_wall_seconds();

    // Free memory
    // free(pos_and_mass);
    //free(vel);
    //free(brightness);

    double time6 = get_wall_seconds();

    printf("Total time: %f \n", time6-time1);
    printf("Time for parsing: %f \n", time2-time1);
    printf("Time for reading file: %f \n", time3-time2);
    printf("Time for simulation: %f \n", time4-time3);
    printf("Time for writing file: %f \n", time5-time4);
    printf("Time for freeing memory: %f \n", time6-time5);


    return 0;
}