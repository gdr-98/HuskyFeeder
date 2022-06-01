#ifndef HUSKY_FEED
#define HUSKY_FEED

#include "includes.h"
/****************
 *  FILE DESCR:
 *      This file contains definitions, constant, function prototypes for Husky Feeder App.
 ****************/

/****************
 *  WORKING MODES:
 * 
 *      1- Manual:          In the manual mode the food is instantly served.
 *                          After serving food in working mode the app automatically returns
 *                          in Waiting for mode state.
 * 
 *      2-  Time:           In the time mode the application waits the next deadline.
 *                          After the last deadline is served, if periodic flag is enabled then the
 *                          state persists in wait for deadline( waiting for the first deadline of the list)
 *                          else the new state is wait for mode.
 * 
 *      3-  Automatic:      When the dog comes near to the structure then the food is served.
 *                          Like manual mode, it returns to the Wait For Mode state
 * STATE DESCR:
 * 
 *  1-  WAITING FOR MODE    =   The application is currently waiting for the user input, this state changes
 *                              when a new configuration is received.
 *                              It must be said that if a new cfg is received in every state then the new cfg, with the new state,
 *                              is immediately changed.
 *                               
 *  2-   SERVING             =  The food is being served.
 *                              The state changes when the food quantity is reached or a timer expires, indicating 
 *                              food quantity to be charged or a problem with the motor.
 * 
 *  3-  WAIT_FOR_DEADLINE   =   We're waiting for the next deadline to be reached.
 *                              This state is entered when the mode is time.
 * 
 *  4-  WAIT_FOR_DOG        =   In this state we're waiting the dog to come near the structure.  
 * *****************/
#define HUSKY_FEED_MODE_MANUAL      0
#define HUSKY_FEED_MODE_TIME        1
#define HUSKY_FEED_MODE_AUTOMATIC   2
#define HUSKY_FEED_MODE_BLANK       3   //  This mode is used only internally to express a mode in which the application does nothing

#define HUSKY_FEED_NUM_STATES 4
enum HuskyFeed_State{
    WAIT_FOR_MODE=0,
    SERVING=1,
    WAIT_FOR_DEADLINE=2,
    WAIT_FOR_LOPPIDEH=3
};

// Macro that checks if the mode is valid
#define IS_VALID_MODE(mode)   (mode == HUSKY_FEED_MODE_MANUAL) || (mode == HUSKY_FEED_MODE_TIME) || (mode == HUSKY_FEED_MODE_AUTOMATIC)||(mode == HUSKY_FEED_MODE_BLANK)         

// Convert a num to a state
#define NUM_TO_STATE(num)    static_cast<HUSKY_FEED_STATE>(num)
// Convert a num to a state
#define STATE_TO_NUM(state)    static_cast<uint16_t>(state)

//  Maximum number of deadlines in the deadline list.
#define DEADLINES_UB 16
//  Checks if the number of deadline is valid 
#define IS_DEADLINES_NUM_VALID(dealines_num) deadlines_num<=DEADLINES_UB

const char* state_to_cstr(const HuskyFeed_State&) ;
const char* mode_to_cstr(const uint8_t&);


//This is the configuration structure for the application
struct HuskyFeed_CFG{
    //  Working mode
    uint8_t     mode;
    //  Quantity of food to be served.
    uint8_t    food_quantity;

    /**** Time fields only ****/ 

    //  Number of deadlines in the list
    uint8_t    	deadlines_num;
    uint8_t     deadlines_hours[DEADLINES_UB];
    uint8_t     deadlines_minutes[DEADLINES_UB];
    uint8_t     deadlines_seconds[DEADLINES_UB];
    //Starting hour
    uint8_t     starting_hours[DEADLINES_UB];
    uint8_t     starting_minutes[DEADLINES_UB];
    uint8_t     starting_seconds[DEADLINES_UB];

    //  Tells if the configuration must persist after the last deadline, 
    //  it is periodic if this field is >0
    uint8_t    periodic;

    /**** Some operators and methods.. ****/
    struct HuskyFeed_CFG& operator=(const struct HuskyFeed_CFG& );
    void   to_cstring(char* const );
};
const struct HuskyFeed_CFG default_cfg={
    .mode=HUSKY_FEED_MODE_BLANK,
    .food_quantity=0,
    .deadlines_num=0,
    .deadlines_hours={0},
    .deadlines_minutes={0},
    .deadlines_seconds={0},
    .periodic=0
};

#define HUSKY_FEED_PERIODIC_TIME    1
#define HUSKY_FEED_NOTPERIODIC_TIME 0


/*************
 *  Macros and functions for state transactions and execution
 *******/

/**
 * @Descr:  Tells if we can swap state after receiving a new cfg
 * @In:     The actual state
 * @Ret:    True if we can swap
 * */
#define HUSKY_FEED_CAN_SWAP_STATE(actual_state) actual_state!=SERVING

/**
 * @Descr:  When a new cfg arrive, if it's valid we set the new config and 
 *          change the state.
 *          This function sets the new  state
 * @In:     The new mode
 * @Out:    The correspondent state
 * */
#define HUSKY_FEED_MOD_TO_STATE(in_mode,out_state){     \
    switch(in_mode){                        \
        case HUSKY_FEED_MODE_MANUAL:        \
            out_state=SERVING;              \
            break;                          \
        case HUSKY_FEED_MODE_TIME:          \
            out_state=WAIT_FOR_DEADLINE;    \
            break;                          \
        case HUSKY_FEED_MODE_AUTOMATIC:     \
            out_state=WAIT_FOR_LOPPIDEH;    \
            break;                          \
        default:                            \
            out_state=WAIT_FOR_MODE;        \
            in_mode=HUSKY_FEED_MODE_BLANK;  \
            break;                          \   
    }                                       \
}

/*********
 * User class
 ********/
class HuskyFeeder{
    private:
        //  Private because it's singleton
        HuskyFeeder();
        //  Table containing state function pointers
        void (HuskyFeeder::*exec_table[HUSKY_FEED_NUM_STATES])();
    public:
        /****   Members ****/
        struct HuskyFeed_CFG    current_configuration;
        HuskyFeed_State         current_state;
        uint16_t                deadline_ctr;

        static HuskyFeeder& getFeeder(){
            static HuskyFeeder globalFeeder;
            return globalFeeder;
        }
        inline void to_cstring(char * const k){
            //flushes the string
            memset(k,0,strlen(k));
            strcat(k,"CFG: \n");
            this->current_configuration.to_cstring(k);
            strcat(k,"STATE: \n");
            strcat(k,state_to_cstr(this->current_state));

        }
        inline void changeCFG(const struct HuskyFeed_CFG& to_set){ 
            current_configuration=to_set;
            HUSKY_FEED_MOD_TO_STATE(current_configuration.mode,current_state);
            deadline_ctr=0;
        }
        inline void hf_reset(){
            current_state=WAIT_FOR_MODE;
            current_configuration=default_cfg;
            deadline_ctr=0;
        }
        //  Methods not desired
        HuskyFeeder(HuskyFeeder const&)     = delete;
        void operator=(HuskyFeeder const&)  = delete;

        inline void exec_wait_for_mode(){};
        void exec_serving();
        void exec_wait_for_deadline();
        void exec_wait_for_BAUBAU();
        inline void exec_state(){
            (this->*exec_table[STATE_TO_NUM(current_state)])();
        }

};
#endif
