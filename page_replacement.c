#include <stdio.h>

#define SIZE 8
#define MEMORY_SIZE 4

int page_stream[] = {0, 2, 1, 3, 5, 4, 6, 3, 7, 4, 7, 3, 3, 5, 5, 3, 1, 1, 1, 7, 2, 3, 4, 1};
size_t n = sizeof(page_stream)/sizeof(page_stream[0]);

int memory[SIZE];

int IsPageInMemory(int page){
    for(int i = 0; i < MEMORY_SIZE; i++)
      if(page == memory[i])
        return 1;
    return 0;
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
        pageFault = !IsPageInMemory(page);
        if(pageFault)
          insert(page);
        display(pageFault);
    }
}

typedef struct map{
  int key, val;
} map;

//  TODO: change counter to age and remove the one with maximum age.
//  TODO: on each iteration increment the age of all the pages,
//        insert the page with age 0, so increment would make it 1,
//        it would be uniform
//  TODO: 
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
        pageFault = !IsPageInMemory(page);
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

int main(){
    initializeMemory();
    // FIFO();
    // LRU();
    return 0;
}