nvcc -forward-unknown-to-host-compiler -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) touch_gpu.cu -o touch_gpu$(python3-config --extension-suffix) -arch=compute_61 -code=sm_61
