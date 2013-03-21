#!/urs/local/bin/python

import sys

result = []
testing_set = open(sys.argv[1], 'r')
for line in testing_set:
    line = line.strip()
    line += '\t4.0\n'
    result.append(line)
testing_set.close()

predict = open(sys.argv[2], 'w')
predict.writelines(result)
predict.close()
