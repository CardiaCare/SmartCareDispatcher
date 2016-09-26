#pragma once

#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include "ontology/smartcare.h"
#include "AlarmClass.hpp"

#define KP_SS_NAME "X"
//#define KP_SS_ADDRESS "127.0.0.1"
#define KP_SS_ADDRESS "78.46.130.194"
//#define KP_SS_ADDRESS "194.85.173.9"
#define KP_SS_PORT 10010

extern boost::atomic<sslog_node_t *> global_node;
extern boost::atomic<int> processed_alarms_ccount;
extern boost::lockfree::queue<Alarm*, boost::lockfree::fixed_sized<false>> alarm_queue;
extern bool global_stop_signal_recieved;
extern std::string filename;
