#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset.h"
#define BUFFER_SIZE 1024

House* read_house_data(char* print_to_file) {
    FILE* fp = fopen(print_to_file, "r");
    if(!fp) {
        printf("Error while opening the print_to_file. Check if the print_to_file name is correct.\n");
        exit(1);
    } 
    House* houses = calloc(NUM_HOUSE, sizeof(House));
    if(!houses) exit(1);
    char buffer[BUFFER_SIZE], *token;
    int i = 0, lenBuf;
    while(fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        lenBuf = strlen(buffer);
        buffer[lenBuf - 1] = (buffer[lenBuf - 1] == '\n') ? '\0' : buffer[lenBuf - 1];
        if(buffer[0] == ',') continue;
        token = strtok(buffer, ",");
        houses[i].index = atoi(token);
        token = strtok(NULL, ",");
        houses[i].id = atoi(token);
        token = strtok(NULL, ",");
        houses[i].saleprice = atoi(token);
        token = strtok(NULL, ",");
        houses[i].lotarea = atoi(token);
        token = strtok(NULL, ",");
        strcpy(houses[i].street, token);
        token = strtok(NULL, ",");
        houses[i].overallqual = atoi(token);
        token = strtok(NULL, ",");
        houses[i].overallcond = atoi(token);
        token = strtok(NULL, ",");
        strcpy(houses[i].kitchenqual, token);
        token = strtok(NULL, ",");
        houses[i].yearbuilt = atoi(token);
        token = strtok(NULL, ",");
        strcpy(houses[i].neighborhood, token);
        token = strtok(NULL, ",");
        houses->size++;
        ++i;
    } 
    fclose(fp);
    return houses;
} 
House* read_house_data_test(char* print_to_file) {
    FILE* fp = fopen(print_to_file, "r");
    if(!fp) {
        printf("Error while opening the print_to_file. Check if the print_to_file name is correct.\n");
        exit(1);
    } 
    House* houses = calloc(NUM_HOUSE, sizeof(House));
    if(!houses) exit(1);
    char buffer[BUFFER_SIZE], *token;
    int i = 0, lenBuf;
    while(fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        lenBuf = strlen(buffer);
        buffer[lenBuf - 1] = (buffer[lenBuf - 1] == '\n') ? '\0' : buffer[lenBuf - 1];
        if(buffer[0] == ',') continue;
        token = strtok(buffer, ",");
        houses[i].index = atoi(token);
        token = strtok(NULL, ",");
        houses[i].id = atoi(token);
        houses[i].saleprice = 0;
        token = strtok(NULL, ",");
        houses[i].lotarea = atoi(token);
        token = strtok(NULL, ",");
        strcpy(houses[i].street, token);
        token = strtok(NULL, ",");
        houses[i].overallqual = atoi(token);
        token = strtok(NULL, ",");
        houses[i].overallcond = atoi(token);
        token = strtok(NULL, ",");
        strcpy(houses[i].kitchenqual, token);
        token = strtok(NULL, ",");
        houses[i].yearbuilt = atoi(token);
        token = strtok(NULL, ",");
        strcpy(houses[i].neighborhood, token);
        token = strtok(NULL, ",");
        houses->size++;
        ++i;
    } 
    fclose(fp);
    return houses;
} 

void struct_filler(House* new, House* old) {
    new->id = old->id;
    new->lotarea = old->lotarea;
    new->overallcond = old->overallcond;
    new->overallqual = old->overallqual;
    new->saleprice = old->saleprice;
    new->yearbuilt = old->yearbuilt;
    strcpy(new->kitchenqual, old->kitchenqual);
    strcpy(new->neighborhood, old->neighborhood);
    strcpy(new->street, old->street);
} 

