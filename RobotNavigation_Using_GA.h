// Pre-Defined Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// User Defined Macros
#define grid_size 16                // Grid Size
#define population_size 500         // Total Population
#define max_iterations 3000         // Maximum Iterations
#define random_size (grid_size - 2) // Chromosome size [index 1 to (gridsize - 2)] for random population
#define w_f 3                       // Weightage of Infeasibile_Steps
#define w_t 3                       // Weightage of Turns
#define w_l 2                       // Weightage of Path_Lenght

// Grid Arena
int grid_Define[grid_size][grid_size] =
    {{0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1},
     {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
     {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1},
     {0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
     {0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0},
     {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0}};

// Global Variables & Arrays
int random_population[population_size][random_size]; // Random Population, Genes = random_size (grid_size - 2)
int final_population[population_size][grid_size];    // Final Population, Genes = grid_size
int direction_bit[population_size];                  // Bit for controlling Column-First or Row-First
int orientation_bit[population_size];                // Bit for controlling Column-Wise or Row-Wise
int inf_steps_storing[population_size];              // Storing Infeasibile Steps of each chromosome
int turns_storing[population_size];                  // Storing Turns of each chromosome
int path_length_storing[population_size];            // Storing Path_Length of each chromosome
int inf_steps_min = 0, inf_steps_max = 0;            // inf_steps_min => Ideally zero for obstacle-less path
int turns_min = 1000, turns_max = 0;
int path_length_min = 1000, path_length_max = 0;
float actual_fitness[population_size]; // Storing Actual Fitness of each generation
int solution_index = 0;                // Index of Solution Chromosome
char sGrid[grid_size][grid_size];      // Soluiton Grid
const char emptySpace = ' ';
const char obstacle = 178;
const char start = 175; // >>
const char goal = 42;   // *
const char rStep = 46;  // .
int iterations = 0;

// User Defined Functions
void Generate_Random_Population();                         // Generate Random Population, Genes = random_size (grid_size - 2)
void Final_Population();                                   // Final Population, Genes = grid_size
void Check_Turns();                                        // Storing Turns of each chromosome
void Check_infsteps_pathlength();                          // Storing Infeasibile Steps & Path length of each chromosome
void Min_Max();                                            // Min & Max values in the each Generation
void Fitness();                                            // Storing Actual Fitness of each chromosome
void Parent_Selection();                                   // Parent Selection w.r.t actual_fitness value
void crossover();                                          // Crossover parents to get fit offsprings
void mutation();                                           // Muatating to get fit chromosomes
int Check_Solution(float actual_fitness[population_size]); // Check for Fittest chromosome
void Display_Solution();                                   // Display Solution in Terminal & FILE

// File Pointers
FILE *Solution_File;   // For saving solution in txt file
FILE *Graph_Data_File; // For saving fitness variables in txt file

void Generate_Random_Population()
{
    srand(time(NULL)); // For generating random numbers
    for (int i = 0; i < population_size; i++)
    {
        for (int j = 0; j < random_size; j++)
        {
            random_population[i][j] = rand() % grid_size; // Storing random genes
        }
        direction_bit[i] = rand() % 2;   // Random direction_bit '0' or '1' (column-first or row-first)
        orientation_bit[i] = rand() % 2; // Random orientation_bit '0' or '1' (column-wise or row-wise)
    }
}
void Final_Population()
{
    for (int i = 0; i < population_size; i++)
    {
        final_population[i][0] = 0; // First value of each population as '0'
        for (int j = 0; j < grid_size - 1; j++)
        {
            final_population[i][j + 1] = random_population[i][j];
        }
        final_population[i][grid_size - 1] = (grid_size - 1); // Last value of each population as 'grid_size - 1'
    }
}
void Check_Turns()
{
    int turns = 0;
    for (int i = 0; i < population_size; i++)
    {
        for (int j = 0; j < grid_size - 1; j++)
        {
            if (final_population[i][j] != final_population[i][j + 1])
            {
                turns++;
            }
        }
        turns_storing[i] = turns; // Storing Turns of each chromosome
        turns = 0;
    }
}
void Check_infsteps_pathlength() // column-first or column-wise = 0
{                                // row-first or row-wise = 1
    int length = 0;
    int k = 0, decide;
    int infeasible_steps = 0;
    for (int i = 0; i < population_size; i++)
    {
        if ((direction_bit[i] ^ orientation_bit[i]) == 0)
            decide = 0;
        else
            decide = 1;
        if ((Check_Solution(actual_fitness) == 1) && (i == solution_index))
        {
            if (decide != 0)
                printf("(0 , 0), ");
        }
        for (int j = 0; j < (grid_size - 1); j++)
        {
            if (final_population[i][j + 1] > final_population[i][j]) // If next gene is greater than the previous one
            {
                while (k != ((final_population[i][j + 1]) + 1))
                {
                    if (orientation_bit[i] == 0) // *** Column-Wise ***
                    {
                        if ((Check_Solution(actual_fitness) == 1) && (i == solution_index)) // Check_Solution == 1, print the steps
                        {
                            printf("(%d , %d), ", k, j + decide);
                            sGrid[k][j + decide] = rStep; // For display purpose assigning step at that place of sGrid
                        }
                        if (grid_Define[k][j + decide] == 1)
                            infeasible_steps++; // Checking obstacles in path of each chromosome in defined Arena
                    }
                    else // *** Row-Wise ***
                    {
                        if ((Check_Solution(actual_fitness) == 1) && (i == solution_index))
                        {
                            printf("(%d , %d), ", j + decide, k);
                            sGrid[j + decide][k] = rStep;
                        }
                        if (grid_Define[j + decide][k] == 1)
                            infeasible_steps++;
                    }
                    k++;
                    length++; // Checking no. of steps of each chromosome in defined Arena
                }
                k--; // to assign the previous value
            }
            else
            {
                while (k != ((final_population[i][j + 1]) - 1)) // If next gene is lesser than the previous one
                {
                    if (orientation_bit[i] == 0) // Column-Wise
                    {
                        if ((Check_Solution(actual_fitness) == 1) && (i == solution_index)) // Check_Solution == 1, print the steps
                        {
                            printf("(%d , %d), ", k, j + decide);
                            sGrid[k][j + decide] = rStep; // For display purpose assigning step at that place of sGrid
                        }
                        if (grid_Define[k][j + decide] == 1)
                        {
                            infeasible_steps++; // Checking obstacles in path of each chromosome in defined Arena
                        }
                    }
                    else // Row-Wise
                    {
                        if ((Check_Solution(actual_fitness) == 1) && (i == solution_index))
                        {
                            printf("(%d , %d), ", j + decide, k);
                            sGrid[j + decide][k] = rStep;
                        }
                        if (grid_Define[j + decide][k] == 1)
                        {
                            infeasible_steps++;
                        }
                    }
                    k--;
                    length++; // Checking no. of steps of each chromosome in defined Arena
                }
                k++; // to assign the previous value.
            }
        }
        if ((Check_Solution(actual_fitness) == 1) && (i == solution_index))
        {
            if (decide == 0)
            {
                printf("(%d , %d)", grid_size - 1, grid_size - 1); // Printing path coordinates of fittest chromosome
            }
        }
        path_length_storing[i] = length; // Storing Path_Length of each chromosome
        length = 0;
        inf_steps_storing[i] = infeasible_steps; // Storing Ifeasivle_Steps of each chromosome
        infeasible_steps = 0;
        k = 0; // Initializing for the next generation
    }
}
void Min_Max()
{
    for (int i = 0; i < population_size; i++) // Min & Max values by comparing their values of each chromosome in the each Generation
    {
        if (inf_steps_max < inf_steps_storing[i]) // Maximum infeasible steps of generation
            inf_steps_max = inf_steps_storing[i];
        if (path_length_min > path_length_storing[i]) // Minimum infeasible steps of generation
            path_length_min = path_length_storing[i];
        if (path_length_max < path_length_storing[i]) // Maximum infeasible steps of generation
            path_length_max = path_length_storing[i];
        if (turns_min > turns_storing[i]) // Minimum infeasible steps of generation
            turns_min = turns_storing[i];
        if (turns_max < turns_storing[i]) // Maximum infeasible steps of generation
            turns_max = turns_storing[i];
    }
}
void Fitness()
{
    float f_infeasible_steps = 0, f_turns = 0, f_path_length = 0;
    for (int i = 0; i < population_size; i++)
    {
        f_turns = 1 - (float)(turns_storing[i] - turns_min) / (turns_max - turns_min);
        f_path_length = 1 - (float)(path_length_storing[i] - path_length_min) / (path_length_max - path_length_min);
        f_infeasible_steps = 1 - (float)(inf_steps_storing[i] - inf_steps_min) / (inf_steps_max - inf_steps_min);
        actual_fitness[i] = (100 * w_f * f_infeasible_steps) * (float)((w_l * f_path_length) + (w_t * f_turns)) / (w_l + w_t); // Storing Actual Fitness of each chromosome
    }
}
void Parent_Selection()
{
    int temp = 0;
    int temp_array[random_size];

    for (int i = 0; i < population_size; i++)
    {
        for (int j = i + 1; j < population_size; j++)
        {
            if (actual_fitness[j] > actual_fitness[i]) // Parent Selection w.r.t actual_fitness value
            {
                for (int k = 0; k < random_size; k++)
                {
                    temp_array[k] = random_population[i][k]; // "---Random Population Swamping Started---"
                }
                for (int k = 0; k < random_size; k++)
                {
                    random_population[i][k] = random_population[j][k];
                }
                for (int k = 0; k < random_size; k++)
                {
                    random_population[j][k] = temp_array[k]; // "--- Random Population Swamping Completed---"
                }
                temp = actual_fitness[i]; // "---Actual Fitness Swamping Started---"
                actual_fitness[i] = actual_fitness[j];
                actual_fitness[j] = temp; // "---Actual Fitness Swamping Completed---"
                temp = 0;
                temp = orientation_bit[i]; // "---Orientation Bit Swamping Started---"
                orientation_bit[i] = orientation_bit[j];
                orientation_bit[j] = temp; // "---Orientation Bit Swamping Completed---"
                temp = 0;
                temp = direction_bit[i]; // "---Direction Bit Swamping Started---"
                direction_bit[i] = direction_bit[j];
                direction_bit[j] = temp; // "---Direction Bit Swamping Completed---"
            }
        }
    }
}
void crossover() // Taking fittest parents after selection. Then crossing, expecting to make more fittest offsprings for ultimate solution.
{
    srand(time(NULL));
    int cross_point = (rand() % (random_size - 1)) + 1; // To reduce the range of cross_point
    int temp_array[random_size - cross_point];          // For swaping the values from cross_point to total numbrer of genes
    int k = 0;
    for (int i = 0; i < population_size; i++) // To copy first half of population to other half.
    {
        for (int j = 0; j < random_size; j++)
        {
            random_population[k + (population_size / 2)][j] = random_population[i][j]; // [k + (population_size / 2)] is next half population
        }
        k++;
    }
    for (int i = (population_size / 2); i < population_size; i = i + 2) // To swap next half to crossover point with other population
    {
        for (int j = 0; j < (random_size - cross_point); j++)
        {
            temp_array[j] = random_population[i + 1][j + cross_point];
        }
        for (int j = 0; j < (random_size - cross_point); j++)
        {
            random_population[i + 1][j + cross_point] = random_population[i][j + cross_point];
        }
        for (int j = 0; j < (random_size - cross_point); j++)
        {
            random_population[i][j + cross_point] = temp_array[j];
        }
    }
}
void mutation()
{
    srand(time(NULL));
    for (int i = 0; i < population_size; i = i + 1) // i = i + 1, doing mutation in one population leaving other
    {
        int index = rand() % random_size; // For each population different mutaion index & mutaion value
        int value = rand() % grid_size;
        random_population[i][index] = value;
    }
    for (int i = (population_size / 2); i < population_size; i = i + 3) // Mutating orientation_bit of population having less actual_fitness
    {
        orientation_bit[i] = rand() % 2;
        direction_bit[i] = rand() % 2;
    }
}
int Check_Solution(float fit[population_size])
{
    for (int i = 0; i < population_size; i++)
    {
        if (fit[i] == 300.00 && inf_steps_storing[i] == 0)
        {
            solution_index = i; // Fittest chromosome index
            return 1;
        }
    }
    return 0;
}
void Display_Solution()
{
    fprintf(Solution_File, "Chromosome => [ 0 ");
    printf("\nChromosome => [ 0 ");

    for (int i = 0; i < random_size; i++)
    {
        fprintf(Solution_File, "%d ", random_population[solution_index][i]);
        printf("%d ", random_population[solution_index][i]);
    }
    fprintf(Solution_File, "%d ]\n", grid_size - 1); // Printing in Solution.txt File
    fprintf(Solution_File, "\nOrientation_bit  = %d\n", orientation_bit[solution_index]);
    fprintf(Solution_File, "Direction_bit    = %d\n", direction_bit[solution_index]);
    fprintf(Solution_File, "No. of turns = %d\n", turns_storing[solution_index]);
    fprintf(Solution_File, "Path_Length  = %d\n", path_length_storing[solution_index]);
    fprintf(Solution_File, "Infeasible _Steps = %d\n", inf_steps_storing[solution_index]);
    fprintf(Solution_File, "Fitness = %0.2f\n", actual_fitness[solution_index]);
    printf("%d ]\n", grid_size - 1); // Printing in VS Code Terminal
    printf("\nOrientation_bit  = %d\n", orientation_bit[solution_index]);
    printf("Direction_bit    = %d\n", direction_bit[solution_index]);
    printf("No. of turns = %d\n", turns_storing[solution_index]);
    printf("Path_Length  = %d\n", path_length_storing[solution_index]);
    printf("Infeasible _Steps = %d\n", inf_steps_storing[solution_index]);
    printf("Fitness = %0.2f\n", actual_fitness[solution_index]);

    for (int i = 0; i < grid_size; i++)
    {
        for (int j = 0; j < grid_size; j++)
        {
            if (grid_Define[i][j] == 0)
            {
                sGrid[i][j] = ' ';
            }
            else if (grid_Define[i][j] == 1)
            {
                sGrid[i][j] = obstacle;
            }
        }
    }
    printf("\n");
    Check_infsteps_pathlength(); // For printing the steps of solved population
    printf("\b\n");
    sGrid[0][0] = start;
    sGrid[grid_size - 1][grid_size - 1] = goal;
    fprintf(Solution_File, "\n---------------------------------------");
    printf("\n---------------------------------------");
    for (int i = 0; i < grid_size; i++)
    {
        fprintf(Solution_File, "\n%2d|", i);
        printf("\n%2d|", i);
        for (int j = 0; j < grid_size; j++)
        {
            fprintf(Solution_File, "%c", sGrid[i][j]);
            fprintf(Solution_File, "|");
            printf("%c", sGrid[i][j]);
            printf("|");
        }
    }
    fprintf(Solution_File, "\n---------------------------------------\n  ");
    printf("\n---------------------------------------\n  ");
    for (int i = 0; i < grid_size; i++)
    {
        fprintf(Solution_File, "%2d", i);
        printf("%2d", i);
    }
    printf("\n");
    fclose(Solution_File);
}
// End of Program