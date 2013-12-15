#ifndef WORLDFUNCTIONS_H
#define WORLDFUNCTIONS_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <QDebug>


using namespace std;

// Lowlevel functions copying or making 0s
void initialize_world(int **World_type, int MAX);

// Loop controlling functions
void calculate_next_generation(int **World_original, int **World_color, int& generation, int MAX);
void count_neighbors(int **World, int r, int c, int& num_type_0, int& num_type_1, int& num_type_2);
int  next_bit_state(int current_state, int num_type_1, int num_type_2); // current_state= World[r][c], return int is stored into World_neXtGen[][]

// Menu functions
void cycle_state (int **World, int r, int c);
void populate_random(int **WorldRand, int MAX);

void copyTwoDimensional(int **World_orig, int **World_copy, int MAX);
void get_m_size(char two_D_size[], int& m_rows, int& m_cols);
int  findpos(char here[], char what); // returns position of what in here[] or -1 if DNE
void substr (char whole[], char part[], int startpos, int endpos);

void initialize_world(int **World_type, int MAX)
{
    int r, c;
    for (r = 0; r<MAX; r++)
        for (c = 0; c<MAX; c++)
            World_type[r][c] = 0; // INITIALIZE TO ALL 0s
}

void copyTwoDimensional(int **World_orig, int **World_copy, int MAX) {
    //qDebug() << "Copying array!";
    // Make it circular first!
    // NEW and IMPROVED! Now with only one for loop!
    for (int i = 1; i <= (MAX-2); i++)
    {
        // Lasts -> First Gutter c, r
        World_orig[i][0] = World_orig[i][(MAX - 2)]; // Last col to begin gutter col
        World_orig[0][i] = World_orig[(MAX - 2)][i]; // Last row to begin gutter row
        // Firsts -> Last Gutter c, r
        World_orig[i][(MAX-1)] = World_orig[i][1];   // First col to end gutter col
        World_orig[(MAX-1)][i] = World_orig[1][i];   // First row to end gutter row
    }

    // copy corners
    World_orig[MAX-1][MAX-1] = World_orig[1][1];
    World_orig[0][0] = World_orig[MAX-2][MAX-2];

    World_orig[MAX-1][0] = World_orig[1][MAX-2];
    World_orig[0][MAX-1] = World_orig[MAX-2][1];

    // optimizing process
    for (int i = 0; i < MAX/2+1; i++)
        for (int j = 0; j < MAX; j++)
        {
            World_copy[i][j] = World_orig[i][j]; // MADE A COPY!
            if (i < MAX/2)
            {
                World_copy[MAX-1-i][MAX-1-j] = World_orig[MAX-1-i][MAX-1-j];
            }
        }
    // that copied everything, even the gutter
}

void cycle_state (int **World, int r, int c) {
    if (World[r][c] == 0) {
        World[r][c] = 1;
    }
    else if (World[r][c] == 1) {
        World[r][c] = 2;
    }
    else if (World[r][c] == 2) {
        World[r][c] = 0;
    }
    // cycle through
}

void populate_random(int **WorldRand, int MAX) {
    int i,j;
    //qDebug() << "Randomly generating life...";
    int max_rands = 4*MAX;

    for (int a = 0; a < max_rands; a++)
    {
        i=rand()%(MAX-2)+1; // ROW rand
        j=rand()%(MAX-2)+1; // COL rand
        WorldRand[i][j] = (rand()%2) + 1; // life rand!
    }
}


void count_neighbors(int **World_type, int r, int c, int &num_type_0, int &num_type_1, int &num_type_2) {
    //    int current_state = World[r][c]; // We'll subtract this from the end value
    int maxrow = r+1;
    int maxcol = c+1;
    int mincol = c-1;
    int minrow = r-1;
    int i, j; // need this stored outside loops so we can reset to the "BOX" around current bit
    for (i=minrow; i <= maxrow; i++)
        for (j=mincol; j <= maxcol; j++)
        {
            if ((i == r) && (j == c)) continue; // don't count myself
            else if (World_type[i][j] == 1) num_type_1++; // We found a 1 bit
            else if (World_type[i][j] == 2) num_type_2++; // We found a 2 bit
            else if (World_type[i][j] == 0) num_type_0++;
            // We don't add 0 bits
        }
}

