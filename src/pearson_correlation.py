#!/usr/bin/python

############################################################
# written by chen yan
# 2013-03-18
############################################################

import sys

class MovieRate:
    'a struct of movie id and its rate'

    def __init__():
        self.ids = []
        self.rates = []
        self.avg_rate = 0.0

    def add(id, rate):
        idx = 0
        while idx < len(self.ids):
            if self.ids[idx] > id:
                break
            idx += 1

        self.ids.insert(idx, id)
        self.rates.insert(idx, rate)

    def calc_avg():
        sum = 0.0
        for rate in self.rates:
            sum += rate;
        self.avg_rate = sum / len(self.rates)

class PearsonCorrelation:
    'pearson correlationn measure'

    def __init__():
        # each element of the dict is a pair of <user_id, MoiveRate>
        self.user_rate_dict = {}

    def preprocess_data(training_set):
        training_set_fp = open(training_set, 'r')
        for line in training_set_fp:
            [user_id, movie_id, rate, time] = line.strip().split()
            if user_id in self.user_rate_dict:
                pass
            else:
                pass

        for each_rate in self.user_rate_dict.itervalues():
            each_rate.calc_avg()

    def dump_file():
        pass

