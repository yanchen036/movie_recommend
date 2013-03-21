/***********************************************************
 * written by chen yan
 * 2013-03-20
 * 
 * *********************************************************
 */

#include <string>
#include <cstdlib>

#include "pearson_correlation.h"

int main(int argc, char** argv)
{
    std::string training_set(argv[1]);
    std::string distinct_userid(argv[2]);
    std::string output_path(argv[3]);
    int begin = atoi(argv[4]);
    int len = atoi (argv[5]);

    PearsonCorrelation pearson;
    pearson.preprocess(training_set, distinct_userid);
    pearson.calcCorrelation(begin, len);
    pearson.dump(output_path);

    return 0;
}
