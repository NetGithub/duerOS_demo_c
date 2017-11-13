/**
 * Copyright (2017) Baidu Inc. All rights reserveed.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * File: lightduer_ota_downloader.h
 * Auth: Zhong Shuai (zhongshuai@baidu.com)
 * Desc: OTA Downloader Head File
 */

#ifndef BAIDU_DUER_LIGHTDUER_INCLUDE_LIGHTDUER_OTA_DOWNLOADER_H
#define BAIDU_DUER_LIGHTDUER_INCLUDE_LIGHTDUER_OTA_DOWNLOADER_H

#include "lightduer_mutex.h"
#include "lightduer_http_client.h"

#ifndef URL_LEN
#define URL_LEN 301
#endif

enum DownloaderProtocol {
    HTTP = 0,
    COAP = 1,
    MAX_PROTOCOL_COUNT = 2,
};

typedef struct OTADownloader {
    enum DownloaderProtocol dp;
    void *private_data;              // Set the private data you want to pass
    duer_mutex_t lock;
    char url[URL_LEN + 1];
    struct OTADownloader_ops *ops;
} OTADownloader;

typedef int (*data_handler)(
        void* private_data,
        const char *buf,
        size_t len);

/*
 * We need to consider unblock & timeout situation (TBD)
 * connect_server callback function requires you that
 * connect to server and receive the data immediately
 * I think the abstract is not very good. Refactor (TBD)
 */
struct OTADownloader_ops {
    int (*init)(OTADownloader *downloader);
    int (*register_data_notify)(OTADownloader *downloader, data_handler handler, void *private_data);
    int (*connect_server)(OTADownloader *downloader, const char *url);
    int (*disconnect_server)(OTADownloader *downloader);
    int (*destroy)(OTADownloader *downloader);
};

/*
 * Initialise OTA Downloader Module
 *
 * @param void:
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_init_ota_downloader(void);

/*
 * Uninitialise OTA Downloader Module
 *
 * @param void:
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_uninit_ota_downloader(void);
/*
 * Create OTA Downloader
 *
 * @param void:
 *
 * @return OTADownloader *: Success: OTADownloader *
 *                          Failed:  NULL
 */
extern OTADownloader *duer_ota_downloader_create_downloader(void);

/*
 * Initialise OTA Downloader
 *
 * @param downloader: OTA Downloader object
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_ota_downloader_init_downloader(OTADownloader *downloader);

/*
 * Register a OTA Downloader to OTA Downloader module
 *
 * @param downloader: Downloader object
 *
 * @param dp: Download protocol which the downloader support
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_ota_downloader_register_downloader(
        OTADownloader *downloader,
        enum DownloaderProtocol dp);

/*
 * Unregister a OTA Downloader from OTA Downloader module
 *
 * @param dp: Download protocol
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_ota_downloader_unregister_downloader(enum DownloaderProtocol dp);

/*
 * Get a OTA Downloader from OTA Downloader module
 *
 * @param dp: Download protocol which you want
 *
 * @return OTADownloader *: Success: OTADownloader *
 *                          Failed:  NULL
 */
extern OTADownloader *duer_ota_downloader_get_downloader(enum DownloaderProtocol dp);

/*
 * Destroy a OTA Downloader, free the memory and unregister it from OTA Downloader module
 *
 * @param downloader: OTA downloader object
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_ota_downloader_destroy_downloader(OTADownloader *downloader);

/*
 * Register the downloader operations to a Downloader object
 * If you want to support a new downloader protocol, you need to implement OTADownloader_ops
 *
 * @param downloader: Downloader object
 *
 * @param downloader_ops:  OTADownloader operations
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_ota_downloader_register_downloader_ops(
        OTADownloader *downloader,
        struct OTADownloader_ops *downloader_ops);

/*
 * Connect to server to get the data
 * Note: Once you call it, the OTADownloader will downloade the data from server and pass it to you
 *
 * @param downloader: Downloader object
 *
 * @param url: URL
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_ota_downloader_connect_server(OTADownloader *downloader, const char *url);

/*
 * Disconnect server
 * Close the socket
 *
 * @param downloader: Downloader object
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_ota_downloader_disconnect_server(OTADownloader *downloader);

/*
 * Register a callback functions "hancler" to receive the data which get from URL
 *
 * @param downloader: Downloader object
 *
 * @param handler: Callback function to receive the data which get form URL
 *
 * @param private: param which you want to pass to handler
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_ota_downloader_register_data_notify(
        OTADownloader *downloader,
        data_handler handler,
        void *private_data);

/*
 * Set the private data you want pass to doloader OPS
 *
 * @param downloader: Downloader object
 *
 * @param private_data: data whchi you want to pass
 *
 * @return int: Success: DUER_OK
 *              Failed:  Other
 */
extern int duer_ota_downloader_set_private_data(OTADownloader *downloader, void *private_data);

/*
 * Get the private data
 *
 * @param downloader: Downloader object
 *
 * @return void *: Success: Other
 *                 Failed:  NULL
 */
extern void *duer_ota_downloader_get_private_data(OTADownloader *downloader);

#endif // BAIDU_DUER_LIGHTDUER_INCLUDE_LIGHTDUER_OTA_DOWNLOADER_H
