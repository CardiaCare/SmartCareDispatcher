#pragma once
#include <vector>
#include <cstring>
#include <time.h>
#include <chrono>
typedef std::vector<unsigned char> URI;

class Alarm {
private:
    URI _uri;
    std::chrono::time_point<std::chrono::steady_clock> start_time;
public:
    Alarm(URI uri);
    Alarm(char * uri);
    Alarm();
    URI* getUri();
    ~Alarm();
    std::chrono::time_point<std::chrono::steady_clock> getTime();
    void setTime(std::chrono::time_point<std::chrono::steady_clock>);
};
