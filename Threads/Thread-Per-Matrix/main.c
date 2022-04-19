#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
typedef struct thread_data{
    int *raw;
    int *col;
    int n;
    int *result;
    int R;
    int C;
};


int NUM_OF_THREADS =0;
int **resultOfSum[0], numberOfElements = 0;
/*
 3 x 2    2 x 3
0 -> 0, 0
1 -> 0, 1
2 -> 0, 2
3 -> 1, 0
4 -> 1, 1
5 -> 1, 2
6 -> 2, 0
7 -> 2, 1
8 -> 2, 2



0 1 2
3 4 5
6 7 8
*/
/*Number of raws and cols in each matrix*/
int rawsAndCols1[2], rawsAndCols2[2];
/*check if the char is digit or not*/
bool isDigit(char c);
/*get number of raws and cols of each marix*/
void readRawsAndCols(char input1, char input2);
void getNumberOfRawsAndCols(char arr[], char n);
/**read the two matrices from files*/
void readFile(int numberOfFile, int ** array, char name);
/**thread per matrix algorithm*/
void threadPerMatrix(int **array1,int **array2, char name)
{
    FILE *file;
    char *fileName = (char*) malloc(25*sizeof(char));
    fileName[0] = name;
    strcat(fileName, "_per_Matrix.txt");
    file = fopen(fileName, "w");
    if(file == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
    fputs("Method: A thread per matrix\n", file);
    fputs("row= ", file);
    char r[20];
    sprintf(r, "%d", rawsAndCols1[0]);
    fputs(r, file);
    fputs(" ", file);
    fputs("col= ", file);
    char c[20];
    sprintf(c, "%d", rawsAndCols2[1]);
    fputs(c, file);
    fputs("\n", file);


    int count = rawsAndCols1[0] * rawsAndCols2[1];
    int **res;
    res = (int **) malloc(rawsAndCols1[0] * sizeof(int));
    for(int i=0; i<rawsAndCols1[0]; i++)
        res[i] = (int*) malloc(rawsAndCols2[1]*sizeof(int));
    int rawCount=0, colCount= 0, sum=0, *col;
    col = (int*) malloc(rawsAndCols2[0]*sizeof(int));
    printf("Output matrix: %d x %d\n", rawsAndCols1[0], rawsAndCols2[1]);
    for(int i=0; i<rawsAndCols1[0]; i++)
    {
        for(int j=0; j<rawsAndCols2[1]; j++)
        {
            int s=0;
            for(int k=0; k<rawsAndCols1[1]; k++)
                s += array1[i][k]*array2[k][j];
            char t[20];
            printf("%d\t", s);
            sprintf(t, "%d", s);
            fputs(t, file);
            fputs(" ", file);
        }
        printf("\n");
        fputs("\n", file);
    }
}

int main(int argc, char* argv[])
{
    char input1, input2, output;
    if(argc == 1)
    {
        input1 = 'a';
        input2 = 'b';
        output = 'c';
    }else
    {
        input1 = argv[1][0];
        input2 = argv[2][0];
        output = argv[3][0];
    }
    readRawsAndCols(input1, input2);
    struct timeval stop, start;
    int **data1, **data2;
    data1 = (int **) malloc(rawsAndCols1[0] * sizeof(int));
    data2 = (int **) malloc(rawsAndCols2[0] * sizeof(int));
    for(int i=0; i<rawsAndCols1[0]; i++)
        data1[i] = (int*) malloc(rawsAndCols1[1]*sizeof(int));
    for(int i=0; i<rawsAndCols2[0]; i++)
        data2[i] = (int*) malloc(rawsAndCols2[1]*sizeof(int));
    readFile(1, data1, input1);
    readFile(2, data2, input2);
    printf("Input matrix 1: %d x %d\n", rawsAndCols1[0], rawsAndCols1[1]);
    for(int i=0; i<rawsAndCols1[0]; i++)
    {
        for(int j=0; j<rawsAndCols1[1]; j++)
            printf("%d ", data1[i][j]);
        printf("\n");
    }
    printf("------------THREADS------------\n");
    printf("Input matrix 2: %d x %d\n", rawsAndCols2[0], rawsAndCols2[1]);
    for(int i=0; i<rawsAndCols2[0]; i++)
    {
        for(int j=0; j<rawsAndCols2[1]; j++)
            printf("%d ", data2[i][j]);
        printf("\n");
    }
    
    printf("------------THREADS------------\n");
    gettimeofday(&start, NULL); //start checking time
    //your code goes here
    threadPerMatrix(data1, data2, output);
    gettimeofday(&stop, NULL); //end checking time
    printf("Thread per matrix\nSeconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

}
void readRawsAndCols(char input1, char input2)
{

        char *t;
        char temp[2];
        temp[0] = input1;
        temp[1] = '\0';
        t = temp;
        strcat(t, ".txt");

        FILE *file = fopen(t, "r");

        if(file == NULL)
        {
            printf("0-Can't open file %s\n", t);
            exit(-1);
        }
        char str[100];
        fgets(str, 100, file);
        getNumberOfRawsAndCols(str, 1+'0');
        char *t2;
        char temp2[2];
        temp2[0] = input2;
        temp2[1] = '\0';
        t2 = temp2;
        strcat(t2, ".txt");

        FILE *file2 = fopen(t2, "r");
        if(file == NULL)
        {
            printf("Can't open file\n");
            exit(-1);
        }
        char str2[100];

        fgets(str2, 100, file2);
        getNumberOfRawsAndCols(str2, 2+'0');

}
bool isDigit(char c)
{

    if(c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7' || c=='8' || c=='9')
        return true;
    return false;
}
void getNumberOfRawsAndCols(char arr[], char n)
{

    bool flagRawOrColn = true;
    char temp[20];
    int counter = 0;
    int counter1 = 0, counter2 = 0;
    //printf("state: %s\n", arr);
    char t = n;
    for(int i=0; i<100; i++)
    {
        if(arr[i] == '=')
        {
            i++;
      //      printf("n: %c\n", n);
            while(isDigit(arr[i]) || arr[i] == ' ')
            {
                temp[counter++] = arr[i++];
            }
            if(t == '1')
            {
                rawsAndCols1[counter1++] = atoi(temp);
            }
            else if(t == '2')
            {
        //        printf("hhh: %s %d\n", temp, counter2);
                rawsAndCols2[counter2++] = atoi(temp);
            }
            flagRawOrColn = false;
            memset(temp, 0, 20);
            counter = 0;
        }
    }
}
void readFile(int numberOfFile, int ** array, char name)
{
    char fileName[15];
    fileName[0] = name;
    strcat(fileName, ".txt");
    FILE *file = fopen(fileName, "r");
    memset(fileName, 0, 25);
    if(file == NULL)
    {
        printf("Can't open file\n");
        exit(-1);
    }
    char str[100];
    fgets(str, 100, file);
    if(numberOfFile == 1)
    {
        for(int i=0; i<rawsAndCols1[0]; i++)
        {
            for(int j=0; j<rawsAndCols1[1]; j++)
            {
                fscanf(file, "%d", &array[i][j]);
            }
        }
    }
    else if(numberOfFile == 2){

        for(int i=0; i<rawsAndCols2[0]; i++)
            for(int j=0; j<rawsAndCols2[1]; j++)
            {
                fscanf(file, "%d", &array[i][j]);
            }
    }
}
