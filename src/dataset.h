#ifndef _DATASET_H
#define _DATASET_H

#define ID_START 101
#define ID_END 1460
#define NUM_HOUSE 1360
#define NUM_HOUSE_TEST 100

#define STREET_SIZE 5
#define NHOOD_SIZE 20
#define KIT_SIZE 2

#define KIT_TYPES 5
#define STREET_TYPES 1
#define NHOOD_TYPES 100
#define LOT_SIZE 500000
#define PRICE_SIZE 1000000
#define YEAR_SIZE 2021
#define OVERALL_SIZE 11

typedef struct house{ 
    int size;
    int index;
    int id;
    int lotarea;
    char street[STREET_SIZE];
    int saleprice;
    char neighborhood[NHOOD_SIZE];
    int yearbuilt;
    int overall;
    int overallqual;
    int overallcond;
    char kitchenqual[KIT_SIZE];
} House;

typedef struct hash{
    House house;
    struct hash* next;
} Hash;

void print_houses(House* houses);
void struct_filler(House* new, House* old);
House* read_house_data(char* file);
House* read_house_data_test(char* file);
void print_house(House house);
House get_house_by_id(House* houses, int id);
House* get_neighborhoods(House* houses, House house);
float mean_sale_prices(House* houses);
House* sort_houses(House* houses, int criteria, int size, int order, int a, int b, int c, int print_to_file);

#endif // _DATASET_H
