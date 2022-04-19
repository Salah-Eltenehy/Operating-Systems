#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>

int NUM_OF_THREADS =0;
int **resultOfSum[0], numberOfElements = 0;
int arrayDlobal[100][100];/**Max dimension of result array 100 x 100*/
/**explanation*/
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
/**return coln at the specified index in the matrix*/
int* getColumn(int index, int **array);
/**save Data of thread_per_element algorithm*/
void saveData(int RAW, int COL, char name);
/***/
int getRawIndex(int i, int NUM);
int getColumnIndex(int i, int NUM);
/**Needed data for the algorithm*/
struct thread_data_raw{
    int *raw;
    int **array;
    int index;
    int R;
    int C;
};
/**save The result in text file*/
void saveDataForRawAlgorithm(char name)
{
    char *fileName = (char*) malloc(25*sizeof(char));
    fileName[0] = name;
    strcat(fileName, "_per_Raw.txt");
    FILE *file2;
    file2 = fopen(fileName, "w");
    if(file2 == NULL)
    {
        /* File not created hence exit*/
        printf("3- Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
    fputs("Method: A thread per Raw\n", file2);
    fputs("row= ", file2);
    char r[20];
    sprintf(r, "%d", rawsAndCols1[0]);
    fputs(r, file2);
    fputs(" ", file2);
    fputs("col= ", file2);
    char c[20];
    sprintf(c, "%d", rawsAndCols2[1]);
    fputs(c, file2);
    fputs("\n", file2);
    char c2[20];

    for(int i=0; i<rawsAndCols1[0]; i++)
    {
        for(int j=0; j<rawsAndCols2[1]; j++)
        {

            sprintf(c2, "%d", arrayDlobal[i][j]);
            fputs(c2, file2);
            fputs(" ", file2);
        }
        fputs("\n", file2);
    }


}
/**When thread is in running state it goes here*/
void* threadFunctionTwo(void* threadId)
{

    struct thread_data_raw *my_data;
    my_data = (struct thread_data_raw*) threadId;
    int index = my_data->index;
    int *raw = my_data->raw;
    int **arr = my_data->array;
    int sum=0, tempRaw[rawsAndCols1[1]];
    /**Calcualte the result*/
    for(int i=0; i<rawsAndCols2[0]; i++)
    {
        for(int j=0; j<rawsAndCols1[1]; j++)
        {
            sum += raw[j] * arr[j][i];
        }
        tempRaw[i] = sum;
        arrayDlobal[index][i] = sum;
        sum=0;
    }
    /**free space*/
    memset(tempRaw, 0, rawsAndCols1[1]);
}
void threadPerRaw(int **array1, int **array2)
{
    int numOfThreads = rawsAndCols1[0];
    pthread_t threads[numOfThreads];
    int th, raw = 0;
    for(int i=0; i<numOfThreads; i++)
    {
        struct thread_data_raw *tdata = malloc(sizeof(struct thread_data_raw));
        tdata ->raw = array1[i];
        tdata ->array = array2;
        tdata ->index = i;
        th = pthread_create(&threads[i], NULL, threadFunctionTwo, (void*) tdata);
        if(th)
        {
            printf("Error while creating thread %ld\n", i);
            exit(-1);
        }
    }
    for(int i=0; i<numOfThreads; i++)
    {
        pthread_join(threads[i], NULL);
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
    threadPerRaw(data1, data2);
    saveDataForRawAlgorithm(output);
    gettimeofday(&stop, NULL); //end checking time
    printf("Thread per raw\nSeconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
    
    printf("------------THREADS------------\n");
    printf("Output matrix: %d x %d\n", rawsAndCols1[0], rawsAndCols2[1]);
    for(int i=0; i<rawsAndCols1[0]; i++)
    {
        for(int j=0; j<rawsAndCols2[1]; j++)
            printf("%d ", arrayDlobal1[i][j]);
        printf("\n");
    }
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
