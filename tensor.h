#include<vector>
#include<stdexcept>

class Tensor{
public:
  std::vector<double> data;
  std::vector<size_t> dims;
  
  Tensor(std::vector<size_t> dims) : dims(dims){
    size_t len = 1;
    for(auto d : dims)
      len *= d;
    data.resize(len);
  }

  Tensor(std::vector<size_t> dims,std::vector<std::vector<size_t>> idx,std::vector<double> val) : dims(dims){
    size_t len = 1;
    for(auto d : dims)
      len *= d;
    data.resize(len);
    if(idx.size() != val.size())
      throw std::runtime_error("Mismatched idx and val size");
    for(size_t i = 0;i < idx.size();++i){
      data[index(idx[i])] = val[i];
    }
  }

  static Tensor ones(std::vector<size_t> dims){
    Tensor ret(dims);
    for(size_t i = 0;i < ret.data.size();++i)
      ret.data[i] = 1;
    return ret;
  }

  size_t index(std::vector<size_t> x){
    if(x.size() != dims.size())
      throw std::runtime_error("Mismatched dims in index");
    size_t ret = 0;
    size_t prod = 1;
    for(int i = dims.size() - 1;i >= 0;--i){
      if(x[i] >= dims[i])
        throw std::runtime_error("Index out of bound");
      ret += x[i] * prod;
      prod *= dims[i];
    } 
    return ret;
  }

  Tensor reshape(std::vector<size_t> new_dims){
    size_t len = 1;
    for(auto d : new_dims)
      len *= d;
    if(len != data.size())
      throw std::runtime_error("Mismatched dims in reshape");
    Tensor ret(new_dims);
    ret.data = data;
    return ret;
  }

  Tensor transpose(){
    if(dims.size() == 2){
      Tensor ret({dims[1],dims[0]});
      for(size_t i = 0;i < dims[0];++i){
        for(size_t j = 0;j < dims[1];++j){
          ret.data[ret.index({j,i})] = data[index({i,j})];
        }
      }
      return ret;
    }else if(dims.size() == 3){
      Tensor ret({dims[0],dims[2],dims[1]});
      for(size_t b = 0;b < dims[0];++b){
        for(size_t i = 0;i < dims[1];++i){
          for(size_t j = 0;j < dims[2];++j){
            ret.data[ret.index({b,j,i})] = data[index({b,i,j})];
          }
        }
      }
      return ret;
    }else{
      throw std::runtime_error("The tensor must be 2D or batched 2D tensors");
    }
  }

  Tensor neg(){
    Tensor ret(dims);
    for(size_t i = 0;i < data.size();++i)
      ret.data[i] = -data[i];
    return ret;
  }
  
  Tensor reciprocal(){
    Tensor ret(dims);
    for(size_t i = 0;i < data.size();++i)
      ret.data[i] = 1.0 / data[i];
    return ret;
  }

  Tensor add(Tensor x){
    if(dims != x.dims)
      throw std::runtime_error("Mismatched shape in add");
    Tensor ret(dims);
    for(size_t i = 0;i < data.size();++i)
      ret.data[i] = data[i] + x.data[i];
    return ret;
  }
  
  Tensor subtract(Tensor x){
    if(dims != x.dims)
      throw std::runtime_error("Mismatched shape in subtract");
    return add(x.neg());
  }

  Tensor mult(double x){
    Tensor ret(dims);
    for(size_t i = 0;i < data.size();++i)
      ret.data[i] = data[i] * x;
    return ret;
  }
  
  Tensor elementwise_mult(Tensor x){
    if(dims != x.dims)
      throw std::runtime_error("Mismatched shape in elementwise_mult");
    Tensor ret(dims);
    for(size_t i = 0;i < data.size();++i)
      ret.data[i] = data[i] * x.data[i];
    return ret;
  }
  
  Tensor pow(double x){
    Tensor ret(dims);
    for(size_t i = 0;i < data.size();++i)
      ret.data[i] = std::pow(data[i],x);
    return ret;
  }
  
  Tensor relu(){
    Tensor ret(dims);
    for(size_t i = 0;i < data.size();++i)
      ret.data[i] = data[i] > 0 ? data[i] : 0;
    return ret;
  }

  Tensor binarilize(){
    Tensor ret(dims);
    for(size_t i = 0;i < data.size();++i)
      ret.data[i] = data[i] > 0 ? 1 : 0;
    return ret;
  }

  Tensor exp(){
    Tensor ret(dims);
    for(size_t i = 0;i < data.size();++i)
      ret.data[i] = std::exp(data[i]);
    return ret;

  }

  
  
const size_t BLOCK_SIZE = 32; 

Tensor matmul(Tensor x) {
    if (x.dims.size() != 2) {
        throw std::runtime_error("The right operand of matmul must be 2D tensors");
    }
    if (dims.size() != 2 && dims.size() != 3) {
        throw std::runtime_error("The left operand of matmul must be 2D tensors or batched 2D tensors");
    }
    if (dims[dims.size() - 1] != x.dims[0]) {
        throw std::runtime_error("Mismatched matmul matrix dimensions");
    }

    if (dims.size() == 2) { // simple 2D matrix multiplication
        Tensor ret({dims[0], x.dims[1]});
        size_t M = dims[0];
        size_t K = dims[1];
        size_t N = x.dims[1];

        // 执行阻塞矩阵乘法
        for (size_t i = 0; i < M; i += BLOCK_SIZE) {
            for (size_t j = 0; j < N; j += BLOCK_SIZE) {
                for (size_t k = 0; k < K; k += BLOCK_SIZE) {
                    for (size_t ib = i; ib < std::min(i + BLOCK_SIZE, M); ++ib) {
                        for (size_t jb = j; jb < std::min(j + BLOCK_SIZE, N); ++jb) {
                            double sum = 0.0;
                            for (size_t kb = k; kb < std::min(k + BLOCK_SIZE, K); ++kb) {
                                sum += data[ib * K + kb] * x.data[kb * N + jb];
                            }
                            ret.data[ib * N + jb] += sum;
                        }
                    }
                }
            }
        }
        return ret;
    } else { // batches 3D matrix multiplication
        Tensor ret({dims[0], dims[1], x.dims[1]});
        size_t B = dims[0];
        size_t M = dims[1];
        size_t K = dims[2];
        size_t N = x.dims[1];

        for (size_t b = 0; b < B; ++b) {
            for (size_t i = 0; i < M; i += BLOCK_SIZE) {
                for (size_t j = 0; j < N; j += BLOCK_SIZE) {
                    for (size_t k = 0; k < K; k += BLOCK_SIZE) {
                        for (size_t ib = i; ib < std::min(i + BLOCK_SIZE, M); ++ib) {
                            for (size_t jb = j; jb < std::min(j + BLOCK_SIZE, N); ++jb) {
                                double sum = 0.0;
                                for (size_t kb = k; kb < std::min(k + BLOCK_SIZE, K); ++kb) {
                                    sum += data[b * M * K + ib * K + kb] * x.data[kb * N + jb];
                                }
                                ret.data[b * M * N + ib * N + jb] += sum;
                            }
                        }
                    }
                }
            }
        }
        return ret;
    }
}




  void print(){
    for(auto x : data)
      printf("%s\n",std::to_string(x).c_str());
  }

  std::vector<double> get_data(){
    return data;
  }

  std::vector<size_t> get_dims(){
    return dims;
  }
  
};
