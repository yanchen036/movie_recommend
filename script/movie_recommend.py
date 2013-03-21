#!/usr/bin/python

############################################################
# written by chen yan
# 2013-01-18
############################################################

import pearson_correlation

if __name__ == "__main__":
    pearson = pearson_correlation.PearsonCorrelation()
    pearson.preprocess_data('./data/datasets/training_set.txt', 'data/datasets/distinct_userId.txt')
    pearson.calc_correlation()
    pearson.glance(1, 5)

