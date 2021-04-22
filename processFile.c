/*
* Marcella Petrucci
* petrucma@oregonstate.edu
*
* The purpose of this program is to read a file taken as an argument and parse the data from that file into structs. Then allow a user to access that information on via cmd.
*
* The portion of this program involving reading from file, creating structs and linked list, and parsing the CSV into those structures was taken from the provided student example files
* linked here: https://repl.it/@cs344/studentsc#main.c as of 1/16/2021
* 
* The portion of this program regarding using the stat() library function is taken from Explorations: Directories
https://canvas.oregonstate.edu/courses/1825887/pages/exploration-directories?module_item_id=20268625
* and was modified to search for the smallest and largest file types.
*
* A few problems of limited scope are cited here as well as in line:
*	 clear input for possible newline character, solution posted https://stackoverflow.com/questions/20763240/fgets-not-working-in-a-function Farouq Jouti-- 1/17/2021
*
*/
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define PREFIX "movies"

//this structure holds the data associated with a movie
struct movie {
    char* title;
    char* year;
    char* language;
    char* rating;
    struct movie* next;
};

//Function Declarations
struct movie* create_movie(char* currLine);
struct movie* processFile(char* fileName);
void readFile(char* fileName, struct movie* list);
void writeFile(char* fileName, struct movie* aMovieList);
//int findLength(struct movie* aList);
void printMovieTitle(struct movie* list);
void printMovieList(struct movie* list);
void printMovie(struct movie* movie);
//void printPrompt();
//void moviesByYear(struct movie* list);**************
//void select_2(struct movie* m, int listLength);
//void select_4();
//void user_Input(struct movie* list);
void minFileSize();
void maxFileSize();
void fileByName();
int genRand();
void prompt1();
void prompt2();
int exitProgram();



