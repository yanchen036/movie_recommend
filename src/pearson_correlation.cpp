/***********************************************************
 * written by chen yan
 * 2013-03-20
 * 
 * *********************************************************
 */

#include <iostream>
#include <cstdio>
#include <utility>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "pearson_correlation.h"

void MovieRate::add(uint32_t mid, double rate) {
    size_t i;
    size_t size = _mid.size();
    for (i = 0; i < size; ++i) {
        if (_mid[i] > mid)
            break;
    }
    _mid.insert(_mid.begin() + i, mid);
    _rate.insert(_rate.begin() + i, rate);
}

bool MovieRate::getRate(uint32_t mid, double& rate) {
    size_t lo = 0;
    size_t hi = _mid.size() - 1;
    while (lo <= hi) {
        size_t middle = (lo + hi) / 2;
        if (_mid[middle] == mid) {
            rate = _rate[middle];
            return true;
        }
        else if (_mid[middle] < mid)
            lo = middle + 1;
        else
            hi = middle - 1;
    }
    return false;
}

void MovieRate::calcAvgRate() {
    double sum = 0.0;
    for (std::vector<double>::iterator it = _rate.begin(); it != _rate.end(); ++it) {
        sum += *it;
    }
    _avg_rate = sum / _rate.size();
}

void MovieRate::glance() {
    size_t len = _mid.size();
    for (size_t i = 0; i < len; ++i) {
        printf("uid:%u\trate:%lf\n", _mid[i], _rate[i]);
    }
    return;
}

void MovieRate::getUnion(MovieRate& u, MovieRate& v, std::vector<uint32_t>& union_part) {
    size_t u_idx = 0;
    size_t v_idx = 0;
    union_part.clear();
    while (u_idx < u._mid.size() && v_idx < v._mid.size()) {
        if (u._mid[u_idx] == v._mid[v_idx]) {
            union_part.push_back(u._mid[u_idx]);
            ++u_idx;
            ++v_idx;
        }
        else if (u._mid[u_idx] > v._mid[v_idx])
            ++v_idx;
        else
            ++u_idx;
    }
    return;
}

////////////////////////////////////////////////////////////

void PearsonCorrelation::preprocess(const std::string& training_set, const std::string& distinct_userid) {
    // read training set
    std::ifstream training_set_ifs(training_set.c_str());
    std::string line;
    while (getline(training_set_ifs, line)) {
        uint32_t user_id, m_id, stamp;
        double rate;
        sscanf(line.c_str(), "%u%u%lf%u", &user_id, &m_id, &rate, &stamp);
        std::map<uint32_t, MovieRate*>::iterator mr_it = _userid_movierate_map.find(user_id);
        if (mr_it != _userid_movierate_map.end())
            mr_it->second->add(m_id, rate);
        else {
            MovieRate* mr_p = new MovieRate();
            mr_p->add(m_id, rate);
            _userid_movierate_map.insert(std::make_pair(user_id, mr_p));
        }
    }
    training_set_ifs.close();

    // calculate average rate
    for (std::map<uint32_t, MovieRate*>::iterator it = _userid_movierate_map.begin(); 
            it != _userid_movierate_map.end(); ++it) 
        it->second->calcAvgRate();

    // read distinct user id
    std::ifstream distinct_ifs(distinct_userid.c_str());
    while (getline(distinct_ifs, line)) {
        uint32_t user_id;
        sscanf(line.c_str(), "%u", &user_id);
        _distinct_userid.push_back(user_id);
    }
    distinct_ifs.close();
}

