# -*- coding: utf-8 -*-
"""
Created on Thu Apr 29 15:28:52 2021

@author: onasa
"""

#Authors:
    #Sofia Di Capua Martín Mas : 1603685
    #Andrea Gonzalez Aguilera : 1603921
    #Ona Sánchez Núñez : 1601181


import numpy as np
from abc import ABC, abstractmethod
import sklearn.datasets
from dataset import Dataset
from node import Leaf, Parent
from impurity import Gini, Entropy, MSE
import time
import multiprocessing
import matplotlib.pyplot as plt
import logging
#from mnist import load


class RandomForest(ABC):
    def __init__(self,max_depth,min_size_split,ratio_samples,num_trees,ratio_features,criterion):

        self.num_trees = num_trees
        self.min_size_split = min_size_split
        self.max_depth = max_depth
        self.ratio_samples = ratio_samples
        self.ratio_features = ratio_features
        self.criterion = criterion
        self.decision_trees = []

        assert num_trees >=0, "El nombre d'arbres no pot ser negatiu"
        assert min_size_split > 0, "La mida mínima de divisió ha de ser positiva"
        assert max_depth >=0, "La profunditat màxima no pot ser negativa"
        assert ratio_samples >=0, "La proporció de mostes no pot ser negativa"
        assert ratio_features >=0, "El nombre de característiques no pot ser negatiu"

        logging.debug("This is a debug message")

        """
       The fit method calls the make_decision_trees or make_decision_trees_multiprocessing method 
        
        Arguments:
            X : float np array
                Contains all the samples and features
            y : float np array
                Contains all the samples and features
            answer : the response passed to it by parameter that that says who to call
            
        Initializes a Dataset object.
        
        """

    def fit(self, X, y, answer):
        assert answer=="S" or answer=="-", "Si us plau, torna a executar i introdueix un caràcter vàlid: S o -"
        dataset = Dataset(X,y)
        if(answer=="S"):
            self._make_decision_trees_multiprocessing(dataset)
        else:
            self.make_decision_trees(dataset)
            
        """
         Make_decision_trees method creates different decision trees chosen in
        randomly different samples from the dataset and generating nodes
    
        Arguments:
            dataset : is an object of the Class Dataset
        
        """

    def make_decision_trees(self, dataset):
        t1 = time.time()
        for tree in range(self.num_trees):
            subset = dataset.random_sampling(self.ratio_samples) #creates a random subset of the dataset each time
            new_tree = self.make_node(subset,1) # 1 = starting depth
            self.decision_trees.append(new_tree)
        t2 = time.time()
        print('{} seconds per tree'.format((t2-t1)/self.num_trees))
        
        """
        A partir d'un dataset el mètode make_decision_trees crea diferents arbres de decisió escollit a
        l'atzar diferents mostres del dataset i generant nodes.
        """

    #if necessary execute with Run > Configuration per file > Execute in an external system terminal
    def _target(self, dataset, nproc):
        #print('process {} starts'.format(nproc))
        subset = dataset.random_sampling(self.ratio_samples)
        tree = self.make_node(subset,1)
        #print('process {} ends'.format(nproc))
        return tree
 
    """
    The _target method is responsible for implementing multiprocessing in
            when making decision trees with the intention of optimizing the code.
        
        Arguments:
            dataset : is an object of the Class Dataset
            nproc : int 
                    tells us the number of processes
                    
                
        Return: 
            A decision tree      
    """
    
    def _make_decision_trees_multiprocessing(self, dataset):
        t1 = time.time()
        with multiprocessing.Pool() as pool:
            self.decision_trees = pool.starmap(self._target, \
                                                [(dataset,nprocess) for nprocess in range(self.num_trees)])
            # use pool.map instead if only one argument for _target
        t2 = time.time()
        print('{} seconds per tree'.format((t2-t1)/self.num_trees))
        
        """
        The _target method is responsible for implementing multiprocessing in
            when making decision trees with the intention of optimizing the code.
            
        Arguments:
            dataset : is an object of the Class Dataset 
        """

    def make_node(self, dataset, depth):
        if (depth==self.max_depth) \
                or (dataset.num_samples<=self.min_size_split)\
                    or (len(np.unique(dataset.y))==1):
            node = self.make_leaf(dataset)
        else:
            node = self.make_parent(dataset, depth)
        return node
    
        """make_node will create a sheet or parent, calling the corresponding functions.
        
        Arguments:
            dataset : is an object of the Class Dataset 
            
            depth : int 
                Saves the depth of the node
                
        Return: 
            The node in a decision tree that is created 
        """

    def make_parent(self, dataset, depth):
        # select a random subset of features, to make trees more diverse
        features_subset = np.random.choice(range(dataset.num_features), self.ratio_features, replace=False)

        # find the best pair (feature, threshold) by exploring all possible pairs
        best_feature_index, best_threshold, minimum_cost, left, right = np.Inf, np.Inf, np.Inf, None, None

        for feature in features_subset:
            # our best attemp of optimitzation, selects 1 random value from the column feature
            val=np.random.choice(np.unique(dataset.X[:, feature]),1)
            left_dataset, right_dataset = dataset.divide(feature, val)
            cost = self.cart_cost(left_dataset, right_dataset)

            if cost < minimum_cost:
                best_feature_index, best_threshold, minimum_cost, left, right = feature, val, cost, left_dataset, right_dataset

            if(left_dataset.num_samples<0 and right_dataset.num_samples<0):
                logging.error("This is an error message")

            assert left_dataset.num_samples >= 0 or right_dataset.num_samples >= 0, "El nombre de mostres no pot ser negatiu"
            if(left_dataset.num_samples==0 or right_dataset.num_samples==0):
                return self.make_leaf(dataset)


        node = Parent(best_feature_index, best_threshold, self.make_node(left, depth + 1), self.make_node(right, depth + 1))
        return node
    
        """Decides which is the best division of the dataset.
            Find this best division by testing several values ​​to divide the dataset and calculating the cost of each with
            the cost_cart function. Once it has the best division and the parameters that give it (best_feature_index, best_threshold,
            minimum_cost and left and right datasets), if the number of samples in the created datasets is 0 a leaf node is created,
            otherwise it creates a relative.
        
        
        Arguments: 
            dataset : is an object of the Class Dataset 
            
            depth : int 
                Saves the depth of the node
                
        Return:
            The node in a decision tree that is created (is a parent)
        
        """

    # the threshold and feature that cause the division with least cart_cost are the best ones.
    def cart_cost(self, left_dataset, right_dataset): #J(k,v) of the slides
        impurity_left = self.criterion.calcul_impurity(left_dataset)
        impurity_right = self.criterion.calcul_impurity(right_dataset)
        m_left = left_dataset.num_samples
        m_right = right_dataset.num_samples

        if(m_right==0 and m_left==0):
            logging.error("This is an error message")

        assert m_right>0 or m_left>0, "No es pot fer una divisió per zero" # can't use the cost formula with m_left and m_right dividing --> division by zero
        cost = (m_left/(m_right+m_left))*impurity_left + (m_right/(m_right+m_left))*impurity_right

        return cost
    
        """The cart_cost method calculates the cost of making a division with the relevant criteria of the impurity class.
        
        
        Arguments:
            left_dataset : float
                Is an object of the Class Dataset - the left child  
            
            right_dataset : float
                Is an object of the Class Dataset - the right child
            
            
        Return:
            The cost of the division that we have made
        """

    @abstractmethod
    def make_leaf(self,dataset):
        pass

    """It's an abstract method that will be implemented by RandomForestClassifier and RandomForestRegressor
    """

    def predict(self, X):
        y_pred=[]
        for x in X:
            predictions = [root.predict(x) for root in self.decision_trees] #list of predictions for each node
            y_pred.append(self.combine_predictions(predictions))

        return np.array(y_pred)
    
    """predict method creates a NumPy ndarray object, with the values in X, by using the array() function
                                            
        Arguments : 
            X : float np array
                Contains all the samples and features
            
        Return :  
            A list of predictions 
    
    """
    
    #For Cross Validation
    def next_fold(num_samples_train, num_folds):
        pack=np.random.permutation(range(num_samples_train)) #datasets desordenados
        #Algoritmo que distribuya cada uno de los samples de forma equitativa
        #yield return iterado, vamos a devolver listas de posiciones
        # Le damos listas
        to_distribute = 0 #los que he de repartir
        begin_of_distribution = 0 #por donde empezamos a repartir
        #while(num_folds - distributed != 0): #ha repartido todo
        for distributed in range (num_folds):
            to_distribute = int((num_samples_train-to_distribute)/(num_folds-distributed))
            yield np.append(pack[:begin_of_distribution], pack[begin_of_distribution+to_distribute:]), \
                    pack[begin_of_distribution:begin_of_distribution+to_distribute]
            begin_of_distribution += to_distribute
            
        """Next_fols is a method used to do cross Validation. Divides the data set into distributed mats
            equitably.
        
            Arguments : 
                num_samples_train : int
                                    The number of samples per train
                    
                num_folds : int
                            the number of folds in which you want to divide the train
                
        """


