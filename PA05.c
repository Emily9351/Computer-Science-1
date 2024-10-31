#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 25

int idx = 0;

// declare structs
typedef struct customer {
  char name[MAXLEN + 1];
  int points;
} customer;

typedef struct bstnode {
  customer *custPtr;
  int size;
  struct bstnode *left;
  struct bstnode *right;
} bstode;

//create node
bstode *create_node(customer *customer) {
  bstode *newNode = malloc(sizeof(bstode));
  newNode->custPtr = customer;
  newNode->size = 1;
  newNode->left = NULL;
  newNode->right = NULL;

  return newNode;
}

//print customer info
void printCustomer(bstode *node) {
  printf("%s %d", node->custPtr->name, node->custPtr->points);
}

//update size
void nodeSize(bstode *root) {
  if (root->left != NULL)
    root->size += root->left->size;
  if (root->right != NULL)
    root->size += root->right->size;
}

//insert customer
bstode *insert(bstode *root, customer *element) {
  if (root == NULL) {
    bstode *tempNode = create_node(element);
    printCustomer(tempNode);
    printf("\n");
    return tempNode;
  }

  int cmp = strcmp(element->name, root->custPtr->name);

  //if less then root go to the left
  if (cmp < 0)
    root->left = insert(root->left, element);

  //if greater then go to the right
  else if (cmp > 0)
    root->right = insert(root->right, element);

  //add points and print customer
  else {
    root->custPtr->points += element->points;
    printCustomer(root);
    printf("\n");
    free(element);
    return root;
  }

  //update size
  root->size = 1;

  nodeSize(root);

  return root;
}

//compare names
int compare(char *name, char *name2) { 
  return strcmp(name, name2); 
}

//calcualte height in tree
int findHeight(bstode *root, char *name, int height) {
  if (root == NULL)
    return 0;

  //name found
  if (compare(name, root->custPtr->name) == 0) {
    return height;
  }

  //move to left
  if (compare(name, root->custPtr->name) < 0) {
    return findHeight(root->left, name, height + 1);
  }

  //move to right
  if (compare(name, root->custPtr->name) > 0) {
    return findHeight(root->right, name, height + 1);
  }

  return 0;
}

//find the customer in the tree
bstode *searchDepth(bstode *root, char *name) {
  if (root == NULL)
    return NULL;

  //search for name in tree
  int cmp = compare(name, root->custPtr->name);

  if (cmp < 0)
    return searchDepth(root->left, name);
  else if (cmp > 0)
    return searchDepth(root->right, name);
  else
    return root;
}

//subtract points
void subtract(bstode *customerNode, int numPoints) {
  if (customerNode != NULL) {
    customerNode->custPtr->points -= numPoints;

    //if points is negative make it 0
    if (customerNode->custPtr->points < 0) {
      customerNode->custPtr->points = 0;
    }

    printCustomer(customerNode);
    printf("\n");
  }
}

//find the node in tree and delete it
bstode *parent(bstode *root, bstode *node) {
  if (root == NULL || root == node)
    return NULL;

  //root is direct parent of node
  if (root->left == node || root->right == node)
    return root;

  //search for nodes parent in left side tree
  if (compare(node->custPtr->name, root->custPtr->name) < 0)
    return parent(root->left, node);

  //search for nodes parent in right side tree
  else if (compare(node->custPtr->name, root->custPtr->name) > 0)
    return parent(root->right, node);

  return NULL;
}
bstode *minVal(bstode *root) {
  if (root->left == NULL)
    return root;

  else 
    return minVal(root->left);
}
bstode *maxVal(bstode *root) {
  if (root->right == NULL)
    return root;
  else
    return maxVal(root->right);
}
int isLeaf(bstode *node) { 
  return (node->left == NULL && node->right == NULL); 
}
int hasOnlyLeftChild(bstode *node) {
  return (node->left != NULL && node->right == NULL);
}
int hasOnlyRightChild(bstode *node) {
  return (node->left == NULL && node->right != NULL);
}
bstode *findNode(bstode *currentPtr, char *name) {
  //find node in tree
  if (currentPtr != NULL){
      if(strcmp(name, currentPtr->custPtr->name) == 0)
        return currentPtr;

      if(strcmp(name, currentPtr->custPtr->name) < 0)
        return findNode(currentPtr->left, name);

      else
        return findNode(currentPtr->right, name);
  }

  //tree empty
  else
    return NULL;
  }
