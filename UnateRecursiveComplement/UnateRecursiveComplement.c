#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_VAR_COUNT 20
#define POS 1
#define NEG 2
#define DONTCARE 3
int total_var;
bool onechecker(char** F, int cube_in)
{
    for(int i=0;i<cube_in;i++)
    {
        bool onechecking = true;
        for(int j=0;j<total_var;j++)
            if(F[i][j] != DONTCARE)
            {
                onechecking = false;
                break;
            }
        if(onechecking) return true;
        
    }
    return false;
}

int get_value(int n)
{
    return (n>0)?POS:NEG; // positive: 01, negative: 10
}

char** Cofactor(char** F, int cube_in, int target_var, int* cube_out)
{
    int cube_count = 0;
    char** cofactor_F = (char**)malloc(sizeof(char*) * cube_in);
    for(int i=0;i<cube_in;i++)
    {
        cofactor_F[i] = (char*)malloc(sizeof(char) * total_var);
    }
    for(int i=0;i<cube_in;i++)
    {
        if(F[i][abs(target_var)] == DONTCARE || F[i][abs(target_var)] == get_value(target_var))
        {
            for(int j=0;j<total_var;j++)
            {
                cofactor_F[cube_count][j] = F[i][j];
            }
            cofactor_F[cube_count][abs(target_var)] = DONTCARE;
            cube_count++;
        }
    }
    for(int i=cube_count;i<cube_in;i++) free(cofactor_F[i]);
    cofactor_F = realloc(cofactor_F, sizeof(char*) * cube_count);
    *cube_out = cube_count;
    return cofactor_F;
}

int findBinate(char** F, int cube_in)
{
    int max_binate_score = -1;
    int min_binate_diff = 1 << MAX_VAR_COUNT;
    int selected_var = -1;
    // find the most binate var
    for(int var = 0;var<total_var;var++)
    {
        int pos = 0; int neg = 0;
        for(int i=0;i<cube_in;i++)
        {
            if(F[i][var] == POS) pos++;
            else if(F[i][var] == NEG) neg++;
        }
        if(pos == 0 && neg == 0) continue; // unate
        if(pos + neg > max_binate_score || (pos + neg == max_binate_score && abs(pos-neg) < min_binate_diff))
        {
            selected_var = var;
            max_binate_score = pos+neg;
            min_binate_diff = abs(pos-neg);
        }
    }
    // if there is no binate var, find the most unate var
    if(selected_var != -1) return selected_var;
    int max_unate_score = -1;
    for(int var = 0;var<total_var;var++)
    {
        int score = 0;
        for(int i=0;i<cube_in;i++) if(F[i][var] != DONTCARE) score++;
        if(score > max_unate_score)
        {
            selected_var = var;
            max_unate_score = score;
        }
    }
    return selected_var;
}

