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
* Thanks to the TA Michael Slater for helping me at the 11th hour to debug and write a couple of helper functions: writeOrAppend() and isCsv()
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
void makeNewDirectory(char* fileName, struct movie* list);
void writeFile(char* fileName, struct movie* aMovieList);
void writeOrAppendMovie(char* filePath, struct movie* movie);
void minFileSize();
void maxFileSize();
void fileByName();
void prompt1();
void prompt2();
int exitProgram();
int isCsv(char* file);



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
* This block of functions regards opening, reading from, and writing to files:
* includes calls functions to create Movie Structs, a linked list of movies, and processFile
* 
* This function is adapted from the Exploration: Files: Reading and Writing files
*https://canvas.oregonstate.edu/courses/1825887/pages/exploration-files?module_item_id=20268623
* 
***********************************************/

/*
* This Function reads a csv file, makes a directory, 
* calls functions to parse the csv data, 
* and writes new files with the sorted data
*/
void makeNewDirectory(char* fileName, struct movie* aMovieList) {

    //create unique directory using randomized numbers and print the directory name
    // generate random int
    srandom((unsigned int)time(NULL));
    int randNum = random() % 100000;
    // cast int to string and initialize pointer to it
    char charNum[50];
    sprintf(charNum, "%d", randNum);
    char* randomInt = charNum;

   // set standard directory name format and append random int to make each directory unique
    char onidPath[100] = "./petrucma.movies.";
    char* newDirectoryPath = strcat(onidPath, randomInt);
    printf("Directory Path is: %s\n", newDirectoryPath);

    //make a new directory and call function to write file in that directory
    mkdir(newDirectoryPath, 0750);
    writeFile(newDirectoryPath, aMovieList);
}

/*
* This function  creates files at the file path passed to it, adds the titles
* from the linked list to the files if the file already exists
* 
* Thanks to the TA Michael Slater for helping me at the 11th hour to debug and write this function
*/
void writeOrAppendMovie(char* filePath, struct movie* movie) {
    char fileName[PATH_MAX +1];
    sprintf(fileName, "%s/%s.txt", filePath, movie->year);//memset before this line
    int exists = !access(fileName, F_OK | R_OK);// access returns 0 if permitted
    if (exists) {
        FILE* f = fopen(fileName, "a+");//append
        fprintf(f, "%s\n", movie->title);
        fflush(f);
        fclose(f);
        chmod(fileName, 0640);
    }
    else {
        FILE* f = fopen(fileName, "w+");//write
        fprintf(f, "%s\n", movie->title);
        fflush(f);
        fclose(f);
        chmod(fileName, 0640);
    }
}
/*
*This function writes takes the file path and a movie linked list, loops through the linked list
*calls writeORAppend which creates files adds the titles to the files if the file already exists
*/

void writeFile(char* filePath, struct movie* aMovieList) {
    //char year[5]= "0000";
    //int fd;
    struct movie* curr = aMovieList;
    struct movie* head = aMovieList;

    //loop through movie linked list
    while (curr != NULL) {
        writeOrAppendMovie(filePath, curr);
        //the above line replaced all this below -- keeping in for me to learn from :):)
        /*
        memcpy(year, curr->year, strlen(curr->year) + 1);
        //write the file with the year name
        //write the title in that year file
        char newFilePath[PATH_MAX + 1];
        sprintf(newFilePath, "%s/%s.txt", filePath, year);//memset before this line


        // We first open a file for read write, creating it if necessary and truncating it if it already exists
        fd = open(newFilePath, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);  // file permission is 0640**********************************************
        if (fd == -1) {
            printf("open() failed on \"%s\"\n", newFilePath);
            perror("Error");
            exit(1);
        }

        // We write a string to the file
        char* movieTitle = curr->title;
        write(fd, movieTitle, strlen(movieTitle));

        // Close the file descriptor
        close(fd);
        */
        //next movie node
        curr = curr->next;
    }
    //reset the head of the linked list
    curr = head;  
    
     free(aMovieList);
}
/*
* This function takes a file name and checks for the .csv extension.
* returns 1 on success
* 
* * Thanks to the TA Michael Slater for helping me at the 11th hour to debug and write this function
*/
int isCsv(char* file) {
    char* ext = strrchr(file, '.');
    if (strcmp(ext, ".csv") == 0) {
        return 1;
    }
    return 0;
}

