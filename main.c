#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10000
#define MAX_TOKEN 100

struct bucket {
    char* string;
    struct bucket* next;
};
typedef struct bucket Bucket;
Bucket dictionary[HASH_SIZE];

unsigned hash(char* key) {
    unsigned val = 0;
    while (*key)    //as long as key is not pointing to the \0 at the end of the string
        val = *(key++) + 17 * val;
    return val % HASH_SIZE;
}
void init() {
    int i;
    for (i = 0; i < HASH_SIZE;i++)
        dictionary[i].string = NULL;
}
void printDictionary() {
    int i = 0, word_count = 0;

    Bucket* iterator;
    for (; i < HASH_SIZE; i++)
        if (dictionary[i].string) {//not an empty bucket
            iterator = &dictionary[i];//head of linked list
            while (iterator) {
                printf("%s\n", iterator->string);
                iterator = iterator->next;
                word_count++;
            }
        }
    printf("%d words in bucket\n",word_count);
}
int dictionaryInsert(char* word) {
    unsigned index = hash(word);
    if (dictionary[index].string == NULL) { //empty bucket
        dictionary[index].string = (char*)malloc(strlen(word) + 1);
        strcpy(dictionary[index].string, word);
        dictionary[index].next = NULL;
        return 1;//successful insert
    }
    char* currentHeadOfLinkedList = dictionary[index].next;
    dictionary[index].next = (Bucket*)malloc(sizeof(Bucket));
    dictionary[index].next->string = (char*)malloc(strlen(word) + 1);
    strcpy(dictionary[index].next->string, word);
    dictionary[index].next->next = currentHeadOfLinkedList;
    return 1;
}
int dictionarySearch(char* word) {
    int i = hash(word);
    Bucket* iterator;
    if (dictionary[i].string) {//not an empty bucket
        iterator = &dictionary[i];//head of linked list
        while(iterator) {
            if (!strcmp(iterator->string, word))//matched
                return 1;
            iterator = iterator->next;
        }
    }
    return 0;//unsuccessful search
}
void toLowerCase(char* word){
//    while(*word){
//        if(*word >= 'A' && *word <= 'Z')
//            *word += CASE_OFFSET;
//        word++;
//    }
}
char getCommandWord(char command[], int maxLength) {
    char lastCharacter;//either space or new line
    int i;
    for (i = 0; (lastCharacter = getchar()) == ' '; i++);//skip leading white spaces
    if (lastCharacter == '\n') {
        command[0] = '\0';
        return lastCharacter;
    }
    command[0] = lastCharacter;
    for (i = 1; i < maxLength - 1 && (command[i] = getchar()) != ' ' && command[i] != '\n'; i++);
    lastCharacter = command[i];
    command[i] = '\0';
    return lastCharacter;
}

int main(int argc, char** argv) {
    char dictFile[MAX_TOKEN];
    char txtFile[MAX_TOKEN];
    char lastCharacter1;
    char lastCharacter2;
    char dictTemp[MAX_TOKEN];
    char txtTemp[MAX_TOKEN];
    char *dictFileType;
    char *txtFileType;
    char line[100];

    //need files to have this in front so program can find them
    char d1[MAX_TOKEN] = "../";
    char t1[MAX_TOKEN] = "../";
    char dTemp[MAX_TOKEN]; //to keep original string unchanged
    char tTemp[MAX_TOKEN];

    FILE * fileDictionary;

    printf("Welcome to my spell checker program, any words spelled incorrectly from the input file will be printed on console \n");

    while (1){ //infinite while loop...
        printf("Please enter dictionary file name and text file name, ex: dictionary.txt input.txt \n");
        lastCharacter1 = getCommandWord(dictFile, MAX_TOKEN); //read first input word
        printf("first argument -> %s \n", dictFile);

        if (strcmp(dictFile, "quit") == 0){ //if command === "quit"
            printf("Good bye...");
            break;
        }else if(lastCharacter1 == '\n'){ //user only entered one argument
            printf("Too few arguments, must input dictionary file name and text file name, ex: dictionary.txt input.txt \n");
        }else{
            lastCharacter2 = getCommandWord(txtFile, MAX_TOKEN); //read second input word
            if(lastCharacter2 != '\n'){ //user wrote more than two arguments
                printf("Too many arguments, must only input dictionary file name and text file name, ex: dictionary.txt input.txt \n");
            }else{ //user wrote exactly two arguments
                //extract the file type from the input and use temp variables to keep dictFile and txtFile from changing
                printf("second argument -> %s \n", txtFile);
                strcpy(dictTemp, dictFile);
                strcpy(txtTemp, txtFile);

                // get the input/output file type
                strtok(dictTemp, ".");
                dictFileType = strtok(NULL, ".");

                strtok(txtTemp, ".");
                txtFileType = strtok(NULL, ".");

                if((!strcmp(dictFileType, "txt")) &&  (!strcmp(txtFileType, "txt"))) { //check file types are both .txt
                    printf("\n **** file types accepted, processing request ****\n\n");
                    if (argc == 1) { //redundant check but oh well
                        init();
                        //your code comes here!
//                      start of ../ modification
                        strcpy(dTemp, dictFile);
                        strcpy(tTemp, txtFile);

                        strcat(d1, dTemp); //need to add "../" at beginning of file path to avoid a pop up error
                        strcat(t1, tTemp);
//                      end of ../ modification

                        argv[1] = d1; // ../dictionary.txt
                        argv[2] = t1; // ../input.txt

                        printf("The dictionary is stored in file: %s\n", argv[1]);
                        printf("The raw textual data for spell checking is stored in file: %s\n", argv[2]);

                        fileDictionary = fopen(argv[1],"r");

                        if(fileDictionary != NULL){
                            printf("Hashing all words from dictionary.txt\n");
                            int i = 0;
                            for (; fscanf(fileDictionary, "%[^\n]\n", line) != EOF; i++) {
                                strtok(line,"\n");
//                                printf("line: %s \n", line);
                                dictionaryInsert(line);
                            }
                            printf("Printing all words just added to bucket\n");
                            printDictionary();
                            fclose(fileDictionary);


                        } else { printf("File can not be read\n"); }

                        printf("\n\n~~~~~~~~~~FINISHED SUCCESSFULLY~~~~~~~~~~\n\n");
                        break;

                    }else{
                        printf("The program needs two command line arguments: dictionary.txt input.txt\n");
                    }
                }else{ //unaccepted file types entered
                    printf("\nINVALID FILE TYPE! only .txt files supported\n");
                }
            }
        }
    }
    printf("\nend of program.\n");

}