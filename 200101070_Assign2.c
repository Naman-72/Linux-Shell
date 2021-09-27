#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
int noOfWords(char *command);
void shellPrompt();
static char *currentDirectory;
static char *currDirr;
int commandHandler(char *);
char* HOME;
char* SHELL;
char* TERM;
char* USER;
/********************************************************************************************************/
/********************************************************************************************************/
/**************************************DEALING WITH PIPES************************************************/

/**************************************CHECKING PIPE PRESENT***********************************************/
int pipeCheck(char *ptr)
{
    char *pn = (char *)malloc(100 * sizeof(char));
    strcpy(pn, ptr);
    int i = 0;
    while (pn[i] != '\0')
    {
        if (pn[i] == '|')
        {
            return 1;   // IF | SYMBOL PRESENT IT INCREASES BY ONE
        }
        i++;
    }
    free(pn);
    return i; // RETURNING NO OF | symbol present
}
/********************************************************************************************************/
/*******************************IF PIPE PRESENT IMPLEMENT PIPE*******************************************/
void implementPipe(char *ptr)
{
    char *pn = (char *)malloc(100 * sizeof(char));
    char **token = (char **)malloc(3 * sizeof(char *));
    char **tokenInput = (char **)malloc(40 * sizeof(char *));
    char **tokenOutput = (char **)malloc(40 * sizeof(char *));
    strcpy(pn, ptr);
    int j = 0;
    char *p = strtok(pn, "|");
    // SEPERATING INTO TWO TOKEN ONE BEFORE PIPE ONE AFTER
    while (p != NULL)
    {
        token[j] = p;
        //    printf("%s\n",token[j]);
        j++;
        p = strtok(NULL, "|");
    }
    
    //printf("\n");printf("\n");
    token[j] = NULL;
    strcpy(pn, ptr);
    j = 0;
    /* while(token[j]!=NULL)
   {                                   // JUST FOR CHECK WHETHER IT IS SEPERATING PROPERLY OR NOT
   printf("%s ",token[--j]);
   j++;  
  }
  printf("\n");
  j = 0;
  */
  // STORING AND SEPERATING THE BEFORE | STRING AND DIVIDING INTO WORDS
    p = strtok(pn, "|");
    // printf("%s\n\n",p);
    char *q = strtok(p, " ");
    while (q != NULL)
    {
        tokenInput[j] = q;
        //  printf("%s\n",tokenInput[j]);
        j++;
        q = strtok(NULL, " ");
    }
    tokenInput[j] = NULL;
    j = 0;
    
    // JUST FOR CHECK WHETHER IT IS SEPERATING PROPERLY OR NOT
    /* while(tokenInput[j]!=NULL)
   {
     printf("%s\n",tokenInput[j]);
     j++;  
   }
   if(tokenInput[j]==NULL)
   {
    printf("0\n\n");
   }
  printf("\n");
  printf("\n");
  */
    //strcpy(pn,ptr);
    // STORING AND SEPERATING THE AFTER | STRING AND DIVIDING INTO WORDS
    j = 0;
    q = strtok(token[1], " ");
    while (q != NULL)
    {
        tokenOutput[j] = q;
        // printf("%s\n",tokenOutput[j]);
        j++;
        q = strtok(NULL, " ");
    }
    tokenOutput[j] = NULL;
    /*   if(tokenOutput[j]==NULL)
   {
    printf("0\n\n");
   }
   */
    j = 0;
    //  strcpy(pn,ptr);
    int h = 0;
    /*  while(tokenInput[h]!= NULL)
   {
     printf("%s\n",tokenInput[h]);
     h++;
   }
   printf("\n");
  */
  // NOW IMPLEMENETING LOGIC OF FILE DESCRIPTORS AND IMPLEMENTING FORK
    int fd[2];
    if (pipe(fd) == -1)
    {
        return;
    }
    int pid1 = fork();
    if (pid1 < 0)
    {
        return;
    }
    if (pid1 == 0)
    {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        if (execvp(tokenInput[0], tokenInput) < 0)
        {
            printf("U ARE NOT GIVING RIGHT VALUES BEFORE PIPE\n");
            exit(0);
        }
        execvp(tokenInput[0], tokenInput);
    }
    int pid2 = fork();
    if (pid2 < 0)
    {
        return;
    }
    if (pid2 == 0)
    {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        if (execvp(tokenOutput[0], tokenOutput) < 0)
        {
            printf("U ARE NOT GIVING RIGHT VALUES AFTER PIPE\n");
            exit(0);
        }
        execvp(tokenOutput[0], tokenOutput);
        exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    free(pn);
    free(tokenInput);
    free(tokenOutput);
    free(token);

    return;
}
/********************************************************************************************************/

/********************************************************************************************************/
/********************************************************************************************************/

// FOR CHECKING IF OUTPUT REDIRECTION IS THERE
int RedirectionOutputCheck(char *ptr)
{
    char *pn = (char *)malloc(100 * sizeof(char));
    strcpy(pn, ptr);
    int i = 0;
    int k = 0;
    // FINDING HOW MANY TIMES > SYMBOL CAME
    while (pn[i] != '\0')
    {
        if (pn[i] == '>')
        {
            k++;
        }
        i++;
    }
    free(pn);
    return k;
}

// FOR CHECKING IF INPUT REDIRECTION IS THERE
int RedirectionInputCheck(char *ptr)
{
    char *pn = (char *)malloc(100 * sizeof(char));
    strcpy(pn, ptr);
    int i = 0;
    int k = 0;
    // FINDING HOW MANY TIMES < SYMBOL CAME
    while (pn[i] != '\0')
    {
        if (pn[i] == '<')
        {
            k++;
        }
        i++;
    }
    free(pn);
    return k;
}

// FOR CHECKING IF APPEND REDIRECTION IS THERE
int RedirectionAppendCheck(char *ptr)
{
    char *pn = (char *)malloc(100 * sizeof(char));
    strcpy(pn, ptr);
    int i = 0;
    int k = 0;
    // FINDING HOW MANY TIMES >> SYMBOL CAME
    while (pn[i] != '\0')
    {
        if (pn[i] == '>')
        {
            k++;
        }
        i++;
    }
    free(pn);
    return k;
}
// FOR IMPLEMETING OUTPUT OPERATION
void ImplementOutputRedirection(char *ptr)
{
  // IF THE SYMBOL > PRESENT 1 TIME IMPLEMENTING THIS
    char *pn = (char *)malloc(100 * sizeof(char));
    strcpy(pn, ptr);
    char **token = (char **)malloc(3 * sizeof(char *));
    int j = 0;
    char **tokenInput = (char **)malloc(40 * sizeof(char *));
    char *p = strtok(pn, ">");
    // DIVIDING INTO TWO STRINGS BEFORE AND AFTER > USING TOKENS
    while (p != NULL)
    {
        token[j] = p;
        // printf("%s\n",token[j]);  // JUST FOR CHECK WHETHER CORRECT OR NOT
        j++;
        p = strtok(NULL, ">");
    }
    token[j] = NULL;
    strcpy(pn, ptr);
    j = 0;
    p = strtok(pn, ">");
    // DIVISON INTO WORDS
    char *q = strtok(p, " ");
    while (q != NULL)
    {
        tokenInput[j] = q;
        //  printf("%s\n",tokenInput[j]); // JUST FOR CHECK WHETHER CORRECT OR NOT
        j++;
        q = strtok(NULL, " ");  // DIVISON USING SPACE
    }
    tokenInput[j] = NULL;
    j = 0;
     // JUST FOR CHECK
    /* {   while (tokenInput[j] != NULL)
        printf("%s\n",tokenInput[j]);
        j++;
    }
    if(tokenInput[j]== NULL)
    {
      printf("0\n\n");
    }
  */
  // OPENING FILE AND PERFORMING OPERATION IN IT
    int file = open(token[1], O_WRONLY | O_CREAT | O_TRUNC, 0666); //HERE I AM USING O_WRONLY,O_CREAT,O_TRUNC
    if (file == -1)
    {
        printf("FILE NOT OPEN SUCCESSFULLY\n");
        return;
    }
    /*if( file > -1)   // JUST FOR CHECK
    {
         remove(token[1]);
         file = open(token[1],O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO,0777);
    }
    */
    // IMPLEMENTING FORK
    int pid1 = fork();
    if (pid1 < 0)
    {
        return;
    }
    if (pid1 == 0)
    {
        dup2(file, 1);
        close(file);
        if (execvp(tokenInput[0], tokenInput) < 0)
        {
            exit(0);
        }
        execvp(tokenInput[0], tokenInput);
        exit(0);
    }
    close(file);// CLOSING IT OTHERWISE IT WILL PRINT IN FILE ONLY
    waitpid(pid1, NULL, 0);
    free(pn);
    free(token);
    return;
}
// FOR IMPLEMETING INPUT OPERATION
void ImplementInputRedirection(char *ptr)
{
// OTHER INFO THAT I HAVE NOT COMMENTED SIMILIAR TO OUTPUT REDIRECTION 
//ONLY DIFFERENCE HERE IS USE OF DIFFERENT FLAGS AND DIVISION ON BASIS OF <
// ALSO HERE FILE DESCRIPTOR CHANGED
    char *pn = (char *)malloc(100 * sizeof(char));
    strcpy(pn, ptr);
    char **token = (char **)malloc(3 * sizeof(char *));
    int j = 0;
    char **tokenInput = (char **)malloc(40 * sizeof(char *));
    char *p = strtok(pn, "<");
    while (p != NULL)
    {
        token[j] = p;
        // printf("%s\n",token[j]);
        j++;
        p = strtok(NULL, "<");
    }
    token[j] = NULL;
    strcpy(pn, ptr);
    j = 0;
    p = strtok(pn, "<");
    char *q = strtok(p, " ");
    while (q != NULL)
    {
        tokenInput[j] = q;
        //  printf("%s\n",tokenInput[j]);
        j++;
        q = strtok(NULL, " ");
    }
    tokenInput[j] = NULL;
    /*j = 0;
    while (tokenInput[j] != NULL)
    {
        printf("%s\n",tokenInput[j]);
        j++;
    }
    if(tokenInput[j]== NULL)
    {
      printf("0\n\n");
    }
    */

    int file = open(token[1], O_RDONLY); //NOTE I AM USING HERE O_RDONLY
    if (file == -1)
    {
        printf("CHECK AGAIN FILE NOT PRESENT\n");
        return;
    }

    int pid1 = fork();
    if (pid1 < 0)
    {
        return;
    }
    if (pid1 == 0)
    {
        dup2(file, 0);  // HERE I AM USING 0 IN FILE DESCRIPTOR
        close(file);
        if (execvp(tokenInput[0], tokenInput) < 0)
        {
            exit(0);
        }
        execvp(tokenInput[0], tokenInput);
        exit(0);
    }
    close(file);
    waitpid(pid1, NULL, 0);
    free(pn);
    free(token);
    return;
}
// FOR IMPLEMETING APPEND OPERATION
void ImplementAppendRedirection(char *ptr)
{
// OTHER INFO THAT I HAVE NOT COMMENTED SIMILIAR TO OUTPUT REDIRECTION 
//ONLY DIFFERENCE HERE IS USE OF DIFFERENT FLAGS AND DIVISION ON BASIS OF >>
// ALSO HERE FILE DESCRIPTOR CHANGED
    char *pn = (char *)malloc(100 * sizeof(char));
    strcpy(pn, ptr);
    char **token = (char **)malloc(3 * sizeof(char *));
    int j = 0;
    char **tokenInput = (char **)malloc(40 * sizeof(char *));
    char *p = strtok(pn, ">>");
    while (p != NULL)
    {
        token[j] = p;
        // printf("%s\n",token[j]);
        j++;
        p = strtok(NULL, ">>");
    }
    token[j] = NULL;
    strcpy(pn, ptr);
    j = 0;
    p = strtok(pn, ">>");
    char *q = strtok(p, " ");
    while (q != NULL)
    {
        tokenInput[j] = q;
        //  printf("%s\n",tokenInput[j]);
        j++;
        q = strtok(NULL, " ");
    }
    tokenInput[j] = NULL;
    j = 0;
    /*   while (tokenInput[j] != NULL)
    {
        printf("%s\n",tokenInput[j]);
        j++;
    }
    if(tokenInput[j]== NULL)
    {
      printf("0\n\n");
    }
  */
    int file = open(token[1], O_CREAT | O_WRONLY | O_APPEND, 0666); //HERE I AM USING O_APPEND AND
                                                                    // I REMOVED O_TRUNC
    if (file == -1)
    {
        printf("FILE NOT OPEN SUCCESSFULLY\n");
        return;
    }
    /*if( file > -1)
    {
         remove(token[1]);
         file = open(token[1],O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO,0777);
    }
    */
    int pid1 = fork();
    if (pid1 < 0)
    {
        return;
    }
    if (pid1 == 0)
    {
        dup2(file, 1);
        close(file);
        if (execvp(tokenInput[0], tokenInput) < 0)
        {
            exit(0);
        }
        execvp(tokenInput[0], tokenInput);
        exit(0);
    }
    close(file);
    waitpid(pid1, NULL, 0);
    free(pn);
    free(token);
    return;
}
void ImplementBothRedirection(char *ptr)
{
    char *pn = (char *)malloc(100 * sizeof(char));
    strcpy(pn, ptr);
    char **token = (char **)malloc(3 * sizeof(char *));
    int j = 0;
    char **tokenInput = (char **)malloc(40 * sizeof(char *));
    char *p = strtok(pn, "<");
    while (p != NULL)
    {
        token[j] = p;
        // printf("%s\n",token[j]);
        j++;
        p = strtok(NULL, "<");
    }
    token[j] = NULL;
    strcpy(pn, ptr);
    j = 0;
    p = strtok(pn, "<");
    char *q = strtok(p, " ");
    while (q != NULL)
    {
        tokenInput[j] = q;
        //  printf("%s\n",tokenInput[j]);
        j++;
        q = strtok(NULL, " ");
    }
    tokenInput[j] = NULL;
    /*j = 0;
    while (tokenInput[j] != NULL)
    {
        printf("%s\n",tokenInput[j]);
        j++;
    }
    if(tokenInput[j]== NULL)
    {
      printf("0\n\n");
    }
    */
   // printf("%s\n",token[1]);
    char**tokenout = malloc(3*sizeof(char*));
    char* duplicate = malloc(40*sizeof(char));
     duplicate =strcpy(duplicate,token[1]);
   // printf("%s\n",duplicate);
    char*p2 = strtok(token[1],">");
    //printf("%s\n",p2);
  //  printf("%s\n",duplicate);
   q = strtok(duplicate,">");
   q = strtok(NULL,">");
   //printf("%s\n",q);
   /*  while (q != NULL)
    {
        tokenout[j] = q;
        //  printf("%s\n",tokenInput[j]);
        j++;
        q = strtok(NULL, " ");
    }
    tokenout[j] = NULL;
  */ 
   int file = open(p2, O_RDONLY); //NOTE I AM USING HERE O_RDONLY
    if (file == -1)
    {
        printf("CHECK AGAIN FILE NOT PRESENT\n");
        return;
    }
   int file2= open(q, O_WRONLY | O_CREAT | O_TRUNC, 0666); //HERE I AM USING O_WRONLY,O_CREAT,O_TRUNC
    if (file2 == -1)
    {
        printf("FILE NOT OPEN SUCCESSFULLY\n");
        return;
    }
    int pid1 = fork();
    if (pid1 < 0)
    {
        return;
    }
    if (pid1 ==0)
    {
        dup2(file, 0);  // HERE I AM USING 0 IN FILE DESCRIPTOR
        close(file);
        dup2(file2,1);
        close(file2);
        if (execvp(tokenInput[0], tokenInput) < 0)
        {
            exit(0);
        }
        execvp(tokenInput[0], tokenInput);
        exit(0);
    }
    close(file);
    close(file2);
    waitpid(pid1, NULL, 0);
    free(pn);
    free(token);
    free(tokenout);
    free(duplicate);
    return;






 /* // IMPLEMENTING SIMILIAR TO ABOVE 
    char *pn = (char *)malloc(100 * sizeof(char));
    strcpy(pn, ptr);
    char **token = (char **)malloc(4 * sizeof(char *));
    int j = 0;
    char **tokenInput = (char **)malloc(40 * sizeof(char *));
    char *p = strtok(pn, ">");
    while (p != NULL)
    {
        token[j] = p;
        // printf("%s\n",token[j]); store output file name in token[1]
        j++;
        p = strtok(NULL, ">");
    }
    token[j] = NULL;
    // strcpy(pn,ptr);
    j = 0;
    /*  while (token[j] != NULL)
    {
        printf("%s\n",token[j]);  store output file name in token[1]
        j++;
    }
    */
    //ImplementInputRedirection(token[0]);
    // ImplementInputRedirection(token[1]);

    /*char *pn2 = (char *)malloc(100 * sizeof(char));
    strcpy(pn2, token[0]);
    char **token2 = (char **)malloc(3 * sizeof(char *));
    int j2 = 0;
    char **tokenInput2 = (char **)malloc(40 * sizeof(char *));
    char **tokenInput3 = (char **)malloc(40 * sizeof(char *));
    /*  char *p2 = strtok(pn2, "<");
    while (p2 != NULL)
    {
        token2[j2] = p2;
        j2++;
        p2 = strtok(NULL, "<");
    }
    token2[j2] = NULL;
    j2 = 0;
    printf("%s\n",token2[0]);
    */
/*    char *q2 = strtok(token[0], "<");
    while (q2 != NULL)
    {
        tokenInput2[j2] = q2;
        //printf("%s\n",tokenInput2[j2]);  store input filename in tokenInput[1]
        j2++;
        q2 = strtok(NULL, "<");
    }
    tokenInput2[j2] = NULL;
    j2 = 0;
    
    /* while (tokenInput2[j2] != NULL)
    {
        printf("%s\n",tokenInput2[j2]);
        j2++;
    }
    if(tokenInput2[j2]== NULL)
    {
      printf("0\n\n");
    }
    */
  /*  printf("%s\n%s\n%s\n", token[1], tokenInput2[0], tokenInput2[1]);

    int file = open("input.txt", O_RDONLY | O_CREAT, 0666); //
    if (file == -1)
    {
        printf("CHECK AGAIN FILE NOT PRESENT\n");
    }
    else
    {
    }
    // printf("%s\n",k);
    printf("%s\n", tokenInput2[1]);
    int file2 = open(token[1], O_WRONLY | O_CREAT | O_TRUNC, 0666); //
    if (file2 == -1)
    {
        printf("FILE NOT OPEN SUCCESSFULLY\n");
        return;
    }
    else
    {
    }

    int j3 = 0;
    char *q3 = strtok(tokenInput2[0], " ");
    while (q3 != NULL)
    {
        tokenInput3[j3] = q3;
        // printf("%s\n",tokenInput3[j3]);
        j3++;
        q3 = strtok(NULL, " ");
    }
    tokenInput3[j3] = NULL;
    j3 = 0;
*/
    /* while (tokenInput3[j3] != NULL)
    {
        printf("%s\n",tokenInput3[j3]);
        j3++;
    }
    if(tokenInput3[j3]== NULL)
    {
      printf("0\n\n");
    }
    */

    // close(file3);
   /* int pid1 = fork();
    if (pid1 < 0)
    {
        return;
    }
    if (pid1 == 0)
    {
        dup2(file, 0);
        close(file);
        if (execvp(tokenInput2[0], tokenInput2) < 0)
        {
            exit(0);
        }
        dup2(file2, 1);
        close(file2);
        execvp(tokenInput2[0], tokenInput2);
        exit(0);
    }
    close(file2);
    close(file);

    waitpid(pid1, NULL, 0);
*/
    /**************************************************************/
    /* while (q != NULL)
    {
        tokenInput[j] = q;
        //  printf("%s\n",tokenInput[j]);
        j++;
        q = strtok(NULL, " ");
    }
    tokenInput[j] = NULL;
    j = 0;
   while (tokenInput[j] != NULL)
    {
        printf("%s\n",tokenInput[j]);
        j++;
    }
    if(tokenInput[j]== NULL)
    {
      printf("0\n\n");
    }
 */
    /*  int file = open(token[1],O_CREAT|O_WRONLY|O_APPEND,0666);//
    if(file == -1)
    {
     printf("FILE NOT OPEN SUCCESSFULLY\n");
     return;
    }
    /*if( file > -1)
    {
         remove(token[1]);
         file = open(token[1],O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO,0777);
    }
    */
    /*int pid1 = fork();  
    if (pid1 < 0)
    {
        return ;
    }
    if (pid1 == 0)
    {   dup2(file,1);
        close(file);
        if (execvp(tokenInput[0], tokenInput) < 0)
        {
            exit(0);
        }
        execvp(tokenInput[0], tokenInput);
        exit(0);
    }
    close(file);
    waitpid(pid1, NULL, 0);
    
    free(pn);
    free(pn2);
    free(token);
    free(tokenInput2);
    free(tokenInput);
    */
    return;
}

/********************************************************************************************************/
/********************************************************************************************************/
/*****************************************DEALING WITH HISTORY*******************************************/
/******************************************STORING HISTORY***********************************************/
void storeInput(char *command)
{
    // STORING COMMAND 
    char *Directory = getcwd(currDirr, 1024);
    //STORING DIRECTORY BECAUSE I HAVE TO CHANGE DIRECTORY AS history is in  /tmp
    FILE *fptr;
    chdir("/tmp");
    fptr = fopen("history.txt", "a+");
    fprintf(fptr, "%s", command);
    // STORING COMMAND IN HISTORY FILE LINE WISE
    fprintf(fptr, "\n");
    fclose(fptr);
 // NOW AGAIN CHANGING TO PRESENT DIRECTORY
    chdir(Directory);
    return;
}

/*********************************RETRIEVING HISTORY***************************************/
void printHistory()
{
    //STORING DIRECTORY BECAUSE I HAVE TO CHANGE DIRECTORY AS history is in  /tmp
    char *Directory = getcwd(currDirr, 1024);   
    //WE HAVE TO READ THE HISTORY FILE AND PRINT THE HISTORY
    FILE *fptr;
    chdir("/tmp");
    fptr = fopen("history.txt", "r");   // READING A FILE
    char c;
    c = fgetc(fptr);
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(fptr);
    }
    fclose(fptr);
    // NOW AGAIN CHANGING TO PRESENT DIRECTORY
    chdir(Directory);
    return;
}
/********************************************************************************************************/
/********************************************************************************************************/
/*******************************************NO OF WORDS**********************************************/
int noOfWords(char *command)
{                                         // FINDING NO OF WORDS
    char *p = strtok(command, " ");
    int j = 0;
    while (p != NULL)
    {
        p = strtok(NULL, " ");
        j++;
    }
    return j;
}
   //TRYING TO IMPLEMENT PIPE BUT IT WAS GIVING SOME ERROR SO I LEFT HERE AND I HAVE IMPLEMENTED ABOVE 
