#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *fp;
    char buff[255];
    int y;
    fp = fopen("pages.txt", "r");
    while(buff){
        fscanf(fp, "%s", buff);
        printf("1 : %s\n", buff);
        y = atoi(buff); // Using atoi()
        printf("%d\n", y);
    }
    
    
    fclose(fp);
    return 0;
}