bstode *deleteNode(bstode *root, char *name) {
  bstode *delnode, *new_del_node, *save_node;
  bstode *parentFound;
  char saveName[MAXLEN + 1];
  int points;

  delnode = findNode(root, name); // find the node

  parentFound = parent(root, delnode); //get parent

  // node is a leaf node
  if (isLeaf(delnode)) {

    //node is only node in the tree
    if (parentFound == NULL) {
      free(root->custPtr);
      free(root); 
      return NULL;
    }

    //node is left child
    if (compare(name, parentFound->custPtr->name) < 0) {
      free(parentFound->left->custPtr);
      free(parentFound->left); 
      parentFound->left = NULL;
    }

    //node is right child
    else {
      free(parentFound->right->custPtr);
      free(parentFound->right);
      parentFound->right = NULL;
    }

    //update size
    while(parentFound != NULL) {
      parentFound->size--;
      parentFound = parent(root, parentFound);
    }

    return root; 
  }

  // node to be deleted only has a left child
  if (hasOnlyLeftChild(delnode)) {

    //node is root of the tree
    if (parentFound == NULL) {
      save_node = delnode->left;
      free(delnode->custPtr);
      free(delnode);   
      return save_node; 
    }

    // node is left child
    if (compare(name, parentFound->custPtr->name) < 0) {
      save_node = parentFound->left; 
      parentFound->left = parentFound->left->left; //adjust parent pointer
      free(save_node->custPtr);
      free(save_node);             
    }

    // node is right child
    else {
      save_node = parentFound->right; 
      parentFound->right = parentFound->right->left; //adjust parent pointer
      free(save_node->custPtr);
      free(save_node);              
    }

     //update size
    while(parentFound != NULL) {
      parentFound->size--;
      parentFound = parent(root, parentFound);
    }

    return root; 
  }

  // node only has right child
  if (hasOnlyRightChild(delnode)) {

    //node is root of the tree
    if (parentFound == NULL) {
      save_node = delnode->right;
      free(delnode->custPtr);
      free(delnode);
      return save_node;
    }

    // node is left child
    if (compare(name, parentFound->custPtr->name) < 0) {
      save_node = parentFound->left;
      parentFound->left = parentFound->left->right;
      free(save_node->custPtr);
      free(save_node);
    }

    // node is right child
    else {
      save_node = parentFound->right;
      parentFound->right = parentFound->right->right;
      free(save_node->custPtr);
      free(save_node);
    }

    //update size
    while(parentFound != NULL) {
      parentFound->size--;
      parentFound = parent(root, parentFound);
    }

    return root;
  }

  //find the new physical node to delete.
  new_del_node = maxVal(delnode->left);
  strcpy(saveName, new_del_node->custPtr->name);
  points = new_del_node->custPtr->points;

  deleteNode(root, saveName); // delete the proper value

  // Restore data to the original node to be deleted
  strcpy(delnode->custPtr->name, saveName);
  delnode->custPtr->points = points;

  return root;
}

//count smaller nodes
int countAllNodes(bstode *root) {
  if (root == NULL) {
    return 0;
  }

  int count = 1; // Count current node

  if (root->left != NULL) {
    count += countAllNodes(
        root->left); // count nodes in the left subtree
  }

  if (root->right != NULL) {
    count += countAllNodes(
        root->right); // count nodes in the right subtree
  }

  return count;
}
int countSmaller(bstode *root, char *name) {
  //check to make sure people in tree
  if (root == NULL) {
    return 0;
  }

  if (strcmp(name, root->custPtr->name) < 0)
    return countSmaller(root->left, name);

  if (strcmp(name, root->custPtr->name) == 0) {
    if (root->left != NULL)
      return root->left->size; 
    else
      return 0;
  }

  int value = 1;

  if (root->left != NULL)
    value += root->left->size;

  return value + countSmaller(root->right, name);
}

