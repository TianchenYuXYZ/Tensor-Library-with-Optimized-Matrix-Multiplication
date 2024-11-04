#include<stdio.h>
#include<string.h>
#include<pybind11/pybind11.h>
#include<pybind11/numpy.h>
#include<pybind11/stl.h>

namespace py = pybind11;

void touch_alloc(){
  int* tmp;
  cudaMalloc(&tmp,1);
  cudaFree(tmp);
}

PYBIND11_MODULE(touch_gpu,m){
  m.def("touch_alloc",&touch_alloc);
}

