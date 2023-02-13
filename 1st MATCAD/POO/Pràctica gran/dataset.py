# -*- coding: utf-8 -*-
"""
Created on Sun Jun  6 15:57:42 2021

@author: sofia
"""

#Authors:
    #Sofia Di Capua Martín Mas : 1603685
    #Andrea Gonzalez Aguilera : 1603921
    #Ona Sánchez Núñez : 1601181


import numpy as np
from abc import ABC, abstractmethod
import logging


class Dataset():
    def __init__(self,X,y):
        
        #necessary try-except in order to use the same class Dataset for both Classifier and Regressor
        try:
            self.num_samples, self.num_features = X.shape
            self.X=X
            self.y=y
            
        except:
            self.num_samples = len(y)
            self.X=X
            self.y=y
         
    def divide(self, column, threshold):
        left_dataset_X=[]
        left_dataset_y=[]
        right_dataset_X=[]
        right_dataset_y=[]
        index = self.X[:,column] < threshold #index being a list of booleans

        for j in range(self.num_samples):
            
            if (index[j]): #(index[j]==True)
            
              left_dataset_X.append(self.X[j])
              left_dataset_y.append(self.y[j])
              
            else: #(index[j]==False)
                
              right_dataset_X.append(self.X[j])
              right_dataset_y.append(self.y[j])

        left_dataset_X, left_dataset_y = np.array(left_dataset_X),np.array(left_dataset_y)
        right_dataset_X, right_dataset_y = np.array(right_dataset_X),np.array(right_dataset_y)
                        
        return Dataset(left_dataset_X,left_dataset_y),Dataset(right_dataset_X,right_dataset_y)

    """divide method divides the data set into two Dataset --> left and right
    
        Arguments : 
            column : int
            threshold : float
            
        Return:
            
            Two data sets; with the X and y of the corresponding side
    """

    def random_sampling(self, ratio, replace=True):
        random_samples = np.random.choice(range(self.num_samples), \
                        int(ratio*self.num_samples), replace=True) #random_samples being a list
                    
        list_x = []
        list_y = []
        
        for i in range(len(random_samples)):
            list_x.append(self.X[random_samples[i]])
            list_y.append(self.y[random_samples[i]])
            
        list_x = np.array(list_x)
        list_y = np.array(list_y)
        
        return Dataset(list_x, list_y)
    
        """random_sampling  is a part of the sampling technique 
            in which each sample has an equal probability of being chosen
        
            Arguments : 
                ratio : float
                replace : boolean
                
            Return:
                A Dataset
        """

    def distribution(self):
        sum_each_label = np.array([list(self.y).count(label) for label in np.unique(self.y)]) #list with the sum of each label
        distribution_labels = sum_each_label/float(np.sum(sum_each_label)) #sum of each label / sum of labels
        return distribution_labels
    
    """distribution calculates the distribution of labels by doing a list with the sum of each label and dividing by the number of labels
                
            Return:
                float[]
    """

    def most_frequent_label(self):
         labels = list(self.y)
         return max(set(labels), key=labels.count)
     
         """most_frequent_label makes a list of y, then counts how many labels there are of each and gets the maximum.
                
            Return:
                The maximum number of the list
        """
     