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
 * File: duer_log.h
 * Auth: Su Hao(suhao@baidu.com)
 * Desc: Print log.
 */

#ifndef LIBDUER_DEVICE_FRAMEWORK_CORE_LIGHTDUER_LOG_H
#define LIBDUER_DEVICE_FRAMEWORK_CORE_LIGHTDUER_LOG_H

#include "lightduer_debug.h"

#define DUER_STRING_OUTPUT(_x)	(((_x) == NULL) ? ("NULL") : (_x))

#ifndef DUER_DEBUG_LEVEL
#define DUER_DEBUG_LEVEL 3
#endif

#ifdef DUER_DEBUG_LEVEL
#define DUER_DEBUG_EXT(_l, _f, _li, ...) duer_debug(_l, _f, _li, __VA_ARGS__)
#else
#define DUER_DEBUG_EXT(...)
#endif

#if defined(DUER_DEBUG_LEVEL) && (DUER_DEBUG_LEVEL >= 5)
#define DUER_LOGV_EXT(_f, _l, ...)   DUER_DEBUG_EXT(5, _f, _l, __VA_ARGS__)
#else
#define DUER_LOGV_EXT(...)
#endif

#if defined(DUER_DEBUG_LEVEL) && (DUER_DEBUG_LEVEL >= 4)
#define DUER_LOGD_EXT(_f, _l, ...)   DUER_DEBUG_EXT(4, _f, _l, __VA_ARGS__)
#else
#define DUER_LOGD_EXT(...)
#endif

#if defined(DUER_DEBUG_LEVEL) && (DUER_DEBUG_LEVEL >= 3)
#define DUER_LOGI_EXT(_f, _l, ...)   DUER_DEBUG_EXT(3, _f, _l, __VA_ARGS__)
#else
#define DUER_LOGI_EXT(...)
#endif

#if defined(DUER_DEBUG_LEVEL) && (DUER_DEBUG_LEVEL >= 2)
#define DUER_LOGW_EXT(_f, _l, ...)   DUER_DEBUG_EXT(2, _f, _l, __VA_ARGS__)
#else
#define DUER_LOGW_EXT(...)
#endif

#if defined(DUER_DEBUG_LEVEL) && (DUER_DEBUG_LEVEL >= 1)
#define DUER_LOGE_EXT(_f, _l, ...)   DUER_DEBUG_EXT(1, _f, _l, __VA_ARGS__)
#else
#define DUER_LOGE_EXT(...)
#endif

#if defined(DUER_DEBUG_LEVEL) && (DUER_DEBUG_LEVEL >= 0)
#define DUER_LOGWTF_EXT(_f, _l, ...)   DUER_DEBUG_EXT(0, _f, _l, __VA_ARGS__)
#else
#define DUER_LOGWTF_EXT(...)
#endif

#define DUER_DEBUG(_l, ...)      DUER_DEBUG_EXT(_l, __FILE__, __LINE__, __VA_ARGS__)
#define DUER_LOGV(...)           DUER_LOGV_EXT(__FILE__, __LINE__, __VA_ARGS__)
#define DUER_LOGD(...)           DUER_LOGD_EXT(__FILE__, __LINE__, __VA_ARGS__)
#define DUER_LOGI(...)           DUER_LOGI_EXT(__FILE__, __LINE__, __VA_ARGS__)
#define DUER_LOGW(...)           DUER_LOGW_EXT(__FILE__, __LINE__, __VA_ARGS__)
#define DUER_LOGE(...)           DUER_LOGE_EXT(__FILE__, __LINE__, __VA_ARGS__)
#define DUER_LOGWTF(...)         DUER_LOGWTF_EXT(__FILE__, __LINE__, __VA_ARGS__)
#endif/*LIBDUER_DEVICE_FRAMEWORK_CORE_LIGHTDUER_LOG_H*/
