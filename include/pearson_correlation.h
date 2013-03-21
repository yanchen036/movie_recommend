/***********************************************************
 * written by chen yan
 * 2013-03-20
 * 
 * *********************************************************
 */

#include <map>
#include <vector>
#include <string>

class MovieRate
{
public:
    MovieRate(): _avg_rate(0.0) {}
    ~MovieRate() {}

    void add(uint32_t mid, double rate);
    bool getRate(uint32_t mid, double& rate);
    void calcAvgRate();
    void glance();

    static void getUnion(MovieRate& u, MovieRate& v, std::vector<uint32_t>& union_part);

private:
    std::vector<uint32_t> _mid;
    std::vector<double> _rate;
    double _avg_rate;
};

class PearsonCorrelation
{
public:
    PearsonCorrelation() {}
    ~PearsonCorrelation() {
        for (std::map<uint32_t, MovieRate*>::iterator it = _userid_movierate_map.begin();
                it != _userid_movierate_map.end(); ++it) {
            delete it->second;
        }
    }

    void preprocess(const std::string& training_set, const std::string& distinct_userid);
    void calcCorrelation(uint32_t begin, uint32_t len);
    void glance(uint32_t user_u, uint32_t user_v);
    void dump(const std::string& correlation_file);
    void load(const std::string& correlation_file);

private:
    // user_u > user_v
    void addCorrelation(uint32_t user_u, uint32_t user_v, double correlation);
    bool getCorrelation(uint32_t user_u, uint32_t user_v, double& correlation);

private:
    std::map<uint32_t, MovieRate*> _userid_movierate_map;
    std::vector<uint32_t> _distinct_userid;
    std::map<uint64_t, double> _correlation;
};