char** Complement(char** F, int cube_in, int* cube_out)
{
    // if F is simple and we can complement it directly
    if(cube_in == 0) // boolean equation 0, complement is 1
    {
        // printf("F is 0\n");
        char** comp_F = (char**)malloc(sizeof(char*));
        comp_F[0] = (char*)malloc(sizeof(char) * total_var);
        for(int i=0;i<total_var;i++) comp_F[0][i] = DONTCARE;
        *cube_out = 1;
        return comp_F;
    }
    else if (onechecker(F, cube_in)) // boolean equation 1, complement is 0
    {
        // printf("F is 1\n");
        *cube_out = 0;
        return NULL;
    }
    else if(cube_in == 1) // boolean equation has only one term. ex) yz'w -> y'+z+w'
    {
        // printf("F has only one term\n");
        int cube_count=0;
        char** comp_F = (char**)malloc(sizeof(char*) * total_var);
        for(int i=0;i<total_var;i++)
        {
            comp_F[i] = (char*)malloc(sizeof(char) * total_var);
            for(int j=0;j<total_var;j++) comp_F[i][j] = DONTCARE;
        }
        for(int i=0;i<total_var;i++)
            if(F[0][i] != DONTCARE) comp_F[cube_count++][i] = POS + NEG - F[0][i]; // invert 1<->2
        for(int i=cube_count;i<total_var;i++) free(comp_F[i]);
        comp_F = realloc(comp_F, sizeof(char*) * cube_count);
        *cube_out = cube_count;
        return comp_F;
    }
    // else, do recursion
    else
    {
        // find most binate variable for splitting
        int target_var = findBinate(F, cube_in);
        // P = Complement(positive x-cofactor of F)
        int PF_count, P_count;
        char** PF = Cofactor(F, cube_in, target_var, &PF_count);
        char** P = Complement(PF, PF_count, &P_count);
        // N = Complement(negative x-cofactor of F)
        int NF_count, N_count;
        char** NF = Cofactor(F, cube_in, -target_var, &NF_count);
        char** N = Complement(NF, NF_count, &N_count);
        // P = AND(x, P), N = AND(x', N)
        for(int i=0;i<P_count;i++) P[i][target_var] = POS;
        for(int i=0;i<N_count;i++) N[i][target_var] = NEG;
        // return OR(P, N)
        char** comp_F = (char**)malloc(sizeof(char*) * (P_count + N_count));
        for(int i=0;i<P_count;i++)
        {
            comp_F[i] = (char*)malloc(sizeof(char) * total_var);
            for(int j=0;j<total_var;j++) comp_F[i][j] = P[i][j];
        }
        for(int i=0;i<N_count;i++)
        {
            comp_F[P_count + i] = (char*)malloc(sizeof(char) * total_var);
            for(int j=0;j<total_var;j++) comp_F[P_count + i][j] = N[i][j];
        }
        // memory free
        for(int i=0;i<PF_count;i++) free(PF[i]);
        for(int i=0;i<P_count;i++) free(P[i]);
        for(int i=0;i<NF_count;i++) free(NF[i]);
        for(int i=0;i<N_count;i++) free(N[i]);
        free(PF); free(P); free(NF); free(N);

        *cube_out = P_count + N_count;
        return comp_F;
    }
}

int main(void)
{
    // example
    // 5
    // 3
    // 3 2 3 4 x2x3x4
    // 2 -1 5 x1'x5
    // 3 1 -3 -4 x1x3'x4'
    FILE* file;
    int cube_in, cube_out;
    char filename[64];
    printf("Enter File: ");
    scanf_s("%s", filename, sizeof(filename));
    file = fopen(filename, "r"); // will be changed later
    if(file == NULL)
    {
        printf("file does not exist\n");
        exit(1);
    }
    fscanf(file, "%d", &total_var);
    if(total_var>MAX_VAR_COUNT || total_var <= 0)
    {
        printf("too many or less variables!\n");
        exit(1);
    }
    fscanf(file, "%d", &cube_in);
    char** F = (char**)malloc(sizeof(char*) * cube_in);
    for(int i=0;i<cube_in;i++)
    {   
        F[i] = (char*)malloc(sizeof(char) * total_var);
        int term_var_count;
        fscanf(file, "%d", &term_var_count);
        for(int j=0;j<total_var;j++)
        {
            F[i][j] = DONTCARE;
        }
        for(int j=0;j<term_var_count;j++)
        {
            int temp;
            fscanf(file, "%d", &temp);
            F[i][abs(temp)-1] = get_value(temp);
        }
    }
    char** complement_F = Complement(F, cube_in, &cube_out);
    printf("Answer\n");
    file = fopen("result.pcn", "w");
    fprintf(file, "%d\n", cube_out);
    for(int i=0;i<cube_out;i++)
    {
        int cube_var_count = 0;
        for(int j=0;j<total_var;j++)
        {
            if(complement_F[i][j] != DONTCARE) cube_var_count++;
        }
        fprintf(file, "%d ", cube_var_count);
        for(int j=0;j<total_var;j++)
        {
            if(complement_F[i][j] == POS) fprintf(file, "%d ", j+1);
            else if(complement_F[i][j] == NEG) fprintf(file, "%d ", -(j+1));
        }
        free(complement_F[i]);
        fprintf(file, "\n");
    }
    free(complement_F);
    for(int i=0;i<cube_in;i++)
    {
        free(F[i]);
    }
    free(F);
    fclose(file);
    return 0;
}