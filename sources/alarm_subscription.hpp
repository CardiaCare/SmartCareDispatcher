#pragma once
#include <time.h>
extern "C"{
    #include "ontology/smartcare.h"
}

#include "globals.hpp"
#include "smartspace_utils.hpp"
#include <fstream>
#include <chrono>

void sbcr_thread();
int subscribe_to_alarm();
void new_alarm_handler(sslog_subscription_t*);
