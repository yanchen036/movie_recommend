#!/bin/sh

set -x


PART1=(0 5000 10000 15000 20000 25000 30000 35000)
PART2=(40000 50000 60000 70000 80000)
PART3=(90000 110000 130000)

LEN1=5000
LEN2=10000
LEN3=20000

#    ./movie_recommend data/testsets/test_training_set.txt data/testsets/test_userid.txt data/result/pearson_$i $i 2 &

for i in ${PART1[@]}
do
    ./convex data/datasets/training_set.txt data/datasets/distinct_userId.txt data/result/pearson_$i $i $LEN1 &
done

wait

for i in ${PART2[@]}
do
    ./convex data/datasets/training_set.txt data/datasets/distinct_userId.txt data/result/pearson_$i $i $LEN2 &
done

wait

for i in ${PART3[@]}
do
    ./convex data/datasets/training_set.txt data/datasets/distinct_userId.txt data/result/pearson_$i $i $LEN3 &
done

wait

exit 0
