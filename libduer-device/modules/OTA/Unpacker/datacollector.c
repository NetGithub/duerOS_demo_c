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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "fs.h"
#include "datacollector.h"
#include "pack_include.h"
#include "ctype.h"
#include "lightduer_memory.h"

int mbed_str_case_cmp(const char *s1,const char *s2);
cJSON* mbed_find_cjson_object(cJSON* object, const char* key);
uint32_t mbed_get_module_offset(cJSON* root, uint8_t target_index);
#define SUPPORT_FILE_TYPE_NUM	5
file_type support_file_type[SUPPORT_FILE_TYPE_NUM] = {
					{"js",ModuleTypeJS},
					{"bin",ModuleTypeBIN},
					{"so",ModuleTypeSO},
					{"json",ModuleTypeJSON},
					{"img",ModuleTypeIMG}
				};

int mbed_str_case_cmp(const char *s1,const char *s2)
{
   if (!s1) {
	return (s1==s2) ? 0 : 1;
   }

   if (!s2) {
	return 1;
   }

   for (; tolower(*s1) == tolower(*s2); ++s1, ++s2) {
	if(*s1 == 0) {
	   return 0;
	}
   }
   return tolower(*(const unsigned char *)s1) - tolower(*(const unsigned char *)s2);
}

cJSON* mbed_find_cjson_object(cJSON* object, const char* key)
{
   cJSON* subitem = object->child;
   cJSON* tmp = NULL;
   while(subitem){

	if(!mbed_str_case_cmp(subitem->string, key)) {
	   return subitem;
	}

	if(subitem->child) {
	   tmp = mbed_find_cjson_object(subitem, key);
	}

	if(tmp) {
	   return tmp;
	}
	subitem = subitem->next;
   }

   return subitem;
}

/**
 *
 * build meta.json data object
 *
 * \return ctx to collect package data
 *
 */
