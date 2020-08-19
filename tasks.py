import invoke
import sys


def compile_python_module(cpp_name, extension_name):

    invoke.run("g++ -O3 -Wall -Werror -shared -std=c++11 -fPIC "
               "`python3 -m pybind11 --includes` "
               "-I /usr/include/python3.6 -I .  "
               "{0} "
               "-o {1}`python3.6-config --extension-suffix` "
               "-L. -lheat -Wl,-rpath,.".format(cpp_name, extension_name)
    )


def build_cython():

    """ Build the cython extension module """

    print("Building Cython Module")

    invoke.run("g++ -O3 -Wall -Werror -shared -std=c++11 -fPIC heat.cpp skyline/skyline.cpp -o libheat.so")

    invoke.run("cython --cplus -3 cython_example.pyx -o cython_wrapper.cpp")

    compile_python_module("cython_wrapper.cpp", "cython_example")

    print("* Complete")

if __name__ == '__main__':
    build_cython()
