    #include "protocol-task.h"
    #include "stdio.h"
    #include "pico/stdlib.h"
    #include "string.h"
    #include <stdint.h>


    static api_t* api = NULL;
    static int commands_count = 0;

    void protocol_task_init(api_t* device_api) {
        api = device_api;
        commands_count = 0;

        if (device_api != NULL) {
            while (api[commands_count].command_name != NULL) {
                commands_count++;
            }
        }
    }

    void protocol_task_handle(char* command_string)

    {

        if (!command_string)
        {
            printf("command is not recieved");
            return;
        }


        const char* command_name = command_string;
        const char* command_args = NULL;

        char* space_symbol = strchr(command_string, ' ');

        if (space_symbol)
        {
            *space_symbol = '\0';
            command_args = space_symbol + 1;
        }
        else
        {
            command_args = "";
        }

        printf("recieved command:\n", command_name);
        printf("command args:\n", command_args);

        for (int i = 0; i < commands_count; i++)
        {

            if (strcmp(command_name, api[i].command_name) == 0) {
                api[i].command_callback(command_args);
                return;
            }

        }

        printf("command is not in command list\n");
        return;
    }

    void mem_prot(uint32_t addr){
        uint32_t value = *(uint32_t*)addr;
        printf("Value at 0x%08X: 0x%08X\n", addr, value);
    }

    void wmem_prot(uint32_t addr, uint32_t value) {
        *(uint32_t*)addr = value;
        printf("Written 0x%08X to address 0x%08X\n", value, addr);
    }