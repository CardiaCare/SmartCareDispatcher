#pragma once

#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <functional>
#include <time.h>
#include <fstream>
#include <chrono>
extern "C"{
#include "ontology/smartcare.h"
}
#include "smartspace_utils.hpp"
#include "volunteer_algorythm.hpp"
#include "globals.hpp"
#include "AlarmClass.hpp"
void alarm_thread();
void process_alarm( Alarm *);
void single_alarm_worker( Alarm * );
sslog_individual_t* find_nearest_volunteer(sslog_individual_t*);
void ask_help_of_volunteer(sslog_individual_t*, sslog_individual_t*);
void help_response_handler(sslog_subscription_t*);
