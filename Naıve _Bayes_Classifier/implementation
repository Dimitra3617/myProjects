#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov  8 19:40:58 2021

@author: dimitra
"""
import pandas as pd
import numpy as np
import math
from sklearn.model_selection import train_test_split
import random

# 1.1 Data Manipulation and Operations using Pandas

# Read datasets
categoricalX = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_X_categorical.csv', header=None)
categoricalX = categoricalX.to_numpy()

continuousX = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_X_continuous.csv', header=None)
continuousX  = continuousX.to_numpy()

mixedX = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_X_mixed.csv', header=None)
mixedX = mixedX.to_numpy()

categoricalY = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_Y_categorical.csv', header=None)
categoricalY = categoricalY.to_numpy()

continuousY = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_Y_continuous.csv', header=None)
continuousY = continuousY.to_numpy()

mixedY = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_Y_mixed.csv', header=None)
mixedY = mixedY.to_numpy()

categoricalDtypes = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_dtypes_categorical.csv', header=None)
continuousDtypes = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_dtypes_continuous.csv', header=None)
mixedDtypes = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_dtypes_mixed.csv', header=None)


categoricalPossibleVal = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_D_categorical.csv', header=None)
mixedPossibleVal = pd.read_csv('Datasets_Assignment_2_3/Dataset_2.4_D_mixed.csv', header=None)


def  laplaceTrick(np_array, L):
    np_array += L;
    return np_array;

def norm(np_array, count0, count1):
    for i in range(len(np_array)):
        np_array[i][0] = np_array[i][0]/count0;
        np_array[i][1] = np_array[i][1]/count1;
    return np_array;

def train_NBC(X, X_dtype , Y, L, D_categorical):
    count0 = 0; count1 = 0;
    for row in range(len(Y)):
        if Y[row,0] == 0:
            count0 += 1;
        elif Y[row,0] == 1:
            count1 += 1;

    samples = count0 + count1;
    prob_U0 = count0/samples;
    prob_U1 = count1/samples;
    print(count0,count1,samples, prob_U0, prob_U1);
    
    numberOfFeatures = len(X_dtype);
    listWithFrequencies = [];
    thereAreZeros = 0;
    
    for i in range(numberOfFeatures):
            
        if X_dtype.iloc[i,0] == 'discrete':
            possibleValues = D_categorical.iloc[i][0];
            frequency_table = np.zeros((possibleValues, 2), dtype=object)
            for j in range(len(Y)):
                valueOfFeature = int(X[j,i]);
                if Y[j,0] == 0:
                    frequency_table[valueOfFeature][0] += 1;
                elif Y[j,0] == 1:
                    frequency_table[valueOfFeature][1] += 1;
            
            if thereAreZeros == 0:
                for z in range(len(frequency_table)):
                    if frequency_table[z][0] == 0 or frequency_table[z][1] == 0:
                        thereAreZeros = 1;
                        break;
            listWithFrequencies.append(frequency_table);     
        elif X_dtype.iloc[i,0] == 'continuous':
            grouping_table0 = [];
            grouping_table1 = [];
            meanAndVariance = np.zeros((2, 2), dtype=object)
            for j in range(len(Y)):
                if Y[j,0] == 0:
                    grouping_table0.append(X[j][i]);
                elif Y[j,0] == 1:
                    grouping_table1.append(X[j][i]);
                    
            for i in range(len(grouping_table0)):
                meanAndVariance[0][0] += grouping_table0[i];
            for i in range(len(grouping_table1)):
                meanAndVariance[0][1] += grouping_table1[i];
                
            meanAndVariance[0][0] = meanAndVariance[0][0] / len(grouping_table0);
            meanAndVariance[0][1] = meanAndVariance[0][1] / len(grouping_table1);
            #print(meanAndVariance[0][0]);
            #print(meanAndVariance[0][1]);
            for i in range(len(grouping_table0)):
                meanAndVariance[1][0] += (grouping_table0[i] - meanAndVariance[0][0])**2;
            for i in range(len(grouping_table1)):
                meanAndVariance[1][1] += (grouping_table1[i] - meanAndVariance[0][1])**2;
            meanAndVariance[1][0] = meanAndVariance[1][0] / len(grouping_table0) - 1;
            meanAndVariance[1][1] = meanAndVariance[1][1] / len(grouping_table1) - 1;
            listWithFrequencies.append(meanAndVariance);   
  
    #Laplace trick
    if thereAreZeros == 1:
        for i in range(len(listWithFrequencies)):
            if X_dtype.iloc[i,0] == 'discrete':
                print(i, listWithFrequencies[i])
                listWithFrequencies[i] = laplaceTrick(listWithFrequencies[i],L);
   
    print("After laplace")
    for i in listWithFrequencies:
        print(i)
    
    # Normalise the frequency tables towards the respective number of data
    for i in range(len(listWithFrequencies)):
        if X_dtype.iloc[i,0] == 'discrete':
             listWithFrequencies[i] = norm(listWithFrequencies[i],count0,count1);
        
    print("After normalization")
    for i in listWithFrequencies:
        print(i) 
        
    model = (count0, count1, listWithFrequencies);
    return model;
    


def predict_NBC(model, X, X_dtype):
    print('predictFunc');
    
    samples = model[0] + model[1];
    prob_U0 = model[0]/samples;
    prob_U1 = model[1]/samples;
    
    prob0givenSample = prob_U0;
    prob1givenSample = prob_U1;
    
    features = len(X_dtype);
    test_samples = len(X);
    predictions = np.zeros((test_samples), dtype=object);
    for i in range(test_samples):

        prob0givenSample = prob_U0;
        prob1givenSample = prob_U1;
        for j in range(features):

            if X_dtype.iloc[j,0] == 'discrete':
                prob0givenSample = prob0givenSample * model[2][j][int(X[i,j]),0];
                prob1givenSample = prob1givenSample * model[2][j][int(X[i,j]),1];
            elif X_dtype.iloc[j,0] == 'continuous':
                probSampleGiven0 = (1 / (math.sqrt(2 * math.pi * model[2][j][1,0] ** 2))) * math.exp(-((int(X[i,j]) - model[2][0][0,0] ** 2) / (2 * model[2][0][1,0] ** 2)))
                prob0givenSample = prob0givenSample * probSampleGiven0;
                probSampleGiven1 = (1 / (math.sqrt(2 * math.pi * model[2][j][1,1] ** 2))) * math.exp(-((int(X[i,j]) - model[2][0][0,1] ** 2) / (2 * model[2][0][1,1] ** 2)))
                prob1givenSample = prob1givenSample * probSampleGiven1;

        if prob0givenSample > prob1givenSample:
            predictions[i] = 0;
        elif prob0givenSample < prob1givenSample:
            predictions[i] = 1;
        else:
            predictions[i] = -1;        
        print(prob0givenSample,prob1givenSample)
    return predictions;
    #print(predictions);   
  
            
### Prepare datasets
def prepare_trainingSet(X,percentage):
    splitPoint = int(len(X)*(percentage/100))
    #print(splitPoint)
    trainingSet = X.head(splitPoint)
    #print(trainingSet)
    return trainingSet;

def prepare_testSet(X,percentage):
    splitPoint = int(len(X)*(percentage/100))
    #print(splitPoint)
    testSet = X.tail(len(X) - splitPoint)
    #print(testSet)
    return testSet;
    

sumof_categCorrectPredictions = 0;
sumof_continuousCorrectPredictions = 0;
sumof_mixedCorrectPredictions = 0;

s = random.uniform(0.10, 0.40);
Xmixed_trainSet, Xmixed_testSet, Ymixed_trainSet, Ymixed_testSet  = train_test_split(mixedX , mixedY, test_size= s, random_state=42)
    
model = train_NBC(Xmixed_trainSet,mixedDtypes,Ymixed_trainSet,1,mixedPossibleVal);
prediction_table3 = predict_NBC(model, Xmixed_testSet, mixedDtypes);
    
mixedCorrectPredictions = 0;
for x in range(len(Ymixed_testSet)):
    if Ymixed_testSet[x] == prediction_table3[x]:
        mixedCorrectPredictions += 1;
print((mixedCorrectPredictions/len(Ymixed_testSet))*100)
sumof_mixedCorrectPredictions = sumof_mixedCorrectPredictions + mixedCorrectPredictions/len(Ymixed_testSet)*100;

for i in range(100):
    s = random.uniform(0.10, 0.40);
    Xcat_trainSet, Xcat_testSet, Ycat_trainSet, Ycat_testSet  = train_test_split(categoricalX, categoricalY, test_size= s, random_state=42)
    Xcon_trainSet, Xcon_testSet, Ycon_trainSet, Ycon_testSet  = train_test_split(continuousX , continuousY, test_size= s, random_state=42)
    
    model = train_NBC(Xcat_trainSet,categoricalDtypes,Ycat_trainSet,1,categoricalPossibleVal);
    prediction_table1 = predict_NBC(model, Xcat_testSet, categoricalDtypes);
    
    model = train_NBC(Xcon_trainSet,continuousDtypes,Ycon_trainSet,1,0);
    prediction_table2 = predict_NBC(model, Xcon_testSet, continuousDtypes)
    
    categCorrectPredictions = 0;
    for x in range(len(Ycat_testSet)):
         if Ycat_testSet[x] == prediction_table1[x]:
             categCorrectPredictions += 1;
    print((categCorrectPredictions/len(Ycat_testSet))*100)
    sumof_categCorrectPredictions = sumof_categCorrectPredictions + categCorrectPredictions/len(Ycat_testSet)*100;
    
    conCorrectPredictions = 0;
    for x in range(len(Ycon_testSet)):
         if Ycon_testSet[x] == prediction_table2[x]:
             conCorrectPredictions += 1;
    print((conCorrectPredictions/len(Ycon_testSet))*100)
    sumof_continuousCorrectPredictions = sumof_continuousCorrectPredictions + conCorrectPredictions/len(Ycon_testSet)*100 ;


print("Average accuracy: ", (sumof_categCorrectPredictions/100))
print("Average accuracy: ", sumof_continuousCorrectPredictions/100)
print("Accuracy: ", mixedCorrectPredictions/len(Ymixed_testSet)*100)
