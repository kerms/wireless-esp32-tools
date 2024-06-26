#include "api_json_module.h"
#include "wifi_api.h"
#include "wifi_json_utils.h"

#include <stdio.h>
#include <esp_log.h>

#define TAG __FILENAME__

static int wifi_api_json_sta_get_ap_info(api_json_req_t *req);

static int wifi_api_json_get_scan(api_json_req_t *req);

static int wifi_api_json_connect(api_json_req_t *req);

static int wifi_api_json_disconnect(api_json_req_t *req);

static int wifi_api_json_ap_get_info(api_json_req_t *req);

/* the upper caller call cb() with void *, this let us use custom function arg */
static int async_helper_cb(void *arg)
{
	api_json_module_req_t *req = arg;
	return req->func(req->arg);
}

static inline int set_async(api_json_req_t *req, api_json_module_async_t *async, int (*func)(api_json_req_t *))
{
	async->module.func = func;
	async->module.arg = req;
	async->req_task.module.cb = async_helper_cb;
	async->req_task.module.arg = &async->module;
	return API_JSON_ASYNC;
}

static int on_json_req(uint16_t cmd, api_json_req_t *req, api_json_module_async_t *async)
{
	wifi_api_json_cmd_t wifi_cmd = cmd;
	switch (wifi_cmd) {
	case UNKNOWN:
	default:
		break;
	case WIFI_API_JSON_STA_GET_AP_INFO:
		return wifi_api_json_sta_get_ap_info(req);
	case WIFI_API_JSON_CONNECT:
		return set_async(req, async, wifi_api_json_connect);
	case WIFI_API_JSON_GET_SCAN:
		return set_async(req, async, wifi_api_json_get_scan);
	case WIFI_API_JSON_DISCONNECT:
		return wifi_api_json_disconnect(req);
	case WIFI_API_JSON_AP_GET_INFO:
		return wifi_api_json_ap_get_info(req);
	}

	ESP_LOGI(TAG, "cmd %d not executed\n", cmd);
	return API_JSON_UNSUPPORTED_CMD;
}

static int wifi_api_json_sta_get_ap_info(api_json_req_t *req)
{
	wifi_api_ap_info_t ap_info;
	wifi_api_sta_get_ap_info(&ap_info);
	req->out = wifi_api_json_serialize_ap_info(&ap_info, WIFI_API_JSON_STA_GET_AP_INFO);
	return 0;
}

static int wifi_api_json_ap_get_info(api_json_req_t *req)
{
	wifi_api_ap_info_t ap_info;
	wifi_api_ap_get_info(&ap_info);
	req->out = wifi_api_json_serialize_ap_info(&ap_info, WIFI_API_JSON_AP_GET_INFO);
	return 0;
}

static int wifi_api_json_get_scan(api_json_req_t *req)
{
	wifi_api_ap_info_t ap_info[20];
	uint16_t max_count = 20;
	int err;

	ESP_LOGI(TAG, "get scan\n");

	err = wifi_api_get_scan_list(&max_count, ap_info);
	if (err == ESP_ERR_NOT_FINISHED) {
		req->out = wifi_api_json_create_err_rsp(req->in, "Wi-Fi scan busy");
		return 1;
	}

	ESP_LOGI(TAG, "scan ok\n");

	req->out = wifi_api_json_serialize_scan_list(ap_info, max_count);
	return 0;
}


int wifi_api_json_connect(api_json_req_t *req)
{
	char *ssid;
	char *password;

	ssid = cJSON_GetStringValue(cJSON_GetObjectItem(req->in, "ssid"));
	password = cJSON_GetStringValue(cJSON_GetObjectItem(req->in, "password"));
	if (ssid == NULL || password == NULL) {
		return 1;
	}

	int err = wifi_api_connect(ssid, password);
	if (err) {
		return err;
	}

	return wifi_api_json_sta_get_ap_info(req);
};

int wifi_api_json_disconnect(api_json_req_t *req)
{
	return wifi_api_disconnect();
}

/* ****
 *  register module
 * */

static int wifi_api_json_init(api_json_module_cfg_t *cfg)
{
	cfg->on_req = on_json_req;
	cfg->module_id = WIFI_MODULE_ID;
	return 0;
}

API_JSON_MODULE_REGISTER(wifi_api_json_init)
