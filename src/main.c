#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_core_dump.h"

void app_main()
{
	printf("Hello, world!\n");

	esp_core_dump_init();
	esp_core_dump_summary_t *summary = malloc(sizeof(esp_core_dump_summary_t));
	if (summary) {
		esp_err_t err = esp_core_dump_get_summary(summary);	
		if (err == ESP_OK) {
			printf("Getting core dump summary ok.");
		} else {
			printf("Getting core dump summary not ok. Error: %d\n", (int) err);
			printf("Probably no coredump present yet.\n");
			printf("esp_core_dump_image_check() = %d\n", esp_core_dump_image_check());
		}		
		free(summary);
	}
	
    while(1) {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		printf("Doing nothing...\n");
    }
}
