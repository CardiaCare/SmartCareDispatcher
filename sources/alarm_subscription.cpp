#include "alarm_subscription.hpp"

void sbcr_thread()
{
    std::cout << "Trying to subscribe to panic signals.\n";

    /*
    * Subscribe to alarm
    */
    if( 0 != subscribe_to_alarm() ){
        std::cout << "Error in alarm subscription.\n";

        sslog_shutdown();
        exit(0);
    }
    std::cout << "Subscription is successful\n";
}
int subscribe_to_alarm(){
   fetch_alarms();


   sslog_subscription_t *subscription = sslog_new_subscription(global_node, true);
   sslog_sbcr_add_class(subscription, CLASS_ALARM);


   extern void new_alarm_handler(sslog_subscription_t *);
   void (*new_alarm_handler_pointer)(sslog_subscription_t *) = &new_alarm_handler;

   sslog_sbcr_set_changed_handler(subscription, new_alarm_handler_pointer);

   if( sslog_sbcr_subscribe(subscription) != SSLOG_ERROR_NO){
       return -1;
   }

   return 0;
}

/**
 * @brief fetch_alarms
 * @todo REFACTOR
 */
void fetch_alarms() {
    list_t* alarms = sslog_node_get_individuals_by_class(global_node, CLASS_ALARM);

    if (list_is_null_or_empty(alarms) == true) {
        std::cout << "There are no such alarms on init." << std::endl;
        return;
    }

    list_head_t *pos = NULL;

    list_for_each(pos, &alarms->links)
    {
        list_t *node = list_entry(pos, list_t, links);
        sslog_individual_t* _alarm = (sslog_individual_t *) node->data;
        sslog_triple_t *alarm_uri_from_triple = sslog_individual_to_triple (_alarm);
        char *_alarm_uri  = alarm_uri_from_triple->subject;
        Alarm *alarm = new Alarm(_alarm_uri);
        alarm->setTime(std::chrono::steady_clock::now());
        std::cout << "Alarm with URI: ";
        std::cout << _alarm_uri;
        std::cout << " was retrieved" << std::endl;

        alarm_queue.push(alarm);
    }
    return;
}

void new_alarm_handler(sslog_subscription_t *subscription){

    sslog_sbcr_changes_t *changes = sslog_sbcr_get_changes_last(subscription);

    const list_t *alarms =
            sslog_sbcr_ch_get_individual_by_action(changes, SSLOG_ACTION_REMOVE);
    if (alarms == NULL) {
        return;
    }
    list_head_t *iterator = NULL;
    list_for_each(iterator, &alarms->links) {
        list_t *list_node = list_entry(iterator, list_t, links);
        char *_alarm_uri = (char *) list_node->data;
        std::cout << "Removed URI: " << _alarm_uri <<std::endl;
        break;
        Alarm *alarm = new Alarm(_alarm_uri);
        alarm->setTime(std::chrono::steady_clock::now());
//        URI *_uri = new URI();
//        URI::size_type size = strlen((char*)_alarm_uri) ;
//        _uri->assign(_alarm_uri, _alarm_uri + size + 1);

//        std::cout << "Alarm with URI: ";
//        std::cout << _alarm_uri;
//        std::cout << " was retrieved" << std::endl;

        alarm_queue.push(alarm);
    }
    return;
}
