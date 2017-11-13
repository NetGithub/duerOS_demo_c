/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_baidu_lightduer_lib_jni_LightduerOsJni */

#ifndef _Included_com_baidu_lightduer_lib_jni_LightduerOsJni
#define _Included_com_baidu_lightduer_lib_jni_LightduerOsJni

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    initialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_initialize
  (JNIEnv *, jclass);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    setEventListener
 * Signature: (Lcom/baidu/lightduer/lib/jni/event/LightduerEventListener;)V
 */
JNIEXPORT void JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_setEventListener
  (JNIEnv *, jclass, jobject);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    setVoiceResultListener
 * Signature: (Lcom/baidu/lightduer/lib/jni/voice/LightduerVoiceResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_setVoiceResultListener
  (JNIEnv *, jclass, jobject);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    startServerByProfile
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_startServerByProfile
  (JNIEnv *, jclass, jbyteArray);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    stopServer
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_stopServer
  (JNIEnv *, jclass);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    setVoiceMode
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_setVoiceMode
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    startVoice
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_startVoice
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    sendVoiceData
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_sendVoiceData
  (JNIEnv *, jclass, jbyteArray);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    stopVoice
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_stopVoice
  (JNIEnv *, jclass);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    reportData
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_reportData
  (JNIEnv *, jclass, jstring);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    addResource
 * Signature: ([Lcom/baidu/lightduer/lib/jni/controlpoint/LightduerResource;)I
 */
JNIEXPORT jint JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_addResource
  (JNIEnv *, jclass, jobjectArray);

/*
 * Class:     com_baidu_lightduer_lib_jni_LightduerOsJni
 * Method:    sendResponse
 * Signature: (Lcom/baidu/lightduer/lib/jni/utils/LightduerMessage;S[B)I
 */
JNIEXPORT jint JNICALL Java_com_baidu_lightduer_lib_jni_LightduerOsJni_sendResponse
  (JNIEnv *, jclass, jobject, jshort, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif
