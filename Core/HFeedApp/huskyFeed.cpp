#include "huskyFeed.h"

/*These two function are a programmer's shame, but I'm too lazy*/

const char* state_to_cstr(const HuskyFeed_State& state) {
    switch(state){
        case WAIT_FOR_MODE:
            return ("WAIT_FOR_MODE");
        case WAIT_FOR_LOPPIDEH:
            return "WAITING_FOR_LOPPIDEH";
        case WAIT_FOR_DEADLINE:
            return "WAIT_FOR_DEADLINE";
        case SERVING:
            return "SERVING";   
        default:
            return "Invalid State";        
    }
}
const char* mode_to_cstr(const uint8_t& mode){
    switch(mode){
        case HUSKY_FEED_MODE_MANUAL:
            return "MANUAL";
        case HUSKY_FEED_MODE_AUTOMATIC:
            return "AUTOMATIC";
        case HUSKY_FEED_MODE_TIME:
            return "TIME";
        case HUSKY_FEED_MODE_BLANK:
            return "NO MODE SET";   
        default:
            return "Invalid mode";        
    }
}

struct HuskyFeed_CFG&  HuskyFeed_CFG ::operator=(const struct HuskyFeed_CFG& a)
{
    if(IS_VALID_MODE(a.mode)){
        if(a.mode==HUSKY_FEED_MODE_TIME )
        {
            if (IS_DEADLINES_NUM_VALID(a.deadlines_num))
                memcpy(this,&a,sizeof(HuskyFeed_CFG));
            else
                memcpy(this,&default_cfg,sizeof(HuskyFeed_CFG));
        }
        else
                memcpy(this,&a,sizeof(HuskyFeed_CFG));
    }
    else
        memcpy(this,&default_cfg,sizeof(HuskyFeed_CFG));
    return *this;
}
void HuskyFeed_CFG::to_cstring(char* const to_ret){
    char temp[32];
    uint16_t idx;
    sprintf(to_ret," Mode: %s \n Quantity %lu \n NumDeadlines %lu :\n Deadlines: \n ",
        mode_to_cstr(this->mode),this->food_quantity,this->deadlines_num);
    for(idx=0;idx<this->deadlines_num;idx++){
        sprintf(temp,"idx: %u h: %u m: %u s: %u \n ",idx,
            this->deadlines_hours[idx],this->deadlines_minutes[idx],this->deadlines_seconds[idx]);
        strcat(to_ret,temp);
    }
    if(this->periodic==HUSKY_FEED_NOTPERIODIC_TIME)
        strcat(to_ret,"Not Periodic \n");
    else
        strcat(to_ret,"Periodic \n");

}

void HuskyFeeder::exec_serving(){

}
void HuskyFeeder::exec_wait_for_deadline(){

}
void HuskyFeeder::exec_wait_for_BAUBAU(){

}

HuskyFeeder::HuskyFeeder(){
    hf_reset();
    /** Set the state execution table**/
    //exec_table=&HuskyFeeder::exec_wait_for_mode;
    exec_table[STATE_TO_NUM(WAIT_FOR_MODE)]        =&HuskyFeeder::exec_wait_for_mode;
    exec_table[STATE_TO_NUM(SERVING)]              =&HuskyFeeder::exec_serving;
    exec_table[STATE_TO_NUM(WAIT_FOR_DEADLINE)]    =&HuskyFeeder::exec_wait_for_deadline;
    exec_table[STATE_TO_NUM(WAIT_FOR_LOPPIDEH)]    =&HuskyFeeder::exec_wait_for_BAUBAU;
}

