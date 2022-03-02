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
    for (i = 0; i <= SIZE; i++)
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

void LRU(){
    
}

int main(){
    initializeMemory();
    // FIFO();
    LRU();
    return 0;
}