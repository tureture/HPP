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

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

// Define a struct for a particle
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
    double time1 = get_wall_seconds();

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
    /*
    double *pos_and_mass = (double *)malloc(3 * N * sizeof(double)); // x, y, mass
    double *vel = (double *)malloc(2 * N * sizeof(double));          // vx, vy
    double *brightness = (double *)malloc(N * sizeof(double));       // brightness
    
    */

    
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

    for (int i = 0; i < N; i++)
    {
        fread(&pos_and_mass[3 * i], sizeof(double), 3, file);
        fread(&vel[2 * i], sizeof(double), 2, file);
        fread(&brightness[i], sizeof(double), 1, file);
    }
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
                
                
                    // all particles before current particle
                    rij = sqrt((pos_and_mass[3 * j] - pos_and_mass[3 * k]) * (pos_and_mass[3 * j] - pos_and_mass[3 * k]) + (pos_and_mass[3 * j + 1] - pos_and_mass[3 * k + 1]) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * k + 1]));
                    acc_x += pos_and_mass[3 * k + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j] - pos_and_mass[3 * k]);
                    acc_y += pos_and_mass[3 * k + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * k + 1]);          
                

            }
            for (k = j + 1; k < N; k++) // all particles after current particle
            {
                
                    rij = sqrt((pos_and_mass[3 * j] - pos_and_mass[3 * k]) * (pos_and_mass[3 * j] - pos_and_mass[3 * k]) + (pos_and_mass[3 * j + 1] - pos_and_mass[3 * k + 1]) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * k + 1]));
                    acc_x += pos_and_mass[3 * k + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j] - pos_and_mass[3 * k]);
                    acc_y += pos_and_mass[3 * k + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * k + 1]);             
                    
            }
            acc_x *= -G;
            acc_y *= -G;

            // update velocity
            vel[2 * j] += acc_x * delta_t;
            vel[2 * j + 1] += acc_y * delta_t;

            // reset acceleration
            acc_x = 0;
            acc_y = 0;
        }
        for (int j = 0; j < N; j++) // for all particles update pos
        {
            // update position
            pos_and_mass[3 * j] += vel[2 * j] * delta_t;
            pos_and_mass[3 * j + 1] += vel[2 * j + 1] * delta_t;
        }
    }

    double time4 = get_wall_seconds();
    

    // Write to binary file
    FILE *file_out;
    file_out = fopen("result.gal", "w");
    for (int i = 0; i < N; i++)
    {
        fwrite(&pos_and_mass[3 * i], sizeof(double), 3, file_out);
        fwrite(&vel[2 * i], sizeof(double), 2, file_out);
        fwrite(&brightness[i], sizeof(double), 1, file_out);
    }
    fclose(file_out);

    double time5 = get_wall_seconds();

    // Free memory
    free(pos_and_mass);
    free(vel);
    free(brightness);

    double time6 = get_wall_seconds();

    printf("Total time: %f \n", time6-time1);
    printf("Time for parsing: %f \n", time2-time1);
    printf("Time for reading file: %f \n", time3-time2);
    printf("Time for simulation: %f \n", time4-time3);
    printf("Time for writing file: %f \n", time5-time4);
    printf("Time for freeing memory: %f \n", time6-time5);


    return 0;
}