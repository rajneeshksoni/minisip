

        #ifndef ADB_GETRESERVATIONSTATUSESRESPONSE_H
        #define ADB_GETRESERVATIONSTATUSESRESPONSE_H

       /**
        * adb_getReservationStatusesResponse.h
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/Java version: 1.4.1  Built on : Aug 13, 2008 (05:03:41 LKT)
        */

       /**
        *  adb_getReservationStatusesResponse class
        */
        typedef struct adb_getReservationStatusesResponse adb_getReservationStatusesResponse_t;

        
          #include "adb_ReservationStatus.h"
          

        #include <stdio.h>
        #include <axiom.h>
        #include <axis2_util.h>
        #include <axiom_soap.h>
        #include <axis2_client.h>

        #ifdef __cplusplus
        extern "C"
        {
        #endif

        #define ADB_DEFAULT_DIGIT_LIMIT 64
        #define ADB_DEFAULT_NAMESPACE_PREFIX_LIMIT 64
        

        /******************************* Create and Free functions *********************************/

        /**
         * Constructor for creating adb_getReservationStatusesResponse_t
         * @param env pointer to environment struct
         * @return newly created adb_getReservationStatusesResponse_t object
         */
        adb_getReservationStatusesResponse_t* AXIS2_CALL
        adb_getReservationStatusesResponse_create(
            const axutil_env_t *env );

        /**
         * Free adb_getReservationStatusesResponse_t object
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object to free
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t AXIS2_CALL
        adb_getReservationStatusesResponse_free (
            adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
            const axutil_env_t *env);



        /********************************** Getters and Setters **************************************/
        /******** Deprecated for array types, Use 'Getters and Setters for Arrays' instead ***********/
        

        /**
         * Getter for reservationStatuses. Deprecated for array types, Use adb_getReservationStatusesResponse_get_reservationStatuses_at instead
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @return Array of adb_ReservationStatus_t*s.
         */
        axutil_array_list_t* AXIS2_CALL
        adb_getReservationStatusesResponse_get_reservationStatuses(
            adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
            const axutil_env_t *env);

        /**
         * Setter for reservationStatuses.Deprecated for array types, Use adb_getReservationStatusesResponse_set_reservationStatuses_at
         * or adb_getReservationStatusesResponse_add_reservationStatuses instead.
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @param arg_reservationStatuses Array of adb_ReservationStatus_t*s.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t AXIS2_CALL
        adb_getReservationStatusesResponse_set_reservationStatuses(
            adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
            const axutil_env_t *env,
            axutil_array_list_t*  arg_reservationStatuses);

        /**
         * Resetter for reservationStatuses
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t AXIS2_CALL
        adb_getReservationStatusesResponse_reset_reservationStatuses(
            adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
            const axutil_env_t *env);

        
        /****************************** Getters and Setters For Arrays **********************************/
        /************ Array Specific Operations: get_at, set_at, add, remove_at, sizeof *****************/

        /**
         * E.g. use of get_at, set_at, add and sizeof
         *
         * for(i = 0; i < adb_element_sizeof_property(adb_object, env); i ++ )
         * {
         *     // Getting ith value to property_object variable
         *     property_object = adb_element_get_property_at(adb_object, env, i);
         *
         *     // Setting ith value from property_object variable
         *     adb_element_set_property_at(adb_object, env, i, property_object);
         *
         *     // Appending the value to the end of the array from property_object variable
         *     adb_element_add_property(adb_object, env, property_object);
         *
         *     // Removing the ith value from an array
         *     adb_element_remove_property_at(adb_object, env, i);
         *     
         * }
         *
         */

        
        
        /**
         * Get the ith element of reservationStatuses.
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @param i index of the item to return
         * @return ith adb_ReservationStatus_t* of the array
         */
        adb_ReservationStatus_t* AXIS2_CALL
        adb_getReservationStatusesResponse_get_reservationStatuses_at(
                adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
                const axutil_env_t *env, int i);

        /**
         * Set the ith element of reservationStatuses. (If the ith already exist, it will be replaced)
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @param i index of the item to return
         * @param arg_reservationStatuses element to set adb_ReservationStatus_t* to the array
         * @return ith adb_ReservationStatus_t* of the array
         */
        axis2_status_t AXIS2_CALL
        adb_getReservationStatusesResponse_set_reservationStatuses_at(
                adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
                const axutil_env_t *env, int i,
                adb_ReservationStatus_t* arg_reservationStatuses);


        /**
         * Add to reservationStatuses.
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @param arg_reservationStatuses element to add adb_ReservationStatus_t* to the array
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t AXIS2_CALL
        adb_getReservationStatusesResponse_add_reservationStatuses(
                adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
                const axutil_env_t *env,
                adb_ReservationStatus_t* arg_reservationStatuses);

        /**
         * Get the size of the reservationStatuses array.
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct.
         * @return the size of the reservationStatuses array.
         */
        int AXIS2_CALL
        adb_getReservationStatusesResponse_sizeof_reservationStatuses(
                    adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
                    const axutil_env_t *env);

        /**
         * Remove the ith element of reservationStatuses.
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @param i index of the item to remove
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t AXIS2_CALL
        adb_getReservationStatusesResponse_remove_reservationStatuses_at(
                adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
                const axutil_env_t *env, int i);

        


        /******************************* Checking and Setting NIL values *********************************/
        /* Use 'Checking and Setting NIL values for Arrays' to check and set nil for individual elements */

        /**
         * NOTE: set_nil is only available for nillable properties
         */

        

        /**
         * Check whether reservationStatuses is nill
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @return AXIS2_TRUE if the element is nil or AXIS2_FALSE otherwise
         */
        axis2_bool_t AXIS2_CALL
        adb_getReservationStatusesResponse_is_reservationStatuses_nil(
                adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
                const axutil_env_t *env);


        
        /*************************** Checking and Setting 'NIL' values in Arrays *****************************/

        /**
         * NOTE: You may set this to remove specific elements in the array
         *       But you can not remove elements, if the specific property is declared to be non-nillable or sizeof(array) < minOccurs
         */
        
        /**
         * Check whether reservationStatuses is nill at i
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct.
         * @param i index of the item to return.
         * @return AXIS2_TRUE if the element is nil or AXIS2_FALSE otherwise
         */
        axis2_bool_t AXIS2_CALL
        adb_getReservationStatusesResponse_is_reservationStatuses_nil_at(
                adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
                const axutil_env_t *env, int i);
 
       
        /**
         * Set reservationStatuses to nill at i
         * @param  _getReservationStatusesResponse _ adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct.
         * @param i index of the item to set.
         * @return AXIS2_SUCCESS on success, or AXIS2_FAILURE otherwise.
         */
        axis2_status_t AXIS2_CALL
        adb_getReservationStatusesResponse_set_reservationStatuses_nil_at(
                adb_getReservationStatusesResponse_t* _getReservationStatusesResponse, 
                const axutil_env_t *env, int i);

        

        /**************************** Serialize and Deserialize functions ***************************/
        /*********** These functions are for use only inside the generated code *********************/

        
        /**
         * Deserialize an XML to adb objects
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @param dp_parent double pointer to the parent node to deserialize
         * @param dp_is_early_node_valid double pointer to a flag (is_early_node_valid?)
         * @param dont_care_minoccurs Dont set errors on validating minoccurs, 
         *              (Parent will order this in a case of choice)
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t AXIS2_CALL
        adb_getReservationStatusesResponse_deserialize(
            adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
            const axutil_env_t *env,
            axiom_node_t** dp_parent,
            axis2_bool_t *dp_is_early_node_valid,
            axis2_bool_t dont_care_minoccurs);
                            
            

       /**
         * Declare namespace in the most parent node 
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @param parent_element parent element
         * @param namespaces hash of namespace uri to prefix
         * @param next_ns_index pointer to an int which contain the next namespace index
         */
       void AXIS2_CALL
       adb_getReservationStatusesResponse_declare_parent_namespaces(
                    adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
                    const axutil_env_t *env, axiom_element_t *parent_element,
                    axutil_hash_t *namespaces, int *next_ns_index);

        

        /**
         * Serialize to an XML from the adb objects
         * @param  _getReservationStatusesResponse adb_getReservationStatusesResponse_t object
         * @param env pointer to environment struct
         * @param getReservationStatusesResponse_om_node node to serialize from
         * @param getReservationStatusesResponse_om_element parent element to serialize from
         * @param tag_closed whether the parent tag is closed or not
         * @param namespaces hash of namespace uri to prefix
         * @param next_ns_index an int which contain the next namespace index
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axiom_node_t* AXIS2_CALL
        adb_getReservationStatusesResponse_serialize(
            adb_getReservationStatusesResponse_t* _getReservationStatusesResponse,
            const axutil_env_t *env,
            axiom_node_t* getReservationStatusesResponse_om_node, axiom_element_t *getReservationStatusesResponse_om_element, int tag_closed, axutil_hash_t *namespaces, int *next_ns_index);

        /**
         * Check whether the adb_getReservationStatusesResponse is a particle class (E.g. group, inner sequence)
         * @return whether this is a particle class.
         */
        axis2_bool_t AXIS2_CALL
        adb_getReservationStatusesResponse_is_particle();


     #ifdef __cplusplus
     }
     #endif

     #endif /* ADB_GETRESERVATIONSTATUSESRESPONSE_H */
    

