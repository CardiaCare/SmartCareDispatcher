#include "volunteer_algorythm.hpp"

double calculate_distance(sslog_individual_t* location1, sslog_individual_t* location2) {
    double latitude1 = atof((char*) sslog_node_get_property(global_node, location1, PROPERTY_LAT));
    double longitude1 = atof((char*) sslog_node_get_property(global_node, location1, PROPERTY_LONG));
    double latitude2 = atof((char*) sslog_node_get_property(global_node, location2, PROPERTY_LAT));
    double longitude2 = atof((char*) sslog_node_get_property(global_node, location2, PROPERTY_LONG));
    return sqrt(pow(latitude2 - latitude2, 2) + pow(longitude1 - longitude2, 2));
}
