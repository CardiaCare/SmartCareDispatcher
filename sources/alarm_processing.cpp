#include "alarm_processing.hpp"

boost::thread_group alarm_thread_pool;
std::chrono::time_point<std::chrono::steady_clock> alarm_time;
std::chrono::time_point<std::chrono::steady_clock> time_when_from_queue;
/*
 * 'Main' function analog for alarm thread
 */
void alarm_thread()
{
    while(!global_stop_signal_recieved){
        if( !alarm_queue.empty() ){
            Alarm* alarm;
            alarm_queue.pop(alarm);
            URI *_alarm_uri = alarm->getUri();
            //char *alarm_uri = reinterpret_cast<char*>(_alarm_uri->data());
            std::cout << "Alarm with URI : "
                      << reinterpret_cast<char*>(_alarm_uri->data())
                      << " had been got from queue\n"
                      << std::endl;
//            std::cout << "initial alarm processing time = " << alarm->getTime() << std::endl;
//            std::cout << "current time = " << clock() << std::endl;
            alarm_time = alarm->getTime();
            time_when_from_queue = std::chrono::steady_clock::now();
            process_alarm(alarm);
            //alarm_thread_pool.create_thread( boost::bind( process_alarm , alarm_uri ) );
        }
    }
    std::cout << "Wait for all alarm processing stopping\n";
    alarm_thread_pool.join_all();
}

void process_alarm( Alarm* alarm ){

//    std::cout << "Starting processing Alarm with URI: " << alarm_uri << std::endl;
//    std::cout << "Bind new thread for Alarm with URI: " << alarm_uri << "\n";
    single_alarm_worker(alarm);
//    alarm_thread_pool.create_thread( boost::bind(single_alarm_worker, alarm_uri) );
}
void wait(int milliseconds)
{
    boost::this_thread::sleep_for(boost::chrono::milliseconds{milliseconds});
}
void single_alarm_worker( Alarm* alarm_object ) {
    URI* _alarm_uri = alarm_object->getUri();
    char *alarm_uri = reinterpret_cast<char*>(_alarm_uri->data());
    sslog_individual_t *alarm = sslog_node_get_individual_by_uri(global_node, alarm_uri);
    for(int index = 0; index < 100 && NULL == alarm; index++  ){
        std::cout << "No Alarm" << std::endl;
        wait(10);
        alarm = sslog_node_get_individual_by_uri(global_node, alarm_uri);
    }
    if(alarm == NULL){
        return;
    }
    wait(100);
    sslog_individual_t *patient = retrieve_domain_individual(PROPERTY_SENDALARM, alarm);
    for(int index = 0; index < 100 && NULL == patient; index++  ){
        std::cout << "No Patient" << std::endl;
        wait(10);
        patient = retrieve_domain_individual(PROPERTY_SENDALARM, alarm);

    }
    if(patient == NULL){
        return;
    }
    wait(100);
    sslog_individual_t *location = (sslog_individual_t *) sslog_node_get_property(global_node, patient, PROPERTY_HASPERSLOCATION);/*    retrieve_range_individual(PROPERTY_HASPERSLOCATION, patient);*/
    sslog_node_populate(global_node, location);
    for(int index = 0; index < 100 && NULL == location; index++  ){
        std::cout << "No Location" << std::endl;
        wait(10);
        location = (sslog_individual_t *) sslog_node_get_property(global_node, patient, PROPERTY_HASPERSLOCATION);/*    retrieve_range_individual(PROPERTY_HASPERSLOCATION, patient);*/;
        sslog_node_populate(global_node, location);
    }
    if(location == NULL){
        return;
    }
    wait(100);
    sslog_individual_t* volunteer = find_nearest_volunteer(location);
    for(int index = 0; index < 3 && NULL == volunteer; index++  ){
        std::cout << "No Volunteer" << std::endl;
        wait(100);
        volunteer = find_nearest_volunteer(location);
        std::cout << "There are no free volunteers yet. Waiting...\n";

    }
    if(volunteer == NULL){
        //std::cout << "Push alarm back to queue 'cause of lack of volunteers\n";
        //        URI *_uri = new URI();
        //        URI::size_type size = strlen((char*)alarm_uri) ;
        //        _uri->assign(alarm_uri, alarm_uri + size + 1);
        // Alarm *alarm_object = new Alarm((char *)alarm_uri);
        alarm_object->setTime(alarm_object->getTime());
        alarm_queue.push( alarm_object );
        return;
    }
    //processed_alarms_ccount++;
    std::cout << "Total: " << processed_alarms_ccount << std::endl;
    std::cout << "End Alarm" << std::endl;
    ask_help_of_volunteer(volunteer, alarm);

}
/**
 * Find nearest free volunteer and return it.
 * If no volunteer will be found then the function return NULL.
 */
