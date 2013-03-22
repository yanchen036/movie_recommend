#edit-mode: -*- Makefile -*-
CC=gcc
CXX=g++
CXXFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CPPFLAGS=-D_GNU_SOURCE \
  -D__STDC_LIMIT_MACROS \
  -DVERSION=\"1.9.8.7\"
INCPATH=-I./include


recommend : src/recommend_main.o src/pearson_correlation.o
	$(CXX) -o recommend src/recommend_main.o src/pearson_correlation.o

src/recommend_main.o : ./src/recommend_main.cpp ./include/pearson_correlation.h
	$(CXX) -c $(INCPATH) $(CPPFLAGS) $(CXXFLAGS) -o src/recommend_main.o ./src/recommend_main.cpp

src/pearson_correlation.o : ./src/pearson_correlation.cpp ./include/pearson_correlation.h
	$(CXX) -c $(INCPATH) $(CPPFLAGS) $(CXXFLAGS) -o src/pearson_correlation.o ./src/pearson_correlation.cpp

clean :
	rm recommend src/recommend_main.o src/pearson_correlation.o
