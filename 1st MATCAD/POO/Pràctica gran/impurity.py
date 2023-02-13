
# -*- coding: utf-8 -*-
"""
Created on Sun Jun  6 16:03:48 2021

@author: sofia
"""

#Authors:
    #Sofia Di Capua Martín Mas : 1603685
    #Andrea Gonzalez Aguilera : 1603921
    #Ona Sánchez Núñez : 1601181
    
    
import numpy as np
from abc import ABC, abstractmethod
from math import log
import logging


#necessarily abstract so other methods to calculate impurity can be added in the future
class Impurity(ABC):
    @abstractmethod
    def calcul_impurity(self, dataset):
        pass
    
    """
        It's an abstract method that will be implemented
    """

class Gini(Impurity):
    def calcul_impurity(self, dataset):
        summation = 0
        pc = dataset.distribution() #pc being a list
        
        for c in pc:
            summation += c**2

        #Uncomment if necessary
        #logging.info("impurity: " + str(summation))
        return (1 - summation)

    """
    calcul_impurity calculates the impurity of a dataset,
            that are the shape of the graph when all possible values.
            
            Arguments: 
                    dataset : is an object of the Class Dataset 
                
            Return : float
                    one minus the summation
    """


class Entropy(Impurity):
    def calcul_impurity(self,dataset):
        summation = 0
        pc = dataset.distribution() #pc being a list
        for c in pc:
            if(c < 0):
                logging.error("This is an error message")
            assert c >=0, "No existeix el logaritme d'un número negatiu"
            summation += c*log(c)
        
        #Uncomment if necessary
        #logging.info("impurity: " + str(summation))
        return -summation

    """
    calcul_impurity calculates the impurity of a dataset,
            that are the shape of the graph when all possible values.
            
            Arguments: 
                    dataset : is an object of the Class Dataset 
                
            Return : float
                    one minus the summation
    """

class MSE(Impurity):
    def calcul_impurity(self,dataset):

        m = dataset.num_samples
        assert m >=0, "El nombre de mostres no pot ser negatiu"
        
        if(m!=0):
            yi = np.sum(dataset.y)/m

            mse = 0
            for y in dataset.y:
                mse += (y-yi)**2

            return mse

        else:
            return np.infty

    """calcul_impurity calculates the impurity. If dataset.num_samples is not equal to zero calculates mse.
        If mse is equal to zero, we return infinity so that de cost is greater than the rest and it's not taken as minimum_cost. 
        
        Arguments: 
                dataset : is an object of the Class Dataset 
            
        Return : float
                mse value or infinity
        """