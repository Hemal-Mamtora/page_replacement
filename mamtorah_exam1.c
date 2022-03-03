#include <stdio.h>
#include <stdlib.h>

#define SIZE 8
#define MEMORY_SIZE 4
#define INPUT_SIZE 24

int memory[SIZE];

// SIZE + 1 for storing pagefault
// INPUT_SIZE + 1 for storing initial state and INPUT_SIZE number of columns.s 
int res[SIZE + 1][INPUT_SIZE + 1]; 

int pageMemoryIndex(int page) {
  for (int i = 0; i < MEMORY_SIZE; i++)
    if (page == memory[i])
      return i;
  return -1;
}

void insert(int page) {
  int temp, current = page;
  for (int i = 0; i < SIZE; i++) {
    temp = memory[i];
    memory[i] = current;
    current = temp;
  }
}

void initializeMemory() {
  for (int i = 0; i < SIZE; i++)
    insert(-1);
}

// TODO: modify
void display(int (*res)[INPUT_SIZE + 1], int * input) {
  // Printing the table
  printf("Input pages:      ");
  for (int j = 0; j < INPUT_SIZE + 1; j++) {
      if (j == 0) {
          printf("    ");
      }
      else{
          printf("%2d  ", input[j-1]);
      }
    }
    printf("\n");

  for (int i = 0; i < 9; i++) {
    if (i < 4){
      printf("Memory %d:         ", (i+1));
    }
    else if (i < 8){
      printf("Recently out %d:   ", (i-3));
    }
    else{
      printf("Pagefault:        ");
    }
    for (int j = 0; j < INPUT_SIZE + 1; j++) {
      printf("%2d  ", res[i][j]);
    }
    printf("\n");
  }
}

void fillMatrix(int pageFault, int index) {
  for (int i = 0; i < SIZE; i++) {
    res[i][index] = memory[i];
  }
  res[8][index] = pageFault;
}

void FIFO() {
  int page;
  int pageFault = 0;
  fillMatrix(0, 0);

  // Read File
  FILE *fp = fopen("pages.txt", "r");

  if (fp == NULL) {
    perror("Unable to open the file");
    exit(1);
  }

  int input[INPUT_SIZE];
  int x;
  int i = 0;
  while (fscanf(fp, "%d", &x) == 1) {
    input[i] = x;
    i++;
  }
  fclose(fp);

  int n = sizeof(input) / sizeof(input[0]);

  for (int i = 0; i < n; i++) {
    page = input[i];
    // printf("\nPAGE: %d\n", page);
    pageFault = pageMemoryIndex(page) == -1;
    if (pageFault)
      insert(page);
    fillMatrix(pageFault, i + 1);
  }

  display(res, input);
}

typedef struct map {
  int key, val;
} map;

map age[MEMORY_SIZE];

int initializeAge() {
  for (int i = 0; i < MEMORY_SIZE; i++) {
    age[i] = (map) {-1, -1};
  }
}

int findLRU() {
  int index = 0;
  int maximum = age[index].val;
  // first fill unfilled memory
  for (int i = 0; i < MEMORY_SIZE; i++) {
    // printf("(%d %d %d)", i, age[i].key, age[i].val);
    if (age[i].key == -1) {
      // printf("here");
      return i;
    }
  }

  // give the least recently used index, (most aged)
  for (int i = 0; i < MEMORY_SIZE; i++) {
    if (age[i].val > maximum) {
      maximum = age[i].val;
      index = i;
    }
  }
  return index;
}

int insertLRU(int page, int index) {
  int temp, current;

  temp = memory[index];
  memory[index] = page;
  current = temp;

  // maintaining the recently exited pages out of memory as directed in the
  // question
  for (int i = MEMORY_SIZE; i < SIZE; i++) {
    temp = memory[i];
    memory[i] = current;
    current = temp;
  }

  age[index] = (map) {page, 0}; // frequency of incoming page is 0, incrementAge
                               // makes it 1, which is currect Age.

  return 0;
}

int incrementAge() {
  for (int i = 0; i < MEMORY_SIZE; i++) {
    if (age[i].key != -1) { // if there is actual page in memory
      age[i].val++;         // increment the age
    }
  }
  return 0;
}

int resetAge(int page) {
  for (int i = 0; i < MEMORY_SIZE; i++) {
    if (age[i].key == page) {
      age[i].val = 0;
      return 0;
    }
  }

  // if control reaches here, there is a problem
  return 1;
}

void LRU() {
  initializeAge();
  int page;
  int pageFault = 0;
  int index;

  fillMatrix(0, 0);

  // Read File
  FILE *fp = fopen("pages.txt", "r");

  if (fp == NULL) {
    perror("Unable to open the file");
    exit(1);
  }

  int input[INPUT_SIZE];
  int x;
  int i = 0;
  while (fscanf(fp, "%d", &x) == 1) {
    input[i] = x;
    i++;
  }
  fclose(fp);

  int n = sizeof(input) / sizeof(input[0]);

  for (int i = 0; i < n; i++) {
    page = input[i];
    pageFault = pageMemoryIndex(page) == -1;
    if (pageFault) {
      index = findLRU();
      insertLRU(page, index);
    } else {
      resetAge(page);
    }
    incrementAge();
    fillMatrix(pageFault, i + 1);
  }

  display(res, input);
}

