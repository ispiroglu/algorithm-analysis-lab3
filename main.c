#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Item {
    char key[255];
    char value[255];
    struct Item *next;
} typedef Item;

struct HashTable {
    Item **items;
    int size;
} typedef HashTable;

struct Link {
    char value[255];
    int collison;
} typedef Link;

struct LinkHashTable {
    Link **links;
    int size;
} typedef LinkHashTable;


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

void printList(Item *head) {
    while (head != NULL) {
        printf("\t| %s |\n", head->value);
        head = head->next;
    }
}

void printTable(HashTable hashTable) {
    int i;
    printf("_________________________________\n");
    for (i = 0; i < hashTable.size; i++) {
        printf("Idx -> %d \n", i);
        if (hashTable.items[i] != NULL) {
            printf("\t |%s| \n", hashTable.items[i]->key);
            printList(hashTable.items[i]);
        } else {
            printf("NULL NULL \n");
        }
    }
}

int getIdxOfKey(HashTable hashTable, char *key) {
    int idx = hash(key, strlen(key), hashTable.size);
    const int constIdx = idx;
    int flag = 1;

    if (strcmp(hashTable.items[idx]->key, key) == 0)
        return idx;
    else {
        idx++;
        do {
            idx++;
            idx = idx % hashTable.size;
            while (idx != constIdx && hashTable.items[idx] == NULL) {
                idx++;
                idx = idx % hashTable.size;
            }

            if (strcmp(hashTable.items[idx]->key, key) == 0) {
                flag = 0;
            }
        } while (idx != constIdx && flag);

        if (idx == constIdx)
            return -1;
        else
            return idx;
    }
}

void search(HashTable hashTable, char *key) {
    int idx = getIdxOfKey(hashTable, key);
    idx == -1 ? printf("Bulunamadi") : printList(hashTable.items[idx]);
}

void errorOnInsert() {
    int i;
    for (i = 0; i < 10; i++)
        printf("Error On Insert\n");
}

int insert(HashTable hashTable, Item *item) {
    int idx = hash(item->key, strlen(item->key), hashTable.size);
    const int constIdx = idx;
    int count = 1;
    int flag = 1;

    if (hashTable.items[idx] == NULL)
        hashTable.items[idx] = item;
    else if (strcmp(hashTable.items[idx]->key, item->key) == 0) {
        Item *temp = hashTable.items[idx];
        hashTable.items[idx] = item;
        hashTable.items[idx]->next = temp;
    } else {
        while (hashTable.items[idx] != NULL &&
               strcmp(hashTable.items[idx]->key, item->key) != 0 &&
               flag) {
            idx++;
            count++;
            idx = idx % hashTable.size;
            if (constIdx == idx)
                flag = 0;
        }
        if (flag) {
            if (hashTable.items[idx] == NULL) {
                hashTable.items[idx] = item;
            } else {
                Item *temp = hashTable.items[idx];
                hashTable.items[idx] = item;
                hashTable.items[idx]->next = temp;
            }
        }
    }

    if (!flag) {
        errorOnInsert();
        return -1;
    }

    return count;
}

void insertForLinkHashTable(LinkHashTable table, Link *link) {
    int idx = hash(link->value, strlen(link->value), table.size);
    int constIdx = idx;

    while (table.links[idx] != NULL) {
        idx++;
    }
    if (constIdx != idx)
        table.links[constIdx]->collison = 1;

    table.links[idx] = link;
}

void searchForAnd(HashTable table, char *key1, char *key2) {
    int idxOfKey1 = getIdxOfKey(table, key1);
    int idxOfKey2 = getIdxOfKey(table, key2);

    LinkHashTable linkHashTable;
    linkHashTable.links = (Link **) calloc(255, sizeof(Link));
    linkHashTable.size = 255;

    Item *head = table.items[idxOfKey1];
    while (head != NULL) {
        Link *link = (Link *) calloc(1, sizeof(Link));
        strcpy(link->value, head->value);
        insertForLinkHashTable(linkHashTable, link);
        head = head->next;
    }
    head = table.items[idxOfKey2];
    while (head != NULL) {
        int idxOfSearch = hash(head->value, strlen(head->value), linkHashTable.size);

        if (linkHashTable.links[idxOfSearch] != NULL &&
        strcmp(head->value, linkHashTable.links[idxOfSearch]->value) == 0) {
            printf("%s \n", head->value);
        } else if ( linkHashTable.links[idxOfSearch] != NULL &&
                    linkHashTable.links[idxOfSearch]->collison == 1) {
            int constIdx = idxOfSearch;
            idxOfSearch++;
            while ( constIdx != idxOfSearch &&
                    strcmp(head->value, linkHashTable.links[idxOfSearch]->value) == 0) {
                idxOfSearch++;
                idxOfSearch = idxOfSearch % linkHashTable.size;
            }
            if (constIdx != idxOfSearch)
                printf("%s \n", head->value);
        }


        head = head->next;
    }
}

