/*

1. To run the program use gcc merge_sort.c -o merge -lpthread
2. Usage is like ./merge 
3. I added reading both reading from array in program and from txt file. Both of them are working.


*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>

//int a[] = {34, 25, 47, 19, 23, 14, 27, 9, 2, 5}; //uncomment to use
int a[10];
typedef struct node {
    int i;
    int j;
} nodearr;

void merge(int i, int j) {
        int midpoint = (i+j)/2;
        int arri = i;
        int arrj = midpoint+1;

        int newarr[j-i+1], newarri = 0;

        while(arri <= midpoint && arrj <= j) {
                if (a[arri] > a[arrj])
                        newarr[newarri++] = a[arrj++];
                else                    
                        newarr[newarri++] = a[arri++];
        }

        while(arri <= midpoint) {
                newarr[newarri++] = a[arri++];
        }

        while(arrj <= j) {
                newarr[newarri++] = a[arrj++];
        }

        for (arri = 0 ; arri < (j-i+1) ; arri++)
                a[i + arri] = newarr[arri];

}

void *mergesort(void *a) {
        nodearr *p = (nodearr *)a;
        nodearr n1, n2;
        int midpoint = (p->i+p->j)/2;
        pthread_t tid1, tid2;

        n1.i = p->i;
        n1.j = midpoint;

        n2.i = midpoint + 1;
        n2.j = p->j;

        if (p->i >= p->j) return;

        pthread_create(&tid1, NULL, mergesort, &n1);
        pthread_create(&tid2, NULL, mergesort, &n2);

        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

        merge(p->i, p->j);
        pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
        FILE *fd; 
        //int fd = open(argv[0], O_RDONLY, 0777);
        fd = fopen("mergesort.txt", "r");
        freopen("mergesort.txt", "r", stdin);
        int i;
        for(i = 0; i < 9; i++) {
                scanf("%d, ", &a[i]);
        }
        scanf("%d", &a[9]);
        for (i = 0; i < 10; i++)
        {
                //printf("Number is %d\n\n", a[i]);
        }
        
        nodearr m;
        m.i = 0;
        m.j = 9;
        pthread_t tid;
        pthread_create(&tid, NULL, mergesort, &m);
        pthread_join(tid, NULL);

        for (i = 0; i < 10; i++)
                        printf ("%d ", a[i]);

        printf ("\n");
}