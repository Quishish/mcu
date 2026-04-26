#include "stdio-task/stdio-task.h"
#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "stdint.h" 

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

char* comm;

void led_on_callback(const char* args) {
    printf("light turned on\n");
    led_task_state_set(LED_STATE_ON);
}

void led_off_callback(const char* args) {
    printf("light turned off\n");
    led_task_state_set(LED_STATE_OFF);
}

void led_blink_callback(const char* args) {
    printf("blink -.<\n");
    led_task_state_set(LED_STATE_BLINK);
}

void led_blink_set_period_ms_callback(const char* args) {
    uint period_ms = 0;
    sscanf(args, "%u", &period_ms);
    if (period_ms == 0) {
        printf("error, period can't be zero(\n");
        return;
    } else {
        led_task_set_blink_period_ms(period_ms);
    }
}

void version_callback(const char* args)
{
	printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void help_callback(const char* args);

void mem_callback(const char* args){
    uint32_t addr;
    if (sscanf(args, "%x", &addr) != 1) {
        printf("Error: invalid address format.\n");
        return;
    }
    mem_prot(addr);
}

void wmem_callback(const char* args) {
    uint32_t addr;
    uint32_t value;
    if (sscanf(args, "%x", &addr, &value) != 1) {
        printf("Error: invalid address format.\n");
        return;
    }
    wmem_prot(addr, value);
}

api_t device_api[] =
{
	{"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "led on"},
    {"off", led_off_callback, "led off"},
    {"blink", led_blink_callback, "led blink"},
    {"set_period", led_blink_set_period_ms_callback, "period"},
    {"help", help_callback, "shows command list"},
    {"mem", mem_callback, "read from adress"},
    {"wmem", wmem_callback, "write to adress"},
	{NULL, NULL, NULL}, 
};

void help_callback(const char* args) {
    printf("Available commands:\n");
    for (int i = 0; device_api[i].command_name != NULL; i++) {
        printf("  %s: %s\n", device_api[i].command_name, device_api[i].command_help);
    }
}

int main(){

    stdio_task_init();
    protocol_task_init(device_api);
    led_task_init();

    while(1) {
        led_task_handle();
        comm = stdio_task_handle();
        if (comm != NULL) {
            protocol_task_handle(comm);
        }
    }
}

