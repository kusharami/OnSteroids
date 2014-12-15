/*
 * types.h
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#ifndef TYPES_H_
#define TYPES_H_

#ifndef NULL
#define NULL 0
#endif

#include <android/log.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define STRINGIFY(x) #x
#define LOG_TAG    __FILE__ ":" STRINGIFY(__MyNative__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


#endif /* TYPES_H_ */
