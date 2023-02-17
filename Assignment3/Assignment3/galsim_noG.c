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
    for (int i = 0; i < nsteps; i++){
            for (int j = 0; j < N; j++) {
            // pre-calculate particle j values
            struct Particle p1 = particles[j];
            double j_vx = p1.vx;
            double j_vy = p1.vy;
            double j_x = p1.x;
            double j_y = p1.y;

            for (int k = 0; k < N; k++) {
                if (j == k) {
                    continue;
                }

                // pre-calculate particle k values
                struct Particle p2 = particles[k];
                double dx = p1.x - p2.x;
                double dy = p1.y - p2.y;
                double rij = sqrt(dx * dx + dy * dy);
                double acc_k = p2.mass / ((rij + e0) * (rij + e0) * (rij + e0));

                j_vx += acc_k * dx;
                j_vy += acc_k * dy;
            }

            j_vx *= -G;
            j_vy *= -G;

            // update velocity
            j_x += j_vx * delta_t;
            j_y += j_vy * delta_t;

            // update position
            j_x += j_vx * delta_t;
            j_y += j_vy * delta_t;

            // write back particle j values
            particles[j].vx = j_vx;
            particles[j].vy = j_vy;
            particles[j].x = j_x;
            particles[j].y = j_y;
}

    }


    double time4 = get_wall_seconds();
    



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