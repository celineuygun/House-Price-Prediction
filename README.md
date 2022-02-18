# House-Price-Prediction
The files desired to be used in the program can be added to the dataset folder. To run the program, first compile with the command *make* 
then *./main house_price_data.csv* (Or the name of the file you want to use as the dataset).
## Project Architecture
```
dataset/
  house_price_data.csv
  house_price_test.csv  // contains house informations without prices. This data is used for price predictions.
src/ 
  dataset.c
  dataset.h
  main.c
  makefile
  models.c              // contains price prediction models: by similarity and by linear regression
  models.h
  price_prediction_by_linear_regression.txt
  price_prediction_by_similarity.txt
  sorted_data.txt
```

## 'House' Structure
This structure contains house informations in the dataset and is defined with 12 attributes.
```
1.  size                // total number of houses in the dataset
2.  index
3.  id
4.  lotarea
5.  street
6.  saleprice
7.  neighborhood
8.  yearbuilt
9.  overall            // mean(overallqual * a + overallcond * b + kitchenqual * c)
10. overallqual        // Overall quality of the house out of 10
11. overallcond        // Overall condition of the house out of 10
12. kitchenqual        // Ex(Excellent), Gd(Good), TA(Typical/Average), Fa(Fair), Po(Poor)
```
The whole dataset or only a neighborhood can be sorted by id, saleprice, lotarea, overallqual, 
overallcond, kitchenqual and yearbuilt. The result is going to be stored in sorted_data.txt.

## Price Prediction Models
### 1. Finding the most similar houses
Based on the desired lot area and built year difference to search within, if these was not enough for reducing the number of the similar houses to less than or equal to 5,
overall quality, overall condition and kitchen quality rankings are added together with their corresponding coefficients(a,b,c) and this result is used for finding the most similar house(s).
The mean of the most similar houses' sale prices is the prediction of the sale price of that particular house.

### 2. Linear Regression
A linear relationship can be established with price information and the lot area of the house. As the area of the house increases, we expect the price to increase, this is a directly proportional relationship.
Therefore, y = f(x) + ϵ formula is used but the ϵ(the noise in the data) is not taken into account.
Now the formula becomes y = w1 * x + w0, to find w0 and w1 let's turn them into matrices Y = X x W.
* create_data_matrices(): turns lot areas and prices of the houses in the dataset into matrices X and Y respectively.
* calculate_parameter(): finds W by (X^T x X)^-1 x X^T x Y = W.
* make_prediction(): predicts the prices with **Linear Regression** using W and the matrices of the dataset without prices.

