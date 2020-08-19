CC=g++
CFLAGS=-O3 -Wall -Werror -shared -std=c++11 -fPIC
CY=cython
CYFLAGS=--cplus -3

PY=/usr/include/python3.6
BIND=`python3 -m pybind11 --includes`
SUFF=`python3.6-config --extension-suffix`

PREF=cython_example
FILE=cython_wrapper.cpp

build_lib:
	$(CC) $(CFLAGS) heat.cpp skyline/skyline.cpp -o libheat.so
	$(CY) $(CYFLAGS) cython_example.pyx -o cython_wrapper.cpp
	$(CC) $(CFLAGS) $(BIND) -I $(PY) -I . $(FILE) -o $(PREF)$(SUFF) -L. -lheat -Wl,-rpath,.

clean:
	rm -rf cython_wrapper.cpp libheat.so $(PREF)$(SUFF)
