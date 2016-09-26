#pragma once

#include <vector>

extern "C"{
    #include "ontology/smartcare.h"
}
#include "globals.hpp"
#include "AlarmClass.hpp"


int connect_to_smartspace();
unsigned int digit_count(int);
char* generate_uri(sslog_class_t*);
sslog_triple_t* retrieve_first_triple(const char*, const char*, const char*);
char* retrieve_object_by_property(sslog_property_t*, const char*);
char* retrieve_subject_by_property( sslog_property_t*, const char*);
sslog_individual_t* retrieve_range_individual(sslog_property_t*, sslog_individual_t*);
sslog_individual_t* retrieve_domain_individual(sslog_property_t*, sslog_individual_t*);


