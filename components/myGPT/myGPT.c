#include <stdio.h>
#include "myGPT.h"
#include "driver/gptimer.h"
#include "esp_rom_sys.h"

gptimer_handle_t gptimer = NULL;



void myGPT_setup(void)
{
    gptimer_config_t timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT, 
    .direction = GPTIMER_COUNT_UP,      
    .resolution_hz = 1 * 1000 * 1000,   
    };

        
    gptimer_new_timer(&timer_config, &gptimer);                 
    myTimer_setup_alarm();
    gptimer_enable(gptimer);            
    gptimer_start(gptimer);            

}


static bool alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{


    esp_rom_delay_us(10);

    return false;

}


void myGPT_setup_alarm(void)
{
    gptimer_alarm_config_t alarm_config = {
    .reload_count = 0,      
    .alarm_count = 100000, 
    .flags.auto_reload_on_alarm = true,   
    };

    gptimer_set_alarm_action(gptimer, &alarm_config);           
 
    gptimer_event_callbacks_t cbs = {
        .on_alarm = alarm_cb,                                   
    };   
    gptimer_register_event_callbacks(gptimer, &cbs, NULL);


}