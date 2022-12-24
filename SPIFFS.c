// SPIFFS filesystem usage example

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

static const char *TAG = "SPIFFS";

void app_main(void)
{
    // SPIFFS system initiation
    printf("\nSPIFFS system initiation:\n");
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};
    esp_vfs_spiffs_register(&conf);
    size_t total = 0, used = 0;
    esp_spiffs_info(conf.partition_label, &total, &used);
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);

    // Write and read file
    printf("\nWrite and read file:\n");
    // Create a file.
    ESP_LOGI(TAG, "Opening file");
    FILE *f = fopen("/spiffs/hello.txt", "w");
    fprintf(f, "Hello ESP World!\n");
    fclose(f);
    ESP_LOGI(TAG, "File written");
    // Open file for reading
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/spiffs/hello.txt", "r");
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    char *pos = strchr(line, '\n');
    if (pos)
    {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);

    // Read files in the directory
    DIR *d;
    struct dirent *dir;
    d = opendir("/spiffs");
    printf("\nList of files in the directory:\n");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            ESP_LOGI(TAG, "%s", dir->d_name);
        }
        closedir(d);
    }

    // Unmount partition and disable SPIFFS
    printf("\nSPIFFS system unmount:\n");
    esp_vfs_spiffs_unregister(conf.partition_label);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}