typedef struct frame {
  int page, secondChanceBit;
} frame;

frame memoryFrames[MEMORY_SIZE];

// int insertSecondChance(int page) {


//   int temp, current;

//   while (1) {
//     if (secondChanceBit[roundRobinPointer] == 1) {
//       secondChanceBit[roundRobinPointer] = 0;
//     } else {
//       // secondChanceBit at current position is 0
//       // hence this page is to be replaced
//       temp = memory[roundRobinPointer];
//       memory[roundRobinPointer] = page;
//       current = temp;

//       // maintaining the recently exited pages out of memory as directed in the
//       // question
//       for (int i = MEMORY_SIZE; i < SIZE; i++) {
//         temp = memory[i];
//         memory[i] = current;
//         current = temp;
//       }
//       // roundRobinPointer is currently at the page which was just replaced.
//       // it should be incremented before exiting
//       roundRobinPointer = (roundRobinPointer + 1) % MEMORY_SIZE;
//       return 0;
//     }
//     roundRobinPointer = (roundRobinPointer + 1) % MEMORY_SIZE;
//   }
// }

void initializeSecondChance() {
  for (int i = 0; i < SIZE; i++) {
    memoryFrames[i] = (frame) {-1, 0};
  }
}

int insertSecondChance(int page) {

  for (int i = 0; i < MEMORY_SIZE; i++) {
      if (memoryFrames[i].page == page) {
        memoryFrames[i].secondChanceBit = 1;
        return 0; // no pageFault
      }
  }

  int outIndex = -1;

  // lets see if we find empty page
  for (int i = 0; i < MEMORY_SIZE; i++) {
    if (memoryFrames[i].page == -1) {
      outIndex = i;
      break;
    }
  }

  if (outIndex != -1) {
    for (int i = outIndex; i > 0; i --) {
      memoryFrames[i] = memoryFrames[i-1];
    }
    memoryFrames[0] = (frame){page, 0};
    return 1;
  }

  // if control reaches here, this means, there's page fault, and we need to
  // check second chance bit too.

  // but first lets move the last 4 exited pages
  for (int i = SIZE - 1; i > MEMORY_SIZE; i--) {
    memoryFrames[i] = memoryFrames[i-1];
  }

  for (int i = MEMORY_SIZE - 1; i >= 0; i--) {
    if (memoryFrames[i].secondChanceBit == 1) {
      memoryFrames[i].secondChanceBit = 0; // 2nd chance exhausted
    }
    else{
      // currently at an index to be moved out.
      memoryFrames[MEMORY_SIZE] = memoryFrames[i]; // moved out
      outIndex = i;
      break;
    }
  }

  if (outIndex == -1) {
    // this means, all pageFrames got a second chance.
    // hence, outIndex should be the first page in. (since FIFO) 
    // first page in, would be at index MEMORY_SIZE - 1
    outIndex = MEMORY_SIZE - 1;
  }

  for (int i = outIndex; i > 0; i--) {
    memoryFrames[i] = memoryFrames[i-1];
  }

  // finally, we place the current page
  memoryFrames[0] = (frame){page, 0};
  return 1; // page fault had occured
}

void secondChance() {
  initializeSecondChance();
  int page;
  int pageFault = 0;
  int index = -1;

  fillMatrix(0, 0);

  // Read File
  FILE *fp = fopen("pages.txt", "r");

  if (fp == NULL) {
    perror("Unable to open the file");
    exit(1);
  }

  int input[INPUT_SIZE];
  int x;
  int i = 0;
  while (fscanf(fp, "%d", &x) == 1) {
    input[i] = x;
    i++;
  }
  fclose(fp);

  int n = sizeof(input) / sizeof(input[0]);

  for (int i = 0; i < n; i++) {
    page = input[i];
    pageFault = insertSecondChance(page);

    for (int i = 0; i < SIZE; i++){
      memory[i] = memoryFrames[i].page;
    }

    fillMatrix(pageFault, i + 1);
  }

  display(res, input);
}


int main() {
  int option;
  initializeMemory();
  printf("Choose a page replacement algorithm:\n");
  printf("Press 1 for FIFO\n");
  printf("Press 2 for LRU\n");
  printf("Press 3 for Second Chance\n");
  printf("Your choice: ");
  scanf("%d", &option);
  switch (option) {
  case 1:
    printf("Running FIFO:\n");
    FIFO();
    break;
  case 2:
    printf("Running LRU:\n");
    LRU();
    break;
  case 3:
    printf("Running Second Chance:\n");
    secondChance();
    break;
  default:
    printf("Please choose a valid option!\n");
    break;
  }
  return 0;
}