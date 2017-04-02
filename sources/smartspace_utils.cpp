#include "smartspace_utils.hpp"

int connect_to_smartspace(){
    sslog_init();
    register_ontology();
    global_node = sslog_new_node("VolunteerDispatcherKP", KP_SS_NAME, KP_SS_ADDRESS, KP_SS_PORT);
    if (sslog_node_join(global_node) != SSLOG_ERROR_NO) {

        std::cout << "Can't join to SS: %s\n";
        std::cout << sslog_error_get_last_text();

        return -1;
    }
    return 0;
}

/**
 * Calculate number of digits in the number.
 */
unsigned int digit_count(int inputNumber) {
    int count = 0;
    int number = inputNumber;
    while(number != 0) {
        number /= 10;
        ++count;
    }
}

/**
 * Generate random uri based on class.
 * This function use sslog_generate_uri() function to retrieve base uri
 * and append another random number.
 * We use this function because on several machines sslog_generate_uri()
 * function always return same number.
 */

char* generate_uri(sslog_class_t* _class){
    char* uri = sslog_generate_uri(_class);
    static int i = 0;
    i++;
    char *hash = (char *) malloc(sizeof(char) * (digit_count(i) + 1));
    sprintf(hash, "%d", i);
    strncat(uri, hash, strlen(hash));
    return uri;
}

/**
 * Retrieve triples with specified subject, predicate and object and return
 * first of this triples. You can use SS_RDF_SIB_ANY constant instead of subject or object.
 * Use subject and object with URI type.
 * Return NULL if no triples will be found.
 */
sslog_triple_t* retrieve_first_triple(const char* subject, const char* predicate, const char* object) {
    sslog_triple_t *req_triple = sslog_new_triple_detached(subject, predicate, object, SSLOG_RDF_TYPE_URI, SSLOG_RDF_TYPE_URI);

    if (req_triple == NULL) {
        std::cout << "Error: " << sslog_error_get_last_text() << "\n";
        return NULL;
    }

    list_t *query_result_triples = sslog_node_query_triple(global_node, req_triple);
    sslog_triple_t *result_triple = NULL;
    if (query_result_triples != NULL){
        list_head_t *list_walker = NULL;
        list_for_each(list_walker, &query_result_triples->links) {
            list_t *node = list_entry(list_walker, list_t, links);
            result_triple = (sslog_triple_t *) node->data;
            break;
        }
        list_free_with_nodes(query_result_triples, NULL);
        return result_triple;

    }
    sslog_free_triple(req_triple);
    return result_triple;
}

/**
 * Retrieve first object by property.
 * Subject and object should have URI type.
 * Return null if triple is not found.
 */
char* retrieve_object_by_property(sslog_property_t* property, const char* subject) {
    sslog_triple_t* triple = retrieve_first_triple(subject, sslog_entity_get_uri(property), SS_RDF_SIB_ANY);
    if (triple == NULL) return NULL;
    return triple->object;
}

/**
 * Retrieve first subject by property.
 * Subject and object should have URI type.
 * Return null if triple is not found.
 */
char* retrieve_subject_by_property(sslog_property_t* property, const char* object) {
    sslog_triple_t* triple = retrieve_first_triple(SS_RDF_SIB_ANY, sslog_entity_get_uri(property), object);
    if (triple == NULL) return NULL;
    return triple->subject;
}

/**
 * Retrieve range individual (object property value).
 */
sslog_individual_t* retrieve_range_individual(sslog_property_t* property, sslog_individual_t* domain) {
    const char* domain_uri = sslog_entity_get_uri(domain);
    return sslog_node_get_individual_by_uri(global_node, retrieve_object_by_property(property, domain_uri));
}

/**
 * Retrieve domain individual (object property owner).
 */
sslog_individual_t* retrieve_domain_individual(sslog_property_t* property, sslog_individual_t* range) {

    const char* range_uri = sslog_entity_get_uri(range);
    char *domain_uri = retrieve_subject_by_property(property, range_uri);
    //std::cout << domain_uri << std::endl;
    return sslog_node_get_individual_by_uri(global_node, domain_uri);
}
