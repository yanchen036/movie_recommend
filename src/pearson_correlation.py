#!/usr/bin/python

############################################################
# written by chen yan
# 2013-03-18
############################################################

import sys
import math

class MovieRate:
    'a struct of movie id and its rate'

    def __init__(self):
        self.id_rate = {}
        self.avg_rate = 0.0

    def add(self, id, rate):
        self.id_rate[id] = rate

    def get_rate(self, id):
        return self.id_rate[id]

    def get_ids(self):
        return self.id_rate.keys()

    def get_len(self):
        return len(self.id_rate)

    def calc_avg(self):
        rate_sum = 0.0
        for rate in self.id_rate.itervalues():
            rate_sum += rate;
        self.avg_rate = rate_sum / len(self.id_rate)

    # static function
    def get_union(movie_rate_a, movie_rate_b):
        set_a = set(movie_rate_a.get_ids())
        set_b = set(movie_rate_b.get_ids())
        return set_a & set_b

class PearsonCorrelation:
    'pearson correlation measure'

    def __init__(self):
        # each element of the dict is a pair of <user_id, MoiveRate>
        self.user_rate_dict = {}
        self.distinct_user_ids = []
        self.correlation = {}

    def preprocess_data(self, training_set, distinct_userid):
        training_set_fp = open(training_set, 'r')
        line_count = 1
        for line in training_set_fp:
            fields = line.strip().split()
            user_id = int(fields[0])
            movie_id = int(fields[1])
            rate = float(fields[2])
            if user_id in self.user_rate_dict:
                self.user_rate_dict[user_id].add(movie_id, rate)
            else:
                self.user_rate_dict[user_id] = MovieRate()
                self.user_rate_dict[user_id].add(movie_id, rate)

            if line_count % 100000 == 0:
                print 'read %d lines' % line_count
            line_count += 1
        training_set_fp.close()

        for each_rate in self.user_rate_dict.itervalues():
            each_rate.calc_avg()

        distinct_userid_fp = open(distinct_userid, 'r')
        for line in distinct_userid_fp:
            user_id = int(line.strip())
            self.distinct_user_ids.append(user_id)
        distinct_userid_fp.close()


    def glance(self, user_a, user_b):
        if user_a in self.user_rate_dict:
            print self.user_rate_dict[user_a].id_rate
            print self.user_rate_dict[user_b].id_rate
            sign = str(user_a) + '-' + str(user_b)
            print self.correlation[sign]
        else:
            print 'no such user_id'

    def calc_correlation(self):
        for i in range(0, len(self.distinct_user_ids) - 1):
            if i % 5000 == 0:
                print '%d users calc' % i
            user_a = self.distinct_user_ids[i]
            if user_a in self.user_rate_dict:
                rate_info_a = self.user_rate_dict[user_a]
            else:
                continue
            for j in range(i + 1, len(self.distinct_user_ids)):
                user_b = self.distinct_user_ids[j]
                if user_b in self.user_rate_dict:
                    rate_info_b = self.user_rate_dict[user_b]
                else:
                    continue

                union = MovieRate.get_union(rate_info_a, rate_info_b)
                if len(union) > 0:
                    union_rate_a = []
                    union_rate_b = []
                    for movie_id in union:
                        union_rate_a.append(rate_info_a.get_rate(movie_id))
                        union_rate_b.append(rate_info_b.get_rate(movie_id))
                    avg_rate_a = math.fsum(union_rate_a) / len(union_rate_a)
                    avg_rate_b = math.fsum(union_rate_b) / len(union_rate_b)

                    numerator = 0.0
                    denominator_a = 0.0
                    denominator_b = 0.0
                    for idx in range(0, len(union_rate_a)):
                        union_rate_a[idx] -= avg_rate_a
                        union_rate_b[idx] -= avg_rate_b
                        numerator += union_rate_a[idx] * union_rate_b[idx]
                        denominator_a += union_rate_a[idx] ** 2
                        denominator_b += union_rate_b[idx] ** 2
                    denominator_a = math.sqrt(denominator_a)
                    denominator_b = math.sqrt(denominator_b)
                    if denominator_a != 0 and denominator_b != 0:
                        omega = numerator / (denominator_a * denominator_b)
                        sign = str(user_a) + '-' + str(user_b)
                        self.correlation[sign] = omega