void searchForOr(HashTable table, char *key1, char *key2) {

    int idxOfKey1 = getIdxOfKey(table, key1);
    int idxOfKey2 = getIdxOfKey(table, key2);

    LinkHashTable linkHashTable;
    linkHashTable.links = (Link **) calloc(255, sizeof(Link));
    linkHashTable.size = 255;

    Item *head = table.items[idxOfKey1];
    while (head != NULL) {
        Link *link = (Link *) calloc(1, sizeof(Link));
        strcpy(link->value, head->value);
        insertForLinkHashTable(linkHashTable, link);
        head = head->next;
    }
    head = table.items[idxOfKey2];
    while (head != NULL) {
        Link *link = (Link *) calloc(1, sizeof(Link));
        strcpy(link->value, head->value);
        insertForLinkHashTable(linkHashTable, link);
        head = head->next;
    }
    int i;
    for (i = 0; i < 255; i++)
        if (linkHashTable.links[i] != NULL)
            printf("%s \n", linkHashTable.links[i]->value);
}

int main() {

    FILE *fp;
    fp = fopen("Sample.txt", "r");
    char line[255], tmp[1];
    double loadFactor;
    int count = 0, flag = 1, detailedMode = 0;

    printf("Detayli modda calistirmak icin lutfen 1'e basiniz!\n"
           "Normal modda devam etmek icin lutfen 0'a basiniz!\n");

    scanf("%d", &detailedMode);

    while (fgets(line, sizeof(line), fp) != NULL) {
        char *ch = NULL;
        ch = strtok(line, " \r\n");
        while (ch != NULL) {
            ch = strtok(NULL, " \r\n");
            count++;
        }
    }

    fclose(fp);

    count /= 2;
    printf("Load-Factor?\n");
    scanf("%lf", &loadFactor);
//    loadFactor = .9;


    int lfNumber = count / loadFactor;
    int tableSize = getClosestPrime(lfNumber);

    HashTable hashTable;
    hashTable.items = (Item **) calloc(tableSize, sizeof(Item *));
    hashTable.size = tableSize;

    if (detailedMode == 1)
        printf("Hashtable uzunlugu -> %d\n", hashTable.size);



    fp = fopen("Sample.txt", "r");

    while (fgets(line, sizeof(line), fp) != NULL) {
        char buffer[1][255];

        if (strstr(line, "https://") != NULL) {
            char *value;
            value = strtok(line, "\r\n");
            strcpy(buffer[0], value);
        } else {

            char *key = NULL;
            key = strtok(line, " \r\n");
            while (key != NULL) {
                Item *item = calloc(1, sizeof(Item));
                strcpy(item->key, key);
                strcpy(item->value, buffer[0]);

                int insertTryCount = insert(hashTable, item);
                if (detailedMode == 1)
                    printf("%s, %d. denemede tabloya insert edildi! \n", item -> key, insertTryCount);
                key = strtok(NULL, " \r\n");
            }
        }
    }

    if (detailedMode == 1)
        printTable(hashTable);


    while (flag == 1) {
        char buffer[3][255];
        int conditional = 0;

        printf("Aramaniz kosullu olacaksa 1, tek key aratacaksaniz lutfen 0 tuslayiniz! \n");
        scanf("%d", &conditional);

        if (conditional == 1) {
            printf("Lutfen birinci key'i giriniz! \n");
            scanf("%s", buffer[0]);

            printf("Lutfen kondisyonunuzu giriniz! \n");
            scanf("%s", buffer[1]);

            printf("Lutfen ikinci key'i giriniz! \n");
            scanf("%s", buffer[2]);
        } else {
            printf("Lutfen birinci key'i giriniz! \n");
            scanf("%s", buffer[0]);
        }

        if (conditional == 0)
            search(hashTable, buffer[0]);
        else if (strcmp(buffer[1], "ve") == 0)
            searchForAnd(hashTable, buffer[0], buffer[2]);
        else if (strcmp(buffer[1], "veya") == 0)
            searchForOr(hashTable, buffer[0], buffer[2]);
        else
            printf("Hatali bir giris yaptiniz.");

        printf("Aramaya devam etmek istiyor musunuz? \n");
        printf("0 - HAYIR \n");
        printf("1 - EVET \n");
        scanf("%d", &flag);
    }

    return 0;
}
