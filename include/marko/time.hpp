/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <chrono>  // time: sec, msec, usec
#include <thread>

namespace marko {

inline void msleep(int msec){ std::this_thread::sleep_for(std::chrono::milliseconds(msec)); }
inline void usleep(int usec){ std::this_thread::sleep_for(std::chrono::microseconds(usec)); }
inline void  sleep(int  sec){ std::this_thread::sleep_for(std::chrono::seconds(sec)); }

}
