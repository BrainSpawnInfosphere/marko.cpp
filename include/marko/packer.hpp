// https://en.cppreference.com/w/cpp/utility/variadic
#pragma once

#include <cstdint>
#include <cstring> // memcpy
#include <string>
// #include <cstddef> // byte
#include <cstdarg> // varidac functions
// #include <map>

constexpr int c_size{sizeof(char)};
constexpr int B_size{sizeof(uint8_t)};
constexpr int i_size{sizeof(int)};
constexpr int f_size{sizeof(float)};
constexpr int d_size{sizeof(double)};

constexpr int NUM{128};
typedef uint8_t byte;

class bytearray {
public:
  bytearray() : size(0) {}

  void packf(const char *fmt...) {
    size = 0;
    va_list args;
    va_start(args, fmt);

    while (*fmt != '\0') {
      if (*fmt == 'f') {
        float f = (float)va_arg(args, double);
        memcpy(&buffer.b[size], &f, f_size);
        size += f_size;
      }
      else if (*fmt == 'd') {
        double f = va_arg(args, double);
        memcpy(&buffer.b[size], &f, d_size);
        size += d_size;
      }
      else if (*fmt == 'c') {
        char f = (char)va_arg(args, int);
        memcpy(&buffer.b[size], &f, c_size);
        size += c_size;
      }
      else if (*fmt == 'B') {
        uint8_t f = (uint8_t)va_arg(args, unsigned int);
        memcpy(&buffer.b[size], &f, B_size);
        size += B_size;
      }
      else if (*fmt == 'i') {
        int f = (char)va_arg(args, int);
        memcpy(&buffer.b[size], &f, i_size);
        size += i_size;
      }
      ++fmt;
    }

    va_end(args);
  }

  /**
   * to buffer
   */
  void pack(const std::string &src) {
    size = src.size();
    memcpy(buffer.c, const_cast<char *>(src.data()), src.size());
    // std::cout << "pack " << src << " size " << size << std::endl;
  }

  /**
   * to buffer
   * pack(&struct, sizeof(struct))
   */
  void pack(void *src, int size) {
    memcpy(buffer.b, src, size);
    this->size = size;
  }

  /**
   * string -> struct
   */
  void to_struct(const std::string &s, void *dst) {
    memcpy(dst, const_cast<char *>(s.data()), s.size());
    size = 0;
  }

  /**
   * buffer -> string
   */
  std::string to_string() {
    std::string ss(buffer.c, size);
    // std::cout << "to_string " << ss << " size " << size << std::endl;
    size = 0;
    return ss;
  }

protected:
  int size;
  union {
    byte b[NUM];
    char c[NUM];
  } buffer;
  // byte buffer[NUM];
};

// class Packer {
// public:
//     Packer(): size(0) {}
//
//     // constexpr int num{32};
//     void pack(const char* fmt...) // C-style "const char* fmt, ..." is also
//     valid
//     {
//         va_list args;
//         va_start(args, fmt);
//
//         while (*fmt != '\0') {
//             if (*fmt == 'f') {
//                 float f = (float)va_arg(args, double);
//                 memcpy(&buffer.b[size], &f, 4);
//                 size += 4;
//             }
//             else if (*fmt == 'd') {
//                 double f = va_arg(args, double);
//                 memcpy(&buffer.b[size], &f, 8);
//                 size += 8;
//             }
//             else if (*fmt == 'c') {
//                 char f = (char)va_arg(args, int);
//                 memcpy(&buffer.b[size], &f, 1);
//                 size += 1;
//             }
//             else if (*fmt == 'B') {
//                 uint8_t f = (uint8_t) va_arg(args, unsigned int);
//                 memcpy(&buffer.b[size], &f, 1);
//                 size += 1;
//             }
//             else if (*fmt == 'i') {
//                 int f = (char)va_arg(args, int);
//                 memcpy(&buffer.b[size], &f, 4);
//                 size += 4;
//             }
//             ++fmt;
//         }
//
//         va_end(args);
//     }
//
//     std::string to_string() {
//         std::string ss(buffer.c, size);
//         size = 0;
//         return ss;
//     }
//
//     void pack(void* src, int size){
//         memcpy(buffer.b, src, size);
//     }
//
//     // template?
//     void unpack(const char* fmt, int num, void* dst){
//         int base = 0;
//         for (int i=0; i<num; i++) base += fmts[fmt[i]];
//         memcpy(dst, &buffer.b[base], fmts[fmt[num]]);
//     }
//
//     void unpack(const std::string& s, void* dst){
//         memcpy(dst, const_cast<char*>(s.data()), s.size());
//     }
//
//     // std::string fmt;
//     int size;
//     std::map<char, int> fmts { {'c', 1}, {'B', 1}, {'f', 4}, {'d', 8}, {'i',
//     4}, }; union { std::byte b[num*sizeof(float)]; float f[num]; char
//     c[num*sizeof(float)];} buffer;
// };
