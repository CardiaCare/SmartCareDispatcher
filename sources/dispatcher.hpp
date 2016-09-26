#pragma once

extern "C"{
    #include "ontology/smartcare.h"
}
#include <signal.h>
#include <cstdlib>
#include <iostream>
#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "globals.hpp"
#include "alarm_processing.hpp"
#include "smartspace_utils.hpp"
#include "alarm_subscription.hpp"
#include <fstream>

void ctrl_c_handler(int);
int connect_to_smartspace();
int subscribe_to_alarm();
void new_alarm_handler(sslog_subscription_t*);