void print_houses(House* houses) {
    printf("\n==========================================================================================================================\n");
    printf(" INDEX     ID      SALE PRICE      LOT AREA     STREET     OVERALL      OVERALL      KITCHEN      YEAR      NEIGHBORHOOD\n");
    printf("   ↓       ↓           ↓              ↓           ↓        QUALITY     CONDITION     QUALITY      BUILT          ↓\n");
    printf("==========================================================================================================================\n");
    for(int i = 0; i < houses->size; ++i)
        printf("[%-4d]    %-10d %-14d %-12d %-12s %-12d %-11d %-10s %-12d %-12s\n", houses[i].index, houses[i].id, houses[i].saleprice, houses[i].lotarea, houses[i].street, houses[i].overallqual,
                                                    houses[i].overallcond, houses[i].kitchenqual, houses[i].yearbuilt, houses[i].neighborhood);
    printf("==========================================================================================================================\n");
    printf("   ↑       ↑           ↑              ↑           ↑        OVERALL      OVERALL      KITCHEN      YEAR            ↑\n");
    printf(" INDEX     ID      SALE PRICE      LOT AREA     STREET     QUALITY     CONDITION     QUALITY      BUILT      NEIGHBORHOOD\n");
    printf("==========================================================================================================================\n");
} 

void print_house(House house) {
    printf("\n===== HOUSE INFO\n");
    printf("ID                : %d\n", house.id);
    printf("LOT AREA          : %d\n", house.lotarea);
    printf("STREET            : %s\n", house.street);
    printf("NEIGHBORHOOD      : %s\n", house.neighborhood);
    printf("SALE PRICE        : %d\n", house.saleprice);
    printf("BUILT YEAR        : %d\n", house.yearbuilt);
    printf("OVERALL QUALITY   : %d\n", house.overallqual);
    printf("OVERALL CONDITION : %d\n", house.overallcond);
    printf("KITCHEN QUALITY   : %s\n", house.kitchenqual);
} 

House get_house_by_id(House* houses, int id) {
    int l = 0, r = NUM_HOUSE - 1;
    while(l <= r) {
        int m = (l + r)/2;
        if(houses[m].id == id) return houses[m];
        else if(houses[m].id < id) l = m + 1;
        else r = m - 1;
        
    } 
    printf("Not found.\n");
    exit(1);
} 

House* get_neighborhoods(House* houses, House house) {
    char *neighborhood = house.neighborhood;
    House *n_houses = calloc(10, sizeof(House));
    if(!n_houses) exit(1);
    int i = 0, ind = 0;
    while(houses[i].id != NUM_HOUSE) {
        if(!strcmp(houses[i].neighborhood, neighborhood)) {
            if(i > 10) {
                n_houses = realloc(n_houses, sizeof(House)*i);
            } 
            n_houses[ind].index = ind;
            struct_filler(&(n_houses[ind]), &(houses[i]));
            ind++;
        } 
        ++i;
    } 
    n_houses->size= ind;
    return n_houses;
} 

float mean_sale_prices(House* houses) {
    float prices = 0;
    for(int i = 0; i < houses->size; ++i) {
        prices += houses[i].saleprice;
    } 
    return (houses->size == 0) ? 0 : (float)(prices / houses->size);
} 

