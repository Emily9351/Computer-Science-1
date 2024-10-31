#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define PROCESSINGTIME 30
#define TICKETTIME 5
#define MAXQUEUES 12

typedef struct
{
  char *name;
  int tickets;
  int lineNumber;
  int arrivalTime;
} Customer;

typedef struct Node
{
  Customer *customer;
  struct Node *next;
} Node;

typedef struct Queue
{
  Node *front;
  Node *back;
  int size;
} Queue;

Customer *createCustomer(char name[], int tickets, int arrivalTime, int lineNumber)
{
    //allocate memory for pointer and name
  Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
  newCustomer->name = malloc(sizeof(char) * strlen(name) + 1);

  //copy name and assign variables
  strcpy(newCustomer->name, name);
  newCustomer->tickets = tickets;
  newCustomer->arrivalTime = arrivalTime;
  newCustomer->lineNumber = lineNumber;

  //return newCustomer
  return newCustomer;
}

Node *createNode(Customer *customer)
{
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->customer = customer;
  newNode->next = NULL;
  return newNode;
}

void createQueue(Queue lines[])
{
    //initalize queues to have a front and back of NULL and size of 0
  for (int i = 0; i < MAXQUEUES; i++)
  {
    lines[i].front = NULL;
    lines[i].back = NULL;
    lines[i].size = 0;
  }
}

//add customer to queue
void enqueue(Queue *queue, Customer *customer)
{
  Node *newNode = createNode(customer);
  if (queue->back == NULL)
  {
    queue->front = newNode;
    queue->back = newNode;
  }
  else
  {
    queue->back->next = newNode;
    queue->back = newNode;
  }
  queue->size++;
}

//remove from queue
Customer *dequeue(Queue *lines)
{
  if (lines->front == NULL)
  {
    lines->back = NULL;
    return NULL;
  }
  Node *temp = lines->front;
  Customer *customer = temp->customer;
  lines->front = lines->front->next;

  if (lines->front == NULL)
  {
    lines->back = NULL;
  }

  free(temp);
  lines->size--;

  return customer;
}

//look at first customer
Customer *peek(Queue *booths, int num)
{
  if (booths[num].front == NULL)
  {
    return NULL;
  }
  return booths[num].front->customer;
}

//see if queue is empty
int isEmpty(Queue *queue)
{
  return queue->front == NULL;
}

//return size of queue
int getSize(Queue *booth, int num)
{
  return booth[num].size;
}

int getSmallestQueue(Queue *lines)
{
  int smallestLineSize = INT_MAX; //start from biggest number possible to find smallest
  int smallestLine = 0;

    //loop through all queues to see if the queue is not empty and is less than the INT_MAX value
  for (int j = 0; j < MAXQUEUES; j++)
  {
    if (!isEmpty(&lines[j]) && lines[j].size < smallestLineSize)
    {
        //update values
      smallestLine = j;
      smallestLineSize = lines[j].size;
    }
  }
  //return newLine number or if still 0 set to first queue
  return smallestLine + 1;
}

void assignLineNum(char name[], int nt, int t, Queue lines[])
{
  //find postion
  int lineNumber = (name[0] - 'A') % 13;

  //if postion is 0 find smallest size and update line number
  if (lineNumber == 0)
  {
    lineNumber = getSmallestQueue(lines);
  }

  //create customer and add it to the queue
  Customer *newCustomer = createCustomer(name, nt, t, lineNumber);

  // Enqueue customer to the corresponding booth's queue
  enqueue(&lines[lineNumber - 1], newCustomer);
}

void assignBooths(int booths[], int b, int numNonEmptyQueues)
{
    int minNumQueues = numNonEmptyQueues / b; //each booth gets
    int numExtraQueues = numNonEmptyQueues % b; //how many get one extra queue
    for (int l = 0; l < b; l++)
    {
        booths[l] = minNumQueues;
        if (numExtraQueues > 0)
        {
            booths[l]++;
            numExtraQueues--;
        }
    }
}

