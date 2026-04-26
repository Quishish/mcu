    #include "hardware/adc.h"
    #include "pico/stdlib.h"
    #include "hardware/gpio.h"
    #include "stdint.h"
    #include "adc-task.h"


    const uint adc_pin = 26;
    const uint adc_channel = 0;
    const uint temp_channel = 4;

    adc_task_state_t adc_state = ADC_TASK_STATE_IDLE;
    uint64_t time_mark = 0;
    const uint64_t ADC_TASK_MEAS_PERIOD_US = 100000;


    //float voltage;

    adc_task_init() {
        adc_gpio_init(adc_pin);
        adc_init();
        adc_set_temp_sensor_enabled(true);
    }

    float adc_task_voltage() {
        adc_select_input(adc_channel);
        uint16_t voltage_counts = adc_read();
        float voltage = 3.3 * voltage_counts / 4096;
        return voltage;
    }

    float adc_task_temp() {
        adc_select_input(temp_channel);
        uint16_t temp_counts = adc_read();
        float temp_V = 3.3 * temp_counts / 4096;
        float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;
        return temp_C;
    }

    void adc_task_set_state(adc_task_state_t state) {
        adc_state = state;
        if (state == ADC_TASK_STATE_RUN) {
            time_mark = time_us_64();
        }
    }

    void adc_task_handle() {
        if (adc_state != ADC_TASK_STATE_RUN) {
            return;
        }

        uint64_t now_us = time_us_64();
        if (now_us - time_mark < ADC_TASK_MEAS_PERIOD_US) {
            return;
        }

        time_mark = now_us; 

        uint64_t uptime_ms = now_us / 1000;

        float voltage_V = adc_task_voltage();
        float temp_C = adc_task_temp();

        printf("%f %f\n", voltage_V, temp_C);
    }
