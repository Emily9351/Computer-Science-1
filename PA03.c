#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//define sizes
#define MAXPEOPLE 10
#define NAMELEN 20

//Initialize Global Variables
int permCount = 0; //track number of permutations
char names[MAXPEOPLE][NAMELEN];
int noSittingNextToEachOther[MAXPEOPLE][MAXPEOPLE];
int popcorn[MAXPEOPLE];

//check to make sure they have popcorn near them
int popCheck(int* perm, int numPeople)
{
    for(int i = 0; i < numPeople; i++)
    {
        //Person or someone around them had popcorn return 1
        if(popcorn[perm[i]] || (i + 1 < numPeople && popcorn[perm[i + 1]]) || (i - 1 >= 0 && popcorn[perm[i - 1]]))
            continue;

        return 0; //No popcorn
    }
    return 1;
}

//check people for conflicts
int peopleCheck(int *perm, int numPeople)
{
    for(int i = 0; i < numPeople; i++)
    {
        //Check for conflict if yes return 0
        if((i + 1 < numPeople && noSittingNextToEachOther[perm[i]][perm[i + 1]]) || ((i - 1) >= 0 && noSittingNextToEachOther[perm[i]][perm[i -1]]))
            return 0;
    }
    return 1;
}

//call both functions and return 1 (True) or 0 (False)
int permCheck(int* perm, int n)
{
    return popCheck(perm, n) && peopleCheck(perm, n);
}

void printperms(int* perm, int* used, int k, int n) {
    if (k == n)
    {
        //if pass all checks add one to the count
        if(permCheck(perm, n))
        {
            permCount++; 
            return;
        }
    }

    int i;
    for (i = 0; i < n; i++) {
        if (!used[i]) {
            used[i] = 1;
            perm[k] = i;

            printperms(perm, used, k+1, n);

            used[i] = 0;
        }
    }
}

int main(void) {
    int numPeopleAttending;
    int numPairs;

    //collect num people and pairs
    scanf("%d %d", &numPeopleAttending, &numPairs);

    //collect names of people and popcorn marker
    for(int i = 0; i < numPeopleAttending; i++)
    {
        scanf("%s %d", names[i], &popcorn[i]);
    }

    for(int j = 0; j < numPairs; j++)
    {
        //collect two names that conflict
        char name1[NAMELEN];
        char name2[NAMELEN];

        scanf("%s %s", name1, name2);

        //name1 is not found
        int idx1 = -1; 

        //name2 is not found
        int idx2 = -1;

        //loop through all people
        for(int k = 0; k < numPeopleAttending; k++)
        {
            //name1 found
            if(strcmp(name1, names[k]) == 0)
            {
                idx1 = k;
            }

            //name2 found
            if(strcmp(name2, names[k]) == 0)
            {
                idx2 = k;
            }

            //Both names were found exit loop
            if (idx1 != -1 && idx2 != -1)
                break;
        }

        //Both names have been found
        if (idx1 != -1 && idx2 != -1)
        {
            noSittingNextToEachOther[idx1][idx2] = 1;
            noSittingNextToEachOther[idx2][idx1] = 1;
        }
    }

    //initalize and call functions
    int perm[MAXPEOPLE] = {0};
    int used[MAXPEOPLE] = {0};

    printperms(perm, used, 0, numPeopleAttending);

    //print perm count
    printf("%d\n", permCount);

    return 0;
}