//make array
void fillArray(bstode *root, customer **myCustomer) {
  if (root != NULL) {
    fillArray(root->left, myCustomer);
    myCustomer[idx++] = root->custPtr;
    fillArray(root->right, myCustomer);
  }
}
void swap(customer *xp, customer *yp) {
  customer temp = *xp;
  *xp = *yp;
  *yp = temp;
}
int sortCompare(customer *customer1, customer *customer2) {
  //sort by points
  if (customer1->points != customer2->points)
    return customer1->points - customer2->points;
  else //sort by name if points the same
    return compare(customer2->name, customer1->name);
}
int partition(customer **array, int low, int high) {
  //pick a random element and swap it into index low 
  int i = low + rand() % (high - low + 1);
  swap(array[low], array[i]);
  int lowpos = low; 
  low++;            
  
  while (low <= high) {
    // Move low pointer until a value too large for this side
    while (low <= high && sortCompare(array[low], array[lowpos]) >= 0)
      low++;
    // Move the high pointer until a value too small for this side
    while (high >= low && sortCompare(array[high], array[lowpos]) < 0)
      high--;
    
    //swap
    if (low < high)
      swap(array[low], array[high]);
  }
  // Swap pivot value into correct location.
  swap(array[lowpos], array[high]);
  return high; // return the partition point
}
void quickSort(customer **array, int low, int high) {
  //sort if more than one customer
  if (low < high) {
    int split = partition(array, low, high);
    quickSort(array, low, split - 1);
    quickSort(array, split + 1, high);
  }
}
void makeArray(bstode *myRoot) {
  //declare amount of customers
  int numCustomers = countAllNodes(myRoot);

  //create, fill, and sort array
  customer **array = malloc(sizeof(customer *) * numCustomers);

  fillArray(myRoot, array);

  quickSort(array, 0, numCustomers - 1);

  //print and free array
  for (int i = 0; i < numCustomers; i++)
    printf("%s %d\n", array[i]->name, array[i]->points);

  free(array);
}

//free tree
void freeTree(bstode *root) {
  if (root != NULL) {
    freeTree(root->left);
    freeTree(root->right);
    free(root->custPtr);
    free(root);
  }
}

int main() {
  //declare values
  int numCommands = 0;
  int numCustomers = 0;

  //make tree
  bstode *myRoot = NULL;

  scanf("%d", &numCommands);

  for (int i = 0; i < numCommands; i++) {
    char command[14];
    char name[MAXLEN + 1];
    int numPoints;
    scanf("%s", command);

    //add customer to tree or add points to existing customer
    if (strcmp(command, "add") == 0) {
      scanf("%s %d", name, &numPoints);

      //create customer
      customer *currCustomer = malloc(sizeof(customer));
      strcpy(currCustomer->name, name);
      currCustomer->points = numPoints;

      //insert into tree
      myRoot = insert(myRoot, currCustomer);
    }

      //subtract points from existing customer
    else if (strcmp(command, "sub") == 0) {
      scanf("%s %d", name, &numPoints);

      //find customer in tree
      bstode *customerNode = searchDepth(myRoot, name);

      if (customerNode == NULL)
        printf("%s not found\n", name);

      else //subtract points
        subtract(customerNode, numPoints);
    }

      //delete customer
    else if (strcmp(command, "del") == 0) {
      scanf("%s", name);

      //name not found
      if (!searchDepth(myRoot, name))
        printf("%s not found\n", name);

      else {
        //delete customer
        bstode *del = deleteNode(myRoot, name);
        printf("%s deleted\n", name);
      }

    }

      //search tree to find depth of the customer
    else if (strcmp(command, "search") == 0) {
      scanf("%s", name);

      //find customer and height
      bstode *searchNode = searchDepth(myRoot, name);

      int height = findHeight(myRoot, name, 0);

      if (searchNode == NULL)
        printf("%s not found\n", name);

      else
        printf("%s %d %d\n", searchNode->custPtr->name, searchNode->custPtr->points, height);
    }

      //count how many customers are smaller
    else if (strcmp(command, "count_smaller") == 0) {
      scanf("%s", name);

      int depth = countSmaller(myRoot, name);

      printf("%d\n", depth);
    }
  }

  makeArray(myRoot);
  
  freeTree(myRoot);

  return 0;
}

