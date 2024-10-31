#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITSIZE 10    // initial size of any row (in terms of number of orders)
#define MAXLEN 50      // maximum length of a name for an order
#define MAXROWS 100000 // maximum number of rows in the theater

typedef struct order {
  int s_seat;
  int e_seat;
  char *name;
} order;

typedef struct theaterrow {
  order **list_orders;
  int max_size;
  int cur_size;
} theaterrow;

order *make_order(int start, int end, char *this_name) {
  // dynamic memory allocation
  order *neworder = (order *)malloc(sizeof(order));

  // store numbers in dynamic memory
  neworder->s_seat = start;
  neworder->e_seat = end;

  // allocate space for name
  neworder->name = (char *)malloc((strlen(this_name) + 1) * sizeof(char));

  // copy name to dynamic
  strcpy(neworder->name, this_name);

  return neworder;
}

theaterrow *make_empty_row() {
  // allocate memory
  theaterrow *newrow = (theaterrow *)malloc(sizeof(theaterrow));

  // check if newrow is NULL
  if (newrow == NULL) {
    return NULL;
  } else {
    // update sizes
    newrow->max_size = INITSIZE;

    newrow->cur_size = 0;

    // allocate memory
    newrow->list_orders = (order **)malloc(newrow->max_size * sizeof(order *));
  }
  return newrow;
}

int conflict(order *order1, order *order2) {
  // check orders for conflicts by checking start and end seats
  if (order1->s_seat <= order2->e_seat && order1->s_seat >= order2->s_seat)
    return 1; // conflict
else if(order2->s_seat <= order1->e_seat && order2->s_seat >= order1->s_seat)
    return 1; //conflict
else
	return 0;// no conflict
}

int can_add_order(theaterrow *this_row, order *this_order) {
  // no seats in this row
  if (this_row->cur_size == 0) {
    return 1; // can be added
  }

  // check through all seats looking for conflict
  for (int i = 0; i < this_row->cur_size; i++) {
    if (conflict(this_row->list_orders[i], this_order) == 1) {
      return 0;
    }
  }
  return 1; // can be added
}

int add_order(theaterrow *this_row, order *this_order) {
  int returned;

  returned = can_add_order(this_row, this_order);

  if (returned == 1) {
      //double size if needed
    if (this_row->max_size == this_row->cur_size) {
      this_row->max_size *= 2;
      this_row->list_orders = realloc(this_row->list_orders, this_row->max_size * sizeof(order *));
    } 
    //then add the order
    this_row->list_orders[this_row->cur_size] = this_order;
    this_row->cur_size++;
    
    return 1; //added
  }
  //could not be added
  return 0;
}

char *get_row_owner(theaterrow *this_row, int seat_num);

char *get_owner(theaterrow **theater, int row, int seat_num) {

  // return owner name
  char *owner = get_row_owner(theater[row], seat_num);

  // seat not taken
  if (owner == NULL) {
    return NULL;
  }

  // name of owner
  return owner;
}

char *get_row_owner(theaterrow *this_row, int seat_num) {
  // loop through seats and see if someone ones the seat return name if yes
  for (int i = 0; i < this_row->cur_size; i++) {
    if (seat_num >= this_row->list_orders[i]->s_seat &&
        seat_num <= this_row->list_orders[i]->e_seat) {
      return this_row->list_orders[i]->name;
    }
  }
  // no one ones the seat
  return NULL;
}

int contains(order *myorder, int seat_no) {
  // check for overlap
  if (seat_no >= myorder->s_seat && seat_no <= myorder->e_seat)
    return 1;

  return 0;
}

void free_order(order *this_order) {
  free(this_order->name);
  free(this_order);
}

void free_row(theaterrow *this_row) {
  for (int i = 0; i < this_row->cur_size; i++)
    free_order(this_row->list_orders[i]);
  free(this_row->list_orders);
  free(this_row);
}

int main(void) {

  int row, start, end, seat;
  char name[MAXLEN], choice[10];
  theaterrow **amc_ = calloc(MAXROWS + 1, sizeof(theaterrow *));

    //make theater rows
  for (int x = 0; x < MAXROWS + 1; x++) {
    amc_[x] = make_empty_row();
  }


  scanf("%s", choice);

  while (1) {
    // break if EXIT typed
    if (strcmp(choice, "EXIT") == 0 || strcmp(choice, "exit") == 0 ||
        strcmp(choice, "Exit") == 0)
      break;

    // BUY
    if ((strcmp(choice, "buy") == 0 || strcmp(choice, "BUY") == 0 ||
         strcmp(choice, "Buy") == 0)) {
      scanf("%d %d %d %s", &row, &start, &end, name);

      // make new order using inputs
      order *neworder = make_order(start, end, name);
      if (add_order(amc_[row], neworder) == 1) {
        printf("SUCCESS\n");
      } else {
        printf("FAILURE\n");
        //free the order
        free_order(neworder);
      }

    }

    // LOOKUP
    else if (strcmp(choice, "lookup") == 0 || strcmp(choice, "Lookup") == 0 ||
             strcmp(choice, "LOOKUP") == 0) {
      scanf("%d %d", &row, &seat);

      // check for owner
      char *owner = get_owner(amc_, row, seat);
      if (owner != NULL) {
        printf("%s\n", owner);
      } else {
        printf("None\n");
      }

      // ANYTHING ELSEche
    } else {
      printf("Invalid Option Try Again\n");
    }


    // get next choice
    scanf("%s", choice);
  }

  // free memory
  for (int i = 0; i < MAXROWS + 1; i++) {
      free_row(amc_[i]);
  }
  free(amc_);

  return 0;
}
