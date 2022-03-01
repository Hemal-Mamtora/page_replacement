#include <stdio.h>

// Queue implementation in C
#define SIZE 8
#define MEMORY_SIZE 4

void enQueue(int);
void deQueue();
void display();

int page_stream[] = {0, 2, 1, 3, 5, 4, 6, 3, 7, 4, 7, 3, 3, 5, 5, 3, 1, 1, 1, 7, 2, 3, 4, 1};
size_t n = sizeof(page_stream)/sizeof(page_stream[0]);

int memory[SIZE], front = -1, rear = -1;

// int testQueue() {
//   //deQueue is not possible on empty queue
//   deQueue();

//   //enQueue 5 elements
//   enQueue(1);
//   enQueue(2);
//   enQueue(3);
//   enQueue(4);
//   enQueue(5);

//   // 6th element can't be added to because the queue is full
//   enQueue(6);

//   display();

//   //deQueue removes element entered first i.e. 1
//   deQueue();

//   //Now we have just 4 elements
//   display();

//   return 0;
// }

void enQueue(int value) {
  if ((front == rear + 1) || (front == 0 && rear == SIZE - 1))
    printf("\nQueue is Full!!");
  else {
    if (front == -1)
      front = 0;
    rear++;
    memory[rear] = value;
    // printf("\nInserted -> %d", value);
  }
}

void deQueue() {
  if (front == -1)
    printf("\nQueue is Empty!!");
  else {
    // printf("\nDeleted : %d", memory[front]);
    front++;
    // printf("front: %d, rear: %d", front, rear);
    if (front > rear)
      front = rear = -1;
  }
}

// Function to print the queue
void display(int pageFault) {
  if (rear == -1){
      printf("\nQueue is Empty!!!");
  }
  else {
    int i;
    //printf("\nQueue elements are:\n");
    for (i = front; i <= rear; i++)
      printf("%2d  ", memory[i]);
  }
  printf("pagefault: %d\n", pageFault);
}


int IsPageInMemory(int page){
    printf("checking page: %d\n", page);
    for(int i = 0; i < MEMORY_SIZE; i++){
        printf("checking index: %d, value: %d \n",(rear-i + SIZE)%SIZE, memory[(rear-i + SIZE)%SIZE]);
        printf("page == memory[(rear-i+SIZE) mod SIZE]: %d\n", page == memory[(rear-i+SIZE)%SIZE]);
        if(page == memory[(rear-i+SIZE)%SIZE])
            return 1;
    }
    return 0;
}

void FIFO(){
    int page;
    int pageFault = 0;
    display(0);
    for(int i = 0; i < n; i++){
        page = page_stream[i];
        printf("\nPAGE: %d\n", page);
        pageFault = !IsPageInMemory(page);
        deQueue();
        enQueue(page);
        display(pageFault);
    }
}

void initializeMemory(){
    for(int i = 0; i < SIZE; i++) enQueue(-1);
}

int main(){
    initializeMemory();
    FIFO();
    return 0;
}