#used to assign classes to unclassified items or values
class RandomForestClassifier(RandomForest):

    def make_leaf(self, dataset):
        return Leaf(dataset.most_frequent_label())
    
    """
    Arguments:
        dataset : is an object of the Class Dataset
    Returns: 
        a Leaf (an object from the leaf class)
        
    """

    def combine_predictions(self, predictions):
        return max(set(predictions), key=(predictions.count))
    
    """Combine_predictions is a method that gets the maximum value
            of all the predictions that are in a list named prediction
        
            Arguments:
                predictions : list
                            is a list of predictions that we have done
                
            Return : 
                The maximum of all the predictions in the list
                
    """


#used to predict and estimate values
class RandomForestRegressor(RandomForest):

    def make_leaf(self, dataset):
        return Leaf(np.mean(dataset.y))
    
    """
    Arguments:
        dataset : is an object of the Class Dataset
    Returns: 
        a Leaf (an object from the leaf class)
        
    """

    def combine_predictions(self, predictions):
        return np.mean(predictions) # mean = mitjana


    """
    Combine_predictions is a method that with a np function calculates 
            the mean of all the predictions that are in a list named prediction
        
            Arguments:
                predictions : list
                            is a list of predictions that we have done
                
            Return : 
                The mean of all the predictions in the list
                
        
    """

