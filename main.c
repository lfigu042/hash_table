#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include <stdlib.h>
#include<string.h>

#define HASH_SIZE 10000

struct bucket {
    char* string;
    struct bucket* next;
};
typedef struct bucket Bucket;
Bucket dictionary[HASH_SIZE];

unsigned hash(char* key) {
    unsigned val = 0;
    while (*key)//as long as key is not pointing to the \0 at the end of the string
        val = *(key++) + 17 * val;
    return val % HASH_SIZE;
}
void init() {
    int i;
    for (i = 0; i < HASH_SIZE;i++)
        dictionary[i].string = NULL;
}
void printDictionary() {
    int i = 0;
    Bucket* iterator;
    for (; i < HASH_SIZE; i++)
        if (dictionary[i].string) {//not an empty bucket
            iterator = &dictionary[i];//head of linked list
            while (iterator) {
                printf("%s\n", iterator->string);
                iterator = iterator->next;
            }
        }
}
int dictionaryInsert(char* word) {
    unsigned index = hash(word);
    if (dictionary[index].string == NULL) {//empty bucket
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
int main(int argc, char** argv) {
    if (argc != 3) {
        printf("The program needs two command line arguments: dictionary.txt input.txt");
        return 1;
    }
    printf("The dictionary is stored in file %s\n", argv[1]);
    printf("The raw textual data for spell checking is stored in file %s\n", argv[2]);
    init();
    //your code comes here!
}