void* mbed_build_pck_config_block(unsigned char* filename)
{
   meta_info_t* meta = NULL;
   long filehandle = 0;
   long ret = 0;
#ifdef _DEBUG_
   uint16_t debug = 0;
   uint16_t pck_hdr_sz = 0;
   unsigned char pck_hdr[] = {
	0x6D,0x62,0x65,0x64,0x18,0x01,0x00,0x00,0x80,0x00,0x00,0x00,0x58,0xA4,0xB0,0x4D
,0x7D,0x25,0x44,0x77,0x58,0x91,0x9A,0xAC,0x72,0x57,0xD0,0xD6,0xDE,0xB6,0x2B,0x8B
,0x31,0x00,0xB2,0xD0,0x6E,0x6F,0x18,0x1E,0x06,0x2C,0xF5,0x25,0xC6,0x17,0x11,0x15
,0x0A,0x98,0x06,0x7D,0xFD,0xCB,0xAA,0x2F,0x95,0x62,0x55,0x74,0x8D,0x8E,0x80,0xA5
,0xFB,0xD8,0x7B,0x27,0x2D,0xFD,0xBF,0x6B,0xAB,0x06,0x3D,0xF0,0xF0,0x02,0xDF,0x08
,0x12,0x52,0x41,0x61,0x30,0xCA,0x02,0x18,0xA9,0x67,0x13,0xC2,0x2A,0x63,0xEF,0x18
,0x49,0x07,0x93,0xEC,0x81,0x01,0x9A,0xF7,0x24,0x82,0x78,0xB3,0x4B,0xE5,0x5F,0xB0
,0x7F,0x99,0xB8,0x0A,0x82,0xA1,0x7B,0xBB,0x31,0xA5,0xB3,0xA4,0x1C,0xB6,0xD5,0x1D
,0x5F,0xBA,0x79,0xB0,0xB3,0xAD,0x98,0xF7,0x7F,0xDE,0x9A,0x51,0x80,0x00,0x00,0x00
,0x14,0x3E,0x8D,0xCB,0xAA,0x81,0x39,0x72,0x77,0x41,0xB8,0xFD,0x67,0xE8,0xEB,0xBE
,0x7F,0xF3,0x4C,0x2B,0xE3,0x7F,0xF1,0x68,0x11,0x41,0x43,0x75,0x66,0x8A,0x2B,0x90
,0xB4,0x7C,0xC6,0x1D,0x36,0xF9,0xEB,0x36,0x0F,0x3E,0x67,0x76,0xB3,0xAD,0x50,0x6C
,0x90,0x75,0x98,0x76,0x54,0xDE,0x4C,0xEF,0x06,0xBF,0xD2,0x8B,0xD4,0xD7,0x41,0xFA
,0xA8,0xCB,0x98,0x1F,0x3F,0x63,0x9A,0x7A,0xD5,0x93,0x74,0xB2,0x65,0x50,0xFD,0xB3
,0x96,0x53,0xE5,0x35,0xEC,0xF0,0x58,0x52,0x1E,0x5A,0xCD,0xE0,0x5A,0x60,0x17,0x33
,0x77,0x94,0x85,0x89,0x7F,0x68,0x34,0x3B,0xAB,0x5C,0x49,0xAF,0x02,0x20,0xFD,0xA0
,0x51,0x07,0x9B,0x44,0xD3,0xF5,0xD7,0x59,0xCA,0x8F,0xF4,0x47,0x70,0xA5,0xFE,0x52
,0x51,0x04,0x00,0x00,0x04,0x10,0x03,0x00
   };

   unsigned char meta_json[] = {
	0x7B,0x0A,0x09,0x22,0x62,0x61,0x73,0x69,0x63,0x5F,0x69,0x6E,0x66,0x6F,0x22,0x3A
,0x7B,0x0A,0x09,0x09,0x22,0x61,0x70,0x70,0x5F,0x6E,0x61,0x6D,0x65,0x22,0x3A,0x22
,0x68,0x65,0x6C,0x6C,0x6F,0x20,0x77,0x6F,0x72,0x6C,0x64,0x22,0x2C,0x0A,0x09,0x09
,0x22,0x75,0x70,0x64,0x61,0x74,0x65,0x22,0x3A,0x66,0x61,0x6C,0x73,0x65,0x2C,0x0A
,0x09,0x09,0x22,0x70,0x61,0x63,0x6B,0x61,0x67,0x65,0x5F,0x74,0x79,0x70,0x65,0x22
,0x3A,0x22,0x41,0x70,0x70,0x22,0x0A,0x09,0x7D,0x2C,0x0A,0x09,0x22,0x69,0x6E,0x73
,0x74,0x5F,0x61,0x6E,0x64,0x5F,0x75,0x6E,0x69,0x6E,0x73,0x74,0x5F,0x69,0x6E,0x66
,0x6F,0x22,0x3A,0x7B,0x0A,0x09,0x09,0x22,0x70,0x61,0x63,0x6B,0x61,0x67,0x65,0x5F
,0x69,0x6E,0x73,0x74,0x61,0x6C,0x6C,0x5F,0x70,0x61,0x74,0x68,0x22,0x3A,0x22,0x22
,0x2C,0x0A,0x09,0x09,0x22,0x6D,0x6F,0x64,0x75,0x6C,0x65,0x5F,0x6C,0x69,0x73,0x74
,0x22,0x3A,0x5B,0x0A,0x09,0x09,0x09,0x7B,0x0A,0x09,0x09,0x09,0x09,0x22,0x6E,0x61
,0x6D,0x65,0x22,0x3A,0x22,0x64,0x65,0x62,0x75,0x67,0x2E,0x62,0x69,0x6E,0x22,0x2C
,0x0A,0x09,0x09,0x09,0x09,0x22,0x74,0x79,0x70,0x65,0x22,0x3A,0x22,0x62,0x69,0x6E
,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x73,0x69,0x7A,0x65,0x22,0x3A,0x39,0x37
,0x37,0x36,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x75,0x70,0x64,0x61,0x74,0x65,0x22
,0x3A,0x74,0x72,0x75,0x65,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x73,0x69,0x67,0x6E
,0x61,0x74,0x75,0x72,0x65,0x22,0x3A,0x22,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22
,0x76,0x65,0x72,0x73,0x69,0x6F,0x6E,0x22,0x3A,0x22,0x31,0x2E,0x30,0x2E,0x30,0x2E
,0x31,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x68,0x77,0x5F,0x76,0x65,0x72,0x73
,0x69,0x6F,0x6E,0x22,0x3A,0x22,0x31,0x2E,0x30,0x2E,0x30,0x2E,0x31,0x22,0x0A,0x09
,0x09,0x09,0x7D,0x2C,0x0A,0x09,0x09,0x09,0x7B,0x0A,0x09,0x09,0x09,0x09,0x22,0x6E
,0x61,0x6D,0x65,0x22,0x3A,0x22,0x6F,0x74,0x61,0x2E,0x69,0x6D,0x67,0x22,0x2C,0x0A
,0x09,0x09,0x09,0x09,0x22,0x74,0x79,0x70,0x65,0x22,0x3A,0x22,0x69,0x6D,0x67,0x22
,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x73,0x69,0x7A,0x65,0x22,0x3A,0x34,0x36,0x31
,0x36,0x30,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x75,0x70,0x64,0x61,0x74,0x65,0x22
,0x3A,0x74,0x72,0x75,0x65,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x73,0x69,0x67,0x6E
,0x61,0x74,0x75,0x72,0x65,0x22,0x3A,0x22,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22
,0x76,0x65,0x72,0x73,0x69,0x6F,0x6E,0x22,0x3A,0x22,0x31,0x2E,0x30,0x2E,0x30,0x2E
,0x32,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x68,0x77,0x5F,0x76,0x65,0x72,0x73
,0x69,0x6F,0x6E,0x22,0x3A,0x22,0x31,0x2E,0x30,0x2E,0x30,0x2E,0x32,0x22,0x0A,0x09
,0x09,0x09,0x7D,0x2C,0x0A,0x09,0x09,0x09,0x7B,0x0A,0x09,0x09,0x09,0x09,0x22,0x6E
,0x61,0x6D,0x65,0x22,0x3A,0x22,0x6E,0x6F,0x74,0x69,0x63,0x65,0x2E,0x6A,0x73,0x22
,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x74,0x79,0x70,0x65,0x22,0x3A,0x22,0x6A,0x73
,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x73,0x69,0x7A,0x65,0x22,0x3A,0x31,0x36
,0x32,0x32,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x75,0x70,0x64,0x61,0x74,0x65,0x22
,0x3A,0x66,0x61,0x6C,0x73,0x65,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x73,0x69,0x67
,0x6E,0x61,0x74,0x75,0x72,0x65,0x22,0x3A,0x22,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09
,0x22,0x76,0x65,0x72,0x73,0x69,0x6F,0x6E,0x22,0x3A,0x22,0x31,0x2E,0x30,0x2E,0x30
,0x2E,0x33,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x68,0x77,0x5F,0x76,0x65,0x72
,0x73,0x69,0x6F,0x6E,0x22,0x3A,0x22,0x31,0x2E,0x30,0x2E,0x30,0x2E,0x33,0x22,0x0A
,0x09,0x09,0x09,0x7D,0x2C,0x0A,0x09,0x09,0x09,0x7B,0x0A,0x09,0x09,0x09,0x09,0x22
,0x6E,0x61,0x6D,0x65,0x22,0x3A,0x22,0x6D,0x6F,0x64,0x75,0x6C,0x65,0x31,0x2E,0x73
,0x6F,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x74,0x79,0x70,0x65,0x22,0x3A,0x22
,0x73,0x6F,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x73,0x69,0x7A,0x65,0x22,0x3A
,0x33,0x33,0x35,0x33,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x75,0x70,0x64,0x61,0x74
,0x65,0x22,0x3A,0x66,0x61,0x6C,0x73,0x65,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x73
,0x69,0x67,0x6E,0x61,0x74,0x75,0x72,0x65,0x22,0x3A,0x22,0x22,0x2C,0x0A,0x09,0x09
,0x09,0x09,0x22,0x76,0x65,0x72,0x73,0x69,0x6F,0x6E,0x22,0x3A,0x22,0x31,0x2E,0x30
,0x2E,0x30,0x2E,0x34,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x68,0x77,0x5F,0x76
,0x65,0x72,0x73,0x69,0x6F,0x6E,0x22,0x3A,0x22,0x31,0x2E,0x30,0x2E,0x30,0x2E,0x34
,0x22,0x0A,0x09,0x09,0x09,0x7D,0x2C,0x0A,0x09,0x09,0x09,0x7B,0x0A,0x09,0x09,0x09
,0x09,0x22,0x6E,0x61,0x6D,0x65,0x22,0x3A,0x22,0x6D,0x6F,0x64,0x75,0x6C,0x65,0x32
,0x2E,0x6A,0x73,0x6F,0x6E,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x74,0x79,0x70
,0x65,0x22,0x3A,0x22,0x6A,0x73,0x6F,0x6E,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22
,0x73,0x69,0x7A,0x65,0x22,0x3A,0x34,0x34,0x30,0x34,0x2C,0x0A,0x09,0x09,0x09,0x09
,0x22,0x75,0x70,0x64,0x61,0x74,0x65,0x22,0x3A,0x66,0x61,0x6C,0x73,0x65,0x2C,0x0A
,0x09,0x09,0x09,0x09,0x22,0x73,0x69,0x67,0x6E,0x61,0x74,0x75,0x72,0x65,0x22,0x3A
,0x22,0x22,0x2C,0x0A,0x09,0x09,0x09,0x09,0x22,0x76,0x65,0x72,0x73,0x69,0x6F,0x6E
,0x22,0x3A,0x22,0x31,0x2E,0x30,0x2E,0x30,0x2E,0x35,0x22,0x2C,0x0A,0x09,0x09,0x09
,0x09,0x22,0x68,0x77,0x5F,0x76,0x65,0x72,0x73,0x69,0x6F,0x6E,0x22,0x3A,0x22,0x31
,0x2E,0x30,0x2E,0x30,0x2E,0x35,0x22,0x0A,0x09,0x09,0x09,0x7D,0x0A,0x09,0x09,0x5D
,0x0A,0x09,0x7D,0x2C,0x0A,0x09,0x22,0x75,0x70,0x64,0x61,0x74,0x65,0x5F,0x69,0x6E
,0x66,0x6F,0x22,0x3A,0x7B,0x0A,0x09,0x09,0x22,0x76,0x65,0x72,0x73,0x69,0x6F,0x6E
,0x22,0x3A,0x22,0x31,0x2E,0x30,0x2E,0x30,0x2E,0x31,0x22,0x0A,0x09,0x7D,0x2C,0x0A
,0x09,0x22,0x66,0x65,0x61,0x74,0x75,0x72,0x65,0x22,0x3A,0x7B,0x0A,0x09,0x09,0x22
,0x61,0x63,0x6C,0x22,0x3A,0x22,0x22,0x2C,0x0A,0x09,0x09,0x22,0x65,0x78,0x74,0x65
,0x6E,0x73,0x69,0x6F,0x6E,0x22,0x3A,0x7B,0x0A,0x09,0x09,0x09,0x22,0x65,0x6D,0x70
,0x74,0x79,0x2D,0x6B,0x65,0x79,0x22,0x3A,0x22,0x65,0x6D,0x70,0x74,0x79,0x2D,0x76
,0x61,0x6C,0x75,0x65,0x22,0x0A,0x09,0x09,0x7D,0x0A,0x09,0x7D,0x0A,0x7D,0x0A
   };
#endif

   ///
   /// package header
   package_header_t* package_header = NULL;
   package_header = (package_header_t*)DUER_MALLOC(sizeof(package_header_t));
   if (!package_header) {
	return NULL;
   }

#ifdef _DEBUG_
   pck_hdr_sz = sizeof(package_header_t);
   memcpy(package_header, pck_hdr, pck_hdr_sz);
/*
   MBEDPACK_DEBUG_PRINT("package header size : %u\n\r", pck_hdr_sz);
   for (debug = 0; debug < pck_hdr_sz; debug++) {
	MBEDPACK_DEBUG_PRINT("%.2x ", ((unsigned char*)package_header)[debug]);
   }
   MBEDPACK_DEBUG_PRINT("\n\r");
   MBEDPACK_DEBUG_PRINT("\n\r");
   MBEDPACK_DEBUG_PRINT("\n\r");
*/
#else
   /// open package
   ret = sl_FsOpen(filename, FS_MODE_OPEN_READ, NULL, &filehandle);
   if(ret < 0) {
	ret = sl_FsClose(filehandle, 0, 0, 0);
   }

   /// read package header
   ret = sl_FsRead(filehandle, 0, package_header, sizeof(package_header_t));
   if(ret < 0) {
	ret = sl_FsClose(filehandle, 0, 0, 0);
   }
#endif

   ///
   /// fill meta info
   meta = (meta_info_t*)DUER_MALLOC(sizeof(meta_info_t));
   if (!meta) {
	return NULL;
   }

   meta->meta_data_size = package_header->meta_size;
   MBEDPACK_DEBUG_PRINT("meta data size : %u\n\r", meta->meta_data_size);
   meta->meta_data = (unsigned char*)DUER_MALLOC(meta->meta_data_size);
   if (!meta->meta_data) {
	return NULL;
   }

#ifdef _DEBUG_
   memcpy(meta->meta_data, meta_json, meta->meta_data_size);
/*
   for (debug = 0; debug < meta->meta_data_size; debug++) {
	MBEDPACK_DEBUG_PRINT("%.2x ", meta->meta_data[debug]);
   }
   MBEDPACK_DEBUG_PRINT("\n\r");
   MBEDPACK_DEBUG_PRINT("\n\r");
   MBEDPACK_DEBUG_PRINT("\n\r");
*/
#else
   /// meta data : read package_header->meta_size (byte)from package offset of sizeof(meta_info_t)(byte);
   ret = sl_FsRead(filehandle, package_header->header_size, meta->meta_data, meta->meta_data_size);
   if(ret < 0) {
	ret = sl_FsClose(filehandle, 0, 0, 0);
   }
#endif

   meta->meta_object = (void*)cJSON_Parse((char*)meta->meta_data);
   meta->module_data_offset = package_header->header_size + meta->meta_data_size;
   MBEDPACK_DEBUG_PRINT("module data offset : %u\n\r", meta->module_data_offset);

   return meta;
}


