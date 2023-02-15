#include "graphics/graphics.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
Assignment 3
High Performance Computing
By Ture Hassler & Jacob Malmenstedt
*/

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

    // Allocate memory for N particles
    double *pos_and_mass = (double *)malloc(3 * N * sizeof(double)); // x, y, mass
    double *vel = (double *)malloc(2 * N * sizeof(double));          // vx, vy
    double *brightness = (double *)malloc(N * sizeof(double));       // brightness
    double acc_x = 0, acc_y = 0;                                     // acceleration
    double rij, e0 = 0.001;
    double G = 100 / N; // gravitational constant

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
    /*
    for (int i = 0; i < N; i++)
    {
        printf("particle %d position %lf %lf\n", i, pos_and_mass[3 * i], pos_and_mass[3 * i + 1]);
        printf("particle %d mass %lf\n", i, pos_and_mass[3 * i + 2]);
        printf("particle %d velocity %lf %lf\n", i, vel[2 * i], vel[2 * i + 1]);
        printf("particle %d brightness %lf\n", i, brightness[i]);
    }
    */
    // ***********************Do the simulation ***************************************

    // Initialize graphics
    if (graphics)
    {
        InitializeGraphics("Galaxy Simulation", 800, 800);
        SetCAxes(0, 1);
        printf("Hit q to quit.\n");
        while (!CheckForQuit())
        {
            for (int j = 0; j < N; j++) // for all particles update acc and vel
            {

                // calc acceleration
                for (int kf = 0; kf < j; kf++)
                {
                    // all particles before current particle
                    rij = sqrt((pos_and_mass[3 * j] - pos_and_mass[3 * kf]) * (pos_and_mass[3 * j] - pos_and_mass[3 * kf]) + (pos_and_mass[3 * j + 1] - pos_and_mass[3 * kf + 1]) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * kf + 1]));
                    acc_x += pos_and_mass[3 * kf + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j] - pos_and_mass[3 * kf]);
                    acc_y += pos_and_mass[3 * kf + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * kf + 1]);
                }
                for (int ka = j + 1; ka < N; ka++)
                {
                    // all particles after current particle
                    rij = sqrt((pos_and_mass[3 * j] - pos_and_mass[3 * ka]) * (pos_and_mass[3 * j] - pos_and_mass[3 * ka]) + (pos_and_mass[3 * j + 1] - pos_and_mass[3 * ka + 1]) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * ka + 1]));
                    acc_x += pos_and_mass[3 * ka + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j] - pos_and_mass[3 * ka]);
                    acc_y += pos_and_mass[3 * ka + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * ka + 1]);
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

            ClearScreen();

            for (int i = 0; i < N; i++)
            {
                // DrawCircle((float)pos_and_mass[3 * i], (float)pos_and_mass[3 * i + 1], 1.0, 1.0, pos_and_mass[3 * i + 2] / 200, 0.0);
                DrawCircle((float)pos_and_mass[3 * i], (float)pos_and_mass[3 * i + 1], 1.0, 1.0, 0.025, 0.0);
            }
            Refresh();
            /* Sleep a short while to avoid screen flickering. */
            usleep(1000);
        }
        FlushDisplay();
        CloseDisplay();
    }

    //********************** No Graphics************************************************************
    else
    {

        for (int i = 0; i < nsteps; i++) // for all timesteps
        {
            for (int j = 0; j < N; j++) // for all particles update acc and vel
            {

                // calc acceleration
                for (int kf = 0; kf < j; kf++)
                {
                    // all particles before current particle
                    rij = sqrt((pos_and_mass[3 * j] - pos_and_mass[3 * kf]) * (pos_and_mass[3 * j] - pos_and_mass[3 * kf]) + (pos_and_mass[3 * j + 1] - pos_and_mass[3 * kf + 1]) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * kf + 1]));
                    acc_x += pos_and_mass[3 * kf + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j] - pos_and_mass[3 * kf]);
                    acc_y += pos_and_mass[3 * kf + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * kf + 1]);
                }
                for (int ka = j + 1; ka < N; ka++)
                {
                    // all particles after current particle
                    rij = sqrt((pos_and_mass[3 * j] - pos_and_mass[3 * ka]) * (pos_and_mass[3 * j] - pos_and_mass[3 * ka]) + (pos_and_mass[3 * j + 1] - pos_and_mass[3 * ka + 1]) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * ka + 1]));
                    acc_x += pos_and_mass[3 * ka + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j] - pos_and_mass[3 * ka]);
                    acc_y += pos_and_mass[3 * ka + 2] / ((rij + e0) * (rij + e0) * (rij + e0)) * (pos_and_mass[3 * j + 1] - pos_and_mass[3 * ka + 1]);
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
    }

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

    // Free memory
    free(pos_and_mass);
    free(vel);
    free(brightness);

    return 0;
}