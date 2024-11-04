g++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) hw3tensor.cc -o hw3tensor$(python3-config --extension-suffix)