sslog_individual_t* find_nearest_volunteer(sslog_individual_t* patient_location) {
    sslog_triple_t *req_triple = sslog_new_triple_detached(
                SS_RDF_SIB_ANY,
                sslog_entity_get_uri(PROPERTY_HELPPROVIDINGSTATUS),//"http://www.w3.org/1999/02/22-rdf-syntax-ns#type",
                "READY TO HELP",
                SSLOG_RDF_TYPE_URI, SSLOG_RDF_TYPE_LIT);
    list_t *volunteers = sslog_node_query_triple(global_node, req_triple);
    sslog_free_triple(req_triple);
    list_head_t *iterator = NULL;
    sslog_individual_t* nearest_volunteer = NULL;
    double best_distance = 0;

    list_for_each(iterator, &volunteers->links) {
        list_t *list_node = list_entry(iterator, list_t, links);
        sslog_triple_t *result_triple = (sslog_triple_t*)list_node->data;
        char *volunteer_uri = (char *) result_triple->subject;
        sslog_individual_t *volunteer = sslog_node_get_individual_by_uri(global_node, volunteer_uri);
        sslog_individual_t *volunteer_location = retrieve_range_individual(PROPERTY_HASPERSLOCATION, volunteer);
        if(volunteer_location != NULL){
            if (nearest_volunteer == NULL) {
                nearest_volunteer = volunteer;
                sslog_node_populate(global_node, volunteer_location);
                best_distance = calculate_distance(patient_location, volunteer_location);
                continue;
            }
            sslog_node_populate(global_node, volunteer_location);
            double distance = calculate_distance(patient_location, volunteer_location);
            if (distance < best_distance) {
                best_distance = distance;
                nearest_volunteer = volunteer;
            }
        }
    }
    list_free_with_nodes(volunteers, NULL);

    return nearest_volunteer;
}
/**
 * Send help request and subscribe to response
 */
void ask_help_of_volunteer(sslog_individual_t* volunteer, sslog_individual_t* alarm) {
    std::cout << "Send help request to volunteer with URI " << sslog_entity_get_uri(volunteer) << "\n";

    char ready_to_help[] = "READY TO HELP";
    char help_request[] = "HELP REQUEST";

    sslog_node_update_property(global_node, volunteer, PROPERTY_HELPPROVIDINGSTATUS, ready_to_help, help_request);
    sslog_node_insert_property(global_node, volunteer, PROPERTY_RESPONSETOALARM, alarm);

    sslog_subscription_t *subscription = sslog_new_subscription(global_node, true);
    list_t* properties = list_new();
    list_add_data(properties, PROPERTY_HELPPROVIDINGSTATUS);

    sslog_sbcr_add_individual(subscription, volunteer, properties);
    sslog_sbcr_set_changed_handler(subscription, help_response_handler);
    if (sslog_sbcr_subscribe(subscription) != SSLOG_ERROR_NO) {
        std::cout << "Can't subscribe to help request update.\n";
    }
}
/**
 * Handle help response from a volunteer. If the volunteer confirm the help request
 * then this function finish work. If the volunteer reject the request then the function
 * remove alarm from the volunteer and call find_next_volunteer_and_ask_for_help() function.
 */
void help_response_handler(sslog_subscription_t* subscription) {
    sslog_sbcr_changes_t *changes = sslog_sbcr_get_changes_last(subscription);
    const list_t *status_list =
            sslog_sbcr_ch_get_triples(changes, SSLOG_ACTION_INSERT);
    if (status_list == NULL) {
        return;
    }
    list_head_t *iterator = NULL;
    list_for_each(iterator, &status_list->links) {
        list_t *list_node = list_entry(iterator, list_t, links);
        sslog_triple_t *status = (sslog_triple_t *) list_node->data;

        if (strcmp(status->object, "CONFIRMED") == 0) {
            printf("The volunteer confirm help providing\n");
            //            auto finish = std::chrono::steady_clock::now();
            //            double elapsed_seconds_in_queue = std::chrono::duration_cast<
            //              std::chrono::duration<double> >(time_when_from_queue - alarm_time).count();
            //            double elapsed_seconds_after_queue = std::chrono::duration_cast<
            //              std::chrono::duration<double> >(finish - time_when_from_queue).count();
            //            std::ofstream myfile;
            //            myfile.open (filename, std::ofstream::out | std::ofstream::app);
            //            myfile << elapsed_seconds_in_queue << " " << elapsed_seconds_after_queue << std::endl;
            //            myfile.close();
            // FOR EXPERIMENT. INDIVIDUAL REMOVING
            sslog_individual_t *volunteer = sslog_node_get_individual_by_uri(global_node, status->subject);
            sslog_node_populate(global_node, volunteer);
            processed_alarms_ccount++;
            std::cout << "Total: " << processed_alarms_ccount << std::endl;
            sslog_sbcr_unsubscribe(subscription);
            return;
        }
        if (strcmp(status->object, "REJECTED") == 0) {
            printf("The volunteer reject help providing\n");

            char *volunteer_uri = (char *) status->subject;
            sslog_individual_t *volunteer = sslog_node_get_individual_by_uri(global_node, volunteer_uri);
            if(NULL == volunteer){
                std::cout << "Volunteer is null" << std::endl;
                return;
            }
            sslog_individual_t *alarm = (sslog_individual_t *)sslog_node_get_property(global_node, volunteer, PROPERTY_RESPONSETOALARM);
            if(NULL == alarm){
                std::cout << "Alarm is null" << std::endl;
                return;
            }


            std::cout << "Push alarm back to queue 'cause of volunteer's rejecting\n";
            const char* alarm_uri = sslog_entity_get_uri(alarm);
            std::cout << "Current Alarm URI: " << alarm_uri << std::endl;
            sslog_node_remove_property(global_node, volunteer, PROPERTY_RESPONSETOALARM, alarm);

            Alarm *alarm_object = new Alarm((char *)alarm_uri);
            alarm_object->setTime(alarm_time);
            alarm_queue.push( alarm_object );
            sslog_sbcr_unsubscribe(subscription);
            return;
        }
    }
}   
