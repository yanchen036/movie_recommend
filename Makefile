#COMAKE2 edit-mode: -*- Makefile -*-
####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
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
INCPATH=-I. \
  -I./include \
  -I./output \
  -I./output/include
DEP_INCPATH=

#============ CCP vars ============
CCHECK=@ccheck.py
CCHECK_FLAGS=
PCLINT=@pclint
PCLINT_FLAGS=
CCP=@ccp.py
CCP_FLAGS=


#COMAKE UUID
COMAKE_MD5=c446821084bc76b43f10f04387eabce7  COMAKE


.PHONY:all
all:comake2_makefile_check convex 
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:comake2_makefile_check
comake2_makefile_check:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mcomake2_makefile_check[0m']"
	#in case of error, update 'Makefile' by 'comake2'
	@echo "$(COMAKE_MD5)">comake2.md5
	@md5sum -c --status comake2.md5
	@rm -f comake2.md5

.PHONY:ccpclean
ccpclean:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mccpclean[0m']"
	@echo "make ccpclean done"

.PHONY:clean
clean:ccpclean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	rm -rf convex
	rm -rf ./output/bin/convex
	rm -rf src/convex_recommend_main.o
	rm -rf src/convex_pearson_correlation.o

.PHONY:dist
dist:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdist[0m']"
	tar czvf output.tar.gz output
	@echo "make dist done"

.PHONY:distclean
distclean:clean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdistclean[0m']"
	rm -f output.tar.gz
	@echo "make distclean done"

.PHONY:love
love:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlove[0m']"
	@echo "make love done"

convex:src/convex_recommend_main.o \
  src/convex_pearson_correlation.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mconvex[0m']"
	$(CXX) src/convex_recommend_main.o \
  src/convex_pearson_correlation.o -Xlinker "-("   -lpthread \
  -lcrypto \
  -lrt -Xlinker "-)" -o convex
	mkdir -p ./output/bin
	cp -f --link convex ./output/bin

src/convex_recommend_main.o:src/recommend_main.cpp \
  include/pearson_correlation.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/convex_recommend_main.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/convex_recommend_main.o src/recommend_main.cpp

src/convex_pearson_correlation.o:src/pearson_correlation.cpp \
  include/pearson_correlation.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/convex_pearson_correlation.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/convex_pearson_correlation.o src/pearson_correlation.cpp

endif #ifeq ($(shell uname -m),x86_64)


