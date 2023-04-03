/*-- *** --- "Autonomous_Robot_Navigation_By_Genetic_Algorithm" --- *** --
---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ----
Written by Muawwiz Ali Yousuf, 2021-MC-13 (muawwizaliyousuf74@gmail.com)
---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ----
IDE: Visual Studio Code 1.74.2
C Compiler: gcc (Rev1, Built by MSYS2 project) 12.2.0
---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ----*/

// Pre-Defined Libraries
#include <stdio.h>

// User Defined Macros
#include"RobotNavigation_Using_GA.h"

int main()
{
    Solution_File = fopen("Solution.txt", "a+");
    Graph_Data_File = fopen("Graph_Data.txt", "a+");
    int check = 0;
    Generate_Random_Population();
    while (iterations < max_iterations && check == 0)
    {
        Final_Population();
        Check_Turns();
        Check_infsteps_pathlength();
        Min_Max();
        Fitness();
        fprintf(Graph_Data_File, "%d\t%d\t%d\t%.2f\n", path_length_storing[0], inf_steps_storing[0], turns_storing[0], actual_fitness[0]);
        check = Check_Solution(actual_fitness);
        if (check == 1)
        {
            break;
        }
        Parent_Selection();
        crossover();
        mutation();
        printf("iteration >> %d\n", iterations);
        iterations++;
    }
    if (check == 1)
    {
        fprintf(Solution_File, "\n\n< < < < < < < < < < < < < < <> > > > > > > > > > > > > > >\n");
        printf("\nGeneration No. = %d\n", iterations);
        fprintf(Solution_File, "\nGeneration No. = %d\n", iterations);
        fprintf(Graph_Data_File, "\n< < < < < < < < < < < < < < <> > > > > > > > > > > > > > >\n\n");
        Display_Solution();
    }
    else
    {
        printf("Sorry! No solution found.");
    }
    fclose(Graph_Data_File);
    return 0;
}
// End of Program