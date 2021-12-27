#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_core_dump.h"
#include "driver/uart.h"
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

void crash_me_hard();

void app_main()
{
	printf("Hello, world!\n");

	esp_core_dump_init();
	esp_core_dump_summary_t *summary = malloc(sizeof(esp_core_dump_summary_t));
	if (summary) {
		esp_log_level_set("esp_core_dump_elf", ESP_LOG_VERBOSE); // so that func below prints stuff.. but doesn't actually work, have to set logging level globally through menuconfig
		printf("Retrieving core dump summary..\n");
		esp_err_t err = esp_core_dump_get_summary(summary);	
		if (err == ESP_OK) {
			//get summary function already pints stuff
			printf("Getting core dump summary ok.\n");
			//todo: do something with dump summary
		} else {
			printf("Getting core dump summary not ok. Error: %d\n", (int) err);
			printf("Probably no coredump present yet.\n");
			printf("esp_core_dump_image_check() = %d\n", esp_core_dump_image_check());
		}		
		free(summary);
	}
	fflush(stdout);
	
	for(int i=0; i <= 3; i++) {
		printf("Crashing in %d seconds..\n", 3 - i);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	printf("Crashing..\n");
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	crash_me_hard();
}

void crash_me_hard() {
	//provoke crash through writing to a nullpointer
	volatile uint32_t* aPtr = (uint32_t*) 0x00000000;
	*aPtr = 0x1234567; //goodnight
}
