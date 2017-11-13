// Copyright 2017 Baidu Inc. All Rights Reserved.
// Author: Su Hao (suhao@baidu.com)
//
// Description: The CoAP adapter.

#ifndef BAIDU_IOT_TINYDU_IOT_OS_SRC_IOT_BAIDU_CA_SOURCE_BAIDU_CA_COAP_H
#define BAIDU_IOT_TINYDU_IOT_OS_SRC_IOT_BAIDU_CA_SOURCE_BAIDU_CA_COAP_H

#include "lightduer_types.h"
#include "lightduer_lib.h"
#include "lightduer_network_defs.h"
#include "lightduer_coap_defs.h"
#include "lightduer_coap_ep.h"

#define DUER_COAP_MESSAGE_ID_INVALID     (0)

#ifdef __cplusplus
extern "C" {
#endif

typedef void* duer_coap_handler;

typedef duer_status_t (*duer_coap_result_f)(duer_context ctx,
                                            duer_coap_handler hdlr,
                                            const duer_msg_t* msg,
                                            const duer_addr_t* addr);

/*
 * Acquire the coap handler.
 *
 * @Param f_result, in, the callback for user handling events
 * @Param context, in, the user context for f_result callback
 * @Param ctx_context, in, the user socket context
 * @Param key_info, something generated on up layer, but used on down layer
 * @Return duer_coap_handler, the coap context
 */
DUER_INT duer_coap_handler duer_coap_acquire(duer_coap_result_f f_result,
                                             duer_context context,
                                             duer_transevt_func ctx_context,
                                             const void *key_info);

/*
 * Connect to remote CoAP server
 *
 * @Param hdlr, in, the CoAP context
 * @Param addr, in, the remote server address
 * @Param data, in, the security suite data
 * @Param size, in, the data size
 * @Return duer_status_t, the connect status
 */
DUER_INT duer_status_t duer_coap_connect(duer_coap_handler hdlr,
                                         const duer_addr_t* addr,
                                         const void* data,
                                         duer_size_t size);

/*
 * Disconnect from remote CoAP server
 *
 * @Param hdlr, in, the CoAP context
 * @Return duer_status_t, the connect status
 */
DUER_INT duer_status_t duer_coap_disconnect(duer_coap_handler hdlr);

/*
 * Add the resource for CoAP request from other endpoints
 *
 * @Param hdlr, in, the CoAP context
 * @Param res, in, the resource information
 * @Return duer_status_t, the result
 */
DUER_INT duer_status_t duer_coap_resource_add(duer_coap_handler hdlr,
                                              const duer_res_t* res);

/*
 * Remove the resource for CoAP request from other endpoints
 *
 * @Param hdlr, in, the CoAP context
 * @Param path, in, the resource path
 * @Return duer_status_t, the result
 */
DUER_INT duer_status_t duer_coap_resource_remove(duer_coap_handler coap,
                                                 const char* path);

/*
 * Register endpoint by LWM2M
 *
 * @Param hdlr, in, the CoAP context
 * @Param ep, in, the endpoint information
 * @Return duer_status_t, if > 0, the result is the register message id, else is the error status
 */
DUER_INT duer_status_t duer_coap_register(duer_coap_handler coap,
                                          const duer_coap_ep_t* ep);

/*
 * Unregister the endpoint by LWM2M
 *
 * @Param hdlr, in, the CoAP context
 * @Return duer_status_t, if > 0, the result is the unregister message id, else is the error status
 */
DUER_INT duer_status_t duer_coap_unregister(duer_coap_handler coap);

/*
 * Update registration by LWM2M
 *
 * @Param hdlr, in, the CoAP context
 * @Param lifetime, in, the endpoint lifetime
 * @Return duer_status_t, if > 0, the result is the update registration message id,
 *                       else is the error status
 */
DUER_INT duer_status_t duer_coap_update_registration(duer_coap_handler coap,
                                                     duer_u32_t lifetime);

/*
 * Send the CoAP message
 *
 * @Param hdlr, in, the CoAP context
 * @Param msg, in, the CoAP message
 * @Return duer_status_t, the result
 */
DUER_INT duer_status_t duer_coap_send(duer_coap_handler coap, const duer_msg_t* msg);

/*
 * Receive the CoAP message
 *
 * @Param hdlr, in, the CoAP context
 * @Return duer_status_t, the result
 */
DUER_INT duer_status_t duer_coap_data_available(duer_coap_handler coap);

/*
 * Execute the cached CoAP data
 *
 * @Param hdlr, in, the CoAP context
 * @Param timestamp, in, the timestamp for mark the message
 * @Return duer_status_t, the result
 */
DUER_INT duer_status_t duer_coap_exec(duer_coap_handler hdlr, duer_u32_t timestamp);

/*
 * Release the CoAP context
 *
 * @Param hdlr, in, the CoAP context
 * @Return duer_status_t, the result
 */
DUER_INT duer_status_t duer_coap_release(duer_coap_handler coap);

/*
 * Set read timeout
 *
 * @Param hdlr, in, the CoAP context
 * @Param timeout, in, the timeout value by milliseconds
 * @Return duer_bool, if match return DUER_TRUE, else return DUER_FALSE
 */
DUER_INT duer_status_t duer_coap_set_read_timeout(duer_coap_handler coap, duer_u32_t timeout);

#ifdef __cplusplus
}
#endif

#endif // BAIDU_IOT_TINYDU_IOT_OS_SRC_IOT_BAIDU_CA_SOURCE_BAIDU_CA_COAP_H
