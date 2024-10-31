#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//define epslion value
#define eps 0.0001 

typedef struct group {
  int groupSize; 
  double angle;
  int groupNum; 
} Group;

typedef struct result {
  int firstGroup;
  int secondGroup;
  double angle;
} Result;

//compare two result structs bacsed on group attributes
int compareTo(Result *R1, Result *R2)
{
  if(R1->firstGroup != R2->firstGroup)
  {
    return R1->firstGroup - R2->firstGroup;
  }
  else{
    return R1->secondGroup - R2->secondGroup;
  }
}

//merge sort groups
void merge(Group arr[], int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  /* create temp arrays */
  Group *L = malloc(n1 * sizeof(Group));
  Group *R = malloc(n2 * sizeof(Group));

  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  /* Merge the temp arrays back into arr[l..r]*/
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  while (i < n1 && j < n2) {
    if (L[i].angle <= R[j].angle) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  /* Copy the remaining elements of L[], if there
     are any */
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  /* Copy the remaining elements of R[], if there
     are any */
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
  free(L);
  free(R);
}

void mergeSort(Group arr[], int l, int r) {
  if (l < r) {
    // get the mid point
    int m = l + (r - l) / 2;

    // Sort first and second halves
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);

    // printf("Testing l=%d r=%d m=%d\n", l, r, m);

    merge(arr, l, m, r);
  }
}

//merge sort results
void mergeResults(Result arr[], int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  /* create temp arrays */
  Result *L = malloc(n1 * sizeof(Result));
  Result *R = malloc(n2 * sizeof(Result));

  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  /* Merge the temp arrays back into arr[l..r]*/
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  while (i < n1 && j < n2) {
    if (compareTo(&L[i], &R[j]) <= 0) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  /* Copy the remaining elements of L[], if there
     are any */
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  /* Copy the remaining elements of R[], if there
     are any */
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
  free(L);
  free(R);
}

void mergeSortResults(Result arr[], int l, int r) {
  if (l < r) {
    // get the mid point
    int m = l + (r - l) / 2;

    // Sort first and second halves
    mergeSortResults(arr, l, m);
    mergeSortResults(arr, m + 1, r);

    // printf("Testing l=%d r=%d m=%d\n", l, r, m);

    mergeResults(arr, l, m, r);
  }
}

//print array
void printArray(Group A[], int size) {
  printf("\nGroups: \n");
  for (int i = 0; i < size; i++)
    printf("%d %.4lf %lf \n", A[i].groupSize, A[i].angle, A[i].angle * 180 / M_PI);
}

//compare double values with epsilon values
int doubleCompare(double angle1, double angle2, double esp)
{
  if(fabs(angle1 - angle2) < esp)
  {
    return 1;
  }
  return 0;
}

int main(void) {
  int numGroups, angle;
  
  int x, y, numPeople;

  //read input for number of groups and angles
  scanf("%d %d", &numGroups, &angle);

  //covert angle to radians
  double angleRadians = angle * M_PI / 180;
  
  //allocate memory for each array
  Group *groups = calloc(numGroups, sizeof(Group));
  Result *results = calloc(numGroups, sizeof(Result));

  //read group info  
  for (int i = 0; i < numGroups; i++) {
    scanf("%d %d %d", &x, &y, &numPeople);
    groups[i].groupSize = numPeople;
    groups[i].angle = atan2(y, x);
    groups[i].groupNum = i;
  }

  //sort array of groups
  mergeSort(groups, 0, numGroups - 1);

  //calculate max angle between groups
  double answer = groups[0].angle + 2*M_PI - groups[numGroups - 1].angle;

  //find the closest pairs of groups
  int p1 = 0;
  int p2 = numGroups - 1;
  int numResult = 0;
  
  for (int i = 0; i < numGroups -1; i++) {
    double currentAngle = groups[i+1].angle - groups[i].angle;

    if(currentAngle > answer)
    {
      answer = currentAngle;
      p1 = i;
      p2 = i + 1;
    }
  }

  answer = answer * 180 / M_PI; //convert to degrees

  //check the same result with different calculation
  double result2 = groups[0].angle + 2*M_PI - groups[numGroups - 1].angle;
  result2 = result2 * 180/ M_PI;

  //compare results and store closest pair
  if(doubleCompare(answer, result2, eps))
  {
    int idx1 = groups[0].groupNum;
    int idx2 = groups[numGroups-1].groupNum;

    if(idx1 > idx2)
    {
      int temp = idx1;
      idx1 = idx2;
      idx2 = temp;
    }

    results[numResult].firstGroup = idx1;
    results[numResult].secondGroup = idx2;

    numResult++;
  }

  //loop through groups and find closest pairs
  for (int i = 0; i < numGroups - 1; i++) {
    double currentAngle = groups[i + 1].angle - groups[i].angle;
    currentAngle *= (180 / M_PI);
    if (doubleCompare(currentAngle, answer, eps)) {

      if(groups[i].groupNum > groups[i+1].groupNum)
        {
          results[numResult].firstGroup = groups[i+1].groupNum;
          results[numResult].secondGroup = groups[i].groupNum;
        }
        else{
          results[numResult].firstGroup = groups[i].groupNum;
          results[numResult].secondGroup = groups[i + 1].groupNum;
        }
        numResult++;
    }
      
  }

  //sort results array
  mergeSortResults(results, 0, numResult - 1);

  //print
  printf("Max projection degree without harming %.4f\n", answer);
  printf("Closest possible group pairs in order:\n");
  
  for(int i = 0; i < numResult; i++)
    {
      printf("%d ", results[i].firstGroup);
      printf("%d\n", results[i].secondGroup);
    }

  //free memory
  free(results);
  free(groups);
  
  return 0;
}

