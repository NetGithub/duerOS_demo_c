#
# Copyright (2017) Baidu Inc. All rights reserveed.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

include $(CLEAR_VAR)

MODULE_PATH := $(BASE_DIR)/platform/source-freertos

LOCAL_MODULE := port-freertos

LOCAL_STATIC_LIBRARIES := framework cjson connagent coap voice_engine platform

LOCAL_SRC_FILES := $(wildcard $(MODULE_PATH)/*.c)

LOCAL_INCLUDES :=

ifeq ($(strip $(CUSTOMER)),mw300)
LOCAL_INCLUDES += \
	$(MODULE_PATH)/include-mw300 \
	$(MODULE_PATH)/include-mw300/freertos \
	$(MODULE_PATH)/include-mw300/port \
	$(MODULE_PATH)/include-mw300/ipv4 \
	$(MODULE_PATH)/include-mw300/ipv6
else ifeq ($(strip $(CUSTOMER)),esp32)
LOCAL_INCLUDES += \
	$(MODULE_PATH)/include-esp32 \
	$(IDF_PATH)/components/freertos/include \
	$(IDF_PATH)/components/esp32/include \
	$(IDF_PATH)/components/log/include \
	$(IDF_PATH)/components/heap/include \
	$(IDF_PATH)/components/driver/include \
	$(IDF_PATH)/components/vfs/include \
	$(IDF_PATH)/components/soc/include \
	$(IDF_PATH)/components/soc/esp32/include \
	$(IDF_PATH)/components/lwip/include/lwip \
	$(IDF_PATH)/components/lwip/include/lwip/port
endif


include $(BUILD_STATIC_LIB)
