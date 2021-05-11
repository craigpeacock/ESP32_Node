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

#ifndef MAIN_MQTT_H_
#define MAIN_MQTT_H_

esp_mqtt_client_handle_t mqtt_init(void);
int mqtt_pub(esp_mqtt_client_handle_t client, char * topic, char * msg);
int mqtt_sub(esp_mqtt_client_handle_t client, char * topic);
int mqtt_unsub(esp_mqtt_client_handle_t client, char * topic);
int mqtt_stop(esp_mqtt_client_handle_t client);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

extern unsigned int pub_cnt;

#endif