/* void gotopipe()          
{   int  i=0;
    char*array[noOfWords+!];
    char *p = strtok(command, " ");
    while (p != NULL)
    {
        array[i++] = p;
        p = strtok(NULL, " ");
    }
    int fd[2];
    //fd[0] =read
    //fd[1]=write
    if(pipe(fd)==-1)
    {
    printf("AN ERROR OCURRED\n");
                   return 1;
    }
    int id = fork();
    id(id == -1)
    {
      printf("AN ERROR OCURRED\n");
                   return 1;
    }
    if(id ==0)
    {
      
      close(fd[0]);
    }
}*/

/***********************************************************************************************************/
/***********************************************************************************************************/
/********************************************PRINTING SHELL*************************************************/
void shellPrompt()
{
    // We print the prompt in the form "<cwd> >"
    printf("%s > ", getcwd(currentDirectory, 1024));
    return;
}
/***********************************************************************************************************/
/***********************************************************************************************************/

/***********************************************************************************************************/
/***********************************************************************************************************/
/*************************************************MAIN******************************************************/
/***********************************************************************************************************/
int main()
{ // i was first trying to store path and seperating by : But it got some error so i am doing by other method
    /* int noOfPaths = 0;
    int e = 0;
    while (pathcombined[e] != '\0')
    {
        if (pathcombined[e] == ':')
        {
           noOfPaths++;
        }
        e++;
    }
    printf("%d",noOfPaths);
    char *arrayOfPaths[noOfPaths + 1];
    int s= 0;
    char *q = strtok(pathcombined, ":");
    while (q != NULL)
    {
        arrayOfPaths[s++] = q;
        q= strtok(NULL, ":");
    }
    printf("%s\n",arrayOfPaths[5]);
    */
    SHELL= getenv("SHELL");
    USER=getenv("USER");
    HOME= getenv("HOME");
    TERM= getenv("TERM");
    int compare = 1;
    char *command = (char *)malloc(100 * sizeof(char));
    char *curr_path;
    //int h = strcmp(command, "history");
    curr_path = getcwd(curr_path, 100);
    do
    {
        char **token = (char **)malloc(40 * sizeof(char *));//USEFUL FOR TOKEN ->DIVISION OF STRING INTO WORDS
        command[0] = '\0';
        int i = 0;
        shellPrompt(); // PRINTING THE CURRENT PRESENT DIRECTORY
        // SCANNING OF INPUT UNTIL U GET A NEW LINE CHARACTER
        while (1)
        {
            char c;
            scanf("%c", &c);
            if (c == '\n')
            {
                command[i] = '\0';  // IF WE GET A NEW LINE CHARACTER REPLACE IT BY NULL CHARACTER
                                    //  WHICH WILL BE HELPFUL FOR STRING TERMINATION
                break;
            }
            else
            {
                command[i] = c;  // ELSE STORE CHARACTER
            }
            i++;
        }
         // IF INPUT CONTAINS NOTHING 
        if ((strcmp(command, "\0") == 0))
        {   //DO NOT STORE
            continue;
        }
        else
        {
          // ELSE STORE
            storeInput(command);
        }
        // HERE I WANT TO REMAIN HAVING ACCESS TO THE REAL INPUT BECAUSE ON STRTOK IT WILL MODIFY COMMAND
        // SO I DEFINED AND CREATED A POINTER THAT STORES EXACTLY SAME VALUE AS INPUT
        char *ptr = (char *)malloc(100 * sizeof(char));
        ptr = strcpy(ptr, command);

        // I HAVE EARLIER DEFINED TOKEN SO I AM NOT DEFINING HERE

        //char*token[40];   // USE FOR DIVIDING THE INPUT INTO SEPERATE WORDS
        int j = 0;
        char *p = strtok(command, " ");  //DIVIDING BY SPACE
        while (p != NULL)
        {
            token[j++] = p;
            p = strtok(NULL, " ");        //DIVIDING BY SPACE
        }
        token[j] = NULL;
        // HERE IT WAS BREAKING AND WORKING INTO TOKENS
        // CHECKING AND PRINTING ENVIRONMENTAL VARIABLES
        char *duplicate;  // AGAIN STORING INTO A DUPLICATE VARIABLE WHICH I WILL USE TO CHECK DIRECT
        strcpy(duplicate, ptr);
        int storej = j;
        // IF cd_command == 0 then i have done implemnetation of cd downward
        int cd_command = strncmp(duplicate, "cd", 2);
        int home_compare = strcmp(duplicate, "printenv HOME");
        // IMPLEMENTING cd ~ BECAUSE  ~ is not recognized Directly by chdir
        if(strncmp(duplicate,"help",4)==0)
            {
                printf("cd\nhistory\nhelp\nprintenv\nsetenv\necho\nexit\nquit\nx\t\n");
                continue;
            }
        if(strncmp(duplicate,"cd ~",4)==0)
            {
                chdir(getenv("HOME"));
                curr_path = getcwd(curr_path, 1000);
                continue;
            }
     /*   if(strncmp(duplicate,"setenv HOME",11)==0)
        {          // JUST FOR CHECK
           printf("Enter The Value U Want To Store >> ");
           scanf("%[^\n]",duplicate);
           setenv("HOME", duplicate, 1);
           continue;
        }
       */
        // FOR SETTING UP environmental variable
        // CHECKING IF THE INPUT IS SETENV    
        if(strncmp(duplicate,"setenv HOME",11)==0)
        {
           printf("Enter The Value U Want To Store >> ");
           scanf("%[^\n]",duplicate);
           setenv("HOME", duplicate, 1);
           continue;
        }
        if(strncmp(duplicate,"setenv SHELL",11)==0)
        {
           printf("Enter The Value U Want To Store >> ");
           scanf("%[^\n]",duplicate);
           setenv("SHELL", duplicate, 1);
           continue;
        }
         if(strncmp(duplicate,"setenv TERM",11)==0)
        {
           printf("Enter The Value U Want To Store >> ");
           scanf("%[^\n]",duplicate);
           setenv("TERM", duplicate, 1);
           continue;
        }
         if(strncmp(duplicate,"setenv USER",11)==0)
        {
           printf("Enter The Value U Want To Store >> ");
           scanf("%[^\n]",duplicate);
           setenv("USER", duplicate, 1);
           continue;
        }        
        // PRINTING ENVIRONMENTAL VARIABLES
        // PRINT 4 ENVIRNMENTAL VARIABLE IF INPUT MATCHES
        if (home_compare == 0)
        {
            printf("%s\n", getenv("HOME"));
            continue;
        }
        home_compare = strcmp(duplicate, "echo $HOME");
        if (home_compare == 0)
        {
            printf("%s\n", getenv("HOME"));
            continue;
        }
        int user_compare = strcmp(duplicate, "printenv USER");
        if (user_compare == 0)
        {
            printf("%s\n", getenv("USER"));
            continue;
        }
        user_compare = strcmp(duplicate, "echo $USER");
        if (user_compare == 0)
        {
            printf("%s\n", getenv("USER"));
            continue;
        }
        int shell_compare = strcmp(duplicate, "printenv SHELL");
        if (shell_compare == 0)
        {
            printf("%s\n", getenv("SHELL"));
            continue;
        }
        shell_compare = strcmp(duplicate, "echo $SHELL");
        if (shell_compare == 0)
        {
            printf("%s\n", getenv("SHELL"));
            continue;
        }
        int term_compare = strcmp(duplicate, "printenv TERM");
        if (term_compare == 0)
        {
            printf("%s\n", getenv("TERM"));
            continue;
        }
        term_compare = strcmp(duplicate, "echo $TERM");
        if (term_compare == 0)
        {
            printf("%s\n", getenv("TERM"));
            continue;
        }
        if (cd_command == 0)
        {
            if (j == 1)
            {
                if (strcmp(duplicate, "cd") == 0)
                {
                    chdir(getenv("HOME"));
                    curr_path = getcwd(curr_path, 1000);
                }
            }
            strcpy(duplicate, command);
           if ((strncmp(duplicate, "cd", 2) == 0) && (j > 1))
            {
                int scs = chdir(token[1]);
                if (scs != 0)
                {
                    printf("cd : %s: %s\n", token[1], "No such file or directory");
                }
                curr_path = getcwd(curr_path, 1000);
            }
            strcpy(duplicate, command);
            if ((strncmp(duplicate, "cd..", 4)) == 0)
            {  // SIMILIAR TO WINDOWS I HAVE MAKE PROVISION OF GOING BACK USING cd..
                chdir(".."); // GO ONE DIRECTORY BEHIND
                curr_path = getcwd(curr_path, 1000); // REMEMEBER TO CHANGE THE CURR PATH
            }

            strcpy(duplicate, command);
        }
        /****************************CHECKING FOR EXIT IF INPUT IS X****************************/
        else if ((strcmp(command, "x")) == 0)
        {
            // IF THE INPUT MATCHES FOR  x THEN 
            chdir("/tmp");   // FIRST GO TO /tmp DIRECTORY WHERE FILE IS THERE
            remove("history.txt");  // NOW REMOVE THE FILE PRESENT 
            // SINCE WE HAVE TO EXIT I BREAK HERE TO GO OUT OF LOOP AND EXIT SUCCESSFULLY
            break;
        }
        /**************************************************************************************/
          /**************************CHECKING REDIRECTION************************************/
        /**************************************************************************************/
        else if ((RedirectionOutputCheck(ptr) == 1) && (RedirectionInputCheck(ptr) == 1))
        {  // IMPLEMENT ONLY WHEN BOTH CAME < & >
            printf("Redirection INPUT AND OUTPUT BOTH\n");
            ImplementBothRedirection(ptr);
            continue;
        }
        else if (RedirectionOutputCheck(ptr) == 1)
        {
          // IMPLEMENT ONLY WHEN  > CAME
            //     printf("Redirection Output\n");  // JUST FOR CHECK
            ImplementOutputRedirection(ptr);
            continue;
        }
        else if (RedirectionInputCheck(ptr) == 1)
        {
         // IMPLEMENT ONLY WHEN < CAME 
            // printf("Redirection Input\n");    // JUST FOR CHECK
            ImplementInputRedirection(ptr);
            continue;
        }
        else if (RedirectionAppendCheck(ptr) == 2)
        {  // IMPLEMENT ONLY WHEN >> OCCUR
            //printf("Redirection Append\n");    // JUST FOR CHECK
            ImplementAppendRedirection(ptr);
            continue;
        }
        /**************************************************************************************/
         /*******************************CHECKING FOR PIPE***********************************/
        else if (pipeCheck(ptr) == 1)
        {
            // CHECKING FOR PIPE
            // IF PIPE PRESENT THEN ONLY IMPLEMENT PIPE
            implementPipe(ptr);
        }
       /***********************************CHECKING FOR HISTORY******************************************/
        else if ((strcmp(command, "history")) == 0)
        {
          // IF THE INPUT IS HISTORY GO TO /tmp AND PRINT HISTORY FROM THERE
            printHistory();
        }
        /***********************************CHECKING FOR EXIT********************************************/
        else if (((strcmp(command, "exit")) == 0) || ((strcmp(command, "quit")) == 0) || ((strcmp(command, "x")) == 0))
        {
          // IF THE INPUT MATCHES FOR exit OR quit OR x THEN 
            chdir("/tmp");   // FIRST GO TO /tmp DIRECTORY WHERE FILE IS THERE
            remove("history.txt");  // NOW REMOVE THE FILE PRESENT 
            // SINCE WE HAVE TO EXIT I BREAK HERE TO GO OUT OF LOOP AND EXIT SUCCESSFULLY
            break;
        }
        /**************************************************************************************/
             /******************************GENERAL CASE*********************************/
        /**************************************************************************************/
         /************************************FORKING*****************************************/
        else if (strcmp(command, " \n\t") != 0)
        {
            int stat_loc;
            // here we have written whole method for forking
            int n = j;
            pid_t child;
            child = fork();  // AS FORK RETURN PROCESS ID SO I CREATED AND STORE IT IN A VARIABLE
            if (child < 0)
            { // IF CHILD FORK IS NOT POSSIBLE
                perror("Fork failed");
                exit(1);
            }
             // ELSE IF IT OCCURS SUCCESSFULLY
            if (child == 0)
            {
                if (execvp(token[0], token) != 0) /* Never returns if the call is successful */
                {
                    printf("COMMAND NOT FOUND\n");// NOT WORKING FOR SINGLE COMMAND BUT FOR MULTIPLE WORKING
                    return 0;
                    exit(0);
                }
                execvp(token[0], token);
            }
            else
            {
                waitpid(child, &stat_loc, WUNTRACED);
            }
        }
        // AFTER IT NOW WE FREE THE TOKEN AND PTR WE DEFINED USING MALLOC
        free(token);
        free(ptr);
    } while (compare != 0);
    free(command); // FREEING COMMAND PTR
    //free(pathcombined);
}
// I TRIED THIS ONCE FOR FORK BUT IT IS OT WORKING
/**************************************************************************************************/
/*********************************DEALING WITH THE CHILD PROCESS *************************************/
/*int commandHandler(char *command)
{   int stat_loc;
    char*token[256];   // USE FOR DIVIDING THE INPUT INTO SEPERATE WORDS
    int j = 0;
    char *p = strtok(command, " ");
    while (p != NULL)
    {
        token[j++] = p;
        p = strtok(NULL, " ");
    }
    token[j]=NULL;
    int n = j;
    pid_t child;
    child = fork();
            if (child< 0)
            {
            perror("Fork failed");
            exit(1);
            }
            
        if (child == 0) {
         //    Never returns if the call is successful 
            if (execvp(token[0],token) < 0)
            { 
                return 0;
                exit(0);
            }
            execvp(token[0],token );
           // printf("This won't be printed if execvp is successul\n");
        } 
        else 
        {
            waitpid(child, &stat_loc, WUNTRACED);
        }
}
*/