/**********************************************
*
* This block of functions regards printing menus and taking user selections
* The fileMenu() function is the main caller function for this program
*
***********************************************/
void prompt1() {
    int inputNum = 0;
    char input[256];

    //prompt menu
    printf("1. Select file to process\n");
    printf("2. Exit the program\n");
    printf("Enter a choice 1 or 2:\n");
    //take input
    fgets(input, 265, stdin);
    // convert string to int
    inputNum = atoi(input);

    //loop while out of bounds
    while (inputNum < 1 || inputNum > 2) {

        printf("1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("Enter a choice 1 or 2:\n");
        //take input
        fgets(input, 265, stdin);
        // convert to int
        inputNum = atoi(input);
    }
     // user selections, calls prompt 2 or exits
    switch (inputNum) {
    case 1:
        prompt2();
        break;
    case 2:
        exitProgram();
        break;
    }
}

void prompt2() {
    int inputNum = 0;
    char input[256];

    //prompt menu
    printf("Which file you want to process?\n");
    printf("Enter 1 to pick the largest file\n");
    printf("Enter 2 to pick the smallest file:\n");
    printf("Enter 3 to specify the name of a file\n");
    printf("Enter a choice from 1 to 3:\n");
    //get usert selection as string
    fgets(input, 265, stdin);
    //convert to int
    inputNum = atoi(input);

    // loop if out of range
    while (inputNum < 1 || inputNum > 3) {

        printf("Which file you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file:\n");
        printf("Enter 3 to specify the name of a file\n");
        printf("Enter a choice from 1 to 3:\n");

        fgets(input, 265, stdin);

        inputNum = atoi(input);
    }
    // calls for each menu selection
    switch (inputNum) {
    case 1:
        maxFileSize();
        break;
    case 2:
        minFileSize();
        break;
    case 3:
        fileByName();
        break;
    }
}

int exitProgram() {
    return EXIT_SUCCESS;
}

/**********************************************
*
* This block of functions regards helper functions to help with various tasks
*
***********************************************/
int genRand() {
    return random();
}
/**********************************************
*
* This block of functions regards opening, reading from, and writing to files:
* includes calls functions to create Movie Structs, a linked list of movies, and processFile
* 
* This function is adapted from the Exploration: Files: Reading and Writing files
*https://canvas.oregonstate.edu/courses/1825887/pages/exploration-files?module_item_id=20268623
* 
***********************************************/

/*
* This Function reads a csv file, makes a directory, 

*********************calls functions to parse the csv data, 
* and writes new files with the sorted data
*/
void readFile(char* fileName, struct movie* aMovieList) {

    int fd;
    //append the file name to ./ to create the complete file path
    char dotSlash[50] = "./";
    char* dotPtr = dotSlash;
    char* newFilePath = strcat(dotPtr, fileName);

    //create unique directory using randomized numbers and print the directory name
    // generate random int
    srandom((unsigned int)time(NULL));
    int randNum = random();
    // cast int to string and initialize pointer to it
    char charNum[50];
    sprintf(charNum, "%d", randNum);
    char* randomInt = charNum;

   // set standard directory name format and append random int to make each directory unique
    char onidPath[100] = "./petrucma.movies.";
    char* onidPtr = onidPath;
    char* newDirecoryPath = strcat(onidPath, randomInt);
    printf("Directory Path is: %s\n", newDirecoryPath);

    //make a new directory
    mkdir(newDirecoryPath, 0710);// file is 0640

    // We first open a file for read write, creating it if necessary and truncating it if it already exists
    fd = open(newFilePath, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("open() failed on \"%s\"\n", newFilePath);
        perror("Error");
        exit(1);
    }

    // call function to group movies by year, which takes a list and calls a write File function
    //moviesByYear(aMovieList);**********************************************************************************

    writeFile(newFilePath, aMovieList);
    // Close the file descriptor
    close(fd);
}

void writeFile(char* filePath, struct movie* aMovieList) {// pass in year?
    char year[100] = "./2008";
    char* yearPtr = year;
    char dotTxt[50] = ".txt";
    char* dotPtr = dotTxt;
    int fd;
    //char* newFilePath = strcat(filePath, );
    char *newFilePath = strcat(year, dotPtr);

    // file permission is 0640**********************************************

    // We first open a file for read write, creating it if necessary and truncating it if it already exists
    fd = open(newFilePath, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("open() failed on \"%s\"\n", newFilePath);
        perror("Error");
        exit(1);
    }

    // call function to group movies by year, which takes a list and calls a write File function
    //moviesByYear(aMovieList);**********************************************************************************

    // We write a string to the file
    char message[] = "This be the file!";
    int howMany = write(fd, message, strlen(message));
    printf("wrote %d bytes to the file\n", howMany);
    // Close the file descriptor
    close(fd);
}
//This function loops through the current directory and finds the minimum size file witht the prefix 'movies' -- modified from Explorations: Directories

void minFileSize() {
    // Open the current directory
    DIR* currDir = opendir(".");
    struct dirent* aDir;
    struct stat dirStat;
    int i = 0;
    char entryName[256];
    char* fileName;
    off_t minSize = 10000;// arbitrarily large for min, maybe change later***********************************************************************************
    off_t currSize = 0;
    char* extension = ".csv";

    //set current file name for extension comparison
    memset(entryName, '\0', sizeof(entryName));
    strcpy(entryName, aDir->d_name);

    // Go through all the entries
    while ((aDir = readdir(currDir)) != NULL) {
        //compare prefix and extension before size
        if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0) {
            if (strcmp(&entryName[strlen(extension) - 3], extension)) {
                // Get meta-data for the current entry
                stat(aDir->d_name, &dirStat);

                //store current file size
                currSize = dirStat.st_size;

                //if currSize is less than minSize
                if (currSize < minSize) {
                    minSize = currSize;
                    memset(entryName, '\0', sizeof(entryName));
                    strcpy(entryName, aDir->d_name);
                }
                i++;
            }
        }

    }
    // Close the directory
    closedir(currDir);
    //create char ptr to pass into processFile function
    fileName = entryName;
    printf("The smallest .csv file starting with the prefix \"%s\" in the current directory is %s\n", PREFIX, entryName);
    struct movie* aMovieList = processFile(entryName);// ORIGIN POINT FOR MOVIE LIST*************************************
    readFile(entryName, aMovieList);
    printMovieTitle(aMovieList);
    prompt1();
}

void maxFileSize() {
    // Open the current directory
    DIR* currDir = opendir(".");
    struct dirent* aDir;
    struct stat dirStat;
    int i = 0;
    char entryName[256];
    int maxSize = dirStat.st_size;
    int currSize = 0;
    char* extension = ".csv";
    //set for extension comparison
    memset(entryName, '\0', sizeof(entryName));
    strcpy(entryName, aDir->d_name);

    // Go through all the entries
    while ((aDir = readdir(currDir)) != NULL) {
        //compare prefix and extension before size
        if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0) {
            if (strcmp(&entryName[strlen(extension) - 3], extension)) {
                // Get meta-data for the current entry
                stat(aDir->d_name, &dirStat);

                    //store current file size
                    currSize = dirStat.st_size;

                    //if currSize is greater than minSize
                    if (currSize > maxSize) {
                        maxSize = currSize;
                            memset(entryName, '\0', sizeof(entryName));
                            strcpy(entryName, aDir->d_name);
                    }
                i++;
            }
        }

    }
    // Close the directory
    closedir(currDir);
    printf("The largest .csv file starting with the prefix \"%s\" in the current directory is %s\n", PREFIX, entryName);
    prompt1();
}

