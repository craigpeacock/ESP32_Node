/*
 * MQTT Framework
 * Copyright (C) 2021 Craig Peacock
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "mqtt.h"

static const char *TAG = "MQTT";

#define CONFIG_BROKER_URL "mqtts://xxxxxxats.iot.us-east-2.amazonaws.com"
#define CONFIG_THINGS_NAME "ESP32Thing"
#define CONFIG_BROKER_USERNAME "mqttuser"
#define CONFIG_BROKER_PASSWORD "secret"

// Following certificates are embedded at build time. See main/CMakeLists.txt line:
// set(COMPONENT_EMBED_TXTFILES "AmazonRootCA1.pem" "certificate.pem.crt" "private.pem.key")

// Root Certificate Authority (CA):
// Copy certificate to main/AmazonRootCA1.pem
extern const uint8_t aws_root_ca_pem_start[]   asm("_binary_AmazonRootCA1_pem_start");
extern const uint8_t aws_root_ca_pem_end[]   asm("_binary_AmazonRootCA1_pem_end");

// Device Certificate for SSL Mutual Authentication:
// Copy certificate to main/certificate.pem.crt
extern const uint8_t certificate_pem_crt_start[] asm("_binary_certificate_pem_crt_start");
extern const uint8_t certificate_pem_crt_end[] asm("_binary_certificate_pem_crt_end");

// RSA Private Key:
// Copy key to main/private.pem.key
extern const uint8_t private_pem_key_start[] asm("_binary_private_pem_key_start");
extern const uint8_t private_pem_key_end[] asm("_binary_private_pem_key_end");

unsigned int pub_cnt;

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
	esp_mqtt_event_handle_t event = event_data;

	switch ((esp_mqtt_event_id_t)event_id) {

		case MQTT_EVENT_CONNECTED:
			ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
			break;

		case MQTT_EVENT_DISCONNECTED:
			ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
			break;

		case MQTT_EVENT_SUBSCRIBED:
			ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
			break;

		case MQTT_EVENT_UNSUBSCRIBED:
			ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
			break;

		case MQTT_EVENT_PUBLISHED:
			ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
			pub_cnt--;
			break;

		case MQTT_EVENT_DATA:
			ESP_LOGI(TAG, "MQTT_EVENT_DATA");
			printf("Topic = %.*s, %.*s\r\n", event->topic_len, event->topic, event->data_len, event->data);
			break;

		case MQTT_EVENT_BEFORE_CONNECT:
			ESP_LOGI(TAG, "MQTT_EVENT_BEFORE_CONNECT");
			break;

		case MQTT_EVENT_ERROR:
			ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
			break;

		default:
			ESP_LOGI(TAG, "Unknown event id: %d", event->event_id);
			break;

	}
}

esp_mqtt_client_handle_t mqtt_init(void)
{
	int ret;
	esp_mqtt_client_config_t mqtt_cfg = {
	        .uri = CONFIG_BROKER_URL,
			.port = 8883,
			.transport = MQTT_TRANSPORT_OVER_SSL,
			//.username = CONFIG_BROKER_USERNAME,
			//.password = CONFIG_BROKER_PASSWORD,
			.keepalive = 600,
			.client_id = CONFIG_THINGS_NAME,
			.cert_pem = (const char *)aws_root_ca_pem_start,
			.cert_len = (const char *)aws_root_ca_pem_end - (const char *)aws_root_ca_pem_start,
			.client_cert_pem = (const char *)certificate_pem_crt_start,
			.client_cert_len = (const char *)certificate_pem_crt_end - (const char *)certificate_pem_crt_start,
			.client_key_pem = (const char *)private_pem_key_start,
			.client_key_len = (const char *)private_pem_key_end - (const char *)private_pem_key_start,
	};

	printf("cert_len = %d\r\n", mqtt_cfg.cert_len);

	esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
	esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
	ret = esp_mqtt_client_start(client);
	switch (ret) {
		case ESP_OK:
			printf("MQTT Initialised Successfully\r\n");
			break;
		case ESP_ERR_INVALID_ARG:
			printf("Invalid argument in mqtt_init()\r\n");
			break;
		case ESP_FAIL:
		default:
			printf("Unknown Error in mqtt_init()\r\n");
			break;
	}
	return(client);
}

int mqtt_stop(esp_mqtt_client_handle_t client)
{
	int ret;
	ret = esp_mqtt_client_stop(client);
	if (ret != ESP_OK) {
		char buffer[80];
		printf("%s", esp_err_to_name_r(ret, buffer, sizeof(buffer)));
	}
	ret = esp_mqtt_client_destroy(client);
	if (ret != ESP_OK) {
		char buffer[80];
		printf("%s", esp_err_to_name_r(ret, buffer, sizeof(buffer)));
	}
	return(1);
}

int mqtt_pub(esp_mqtt_client_handle_t client, char * topic, char * msg)
{
	int msg_id = esp_mqtt_client_publish(client, topic, msg, strlen(msg), 1, 0);
	ESP_LOGI(TAG, "Publish successful, msg_id=%d", msg_id);
	pub_cnt++;
	return(msg_id);
}

int mqtt_sub(esp_mqtt_client_handle_t client, char * topic)
{
	int msg_id = esp_mqtt_client_subscribe(client, topic, 0);
	ESP_LOGI(TAG, "Subscribe successful, msg_id=%d", msg_id);
	return(msg_id);
}

int mqtt_unsub(esp_mqtt_client_handle_t client, char * topic)
{
	int msg_id = esp_mqtt_client_unsubscribe(client, topic);
	ESP_LOGI(TAG, "Unsubscribe successful, msg_id=%d", msg_id);
	return(msg_id);
}