House* sort_houses(House* houses, int criteria, int size, int order, int a, int b, int c, int print_to_file) {
    House* sorted = calloc(houses->size, sizeof(House));
    if(!sorted) exit(1);
    Hash** hash_table = calloc(size, sizeof(Hash*));
    if(!hash_table) exit(1);
    Hash *hash_node, *curr, *next;
    int hash_index;
    for(int p = 0; p < size; ++p) {
      hash_table[p] = calloc(1, sizeof(Hash));
      if(!hash_table[p]) exit(1);
      hash_table[p]->house.id = -1;
      hash_table[p]->next = NULL;
    } 
    
    for(int i = 0; i < houses->size; ++i) {
        if(criteria == 1) break; // If there is no need to sort the dataset again (already sorted by id)
        else if(criteria == 2) hash_index = houses[i].saleprice;
        else if(criteria == 3) hash_index = houses[i].lotarea;
        else if(criteria == 4) hash_index = houses[i].overallqual;
        else if(criteria == 5) hash_index = houses[i].overallcond;
        else if(criteria == 6) {
            if(!strcmp(houses[i].kitchenqual, "Ex")) hash_index = 0;
            else if(!strcmp(houses[i].kitchenqual, "Gd")) hash_index = 1;
            else if(!strcmp(houses[i].kitchenqual, "TA")) hash_index = 2;
            else if(!strcmp(houses[i].kitchenqual, "Fa")) hash_index = 3;
            else if(!strcmp(houses[i].kitchenqual, "Po")) hash_index = 4;
        } else if(criteria == 7) hash_index = houses[i].yearbuilt;
        else if(criteria == 0) { // mean(overallqual + overallcond + kitchenqual)
            hash_index = houses[i].overallqual * a + houses[i].overallcond * b;
            if(!strcmp(houses[i].kitchenqual, "Gd")) hash_index += c;
            else if(!strcmp(houses[i].kitchenqual, "TA")) hash_index += 2 * c;
            else if(!strcmp(houses[i].kitchenqual, "Fa")) hash_index += 3 * c;
            else if(!strcmp(houses[i].kitchenqual, "Po")) hash_index += 4 * c;
            hash_index /= 3;
        } 
        hash_node = hash_table[hash_index];
        if(hash_node->house.id == -1) struct_filler(&(hash_node->house), &(houses[i]));
        else{
            for(curr = hash_node; curr->next != NULL; curr = curr->next);
            next = calloc(1, sizeof(Hash));
            if(!next) exit(1);
            next->next = NULL;
            struct_filler(&(next->house), &(houses[i]));
            curr->next = next;
        } 
    } 
    if(print_to_file) {
        FILE *fp = fopen("sorted_data.txt", "w");
        if(!fp) exit(1);
        fprintf(fp, "==================================================================================================================\n");
        fprintf(fp, "   ID      SALE PRICE      LOT AREA     STREET     OVERALL      OVERALL      KITCHEN      YEAR      NEIGHBORHOOD\n");
        fprintf(fp, "   ↓           ↓              ↓           ↓        QUALITY     CONDITION     QUALITY      BUILT          ↓\n");
        fprintf(fp, "==================================================================================================================\n");
        for(int i = 0; i < size; ++i) {
            int m;
            if(order == 0) m = i;
            else m = size - i - 1;
            if(criteria == 1) {
                fprintf(fp,"  %-10d %-14d %-12d %-12s %-12d %-11d %-10s %-12d %-12s\n", houses[m].id, houses[m].saleprice, houses[m].lotarea, houses[m].street, 
                                                houses[m].overallqual, houses[m].overallcond, houses[m].kitchenqual, houses[m].yearbuilt, houses[m].neighborhood);
                continue;
            } 
            hash_node = hash_table[m];
            if(hash_node->house.id == -1) continue;
            for(curr = hash_node; curr != NULL; curr = curr->next) {
                fprintf(fp,"  %-10d %-14d %-12d %-12s %-12d %-11d %-10s %-12d %-12s\n", curr->house.id, curr->house.saleprice, curr->house.lotarea, curr->house.street, 
                                                    curr->house.overallqual, curr->house.overallcond, curr->house.kitchenqual, curr->house.yearbuilt, curr->house.neighborhood);
            } 
        } 
        fclose(fp);
        printf("\nFile successfully created.\n");
    } 
    for(int i = 0, ind = 0; i < size; ++i) {
        int m;
        if(order == 0) m = i;
        else m = size - i - 1;
        hash_node = hash_table[m];
        if(hash_node->house.id == -1) continue;
        for(curr = hash_node; curr != NULL && ind < houses->size; curr = curr->next, ++ind) {
            struct_filler(&(sorted[ind]), &(curr->house));
            sorted[ind].index = ind;
            if(criteria == 0) sorted[ind].overall  = m;
            sorted->size++;
        } 
    } 
    for(int p = 0; p < size; ++p) {
      curr = hash_table[p];
      if(curr) {
        while(curr != NULL) {
           hash_node = curr;
           curr = curr->next;
           free(hash_node);
        } 
     } 
   } 
   free(hash_table);
   return sorted;
} 