/**
 *
 * destory meta.json data object
 *
 * \param ctx to collect package data
 *
 */
void mbed_destroy_pck_config_block(meta_info_t* ctx)
{
   if (ctx == NULL) {
	return;
   }

   if (ctx->meta_data) {
	DUER_FREE(ctx->meta_data);
   }

   if (ctx->meta_object) {
	cJSON_Delete((cJSON*)ctx->meta_object);
   }

   DUER_FREE(ctx);
   return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t mbed_get_module_offset(cJSON* root, uint8_t target_index)
{
   uint8_t index = 0;
   uint8_t total_count = 0;
   uint32_t module_offset = 0;
   cJSON* subitem = NULL;
   cJSON* module_list = NULL;
   cJSON* module_item = NULL;

   MBEDPACK_DEBUG_PRINT("func-mbed_get_module_offset : trace--1\n\r");
   if (!root) {
	return 0;
   }
   module_list = mbed_find_cjson_object(root, "module_list");
   if (!module_list) {
	MBEDPACK_DEBUG_PRINT("func-mbed_get_module_offset : trace--2\n\r");
	return 0;
   }

   total_count = cJSON_GetArraySize(module_list);
   if (total_count <= target_index) {
	MBEDPACK_DEBUG_PRINT("func-mbed_get_module_offset : trace--3\n\r");
	return module_offset;
   }

   MBEDPACK_DEBUG_PRINT("func-mbed_get_module_offset : trace--4\n\r");
   for (index = 0; index < target_index; index++) {
	module_item = cJSON_GetArrayItem(module_list, index);
	subitem = module_item->child;
	if (subitem == NULL) {
	   MBEDPACK_DEBUG_PRINT("func-mbed_get_module_offset : trace--5\n\r");
	   continue;
	}

	for (; subitem != NULL; subitem = subitem->next) {
	   MBEDPACK_DEBUG_PRINT("func-mbed_get_module_offset : trace--5-1\n\r");
	   if (!mbed_str_case_cmp(subitem->string, "size")) {
		module_offset += subitem->valueint;
		break;
	   }
	}
   }

   MBEDPACK_DEBUG_PRINT("func-mbed_get_module_offset : trace--6\n\r");
   return module_offset;
}

int mbed_prebuild_module(cJSON* meta_object, module_info_t* module_info, uint8_t index)
{
   int ret = 0;
   uint32_t length = 0;
   uint32_t module_count = 0;
   cJSON* subitem = NULL;
   cJSON* module_list = NULL;
   cJSON* module_item = NULL;

   MBEDPACK_DEBUG_PRINT("func-mbed_prebuild_module : begin!!!\n\r");
   if (!meta_object) {
	MBEDPACK_DEBUG_PRINT("func-mbed_prebuild_module : invalid parameter meta_data!!!\n\r");
	ret = -1;
	goto exit2;
   }

   if (!module_info) {
	MBEDPACK_DEBUG_PRINT("func-mbed_prebuild_module : invalid parameter module_info!!!\n\r");
	ret = -1;
	goto exit2;
   }

   memset(module_info, 0, sizeof(module_info_t));
   module_count = mbed_get_module_num(meta_object, ModuleTypeALL);
   if (module_count < index) {
	MBEDPACK_DEBUG_PRINT("func-mbed_prebuild_module : invalid parameter index!!!\n\r");
	ret = -1;
	goto exit2;
   }

   module_list = mbed_find_cjson_object(meta_object, "module_list");
   if (!module_list) {
	ret = -1;
	MBEDPACK_DEBUG_PRINT("func-mbed_prebuild_module : goto exit2!!!\n\r");
	goto exit2;
   }

   module_item = cJSON_GetArrayItem(module_list, index);
   subitem = module_item->child;
   while (subitem) {
	if (!mbed_str_case_cmp(subitem->string, "name")) {
	   length = strlen(subitem->valuestring) + 1;
	   if (!(module_info->name = (unsigned char*)DUER_MALLOC(length))) {
		MBEDPACK_DEBUG_PRINT("func-mbed_prebuild_module : malloc module name failed!!!\n\r");
		ret = -1;
		goto exit1;
	   }

	} else if (!mbed_str_case_cmp(subitem->string, "version")) {
	   length = strlen(subitem->valuestring) + 1;
	   if (!(module_info->version = (unsigned char*)DUER_MALLOC(length))) {
		MBEDPACK_DEBUG_PRINT("func-mbed_prebuild_module : malloc module version failed!!!\n\r");
		ret = -1;
		goto exit1;
	   }
	} else if (!mbed_str_case_cmp(subitem->string, "hw_version")) {
	   length = strlen(subitem->valuestring) + 1;
	   if (!(module_info->hw_version = (unsigned char*)DUER_MALLOC(length))) {
		MBEDPACK_DEBUG_PRINT("func-mbed_prebuild_module : malloc module hardware version failed!!!\n\r");
		ret = -1;
		goto exit1;
	   }
	}
	subitem = subitem->next;
   }
   MBEDPACK_DEBUG_PRINT("func-mbed_prebuild_module : goto exit2!!!\n\r");
   goto exit2;

exit1:
   if (module_info->version) {
	DUER_FREE(module_info->version);
   }

   if (module_info->hw_version) {
	DUER_FREE(module_info->hw_version);
   }

   if (module_info->name) {
	DUER_FREE(module_info->name);
   }

exit2:
   MBEDPACK_DEBUG_PRINT("func-mbed_prebuild_module : end!!!\n\r");
   return ret;
}

void mbed_postbuild_module(module_info_t* module)
{
   MBEDPACK_DEBUG_PRINT("func-mbed_postbuild_module : begin!!!\n\r");
   if (!module) {
	MBEDPACK_DEBUG_PRINT("func-mbed_postbuild_module : module is null!!!\n\r");
	return;
   }

   if (module->name) {
	DUER_FREE(module->name);
   }

   if (module->version) {
	DUER_FREE(module->version);
   }

   if (module->hw_version) {
	DUER_FREE(module->hw_version);
   }

   MBEDPACK_DEBUG_PRINT("func-mbed_postbuild_module : end!!!\n\r");
}
/*
int mbed_decompress(decompress_info_t* decompress_info, unsigned char* package_bin, uint32_t package_bin_size)
{
   int ret = 0;
   ///
   /// package header
   package_header_t* package_header = NULL;

   ///
   /// package (original or compress) data and size, not include header
   unsigned char* package_data = NULL;
   uint32_t package_data_size = 0;

   ///
   /// package (decompress) data and size, not include header
   unsigned char* decompress_package_data = NULL;
   uint32_t decompress_package_data_size = 0;

   ///
   /// decompress flag
   unsigned char outProps[10] = {0};
   size_t outPropsSize = 5;

   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : begin!!!\n\r");
   if (!decompress_info) {
	MBEDPACK_DEBUG_PRINT("func-mbed_decompress : invalid parameter!!!\n\r");
	ret = -1;
	goto exit1;
   }

   package_header = (package_header_t*)package_bin;
   package_data = package_bin + package_header->header_size;
   package_data_size = package_bin_size - package_header->header_size;

   ///
   /// store decompress flag
   memcpy(outProps, package_data, outPropsSize);
   package_data += outPropsSize;
   package_data_size -= outPropsSize;

   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : package_bin_size -- %"PRIu32" \n\r", package_bin_size);
   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : package_header->header_size -- %"PRIu32" \n\r", package_header->header_size);
   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : package_data_size -- %"PRIu32" \n\r", package_data_size);

   decompress_package_data_size = package_header->ori_package_size;//package_data_size * 3;// + package_data_size / 10 + 600;
   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : decompress_package_data_size -- %"PRIu32" \n\r", decompress_package_data_size);
   decompress_package_data = (unsigned char*)DUER_MALLOC(sizeof(unsigned char) * decompress_package_data_size);
   if (!decompress_package_data) {
	MBEDPACK_DEBUG_PRINT("func-mbed_decompress : malloc failed!!!\n\r");
	ret = -1;
	goto exit1;
   }

   //ret = mbed_buff2buff_decompress(decompress_package_data, (size_t*)&decompress_package_data_size, package_data, (size_t*)&package_data_size, outProps, outPropsSize);
   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : decompress_package_data_size -- %"PRIu32" \n\r", decompress_package_data_size);
   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : package_data_size -- %"PRIu32" \n\r", package_data_size);
   if (ret != 0 && ret != 6) {
	MBEDPACK_DEBUG_PRINT("func-mbed_decompress : return -- %d\n\r", ret);
	MBEDPACK_DEBUG_PRINT("func-mbed_decompress : decompress failed!!!\n\r");
	ret = -1;
	goto exit0;
   }

   ///
   /// fill meta info
   decompress_info->decompress_meta_data_size = package_header->meta_size;
   decompress_info->decompress_meta_data = (unsigned char*)DUER_MALLOC(sizeof(unsigned char) * decompress_info->decompress_meta_data_size);
   memcpy(decompress_info->decompress_meta_data, decompress_package_data, decompress_info->decompress_meta_data_size);
   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : decompress_info->decompress_meta_data_size -- %"PRIu32" \n\r", decompress_info->decompress_meta_data_size);
   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : decompress_info->decompress_meta_data -- %s\n\r", decompress_info->decompress_meta_data);

   ///
   /// fill module info
   decompress_info->decompress_module_data_size = decompress_package_data_size - decompress_info->decompress_meta_data_size;
   decompress_info->decompress_module_data = (unsigned char*)DUER_MALLOC(sizeof(unsigned char) * decompress_info->decompress_module_data_size);
   memcpy(decompress_info->decompress_module_data, decompress_package_data + decompress_info->decompress_meta_data_size, decompress_info->decompress_module_data_size);
   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : decompress_info->decompress_module_data_size -- %"PRIu32" \n\r", decompress_info->decompress_module_data_size);
   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : decompress_info->decompress_module_data -- %s\n\r", decompress_info->decompress_module_data);

exit0:
   if (decompress_package_data) {
	DUER_FREE(decompress_package_data);
   }

exit1:
   MBEDPACK_DEBUG_PRINT("func-mbed_decompress : end!!!\n\r");
   return 0;
}
*/
uint8_t mbed_get_module_num(cJSON* meta_object, module_type_t type)
{
   uint8_t index = 0;
   uint8_t index2 = 0;
   uint8_t count = 0;
   uint8_t total_count = 0;
   cJSON* subitem = NULL;
   cJSON* module_list = NULL;
   cJSON* module_item = NULL;

   if (!meta_object) {
	return 0;
   }

   module_list = mbed_find_cjson_object(meta_object, "module_list");
   if (!module_list) {
	return 0;
   }

   total_count = cJSON_GetArraySize(module_list);
   if (type == ModuleTypeALL) {
	return total_count;
   }

   for (index = 0; index < total_count; index++) {
	module_item = cJSON_GetArrayItem(module_list, index);
	subitem = module_item->child;
	for (; subitem != NULL; subitem = subitem->next) {
		for (index2 = 0; index2 < SUPPORT_FILE_TYPE_NUM; index2++) {
			if (	type == support_file_type[index2].type &&
				!mbed_str_case_cmp(subitem->string, "type") &&
				!mbed_str_case_cmp(subitem->valuestring, support_file_type[index2].string)) {
				count++;
				break;
			}
		}
/*
	   if (	(type == ModuleTypeJS &&
	   !mbed_str_case_cmp(subitem->string, "type") &&
	   !mbed_str_case_cmp(subitem->valuestring, "js"))
	   ||
	   (type == ModuleTypeBIN &&
	   !mbed_str_case_cmp(subitem->string, "type") &&
	   !mbed_str_case_cmp(subitem->valuestring, "bin"))
	   ||
	   (type == ModuleTypeSO &&
	   !mbed_str_case_cmp(subitem->string, "type") &&
	   !mbed_str_case_cmp(subitem->valuestring, "so"))
	   ||
	   (type == ModuleTypeJSON &&
	   !mbed_str_case_cmp(subitem->string, "type") &&
	   !mbed_str_case_cmp(subitem->valuestring, "json"))) {
		count++;
		break;
	   }
*/
	}
   }

//   cJSON_Delete(root);
   return count;
}

/// TODO
int mbed_get_module_info(cJSON* meta_object, uint32_t module_begin_offset, module_info_t* module_info, uint8_t index)
{
   int ret = 0;
   uint8_t index2 = 0;
   uint32_t length = 0;
   uint32_t module_count = 0;
   uint32_t module_offset = 0;
   cJSON* subitem = NULL;
   cJSON* module_list = NULL;
   cJSON* module_item = NULL;

   MBEDPACK_DEBUG_PRINT("func-mbed_get_module_info : begin!!!\n\r");

   if (!meta_object || !module_info || !meta_object) {
	MBEDPACK_DEBUG_PRINT("func-mbed_get_module_info : invalid pointer parameter!!!\n\r");
	ret = -1;
	goto exit1;
   }

   MBEDPACK_DEBUG_PRINT("func-mbed_get_module_info : trace--1!!!\n\r");
   module_count = mbed_get_module_num(meta_object, ModuleTypeALL);
   if (module_count < index) {
	MBEDPACK_DEBUG_PRINT("func-mbed_get_module_info : invalid parameter index!!!\n\r");
	ret = -1;
	goto exit1;
   }

   MBEDPACK_DEBUG_PRINT("func-mbed_get_module_info : trace--2!!!\n\r");
   MBEDPACK_DEBUG_PRINT("func-mbed_get_module_info : trace--3!!!\n\r");
   module_list = mbed_find_cjson_object(meta_object, "module_list");
   if (!module_list) {
	ret = -1;
	goto exit0;
   }

   module_item = cJSON_GetArrayItem(module_list, index);
   subitem = module_item->child;
   while (subitem) {
	if (!mbed_str_case_cmp(subitem->string, "name")) {
	   length = strlen(subitem->valuestring) + 1;
	   if (!module_info->name) {
		MBEDPACK_DEBUG_PRINT("mbed_get_module_info : module name is null!!!\n\r");
		ret = -1;
		goto exit0;
	   } else {
		memcpy(module_info->name, subitem->valuestring, length);
	   }
	} else if (!mbed_str_case_cmp(subitem->string, "type")) {
		for (index2 = 0; index2 < SUPPORT_FILE_TYPE_NUM; index2++) {
			if (!mbed_str_case_cmp(subitem->valuestring, support_file_type[index2].string)) {
				module_info->type = support_file_type[index2].type;
				break;
			}
		}
/*
	   if (!mbed_str_case_cmp(subitem->valuestring, "so")) {
		module_info->type = ModuleTypeSO;
	   } else if (!mbed_str_case_cmp(subitem->valuestring, "bin")) {
		module_info->type = ModuleTypeBIN;
	   } else if (!mbed_str_case_cmp(subitem->valuestring, "js")) {
		module_info->type = ModuleTypeJS;
	   } else if (!mbed_str_case_cmp(subitem->valuestring, "json")) {
		module_info->type = ModuleTypeJSON;
	   }
*/
	} else if (!mbed_str_case_cmp(subitem->string, "size")) {
	   module_info->module_size = subitem->valueint;
	} else if (!mbed_str_case_cmp(subitem->string, "update")) {
	   module_info->update = subitem->valueint;
	} else if (!mbed_str_case_cmp(subitem->string, "version")) {
	   length = strlen(subitem->valuestring) + 1;
	   if (!module_info->version) {
		MBEDPACK_DEBUG_PRINT("mbed_get_module_info : module version is null!!!\n\r");
		ret = -1;
		goto exit0;
	   } else {
		memcpy(module_info->version, subitem->valuestring, length);
	   }
	} else if (!mbed_str_case_cmp(subitem->string, "hw_version")) {
	   length = strlen(subitem->valuestring) + 1;
	   if (!module_info->hw_version) {
		MBEDPACK_DEBUG_PRINT("mbed_get_module_info : module hardware version is null!!!\n\r");
		ret = -1;
		goto exit0;
	   } else {
		memcpy(module_info->hw_version, subitem->valuestring, length);
	   }
	}
	subitem = subitem->next;
   }
   MBEDPACK_DEBUG_PRINT("func-mbed_get_module_info : trace--6!!!\n\r");
   module_offset = mbed_get_module_offset(meta_object, index);
   MBEDPACK_DEBUG_PRINT("mbed_get_module_info : module_offset -- %"PRIu32"!!!\n\r", module_offset);

   module_info->offset = module_begin_offset + module_offset;

   MBEDPACK_DEBUG_PRINT("mbed_get_module_info : \n\rname : %s!!!\n\r", module_info->name);
   MBEDPACK_DEBUG_PRINT("type : %d!!!\n\r", module_info->type);
   MBEDPACK_DEBUG_PRINT("size : %"PRIu32"!!!\n\r", module_info->module_size);
   MBEDPACK_DEBUG_PRINT("update : %d!!!\n\r", module_info->update);
   MBEDPACK_DEBUG_PRINT("version : %s!!!\n\r", module_info->version);
   MBEDPACK_DEBUG_PRINT("hw_version : %s!!!\n\r", module_info->hw_version);
   MBEDPACK_DEBUG_PRINT("offset : %"PRIu32"!!!\n\r", module_info->offset);

exit0:
   //cJSON_Delete(root);

exit1:
   MBEDPACK_DEBUG_PRINT("mbed_get_module_info : end!!!\n\r");
   return ret;
}

int mbed_get_pkg_name(cJSON* meta_object, unsigned char* name, uint8_t* name_len)
{
   int len = 0;
   cJSON* app_name = NULL;
   cJSON* meta_basic = NULL;

   if (meta_object == NULL || (name == NULL && *name_len) || name_len == NULL) {
	return -1;
   }

   meta_basic = mbed_find_cjson_object(meta_object, "basic_info");
   if (!meta_basic) {
	return -1;
   }

   app_name = mbed_find_cjson_object(meta_basic, "app_name");
   len = strlen(app_name->valuestring);
   if (!len) {
	*name_len = 0;
	return 0;
   }

   if (len > *name_len) {
	*name_len = len;
	return 1;
   }

   memcpy(name, app_name->valuestring, len);
   return 0;
}

int mbed_get_pkg_type(cJSON* meta_object, package_type_t* type)
{
   cJSON* pkg_type = NULL;
   cJSON* meta_basic = NULL;

   if (meta_object == NULL) {
	return -1;
   }

   meta_basic = mbed_find_cjson_object(meta_object, "basic_info");
   if (!meta_basic) {
	return -1;
   }

   pkg_type = mbed_find_cjson_object(meta_basic, "package_type");

   if (!mbed_str_case_cmp(pkg_type->valuestring, "App")) {
	*type = PackageTypeApp;
   } else if (!mbed_str_case_cmp(pkg_type->valuestring, "OS")) {
	*type = PackageTypeOS;
   } else if (!mbed_str_case_cmp(pkg_type->valuestring, "Profile")) {
	*type = PackageTypeProfile;
   } else {
	*type = PackageTypeUnknown;
   }

   return 0;
}
