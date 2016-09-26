#include "AlarmClass.hpp"

Alarm::Alarm(void) : _uri() {}
Alarm::Alarm(URI uri) : _uri( std::move( uri ) ) {}
Alarm::Alarm(char* uri)
{
    URI::size_type size = strlen((char*)uri) ;
    _uri.assign(uri, uri + size + 1);
}
Alarm::~Alarm()
{
    URI temp_uri;
    _uri.swap(temp_uri);
}
URI* Alarm::getUri()
{
    return &_uri;
}
std::chrono::time_point<std::chrono::steady_clock> Alarm::getTime()
{
    return start_time;
}
void Alarm::setTime(std::chrono::time_point<std::chrono::steady_clock> _time){
    start_time = _time;
}
