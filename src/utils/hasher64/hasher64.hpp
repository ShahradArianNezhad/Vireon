#include <cstddef>
#include <cstdint>

struct Hasher64{
  uint64_t value=14695981039346656037ull;

  void combine(const void* data,size_t size){
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    for(size_t i=0;i<size;i++){
      value ^= bytes[i];
      value*= 109951162821ull;
    }
  }


  void clear(){value=14695981039346656037ull;}
  uint64_t digest() const {return value;}
};
