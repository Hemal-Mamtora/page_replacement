#include <stdio.h>

#define SIZE 8
#define MEMORY_SIZE 4

int page_stream[] = {0, 2, 1, 3, 5, 4, 6, 3, 7, 4, 7, 3, 3, 5, 5, 3, 1, 1, 1, 7, 2, 3, 4, 1};
size_t n = sizeof(page_stream)/sizeof(page_stream[0]);

int memory[SIZE];

int pageMemoryIndex(int page){
    for(int i = 0; i < MEMORY_SIZE; i++)
      if(page == memory[i])
        return i;
    return -1;
}

void insert(int page){
    int temp, current = page;
    for(int i = 0; i < SIZE; i++){
        temp = memory[i];
        memory[i] = current;
        current = temp;
    }
}

void initializeMemory(){
    for(int i = 0; i < SIZE; i++) insert(-1);
}

void display(int pageFault){
    int i;
    for (i = 0; i < SIZE; i++)
        printf("%2d  ", memory[i]);
    printf("pagefault: %d\n", pageFault);
}

void FIFO(){
    int page;
    int pageFault = 0;
    display(0);
    for(int i = 0; i < n; i++){
        page = page_stream[i];
        // printf("\nPAGE: %d\n", page);
        pageFault = pageMemoryIndex(page) == -1;
        if(pageFault)
          insert(page);
        display(pageFault);
    }
}

typedef struct map{
  int key, val;
} map;

map age[MEMORY_SIZE];

int initializeAge(){
    for(int i = 0; i < MEMORY_SIZE; i++){
        age[i] = (map){-1, -1}; 
    }
}

int findLRU(){
    int index = 0;
    int maximum = age[index].val;
    // first fill unfilled memory
    for(int i = 0; i < MEMORY_SIZE; i++){
        // printf("(%d %d %d)", i, age[i].key, age[i].val);
        if(age[i].key == -1){
            // printf("here");
            return i;
        }
    }

    // give the least recently used index, (most aged)
    for(int i = 0; i < MEMORY_SIZE; i++){
        if(age[i].val > maximum){
            maximum = age[i].val;
            index = i;
        }
    }
    return index;
}

int insertLRU(int page, int index){
    int temp, current;

    temp = memory[index];
    memory[index] = page;
    current = temp;

    // maintaining the recently exited pages out of memory as directed in the question
    for(int i = MEMORY_SIZE; i < SIZE; i++){
        temp = memory[i];
        memory[i] = current;
        current = temp;
    }

    age[index] = (map){page, 0}; // frequency of incoming page is 0, incrementAge makes it 1, which is currect Age.

    return 0;
}

int incrementAge(){
    for(int i = 0; i < MEMORY_SIZE; i++){
        if (age[i].key != -1){ // if there is actual page in memory
            age[i].val++; // increment the age
        }
    }
    return 0;
}

int resetAge(int page){
    for(int i = 0; i < MEMORY_SIZE; i++){
        if(age[i].key == page){
            age[i].val = 0;
            return 0;
        }
    }

    // if control reaches here, there is a problem
    return 1;
}

void LRU(){
    initializeAge();
    int page;
    int pageFault = 0;
    int index;

    display(0);
    for(int i = 0; i < n; i++){
        page = page_stream[i];
        pageFault = pageMemoryIndex(page) == -1;
        if(pageFault){
            index = findLRU();
            insertLRU(page, index);
        }
        else{
            resetAge(page);
        }
        incrementAge();
        display(pageFault);
    }
}

int secondChanceBit[MEMORY_SIZE];

int initializeSecondChanceBit(){
    for(int i = 0; i< MEMORY_SIZE; i++){
        secondChanceBit[i] = 0;
    }
    return 0;
}

int roundRobinPointer = 0;
int insertSecondChance(int page){

    for(int i = 0; i < MEMORY_SIZE; i++){
        if(memory[i] == -1){
            memory[i] = page;
            return 0;
        }
    }

    int temp, current;

    while(1){
        if(secondChanceBit[roundRobinPointer] == 1){
            secondChanceBit[roundRobinPointer] = 0;
        }
        else{
            // secondChanceBit at current position is 0
            // hence this page is to be replaced
            temp = memory[roundRobinPointer];
            memory[roundRobinPointer] = page;
            current = temp;

            // maintaining the recently exited pages out of memory as directed in the question
            for(int i = MEMORY_SIZE; i < SIZE; i++){
                temp = memory[i];
                memory[i] = current;
                current = temp;
            }
            // roundRobinPointer is currently at the page which was just replaced.
            // it should be incremented before exiting
            roundRobinPointer = (roundRobinPointer + 1) % MEMORY_SIZE;
            return 0;
        }
        roundRobinPointer = (roundRobinPointer + 1) % MEMORY_SIZE;
    }
}

void secondChance(){
    initializeSecondChanceBit();
    int page;
    int pageFault = 0;
    int index = -1;

    display(0);
    for(int i = 0; i < n; i++){
        page = page_stream[i];
        index = pageMemoryIndex(page);
        pageFault = index == -1;
        if(pageFault){
            insertSecondChance(page);
        }
        else{
            secondChanceBit[index] = 1;
        }
        display(pageFault);
    }
}

int main(){
    int option;
    initializeMemory();
    printf("Choose a page replacement algorithm:\n");
    printf("Press 1 for FIFO\n");
    printf("Press 2 for LRU\n");
    printf("Press 3 for Second Chance\n");
    printf("Your choice: ");
    scanf("%d", &option);
    switch(option){
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