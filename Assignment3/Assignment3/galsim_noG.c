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
    double acc_x;
    double acc_y;
    //double brightness;
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
    double *brightness = (double *)malloc(N * sizeof(double));       // brightness


    //double acc_x = 0, acc_y = 0;                                     // acceleration
    //double rij;


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
    fread(&brightness[i], sizeof(double), 1, file);
}
fclose(file);

    double time3 = get_wall_seconds();

    // ***********************Do the simulation ***************************************
    const double e0 = 0.001;
    const double G = 100.0 / N; // gravitational constant

    for (int i = 0; i < nsteps; i++) // for all timesteps
    {
        for (int j = 0; j < N; j++) // for all particles update acc and vel
        {
            struct Particle p1 = particles[j];
                // calc acceleration
                for (int k = j+1; k < N; k++) 
                {
                    struct Particle p2 = particles[k];
                    double rij = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                    //double acc_k = p2.mass / ((rij + e0) * (rij + e0) * (rij + e0));
                    double Fx = (p2.mass*-G*(p1.x - p2.x))/((rij + e0) * (rij + e0) * (rij + e0));
                    double Fy = (p2.mass*-G*(p1.y - p2.y))/((rij + e0) * (rij + e0) * (rij + e0));
                    particles[j].acc_x += Fx;
                    particles[j].acc_y += Fy;
                    particles[k].acc_x += -Fx*p1.mass/p2.mass;
                    particles[k].acc_y += -Fy*p1.mass/p2.mass;
                }

                }
                for (int j = 0; j < N; j++) // for all particles update pos and vel
                {
                    // update velocity
                    particles[j].vx += particles[j].acc_x * delta_t;
                    particles[j].vy += particles[j].acc_y * delta_t;

                    // update position
                    particles[j].x += particles[j].vx * delta_t;
                    particles[j].y += particles[j].vy * delta_t;

                    // reset acceleration
                    particles[j].acc_x = 0;
                    particles[j].acc_y = 0;
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
        fwrite(&brightness[i], sizeof(double), 1, file_out);

    }
    fclose(file_out);
    



    double time5 = get_wall_seconds();

    // Free memory
    // free(pos_and_mass);
    //free(vel);
    free(brightness);
    free(particles);

    double time6 = get_wall_seconds();

    printf("Total time: %f \n", time6-time1);
    printf("Time for parsing: %f \n", time2-time1);
    printf("Time for reading file: %f \n", time3-time2);
    printf("Time for simulation: %f \n", time4-time3);
    printf("Time for writing file: %f \n", time5-time4);
    printf("Time for freeing memory: %f \n", time6-time5);


    return 0;
}