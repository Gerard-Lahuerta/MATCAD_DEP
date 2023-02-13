
# -*- coding: utf-8 -*-
"""
Created on Sun Jun  6 16:01:59 2021

@author: sofia
"""

#Authors:
    #Sofia Di Capua Martín Mas : 1603685
    #Andrea Gonzalez Aguilera : 1603921
    #Ona Sánchez Núñez : 1601181


from abc import ABC, abstractmethod
import logging


class Node(ABC):
    @abstractmethod

    def predict(self,X):
        pass
    
    """It's an abstract method that will be implemented in the Leaf and Parent classes
    """


class Leaf(Node):
    def __init__(self, label):
        self.label = label
        
    def predict(self, X):
        return self.label
    
    """predict method returns the label of X
    
        Arguments : 
             X : Dataset
    """

class Parent(Node):
    def __init__(self,feature_index, threshold, left_dataset ,right_dataset):
        self.feature_index = feature_index
        self.threshold = threshold
        self.left_child = left_dataset
        self.right_child = right_dataset

    def predict(self, X):
        if (X[self.feature_index] < self.threshold):
            return(self.left_child.predict(X))
        else:
            return (self.right_child.predict(X))

    """predict method returns a prediction depending on if the X feature indeix is lower than the threshold
                                            
        Arguments : 
            X : Dataset
            
        Return :  int
            the prediction of the corresponding child
    
    """
