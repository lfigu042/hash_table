#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10000

struct bucket {
    char* string;
    struct bucket* next;
};
typedef struct bucket Bucket;
Bucket dictionary[HASH_SIZE];

void toLowerCase(char* d){
    int i;
    for(i = 0; i <= strlen(d); i++){
        if(d[i] >= 65 && d[i] <= 90)
            d[i] = d[i] + 32;
    }
}
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
                printf("%s, ", iterator->string);
                iterator = iterator->next;
                word_count++;
            }
        }
    printf("\n%d words in bucket\n",word_count);
}
int dictionaryInsert(char* word) {
    toLowerCase(word);
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
    toLowerCase(word);
    if (dictionary[i].string) {//not an empty bucket
        iterator = &dictionary[i];//head of linked list
        while(iterator) {
            if (!strcmpi(iterator->string, word))//matched
                return 1;
            iterator = iterator->next;
        }
    }
    return 0;//unsuccessful search
}

int main(int argc, char** argv) {
    char *string[HASH_SIZE];
    char line[100];

    char* delimit = "0123456789~`@#$%^&*()_+=[]{}\\|:;<,>.?!\t\r\n\v\f\r\"";
    FILE *fileDictionary;
    FILE *fileInput;

    printf("Welcome to my spell checker program, any words spelled incorrectly from the input file will be printed on console \n");

    if (argc != 3) {
        printf("The program needs two command line arguments: dictionary.txt input.txt");
        return 1;
    }
    printf("The dictionary is stored in file %s\n", argv[1]);
    printf("The raw textual data for spell checking is stored in file %s\n", argv[2]);
    init();

    fileDictionary = fopen(argv[1],"r");
    if (fileDictionary != NULL) {
        printf("Hashing all words from dictionary.txt\n");
        int i = 0;
        for (; fscanf(fileDictionary, "%[^\n]\n", line) != EOF; i++) {
            strtok(line, "\n");
            dictionaryInsert(line);
        }
    }
    fclose(fileDictionary);

    fileInput = fopen(argv[2], "r");
    if (fileInput != NULL) {
        int i = 0;
        for (; fscanf(fileInput, "%s", line) != EOF; i++) {
            string[i] = strtok(line, delimit);
            toLowerCase(string[i]);
            if (dictionarySearch(string[i]) == 0)  //if word not in bucket
                printf("%d: %s -> not in dict\n", i, string[i]);
        }
    }
    fclose(fileInput);
}