int next_bit_state(int current_state, int num_type_1, int num_type_2)
{
    int num_neighbors = (num_type_1 + num_type_2);
    // DEBUG! should NOT be >= 3, and else with random birth should be commented out!
    if (( current_state == 0 ) && ( num_neighbors == 3 ))
    {  // rebirth!
        if (num_type_1 > num_type_2)
        {
            return 1;
        } else if (num_type_2 > num_type_1) {
            return 2;
        }
    }
    else if ((current_state == 0) && (num_neighbors < 3))
        return 0; // Stays dead
    else if (((current_state == 1)||(current_state == 2)) && ((num_neighbors == 2) || (num_neighbors == 3)))
        return current_state; // STAYS ALIVE AT CURRENT STATE
    // else...
    return 0; // Death
}

void calculate_next_generation(int **World_original, int **World_color, int& generation, int MAX)
{

    generation++; // counter of generations passed by ref.

    int **World_neXtgen = new int *[MAX]; // LOCAL
    for (int i =0; i< MAX; i++)
        World_neXtgen[i] = new int[MAX];
    int num_type_0, num_type_1, num_type_2;

    copyTwoDimensional(World_original, World_neXtgen, MAX); // copy from 2D original, into 2D copy (2nd arg)
    // -1 from end and start from 1 makes the buffer 1 on all sides
    for (int r = 1; r <= (MAX-2); r++)
    {
        for (int c = 1; c <= MAX-2; c++)
        {
            num_type_0 = 0;
            num_type_1 = 0;
            num_type_2 = 0;
            count_neighbors(World_original, r, c, num_type_0, num_type_1, num_type_2);
            World_neXtgen[r][c] = next_bit_state(World_original[r][c], num_type_1, num_type_2);
            World_color[r][c] = (1*num_type_0 + 10*num_type_1 + 100*num_type_2); // since we can't have 10 neighbors, we're safe!
            //if (r < ((MAX-2)/2))
            //            {
            //                count_neighbors(World_original, MAX-1-r, c, num_type_0, num_type_1, num_type_2);
            //                World_neXtgen[MAX-1-r][c] = next_bit_state(World_original[r][c], num_type_1, num_type_2);
            //                World_colorOverlay[MAX-1-r][c] = 1*num_type_0 + 10*num_type_1+ 100*num_type_2; // since we can't have 10 neighbors, we're safe!
            //            }
        }
    }
    copyTwoDimensional(World_neXtgen, World_original, MAX); // copy back from 2D copy and processed file, into 2D World

    // Free memory!
    for ( int i = 0 ; i < MAX ; i++ )
        delete [] World_neXtgen[i] ;
    delete [] World_neXtgen;
}

void substr (char whole[], char part[], int startpos, int endpos)
{
    int length = endpos - startpos;
    int i;
    for (i=0; i < length; i++)
        part[i] = whole[startpos+i];
    part[i] = '\0';
}

int findpos (char here[], char what)
{
    for (int i = 0; i < strlen(here); i++)
        if (here[i] == what) return i;
    return -1; // NOT FOUND!
}

void get_m_size(char two_D_size[], int& m_rows, int& m_cols)
{
    int pos = findpos(two_D_size, 'x'); // Find the position of the 'x' in the first line!
    char rows[20], cols[20];
    substr(two_D_size, rows, 0, pos);
    substr(two_D_size, cols, pos+1, strlen(two_D_size));
    m_rows = atoi(rows);
    m_cols = atoi(cols);
}


#endif // WORLDFUNCTIONS_H
