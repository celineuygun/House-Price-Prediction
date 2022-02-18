#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset.h"
#include "models.h"

// For price prediction by similarity
// Overall Quality * A + Overall Cond * B + Kitchen Quality * C
#define A 4
#define B 3
#define C 2
// Only for test data (price_prediction_by_similarity.txt)
#define YEAR_DIFF 1000
#define M2_DIFF 12

int input(char *question, int start, int end) {
  int loc = 0;
  printf("\nEnter the %s\n>> ", question);
  scanf("%d", &loc);
  while((loc < start) || (loc > end)) {
    printf("\nPlease enter a valid one.\n>> ");
    scanf("%d", &loc);
  } return loc;
} 

int main(int argc, char* argv[]) {
  if(argc != 2) {
    printf("Please enter the file name you want to read as the second argument.\n");
    exit(1);
  } 
  while(!strstr(argv[1], ".csv")) {
    printf("File name needs to contain .csv extension.\n");
    exit(1);
  }
  char fileloc[] = "../dataset/";
  char *data = calloc(strlen(fileloc) + strlen(argv[1]) + 1, sizeof(char));
  if(!data) exit(1);
  strcpy(data, fileloc);
  strcat(data, argv[1]);

  int response, ind, id, criteria, size, answer, order, year;
  float price, m2;
  float *X = NULL, *Y = NULL, *W = NULL;
  House house, *neighborhood, *sorted;
  House* houses = read_house_data(data);
  House* houses_test = read_house_data_test("../dataset/house_price_test.csv");

  while(1) {
    printf("\nMENU:\n");
    printf("1: PRINT DATASET\n");
    printf("2: PRINT A PARTICULAR HOUSE'S INFO BY INDEX\n");
    printf("3: PRINT A PARTICULAR HOUSE'S INFO BY ID\n");
    printf("4: LIST THE NEIGHBORS OF A PARTICULAR HOUSE\n");
    printf("5: MEAN OF THE SALE PRICES OF A HOUSE'S NEIGHBORHOOD\n");
    printf("6: EXPORT SORTED DATASET TO FILE\n");
    printf("7: SEE MEAN OF THE SIMILAR HOUSES' SALEPRICES OR PRICE PREDICTION BY SIMILARITY\n");
    printf("8: PRICE PREDICTION BY LINEAR REGRESSION\n");
    printf("0: EXIT\n>> ");
    scanf("%d", &response);
    switch(response) {
      case 0:
        printf("\nBYE!\n");
        exit(1);

      case 1:
        print_houses(houses);
        break;
    
      case 2:
        ind = input("index of the house whose info you want to print.", 0, NUM_HOUSE);
        print_house(houses[ind]);
        break;

      case 3:
        id = input("id of the house whose info you want to print.", ID_START, ID_END);
        house = get_house_by_id(houses, id);
        print_house(house);
        break;

      case 4:
        id = input("id of the house whose neighborhood you want to list.", ID_START, ID_END);
        house = get_house_by_id(houses, id);
        neighborhood = get_neighborhoods(houses, house);
        print_houses(neighborhood);
        break;

      case 5:
        id = input("id of the house whose neighborhood you want to see the mean of the sale prices.", ID_START, ID_END);
        house = get_house_by_id(houses, id);
        neighborhood = get_neighborhoods(houses, house);
        printf("\nMEAN OF THE SALE PRICES OF %s: %.2f\n", house.neighborhood, mean_sale_prices(neighborhood));
        break;

      case 6:
        printf("\nWould you like to sort whole dataset(1) or only a neighborhood?(2)\n>> ");
        scanf("%d", &answer);
        printf("\nPlease select an option below to sort the list accordingly.\n");
        printf("1: ID\n");
        printf("2: Sale price\n");
        printf("3: Lot area\n");
        printf("4: Street\n");
        printf("5: Overall quality\n");
        printf("6: Overall condition\n");
        printf("7: Kitchen quality\n");
        printf("8: Year built\n");
        criteria = input("criteria", 1, 8);

        if(criteria == 7)
          order = input("order you want to sort, best to worst(0) or worst to best(1)", 0, 1);
        else if(criteria == 5 || criteria == 6)
          order = input("order you want to sort, worst to best(0) or best to worst(1)", 0, 1);
        else if(criteria == 8)
          order = input("order you want to sort, oldest to newest(0) or newest to oldest(1)", 0, 1);
        else if(criteria == 1 || criteria == 2 || criteria == 3)
          order = input("order you want to sort, lowest to highest(0) or highest to lowest(1)", 0, 1);
        else criteria = 0;
      
        if(criteria == 2) size = PRICE_SIZE;
        else if(criteria == 3) size = LOT_SIZE;
        else if(criteria == 5 || criteria == 6) size = OVERALL_SIZE;
        else if(criteria == 7) size = KIT_TYPES;
        else if(criteria == 8) size = YEAR_SIZE;

        if(answer == 1) {
          if(criteria == 1 || criteria == 4) size = NUM_HOUSE;
          sorted = sort_houses(houses, criteria, size, order, 0, 0, 0, 1);
        } else{
          id = input("id of the house whose neighborhood you want to sort.", ID_START, ID_END);
          house = get_house_by_id(houses, id);
          neighborhood = get_neighborhoods(houses, house);
          if(criteria == 1 || criteria == 4) size = neighborhood->size;
          sorted = sort_houses(neighborhood, criteria, size, order, 0, 0, 0, 1);
        } break;

      case 7:
        printf("\nWould you like to see the price average for similar houses(1) or predict the prices of houses in the test data?(2)\n>> ");
        scanf("%d", &answer);
        if(answer == 1) {
          id = input("id of the house whose similars you want to see.", ID_START, ID_END);
          house = get_house_by_id(houses, id);
          m2 = input("positif m^2 value for the range you want to search within.", 0, LOT_SIZE);
          year = input("value for the built year range.", 0, YEAR_SIZE);
          price = model_by_similarity(houses, house, m2, year, A, B, C);
          printf("\nEstimated price: %.2f\n", price);
        } else if(answer == 2) {
          FILE* fp = fopen("price_prediction_by_similarity.txt", "w");
          if(fp) {
            for(int i = 0; i < houses_test->size; ++i) {
              price = model_by_similarity(houses, houses_test[i], M2_DIFF, YEAR_DIFF, A, B, C);
              fprintf(fp, "House ID: %-3d Estimated price: %.2f\n", houses_test[i].id, price);
            } 
            fclose(fp);
            printf("\nThe file named 'price_prediction_by_similarity.txt' is created.\n");
          } else printf("\nError while opening the file.\n");
        } else printf("\nInvalid option.\n");
        break;

      case 8:
        if(!W) {
          X = calloc(2 * houses->size, sizeof(float));
          Y = calloc(1 * houses->size, sizeof(float));
          if(!X || !Y) exit(1);
          create_data_matrices(houses, houses->size, 2, X, 1, Y);
          W = calculate_parameter(houses->size, 2, X, 1, Y);
          if(!W) exit(1);
          free(X);
          free(Y);
        }
        make_prediction(houses_test, W);
        break;

      default:
        printf("\nPlease enter a valid option.\n");
        break;
    } 
  } 
  free(data);
  free(sorted);
  free(houses);
  free(houses_test);
  free(neighborhood);
  return 0;
} 
