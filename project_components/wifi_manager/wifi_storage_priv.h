#ifndef WIFI_STORAGE_PRIV_H_GUARD
#define WIFI_STORAGE_PRIV_H_GUARD

#define WIFI_MAX_AP_CRED_RECORD 1

typedef struct w_cache_t {
	uint32_t ap_bitmap;
	wifi_credential_t ap_creds[WIFI_MAX_AP_CRED_RECORD];
} w_cache_t;

typedef enum wt_wifi_key_enum {
	KEY_WIFI_RESERVED = 0x000,
/* WIFI */
	KEY_WIFI_AP_CRED = 0x1,


	/* TODO: should have 1 for each AP */
	KEY_WIFI_STA_USE_STATIC = 0x03, /* bit[0:31]=[IP, MASK, GATEWAY, DNS_MAIN, DNS_BACK] */

	/* STA information */
	KEY_WIFI_STA_LAST_AP_CRED = 0x08, /*!< ssid[32] + password[64] */
	KEY_WIFI_STA_AP_BITMAP = 0x09, /* 32 bit */

	KEY_WIFI_STA_STATIC_BASE = 0x10, /* [IP:4B, MASK:4B, GW:4B, DNS1:4B, DNS2:4B] = 20B */
	KEY_WIFI_STA_STATIC_LAST = 0x1F, /* [IP:4B, MASK:4B, GW:4B, DNS1:4B, DNS2:4B] */

	KEY_WIFI_APSTA_MODE = 0xFF, /* 1B */
	KEY_UNUSED_100      = 0x0100, /* avoid: same as 0x1 */
} wt_wifi_key;

#endif //WIFI_STORAGE_PRIV_H_GUARD