void PearsonCorrelation::calcCorrelation(uint32_t begin, uint32_t len) {
    for (size_t i = begin; i < begin + len && i < _distinct_userid.size(); ++i) {
            /////////////////////
            if (i % 10 == 0)
                std::cout << "cur_u:" << i << std::endl;
            /////////////////////
        uint32_t user_u = _distinct_userid[i];
        std::map<uint32_t, MovieRate*>::iterator movie_rate_u = _userid_movierate_map.find(user_u);
        if (movie_rate_u == _userid_movierate_map.end())
            continue;
        for (size_t j = i + 1; j < _distinct_userid.size(); ++j) {
            uint32_t user_v = _distinct_userid[j];
            std::map<uint32_t, MovieRate*>::iterator movie_rate_v = _userid_movierate_map.find(user_v);
            if (movie_rate_v == _userid_movierate_map.end())
                continue;

            std::vector<uint32_t> union_array;
            MovieRate::getUnion(*(movie_rate_u->second), *(movie_rate_v->second), union_array);
            size_t union_array_size = union_array.size();
            if (union_array_size > 0) {
                std::vector<double> union_rate_u, union_rate_v;
                double avg_rate_u = 0.0;
                double avg_rate_v = 0.0;
                for (size_t idx = 0; idx < union_array_size; ++idx) {
                    double rate_u = 0.0;
                    double rate_v = 0.0;
                    if (movie_rate_u->second->getRate(union_array[idx], rate_u) && 
                            movie_rate_v->second->getRate(union_array[idx], rate_v)) {
                        union_rate_u.push_back(rate_u);
                        union_rate_v.push_back(rate_v);
                        avg_rate_u += rate_u;
                        avg_rate_v += rate_v;
                    }
                }
                avg_rate_u /= union_array_size;
                avg_rate_v /= union_array_size;

                double numerator = 0.0;
                double denominator_u = 0.0;
                double denominator_v = 0.0;
                for (size_t idx = 0; idx < union_array_size; ++idx) {
                    union_rate_u[idx] -= avg_rate_u;
                    union_rate_v[idx] -= avg_rate_v;
                    numerator += union_rate_u[idx] * union_rate_v[idx];
                    denominator_u += union_rate_u[idx] * union_rate_u[idx];
                    denominator_v += union_rate_v[idx] * union_rate_v[idx];
                }
                denominator_u = sqrt(denominator_u);
                denominator_v = sqrt(denominator_v);
                if (denominator_u != 0 && denominator_v != 0) {
                    double omega = numerator / (denominator_u * denominator_v);
                    addCorrelation(user_u, user_v, omega);
                }
            }
        }
    }
    return;
}

void PearsonCorrelation::glance(uint32_t user_u, uint32_t user_v) {
    std::map<uint32_t, MovieRate*>::iterator it = _userid_movierate_map.find(user_u);
    if (it != _userid_movierate_map.end()) {
        it->second->glance();
    }
    it = _userid_movierate_map.find(user_v);
    if (it != _userid_movierate_map.end()) {
        it->second->glance();
    }
    double score;
    if (getCorrelation(user_u, user_v, score))
        printf("score: %lf\n", score);
    else
        printf("no score\n");
}

void PearsonCorrelation::addCorrelation(uint32_t user_u, uint32_t user_v, double correlation) {
    uint64_t sign = user_u;
    sign = sign * 100000000 + user_v;
    _correlation.insert(std::make_pair(sign, correlation));
    return;
}


bool PearsonCorrelation::getCorrelation(uint32_t user_u, uint32_t user_v, double& correlation) {
    uint64_t sign = user_u;
    sign = sign * 100000000 + user_v;
    std::map<uint64_t, double>::iterator it = _correlation.find(sign);
    if (it != _correlation.end()) {
        correlation = it->second;
        return true;
    }
    return false;
}

void PearsonCorrelation::dump(const std::string& correlation_file) {
    std::ofstream ofs(correlation_file.c_str());
    for (std::map<uint64_t, double>::iterator it = _correlation.begin();
            it != _correlation.end(); ++it) {
        ofs << it->first << "\t" << it->second << std::endl;
    }
    ofs.close();
}

void PearsonCorrelation::load(const std::string& correlation_file) {
    std::ifstream ifs(correlation_file.c_str());
    uint64_t sign;
    double score;
    while (ifs >> sign >> score) {
        _correlation.insert(std::make_pair(sign, score));
    }
    ifs.close();
}
