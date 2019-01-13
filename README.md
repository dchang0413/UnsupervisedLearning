# UnsupervisedLearning
Unsupervised learning using K-means clustering for CSCI 4350 Introduction to Artificial Intelligence
Overview

Develop a software agent in C++ to perform K-means clustering on labeled classification data.
Procedure

    Create a C++ program (kmeans.cpp) which calculates a K-means clustering of a provided set of input training data, assigns classification labels to each cluster using a majority vote, and then reports the classification performance on a separate set of input testing data.
        The program should take 5 command-line arguments (integer: random seed, integer: the number of clusters, integer: the number of real-valued features in the data set, string: input training data filename, string: input testing data filename)
        The program should read in the training data file (one training example per line, see below)
        The program should read in the testing data file (one testing example per line, see below)
        Each line will contain the specified number of real feature values and a single interger value (the class label) at the end
        The program should perform a K-means clustering by first initializing K centroid vectors (no class labels included) using K random examples from the training data
        The program should then determine the closest vector to each training example (using Euclidean distance), and create a new set of vectors by averaging the feature vectors of the closest training examples.
        The program should repeat the previous step until the centroid vectors no longer change (i.e. until all training examples are assigned to the same vector on two consequtive iterations)
        Once the mean cluster vectors have been calculated, a class label will be assigned to each vector by taking a majority vote amongst it's assigned examples from the training set (ties will be broken by preferring the smallest integer class label).
        Finally, the program will calculate the closest vector to each testing example and determine if the cluster label and the testing example label match (a correct classification).
        The program should then output the number of testing examples classified correctly by the K-means clustering
