#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Item {
    char key[255];
    char value[255];
} typedef Item;

struct HashTable {
    Item **items;
    int size;
} typedef HashTable;

int isPrime(int num) {
    int i = 2, isPrime = 1;

    while (i < sqrt(num) && isPrime) {
        if (num % i == 0)
            isPrime = 0;
        i++;
    }
    return isPrime;
}

int getClosestPrime(int num) {
    int closestPrime = 0;
    do {
        if (isPrime(num))
            closestPrime = num;
        num++;
    } while (!closestPrime);
    return closestPrime;
}

int hash(const char *key, int length, int m) {
    int i;
    const int P = 37;
    unsigned long long hashValue = 0;
    for (i = length - 1; i >= 0; i--)
        hashValue = (P * hashValue) + key[i];   // Gerekli carpma islemlerini
    // Destekleyebilmek icin unsigned long long
    return hashValue % m;
}

void printTable(HashTable hashTable) {
    int i;
    printf("_________________________________\n");
    for (i = 0; i < hashTable.size; i++) {
        if (hashTable.items[i] != NULL)
            printf("%d | %s - %s \n", i, hashTable.items[i] -> key, hashTable.items[i] ->value);
    }
}

void insert(HashTable hashTable, Item* item, int keyLength) {
    int idx = hash(item -> key, keyLength, hashTable.size);

    while(
            hashTable.items[idx] != NULL &&
            hashTable.items[idx] -> key != item -> key
            ) {
        idx++;
        idx = idx % hashTable.size;
    }

    if (hashTable.items[idx] == NULL) {
        hashTable.items[idx] = item;
//        printf("INSERTING --- %d | %s - %s \n", idx, hashTable.items[idx] -> key, hashTable.items[idx] ->value);
//        printf("------------------------- \n");
    }
}

int main() {

    FILE *fp;
    fp = fopen("Sample.txt", "r");
    char line[255], c;
    double loadFactor;
    int count = 0, i;

    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;

    fclose(fp);

    count /= 2;
    printf("Load-Factor? \n");
    scanf("%lf", &loadFactor);

    int lfNumber = count / loadFactor;
//    printf("lfNumber -> %d \n", lfNumber);
    int tableSize = getClosestPrime(lfNumber);
//    printf("%d \n", tableSize);

    HashTable hashTable;
    hashTable.items = (Item **) calloc(tableSize, sizeof(Item*));
    hashTable.size = tableSize;


    fp = fopen("Sample.txt", "r");

    // TODO: INSERTLERDEN ONCE TRIM
    // TODO: KEY VALUE HANGISI HANGISINE CONTROL
    // TODO: VE VEYA NASIL YAPILIR?
    while (fgets(line, sizeof(line), fp) != NULL) {
        char buffer[2][255];

        int lengthOfString = strcspn(line, "\n");

        if (strstr(line, "https://") != NULL) {
            strcpy(buffer[1], line);
        } else {
            strcpy(buffer[0], line);
            Item* item = malloc(sizeof(Item));
            strcpy(item -> key, buffer[0]);
            strcpy(item -> value, buffer[1]);
            insert(hashTable, item, lengthOfString);
        }

//        printf("%d - ", lengthOfString);
//        printf("%lu \n", hash(line, 10, lengthOfString));
    }

    printTable(hashTable);

    return 0;
}
