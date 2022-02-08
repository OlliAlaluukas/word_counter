#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define HASH_TABLE_SIZE 200000
#define WORD_LENGTH 255
// author: Olli Alaluukas

typedef struct WordStruct_t{
    //unsigned long int bucket; //integer value of the word
    int freq;
    char word[WORD_LENGTH];
} WordStruct;

// Returns hashed value, which will be the index of the row
int getIndex(unsigned long int number){
    return number%HASH_TABLE_SIZE;
}

long int num_of_unique_words=0;

// Insert a bucket and the frequency to hash table
// Return -1 if table full
// Use linear probing
int insert_number(unsigned long int number,WordStruct *table, char *tempword){
        int index=getIndex(number);

        if (table[index].freq == 0){
            strcpy(table[index].word,tempword);
            table[index].freq++;
            num_of_unique_words++;
            return 0;
        }

        else if(strcmp(table[index].word,tempword)==0){
            table[index].freq++;
            return 0;
        }

        else{
            //linear probing
            //printf("Linear started\n");
            return number++;
    }
    return 0;
}

//convert the word to integer
static size_t getBucket(const char* source)
{
    if (source == NULL) {
        return 0;
    }
    size_t hash = 5381;
    while (*source)
        hash = 33 * hash ^ (unsigned char) *source++;
    return hash;
}


//create the heap
void heapify(WordStruct *table, int n, int i)
{
    //unsigned long int bucketTemp;
    int freqTemp;
    char wordTemp[WORD_LENGTH];

    int largest = i; // Initialize largest as root
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // if left child is larger than root
    if (left < n && table[left].freq < table[largest].freq){
        largest = left;
    }


    // if right child is larger than largest so far
    if (right < n && table[right].freq < table[largest].freq)
    {
        largest = right;
    }

    // If largest is not root
    if (largest != i) {

        //swap arr[i] and arr[largest];
        freqTemp=table[i].freq;
        strcpy(wordTemp,table[i].word);

        table[i].freq=table[largest].freq;
        strcpy(table[i].word, table[largest].word);

        //table[largest].bucket=bucketTemp;
        table[largest].freq=freqTemp;
        strcpy(table[largest].word, wordTemp);

        // Recursively heapify the affected sub-tree
        heapify(table, n, largest);
    }
}


//build a Max-Heap from the given array
void buildHeap(WordStruct *table, int n)
{
    //index of the last non-leaf node
    int indexOfNonLeaf = (n / 2) - 1;

    for (int i = indexOfNonLeaf; i >= 0; i--) {
        heapify(table, n, i);
    }
}

//sort the table to descending order
void heapSort(WordStruct *table, int n)
{
    //unsigned long int bucketTemp;
    int freqTemp;
    char wordTemp[WORD_LENGTH];

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(table, n, i);

    // one by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        //move current root to end
        freqTemp=table[0].freq;
        strcpy(wordTemp,table[0].word);

        table[0].freq=table[i].freq;
        strcpy(table[0].word, table[i].word);

        table[i].freq=freqTemp;
        strcpy(table[i].word, wordTemp);

        // call max heapify on the reduced heap
        heapify(table, i, 0);
    }
}


void printHeap(WordStruct *table, int n)
{
    for (int i = 0; i < n; i++){
        if(table[i].freq>0) {
            printf("table[%d] : sana = %s \t\t| frekvenssi = %d\n",i,table[i].word, table[i].freq);
        }
    }
}

unsigned long int countWords(WordStruct *table){
    unsigned long int sum=0;
    for (int i=0; i<=HASH_TABLE_SIZE-1; i++){
        if(strlen(table[i].word)>0)
        sum += table[i].freq;
    }
    return sum;

}

int main(int argc, char *argv[])
{
    clock_t start,end;
    double totaltime;
    start = clock();

    FILE *textfile;
    char tempword[WORD_LENGTH];

    WordStruct *pointerHashTable;
    //Max amount of the table is max of rows*length of columns
    pointerHashTable=(WordStruct*) malloc(HASH_TABLE_SIZE * (sizeof (long int)+ sizeof (int) +256));

    if(argc>1){
        textfile = fopen(argv[1],"r");
    }
    else {
      textfile =fopen("c:/Algoritmit/Algoritmit/word_counter/small.txt","r");
    }

    if (textfile == NULL){
          printf("Can not open the file!\n");
          return 0;
      }

    //WordStruct pointerHashTable[HASH_TABLE_SIZE];
    //Replaced with pointerHashTable

    // Initialize the table
    for(int i=0; i < HASH_TABLE_SIZE; i++){
        //pointerHashTable[i].bucket = 0;
        pointerHashTable[i].freq = 0;
        strcpy(pointerHashTable[i].word,"");
    }

    while(fscanf(textfile, "%s",tempword) != EOF){
        fscanf(textfile, "%29[a-zA-Z']",tempword);
        for (unsigned int i=0; i< strlen(tempword); i++){
            tempword[i]=tolower(tempword[i]);
            if(tempword[i]==',') tempword[i]=0;
            if(tempword[i]=='.') tempword[i]=0;
            if(tempword[i]=='!') tempword[i]=0;
            if(tempword[i]=='?') tempword[i]=0;
            if(tempword[i]=='-') tempword[i]=0;
            if(tempword[i]=='_') tempword[i]=0;
            if(tempword[i]==':') tempword[i]=0;
        }
        if(strlen(tempword) > 0){
            size_t bucket = getBucket(tempword);
            if (num_of_unique_words<HASH_TABLE_SIZE){
                unsigned long int num=insert_number(bucket,pointerHashTable,tempword);

                while (num>0 && num<HASH_TABLE_SIZE && num_of_unique_words<HASH_TABLE_SIZE){
                    num=insert_number(num,pointerHashTable,tempword);
                }
            }
            else {
                printf("HASH TABLE FULL. CANCELING\n");
                return 0;
            }
        }

    }
    printf("Sanoja on %lu\n", countWords(pointerHashTable));
    printf("Uniikkeja sanoja on %lu\n", num_of_unique_words);
    //buildHeap(pointerHashTable, HASH_TABLE_SIZE);
    heapSort(pointerHashTable, HASH_TABLE_SIZE);
    printHeap(pointerHashTable, 100);

    //Free the memory
    free(pointerHashTable);
    end = clock();
    totaltime = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Toimitus kesti %.2f\n ",totaltime);
    return 0;
}
