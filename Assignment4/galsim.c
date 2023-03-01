#include "graphics/graphics.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

/*
Assignment 4 - Parallelization with Pthreads
High Performance Computing
By Ture Hassler & Jacob Malmenstedt
*/

// Global variables
int N;
int nsteps;
double delta_t;
double e0 = 0.001;
double G;
struct Particle *particles;

// Pthread stuff 
int NUM_THREADS = 31;
pthread_mutex_t lock;
pthread_cond_t mysignal;
int waiting = 0;
int state = 0;

pthread_mutex_t lock_outer;
pthread_cond_t mysignal_outer;
int waiting_outer = 0;
int state_outer = 0;



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

// Input data for pthreads
struct Pthread_data
{
    int lowerB;
    int upperB;
};

// Borrowed from Lab9/Task-4
void barrier_inner() {
  int mystate; 
  pthread_mutex_lock (&lock);
  mystate=state;
  waiting++;
  if (waiting == NUM_THREADS) {
    waiting = 0;
    state = 1 - mystate;
    pthread_cond_broadcast(&mysignal);
  }
  while (mystate == state) {
    pthread_cond_wait(&mysignal, &lock);
  }
  pthread_mutex_unlock(&lock);
}

// Borrowed from Lab9/Task-4
void barrier_outer() {
  int mystate; 
  pthread_mutex_lock (&lock_outer);
  mystate=state_outer;
  waiting_outer++;
  if (waiting_outer == NUM_THREADS) {
    waiting_outer = 0;
    state_outer = 1 - mystate;
    pthread_cond_broadcast(&mysignal_outer);
  }
  while (mystate == state_outer) {
    pthread_cond_wait(&mysignal_outer, &lock_outer);
  }
  pthread_mutex_unlock(&lock_outer);
}

void* calc_forces(void* arg) {
  /* Calc forces for one specific particle */
    struct Pthread_data *input = (struct Pthread_data*) arg;

    int lb = input -> lowerB;
    int ub = input -> upperB;



    double acc_x = 0, acc_y = 0, acc_k; // acceleration
    double rij;                         // distance between particles
    struct Particle p1, p2;             // particles

    for (int i = 0; i < nsteps; i++)
    {
        for (int j = lb; j<ub; j++) // calculations for all particles between lb and ub
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

        // Wait for all threads to reach the barrier to sync before updating pos
        barrier_inner(); // Is there any faster way to do this?

        for (int j = lb; j<ub; j++) // update pos
        {
            // update position
            particles[j].x += particles[j].vx * delta_t;
            particles[j].y += particles[j].vy * delta_t;

        }

        barrier_outer(); // Wait for all threads to reach the barrier to sync between timesteps
    }

  return NULL;
}



int main(int argc, char *argv[])
{

    // Parse command line arguments and initialize input variables
    if (argc != 6)
    {
        printf("Usage: ./galsim N filename nsteps delta_t graphics\n");
        return 1;
    }
    N = atoi(argv[1]);
    char *filename = argv[2];
    nsteps = atoi(argv[3]);
    delta_t = atof(argv[4]);
    int graphics = atoi(argv[5]);

    // Allocate memory for N particles
    particles = (struct Particle *)malloc(N * sizeof(struct Particle));

    // initialize variables inside for loops
    double acc_x = 0, acc_y = 0, acc_k; // acceleration
    double rij;                         // distance between particles
    const double e0 = 0.001;            // weird constant
    const double G = 100.0 / N;         // gravitational constant
    int k;
    struct Particle p1, p2;             // particles

    // Pthread stuff
    pthread_t threads[NUM_THREADS];
    struct Pthread_data* data = (struct Pthread_data*) malloc((NUM_THREADS + 1) * sizeof(struct Pthread_data));

    pthread_cond_init(&mysignal, NULL);
    pthread_mutex_init(&lock, NULL);

    pthread_cond_init(&mysignal_outer, NULL);
    pthread_mutex_init(&lock_outer, NULL);


    // Read data from file and initialize particles with data
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

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

    // ***********************Do the simulation ***************************************

    // Initialize graphics, separate if else statement to avoid unnecessary calculations. Note this part is not parallelized! 
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

                for (k = 0; k < j; k++) // calculations of acc for all particles before current particle
                {

                    p2 = particles[k];
                    rij = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                    acc_k = p2.mass / ((rij + e0) * (rij + e0) * (rij + e0));
                    acc_x += acc_k * (p1.x - p2.x);
                    acc_y += acc_k * (p1.y - p2.y);
                }

                for (k = j + 1; k < N; k++) // calc for all particles after current particle
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

        int remainder = N % NUM_THREADS;
        int tasks_per_thread = N / NUM_THREADS;
        int tmp = 0;


        for (int j = 0; j < NUM_THREADS; j++) // create threads
        {   
            if (j <= remainder){
                data[j].lowerB = tmp;
                tmp += tasks_per_thread + 1;
                data[j].upperB = tmp;
            } else {
                data[j].lowerB = tmp;
                tmp += tasks_per_thread;
                data[j].upperB = tmp;
            }
            pthread_create(&threads[j], NULL, calc_forces, (void *)&data[j]);
        }




        for (int j = 0; j < NUM_THREADS; j++) // join threads 
        {
            pthread_join(threads[j], NULL);
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


    // Cleanup
    pthread_cond_destroy(&mysignal);
    pthread_mutex_destroy(&lock);

    pthread_cond_destroy(&mysignal_outer);
    pthread_mutex_destroy(&lock_outer);

    return 0;
}