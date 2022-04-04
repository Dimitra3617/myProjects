#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Nov 26 11:01:14 2021

@author: dimitra
"""

import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score
import math
import matplotlib.pyplot as plt

dataset = pd.read_csv('Dataset6.1_XY.csv', index_col=None)
dataset = dataset.to_numpy()

# Part A : Implementation of the Random Forest classifier
def TrainRF(X, Y, n_trees, minSamplesLeaf):
    print("train...")
    
    clfList = []
    num_rows, num_cols = X.shape
    for i in range(n_trees):
        # create the Bootstrapped datasets for the n_trees
        indices = np.random.choice(len(X), len(X))
        bootstrappedX = X[indices]
        bootstrappedY = Y[indices]
        
        clf = DecisionTreeClassifier(criterion='entropy',splitter='best', \
            random_state=None, min_samples_leaf=minSamplesLeaf, \
                max_features=math.floor(math.sqrt(num_cols)));
            
        clfList.append( clf.fit(bootstrappedX, bootstrappedY) )
        
    return clfList;
    
predictionsList = []

def PredictRF(model, X):
    print("predict...")
    
    predY = np.zeros(len(X), dtype=object)
    for i in range(len(model)):
        predictionsList.append( model[i].predict(X) )
        
        for x in range(len(predictionsList[i])):
            if ( predictionsList[i][x] == 1):
                predY[x] = predY[x] + 1
            elif ( predictionsList[i][x] == 0):
                predY[x] = predY[x] - 1
    
        
    for y in range(len(predY)):
        if (predY[y] >= 0): predY[y] = 1;
        elif (predY[y] < 0): predY[y] = 0;
    return predY

# Part B : Test the implementation 
X = dataset[:, :-1]
Y = dataset[:,-1];
Xtrain,Xtest,Ytrain,Ytest = train_test_split(X,Y,test_size=0.3,random_state=42)

model = TrainRF(Xtrain, Ytrain, 100, 1);
predY = PredictRF(model, Xtest);

count = 0;
for i in range(len(predY)):
    if(predY[i] == Ytest[i]): count += 1;
randomForestAccuracy = count / len(predY)  
print("Random forest accuracy:" , randomForestAccuracy)  

# the accuracy of each one decision tree
DTaccuracies = []
total = 0;
for i in range(len(predictionsList)):
    DTaccuracies.append( accuracy_score(Ytest, predictionsList[i]) );
    total = total + DTaccuracies[i]

print("Mean accuracy of DT:", total/len(DTaccuracies))    
print(DTaccuracies)
plt.hist(DTaccuracies,bins=20);
plt.axvline(total/len(DTaccuracies), color='k', linestyle='dashed', linewidth=1.2)
plt.axvline(randomForestAccuracy, color='k', linestyle='solid' , linewidth=1.8)
plt.title("Histogram with accuracies")
plt.xlabel("Accuracy")
plt.ylabel("Total")

# bonus question
DTaccuracies = []
total = 0;
for i in range(0, 100):
    clf = DecisionTreeClassifier(criterion='entropy',splitter='best', \
                random_state=None, min_samples_leaf=1, max_features=None);
    model = clf.fit(Xtrain,Ytrain) 
    predY = model.predict(Xtest)
    DTaccuracies.append( accuracy_score(Ytest, predY ));
    total = total + DTaccuracies[i]
print("~~> Bonus, Mean accuracy of DT:", total/len(DTaccuracies))   
plt.hist(DTaccuracies,bins=20);
plt.axvline(total/len(DTaccuracies), color='k', linestyle='dashdot', linewidth=1.2)
plt.title("Histogram with accuracies")
plt.xlabel("Accuracy")
plt.ylabel("Total") 