//This function loops through the current directory and finds the minimum size file witht the prefix 'movies' -- modified from Explorations: Directories

void minFileSize() {
    // Open the current directory
    DIR* currDir = opendir(".");
    struct dirent* aDir;
    struct stat dirStat;
    int i = 0;
    char entryName[256];
    char minName[256];
    off_t minSize = 10000;
    off_t currSize = 0;
    //char* extension = ".csv";



    // Go through all the entries
    while ((aDir = readdir(currDir)) != NULL) {
        //set current file name for extension comparison
        memset(entryName, '\0', sizeof(entryName));
        strcpy(entryName, aDir->d_name);
        //compare prefix and extension before size
        int hasPrefix = strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0;
        if (hasPrefix && isCsv(aDir->d_name)) 
        {
            printf("found csv : %s\n", aDir->d_name);
                // Get meta-data for the current entry
                stat(aDir->d_name, &dirStat);

                //store current file size
                currSize = dirStat.st_size;

                //if currSize is less than minSize
                if (currSize < minSize) {
                    minSize = currSize;
                    memset(minName, '\0', sizeof(minName));
                    strcpy(minName, aDir->d_name);
                }
                i++;
            
        }

    }
    // Close the directory
    closedir(currDir);  
    printf("The smallest .csv file starting with the prefix %s in the current directory is %s\n", PREFIX, minName);
    //create struct ptr to pass into processFile function
    struct movie* aMovieList = processFile(minName);// ORIGIN POINT FOR MOVIE LIST****
    makeNewDirectory(minName, aMovieList);
    prompt1();
}

void maxFileSize() {
    // Open the current directory
    DIR* currDir = opendir(".");
    struct dirent* aDir;
    struct stat dirStat;
    int i = 0;
    char entryName[256];
    char maxName[256];
    int maxSize = 0;
    int currSize = 0;
    char* extension = ".csv";
   
    // Go through all the entries
    while ((aDir = readdir(currDir)) != NULL) {
        //set for extension comparison
        memset(entryName, '\0', sizeof(entryName));
        strcpy(entryName, aDir->d_name);
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
                            memset(maxName, '\0', sizeof(maxName));
                            strcpy(maxName, aDir->d_name);
                            //memset(entryName, '\0', sizeof(entryName));
                            //strcpy(entryName, aDir->d_name);
                    }
                i++;
            }
        }

    }
    // Close the directory
    closedir(currDir);
    printf("The largest .csv file starting with the prefix \"%s\" in the current directory is %s\n", PREFIX, maxName);
    //create struct ptr to pass into processFile function
    struct movie* aMovieList = processFile(maxName);// ORIGIN POINT FOR MOVIE LIST**********
    makeNewDirectory(maxName, aMovieList);
    prompt1();
}

void fileByName() {

    // Open the current directory
    DIR* currDir = opendir(".");
    struct dirent* aDir;
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
        memset(entryName, '\0', sizeof(entryName));//reset to null
        strcpy(entryName, aDir->d_name);

        if (strncmp(entryName, inputFileName, (strlen(inputFileName) -1)) == 0) {// "-1" == janky patch to fix absence of null character at the end of entryName
            // match found
            flag = 1;
            printf("The file/directory you requested is processing: %s\n", entryName);
            i++;
            break;
        }
    }
    // Close the directory
    closedir(currDir);
    // error message
    if (flag == 0) {
        printf("Could not find the file you requested: %s\n", inputFileName);
        prompt2();
    }
    //create struct ptr to pass into processFile function
    struct movie* aMovieList = processFile(entryName);// ORIGIN POINT FOR MOVIE LIST********
    makeNewDirectory(entryName, aMovieList);
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


int main() {
    prompt1();
    return 0;
}

