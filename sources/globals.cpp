#include "globals.hpp"

boost::atomic<sslog_node_t *> global_node;
boost::lockfree::queue<Alarm*, boost::lockfree::fixed_sized<false>> alarm_queue(0);
boost::atomic<int> processed_alarms_ccount{0};
bool global_stop_signal_recieved;
std::string filename;