void updateTime(Queue lines[], int nonEmptyQueues[], int *firstArrTimePTR, int *lineNum, int start, int end)
{
  for (int s = start; s < end; s++)
  {
      //if line empty then continue
    if (peek(lines, nonEmptyQueues[s]) == NULL)
    {
      continue;
    }

    //update time if needed
    Customer *current = peek(lines, nonEmptyQueues[s]);
    int currArrTime = current->arrivalTime;

    if (currArrTime <= *firstArrTimePTR)
    {
      *firstArrTimePTR = currArrTime;
      *lineNum = nonEmptyQueues[s];
    }
  }
}

int calculateTotalPeople(int start, int end, Queue lines[], int nonEmptyQueues[])
{
  //calculate total number of people within a booth
  int totalPeopleAtBooth = 0;

  for (int j = start; j < end; j++)
  {
    totalPeopleAtBooth += getSize(lines, nonEmptyQueues[j]);
  }
  return totalPeopleAtBooth;
}

int calculateTime(int numTickets)
{
  return PROCESSINGTIME + (numTickets * TICKETTIME);
}

int findTime(Customer *dequeueCustomer, int time)
{
  if (time == 0 || dequeueCustomer->arrivalTime > time) {
    time = dequeueCustomer->arrivalTime;
  }

  time += calculateTime(dequeueCustomer->tickets); 

  return time;
}

void freeMemory(Customer *customer)
{
  free(customer->name);
  free(customer);
}

int main(void)
{
    //initalize and collect data
  int n, b; //num customers and num booths
  scanf("%d %d", &n, &b); 

    //create array of lines and create the queues
  Queue lines[MAXQUEUES];
  createQueue(lines);

    //loop through for number of customers
  for (int i = 0; i < n; i++)
  {
      //collect user data
    char name[51];
    int nt, t; //num tickets and time

    scanf("%s %d %d", name, &nt, &t);

    assignLineNum(name, nt, t, lines);
  }

  // find number of nonempty queues
  int numNonEmptyQueues = 0;
  int nonEmptyQueues[MAXQUEUES];

  for (int i = 0; i < MAXQUEUES; i++)
  {
    if (lines[i].size > 0)
    {
      nonEmptyQueues[numNonEmptyQueues] = i;
      numNonEmptyQueues++;
    }
  }

  // array of booths to find number of nonempty queues
  int booths[b];

  // assign how many queues each booth gets
  assignBooths(booths, b, numNonEmptyQueues);

  int start = 0;
  int end = 0;
  int earliestLineNum = 0;
  int *lineNum = &earliestLineNum;

  for (int c = 0; c < b; c++)
  {
    end += booths[c];
    printf("Booth %d", c + 1);

    //calculate total amount of people at a booth
    int totalPeopleAtBooth = calculateTotalPeople(start, end, lines, nonEmptyQueues);

    //set time to arrival of first customer
    Customer *tempFront = peek(lines, nonEmptyQueues[start]);
    int firstArrTime = tempFront->arrivalTime;
    int *firstArrTimePTR = &firstArrTime;
    
    int time = 0, dequeueCount = 0;

    //go through till each person in a booth has been dequeued
    while (dequeueCount != totalPeopleAtBooth)
    {
      //update time if current customer less than first
      updateTime(lines, nonEmptyQueues, firstArrTimePTR, lineNum, start, end);

        //dequeue customers and calculate time
      Customer *dequeueCustomer = dequeue(&lines[earliestLineNum]);

      //find checkout time of customer
      time = findTime(dequeueCustomer, time);

      printf("\n%s from line %d checks out at time %d.", dequeueCustomer->name, dequeueCustomer->lineNumber, time);
      
        //loop through and update information if customer was dequeued
      for (int b = start; b < end; b++)
      {
        if (lines[nonEmptyQueues[b]].front != NULL)
        {
          firstArrTime = lines[nonEmptyQueues[b]].front->customer->arrivalTime;
          earliestLineNum = nonEmptyQueues[b];
          break;
        }
      }

      freeMemory(dequeueCustomer);

      //count every customer that has been dequeued
      dequeueCount++;
    }

    //reset start value to front new range of queues
    start = end;
    printf("\n\n");
  }

  return 0;
}

