#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>

static char instruction[1000], input[1000], restOfInput[1000], *path, exportArray[100][100], exportArrayVar[100];
static char arrayForExportStorage[100][100];
static bool flag= true;
static int index1=0; //index of char in the string when we call isContain function
/*take input from user */
char* takeInput();
/*compare two strings return true if they are equal otherwise false*/
bool isEqual(char *str1, char *str2);
/*split the input from user to instruction and related commands to instruction*/
void split();
/*search in the string for a specified char */
bool isContain(char c, char array[]);
/*Count number of spaces in the string*/
int numebrOfSpaces(char arr[]);

void shell();
/*destroy the strings after each operation*/
void damage();
/*execute commands*/
void  execute(char *argument_list[], char instruction[]);
void onChildExit();
void proc_exit()
{
		int wstat;
		pid_t	pid;
		while (true) {
			pid = wait3 (&wstat, WNOHANG, (struct rusage *)NULL );
			if (pid == 0)
				return;
			else if (pid == -1)
				return;
		}
}
int main()
{
    signal (SIGCHLD, proc_exit);
	do
    {
        //get current working directory
        getcwd(&path, 200);
        printf("\n%s$ ", &path);
        //copy the input from user to input array
        strcpy(&input,takeInput());
        if(input[0] != '\0')
        {
            split();
            if(restOfInput[strlen(restOfInput)-1] == '&')
            {
                restOfInput[strlen(restOfInput)-1] = '\0';
                flag = false;
            }
            else
                flag = true;
            shell();
            damage();
        }
    } while (true);
	return 0;
}
void onChildExit()
{
    FILE *file;
    file = fopen("/media/salah/01D769BFB6545890/College/2/data.txt", "a");
    if(file == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
    fputs("Child terminated\n", file);
    fclose(file);
}
bool isContain(char c, char array[])
{
    for(int i=0; i<strlen(array); i++)
    {
        if(array[i] == c)
        {
            index1 = i;
            return true;
        }
    }
    return false;
}
char* takeInput(){
    static char a[1000];
    gets(a);
    return a;

}

bool isEqual(char *str1, char *str2)
{
    int size1 = strlen(str1);
    int size2 = strlen(str2);
    /*loop over the smallest string to check eqauality*/
    if (size1 == size2)
    {
        for (size_t i = 0; i < strlen(str1); i++)
        {
            if (str1[i] != str2[i])
                {
                    return false;
                }
        }

    } else if (size1 < size2)
    {
        for (size_t i = 0; i < strlen(str1); i++)
        {
            for (size_t j = 0; j < strlen(str2); j++)
            {
                if (str1[i] != str2[i])
                    {
                        return false;
                    }
            }

        }
    }
    else
    {
        for (size_t i = 0; i < strlen(str2); i++)
        {
            for (size_t j = 0; j < strlen(str1); j++)
            {
                if (str1[i] != str2[i])
                    {
                        return false;
                    }
            }

        }
    }

    return true;
}
void split() {
    int counter1=0, counter2=0;
    while(input[counter1] != ' ' && input[counter1] != '\0')
    {
        instruction[counter1] = input[counter1];
        counter1++;
    }
    if (input[counter1] != '\0')
    {
        counter1++;
    }
    while(input[counter1] != '\0')
    {

        if(input[counter1] != '"' && input[counter1] != '{' && input[counter1] != '}')
            restOfInput[counter2++] = input[counter1];
        counter1++;

    }
}
void damage()
{
    memset(restOfInput, 0, 1000);
    memset(input, 0, 1000);
    memset(instruction, 0, 1000);
}
int numebrOfSpaces(char arr[])
{
    int number_of_spaces = 0;
    for(int i=0; i<strlen(arr); i++)
        if(arr[i] == ' ')
        {
            number_of_spaces++;
        }
    return number_of_spaces;
}
void shell()
{
    /*Exist from the program*/
    if (isEqual(instruction, "exit"))
    {
        _Exit(0);
    }
    else if (isEqual(instruction, "echo"))
    {
        /*check if there is a variable
        e.g. -> echo &x
        */
        if(isContain('&', restOfInput))
        {
            if(isContain(restOfInput[1], exportArrayVar))
                printf("%s\n", arrayForExportStorage[index1]);
            else
                printf("%s\n", restOfInput);
        }
        else
            printf("%s\n", restOfInput);

    }
    else if(isEqual(instruction, "export"))
    {

        char str = restOfInput[0];
        /*check if there is this variable before*/
        if(isContain(str, exportArrayVar))
        {
            memset(arrayForExportStorage[index1], 0, 100);
            for(int i=2; i<strlen(restOfInput); i++)
            {
                arrayForExportStorage[index1][i-2] = restOfInput[i];
            }
        }
        /*if the variable is new save it in a new space*/
        else
        {

            exportArrayVar[strlen(exportArrayVar)] = str;
            for(int i=2; i<strlen(restOfInput); i++)
            {
                arrayForExportStorage[strlen(exportArrayVar)-1][i-2] = restOfInput[i];
            }
        }

    }
    else if (isEqual(instruction, "cd"))
    {
        char dir[300]; // to save the directory
        /*if the user want to change directory to a folder exists in the current directory*/
        if(restOfInput[0] != '/' && restOfInput[0] != '.')
        {
            getcwd(dir, 300);
            int size = strlen(dir);
            dir[size] = '/';size++;
            int i=0;
            while(restOfInput[i] != '\0')
            {
                dir[size]= restOfInput[i];size++;i++;
            }
        }else
        {
            strcpy(dir, restOfInput);
        }
        chdir(dir);
        memset(dir, 0, 300);
    }
     else
    {
        /*if the input not equal
            exist, echo, cd, and export
          call execvp fuction
        */
        if (restOfInput[0] != '\0') //check if there is a related command to the instruction or not
        {
            /*check if the command is a stored or not using export*/
            if(restOfInput[0] != '&')
            {
                int numOfSpaces = numebrOfSpaces(restOfInput);
                char* argument_list[3+numOfSpaces];
                argument_list[0] = instruction;
                /*if number of spaces greater than zero this means there are different coomands
                    e.g. ls -a -l -h
                                    */
                if(numOfSpaces > 0)
                {
                    char *token;
                    token = strtok(restOfInput, " ");
                    argument_list[1] = token;
                    for(int i=1; i<= numOfSpaces; i++)
                    {
                        token = strtok(NULL, " ");
                        argument_list[i+1] = token;
                    }
                    argument_list[2+numOfSpaces] = NULL;
                }
                else if(numOfSpaces == 0)
                {
                    argument_list[1] = restOfInput;
                    argument_list[2] = NULL;
                }
                execute(argument_list, instruction);
            }
            else
            {
                isContain('&', restOfInput);
                int numOfSpaces = numebrOfSpaces(arrayForExportStorage[index1]);
                char* argument_list[3+numOfSpaces];
                argument_list[0] = instruction;
                if(numOfSpaces > 0)
                {
                    char *token;
                    token = strtok(arrayForExportStorage[index1], " ");
                    argument_list[1] = token;
                    for(int i=1; i<= numOfSpaces; i++)
                    {
                        token = strtok(NULL, " ");
                        argument_list[i+1] = token;
                    }
                    argument_list[2+numOfSpaces] = NULL;
                }
                else if(numOfSpaces == 0)
                {
                    argument_list[1] = arrayForExportStorage[index1];
                    argument_list[2] = NULL;
                }
                execute(argument_list, instruction);
            }

        }else
         {
            char* argument_list[2];
            argument_list[0] = instruction;
            argument_list[1] = NULL;
            execute(argument_list, instruction);

         }

    }
}
void  execute(char *argument_list[], char instruction[])
{
     pid_t  pid;
     int    status;

     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
            onChildExit();
          if (execvp(instruction, argument_list) < 0) {     /* execute the command  */
               printf("ERROR: Invalid Instruction\n");
               exit(1);
          }

     }
     else {                                  /* for the parent:      */
          if(flag)
          {
             waitpid(pid, &status, WUNTRACED | WCONTINUED);
          }
                /* wait for completion  */
     }
}