void fileByName() {

    // Open the current directory
    DIR* currDir = opendir(".");
    struct dirent* aDir;
    struct stat dirStat;
    int i = 0;
    char entryName[256];
    char inputFileName[256];
    int flag = 0;

    // take user input
    printf("Please type the exact file name you would like to be processed.\n");
    memset(inputFileName, '\0', sizeof(inputFileName));//reset to null
    fgets(inputFileName, 265, stdin);
    
  
    // Go through all the entries
while ((aDir = readdir(currDir)) != NULL) {
        // Get meta-data for the current entry
        //stat(aDir->d_name, &dirStat);
        memset(entryName, '\0', sizeof(entryName));//reset to null
        strcpy(entryName, aDir->d_name);
        //entryName[sizeof(aDir->d_name) + 1] = '\n';// trying this to add null character to entryName
        //printf("Test input: %s\n", inputFileName);

        if (strncmp(entryName, inputFileName, (strlen(inputFileName) -1)) == 0) {// "-1" == janky patch to fix absence of null character at the end of entryName
            // match found
            flag = 1;
            printf("The file/directory you requested is processing: %s\n", entryName);
            i++;
        }
    }
    // Close the directory
    closedir(currDir);
    // error message
    if (flag == 0) {
        printf("Could not find the file you requested: %s\n", inputFileName);
        prompt2();
    }
    prompt1();
}


/**********************************************
* 
* This block of functions regards creating and manipulating Movie Structs
* 
***********************************************/

/*
* Return a linked list of students by parsing data from
* each line of the specified file.
*/
struct movie* processFile(char* filePath)
{
    // Open the specified file for reading only
    FILE* movieFile = fopen(filePath, "r");

    char* currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char* token;

    // The head of the linked list
    struct movie* head = NULL;
    // The tail of the linked list
    struct movie* tail = NULL;

    // skip header line
    nread = getline(&currLine, &len, movieFile);

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new student node corresponding to the current line
        struct movie* newNode = create_movie(currLine);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);
    return head;
}

/*
* Creates a movie struct, adds it to a linked list, returns a pointer to that list
*/

struct movie* create_movie(char* currLine) {
    struct movie* m = malloc(sizeof(struct movie));

    // For use with strtok_r
    char* saveptr;

    //first token is Title
    char* token = strtok_r(currLine, ",", &saveptr);// takes CSV and breaks first token off of current line
    //find way of skipping first line
    m->title = calloc(strlen(token) + 1, sizeof(char));//allocates with mem availability check
    strcpy(m->title, token);//copies str from token to title

    //second token is Year
    //m->year = "1980";
    token = strtok_r(NULL, ",", &saveptr);// takes CSV and breaks first token off of current line
    m->year = calloc(strlen(token) + 1, sizeof(char));//allocates with mem availability check
    strcpy(m->year, token);//copies str from token to title

    //third token is Language
    //m->language = "English,French";
    token = strtok_r(NULL, ",", &saveptr);// takes CSV and breaks first token off of current line
    m->language = calloc(strlen(token) + 1, sizeof(char));//allocates with mem availability check
    strcpy(m->language, token);//copies str from token to title

    //fourth token is Rating
    token = strtok_r(NULL, ",", &saveptr);// takes CSV and breaks first token off of current line
    m->rating = calloc(strlen(token) + 1, sizeof(char));//allocates with mem availability check
    strcpy(m->rating, token);//copies str from token to title

    m->next = NULL;

    return m;
}

/*
* this function takes a linked list of movie structs, groups them by year, and writes them to a file??
*/

//void moviesByYear(struct movie* list) {
//    char input_year[100];
//    //head pointer
//    struct movie* head = list;
//    // get user input from console, store in input_year as a string
//    //printf("To see the titles of movies released in a specified year, please type a year between 1900 and 2021.\n");
//    //fgets(input_year, 100, stdin); // read line
//
//    // cast to integers
//    int int_year = atoi(input_year);
//    int m_year = atoi(list->year);
//
//
//    while (list != NULL) {
//
//        // cast to integers
//        m_year = atoi(list->year);
//
//        //if (test_year == m_year) {
//        if (int_year == m_year) {
//            //print title of current node
//            printf("%s\n", list->title);
//        }
//        //iterate to next node
//        list = list->next;
//
//    }
//    list = head;
//    user_Input(list);
//
//}

/*
* prints data for a movie struct
*/
void printMovie(struct movie* movie) {
    printf("%s, %s %s, %s\n", movie->title,
        movie->year,
        movie->language,
        movie->rating);
}

/*
* iterates through list and calls function to print all data for all movies
*/
void printMovieList(struct movie* list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

/*
* iterates through list and prints only title for all movies
*/
void printMovieTitle(struct movie* list)
{
    while (list != NULL)
    {
        //printMovie(list->title);
        char* t = list->title;
        printf("%s\n", t);
        list = list->next;
    }
}
int main() {
    prompt1();
    //minFileSize();
    //maxFileSize();
    //fileByName();

  

    return 0;
}