#Code to create regressor datasets
def sumSins():
    rng = np.random.RandomState(1)
    X = np.linspace(0, 6, 300)[:, np.newaxis]
    y = np.sin(X).ravel() + np.sin(6 * X).ravel() + rng.normal(0, 0.1, X.shape[0])
    return X, y

def sin():
    rng = np.random.RandomState(1)
    X = np.sort(5 * rng.rand(200, 1), axis=0)
    y = np.sin(X).ravel()
    return X, y

if __name__ == "__main__":

    answer=input("Cross Validation?(S/-): ")
    if(answer!="S"):

        # This is our best attempt to validate the classifier
        print("COMPROVACIÓ CLASSIFIER")

        iris = sklearn.datasets.load_iris() # it's a dictionary, the iris dataset loaded

        logging.info(iris.DESCR)

        X, y = iris.data, iris.target

        #Other criterions to calculate impurity can be added
        criterion = Gini()

        ratio_train, ratio_test, ratio_val = 0.7, 0.2, 0.1
        # 70% train, 20% test, 10% validation

        num_samples, num_features = X.shape # 150, 4

        idx = np.random.permutation(range(num_samples))
        # shuffle {0,1, ... 149} because samples come sorted by class!

        num_samples_train = int(num_samples*ratio_train)
        num_samples_test = int(num_samples*ratio_test)
        idx_train = idx[:num_samples_train]
        idx_test = idx[num_samples_train : num_samples_train+num_samples_test]
        X_train, y_train = X[idx_train], y[idx_train]
        X_test, y_test = X[idx_test], y[idx_test]

        max_depth = 10 # max. num. of levels of decision trees
        min_size_split_split = 5 # if less do not split a node
        ratio_samples = 0.7 # bootstrap
        num_trees = 70
        num_features_node = int(np.sqrt(num_features))

        print("Vols utilitzar multiprocessing pel classificador?(S/-) \n")
        answer=input()

        rf = RandomForestClassifier(max_depth, min_size_split_split, ratio_samples, num_trees, num_features_node, criterion)

        # train = make the decision trees
        rf.fit(X_train,y_train,answer)
        # classification
        y_pred = rf.predict(X_test)
        # compute accuracy
        num_samples_test = len(y_test)
        num_correct_predictions = np.sum(y_pred == y_test)
        accuracy = num_correct_predictions/float(num_samples_test)
        print('accuracy {} %'.format(100*np.round(accuracy,2)))

        # This is our best attempt to validate the regressor
        print("COMPROVACIÓ REGRESSOR")

        X, y = sumSins() # creates the dataset, can be changed to sin()

        #Other criterions to calculate error can be added
        criterion = MSE()

        ratio_train, ratio_test, ratio_val = 0.7, 0.2, 0.1
        # 70% train, 20% test, 10% validation
        X,y = sumSins()
        num_samples, num_features = X.shape # 150, 4

        idx = np.random.permutation(range(num_samples))

        num_samples_train = int(num_samples*ratio_train)
        num_samples_test = int(num_samples*ratio_test)
        idx_train = idx[:num_samples_train]
        idx_test = idx[num_samples_train : num_samples_train+num_samples_test]

        X_train, y_train = X[idx_train], y[idx_train]
        X_test, y_test = X[idx_test], y[idx_test]

        max_depth = 10 # max. num. of levels of decision trees
        min_size_split_split = 5 # if less do not split a node
        ratio_samples = 0.7 # bootstrap
        num_trees = 70
        num_features_node = int(np.sqrt(num_features))

        print("Vols utilitzar multiprocessing pel regresor?(S/-) \n")
        answer=input()

        rf = RandomForestRegressor(max_depth, min_size_split_split, ratio_samples, num_trees, num_features_node, criterion)

        # train = make the decision trees
        rf.fit(X_train,y_train,answer)
        # regression
        y_pred = rf.predict(X_test)
        # compute error
        rmse = np.sqrt(np.sum((y_pred - y_test) ** 2) / float(len(y_test)))
        print("Error MSE:",rmse)

        #Plot helps to graphically compare the prediction and the real value of the function

        plt.figure()
        plt.plot(X_train, y_train, '.')
        plt.title('train')
        plt.figure()
        plt.plot(X_test, y_test, 'g.', label='test')
        plt.plot(X_test, y_pred, 'y.', label='prediction')
        plt.legend()
        answer=input()


    else:

        print("COMPROVACIÓ REGRESSOR")
        #split X, t into (X_train, y_train),(X_test, y_test)
        X,y=sumSins()
        num_samples = len(y)
        idx = np.random.permutation(range(num_samples))
        ratio_train=0.7
        ratio_test=0.3
        num_samples_train = int(num_samples*ratio_train)
        num_samples_test = int(num_samples*ratio_test)
        idx_train = idx[:num_samples_train]
        idx_test = idx[num_samples_train : num_samples_train+num_samples_test]

        X_train, y_train = X[idx_train], y[idx_train]
        X_test, y_test = X[idx_test], y[idx_test]
        num_folds=4
        #max_depth,min_size_split,ratio_samples,num_trees,num_features,criterion):
        #decide values to explore for each hyperparameter
        max_depth, min_size_split,ratio_samples, num_trees, num_features, criterion_used = [10,7,5], [5,4,3], [0.7, 0.6, 0.5] , [1], [1], MSE()
        # exhaustive search, all possible combinations
        best_values = []
        best_accuracy = np.infty
        for hyper1 in max_depth:
            for hyper2 in min_size_split:
                for hyper3 in ratio_samples:
                    for hyper4 in num_trees:
                        for hyper5 in num_features:
                            accuracies=[]
                            for idx_train, idx_val in RandomForest.next_fold(num_samples_train,num_folds):
                                rf = RandomForestRegressor(hyper1, hyper2, hyper3, hyper4, hyper5, criterion_used)
                                rf.fit(X[idx_train], y[idx_train],"-")
                                y_pred = rf.predict(X[idx_val])
                                acc_fold = np.sqrt(np.sum((y_pred - y[idx_val]) ** 2) / float(len(y[idx_val])))
                                accuracies.append(acc_fold)
                            if np.mean(accuracies)<best_accuracy:
                                best_values = [hyper1, hyper2, hyper3, hyper4, hyper5]
                                best_accuracy = np.mean(accuracies)
        #evaluation on test set
        rf = RandomForestRegressor(best_values[0], best_values[1], best_values[2], best_values[3], best_values[4], criterion_used)
        num_samples = len(y)
        idx = np.random.permutation(range(num_samples))
        ratio_train=0.7
        ratio_test=0.3
        num_samples_train = int(num_samples*ratio_train)
        num_samples_test = int(num_samples*ratio_test)
        idx_train = idx[:num_samples_train]
        idx_test = idx[num_samples_train : num_samples_train+num_samples_test]

        X_train, y_train = X[idx_train], y[idx_train]
        X_test, y_test = X[idx_test], y[idx_test]
        rf.fit(X_train, y_train, "-")
        y_pred = rf.predict(X_test)
        acc = np.sqrt(np.sum((y_pred - y_test) ** 2) / float(len(y_test)))
        print("Error MSE:",acc)
        answer=input("Introdueix qualssevol caràcter per continuar amb el classifier: ")

        print("COMPROVACIÓ CLASSIFIER")
        iris = sklearn.datasets.load_iris() # it's a dictionary, the iris dataset loaded
        X, y = iris.data, iris.target
        #Other criterions to calculate impurity can be added
        criterion = Entropy()

        num_samples = len(y)
        idx = np.random.permutation(range(num_samples))
        ratio_train=0.7
        ratio_test=0.3
        num_samples_train = int(num_samples*ratio_train)
        num_samples_test = int(num_samples*ratio_test)
        idx_train = idx[:num_samples_train]
        idx_test = idx[num_samples_train : num_samples_train+num_samples_test]

        X_train, y_train = X[idx_train], y[idx_train]
        X_test, y_test = X[idx_test], y[idx_test]
        num_folds=4
        #max_depth,min_size_split,ratio_samples,num_trees,num_features,criterion):
        #decide values to explore for each hyperparameter
        max_depth, min_size_split,ratio_samples, num_trees, num_features, criterion_used = [10,7,5], [5,4,3], [0.7, 0.6, 0.5] , [1], [1], criterion
        # exhaustive search, all possible combinations
        best_values = []
        best_accuracy = np.infty
        for hyper1 in max_depth:
            for hyper2 in min_size_split:
                for hyper3 in ratio_samples:
                    for hyper4 in num_trees:
                        for hyper5 in num_features:
                            accuracies=[]
                            for idx_train, idx_val in RandomForest.next_fold(num_samples_train,num_folds):
                                rf = RandomForestClassifier(hyper1, hyper2, hyper3, hyper4, hyper5, criterion_used)
                                rf.fit(X[idx_train], y[idx_train],"-")
                                y_pred = rf.predict(X[idx_val])
                                acc_fold = np.sqrt(np.sum((y_pred - y[idx_val]) ** 2) / float(len(y[idx_val])))
                                accuracies.append(acc_fold)
                            if np.mean(accuracies)<best_accuracy:
                                best_values = [hyper1, hyper2, hyper3, hyper4, hyper5]
                                best_accuracy = np.mean(accuracies)
        #evaluation on test set
        rf = RandomForestClassifier(best_values[0], best_values[1], best_values[2], best_values[3], best_values[4], criterion_used)
        rf.fit(X_train, y_train, "-")
        y_pred = rf.predict(X_test)

        num_correct_predictions = np.sum(y_pred == y_test)
        accuracy = num_correct_predictions/float(num_samples_test)
        print('accuracy {} %'.format(100*np.round(accuracy,2)))
        answer=input()