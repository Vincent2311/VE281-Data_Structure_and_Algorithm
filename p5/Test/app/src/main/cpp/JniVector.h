#include "jni.h"

#ifndef MY_APPLICATION_JNIVECTOR_H
#define MY_APPLICATION_JNIVECTOR_H

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_myapplication_CVector_get(JNIEnv *env, jobject thiz, jint i);

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_myapplication_CVector_size(JNIEnv *env, jobject thiz);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_CVector_finalize(JNIEnv *env, jobject thiz);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_CVector_initialize(JNIEnv *env, jobject thiz);

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_myapplication_CVector_getInt(JNIEnv *env, jobject thiz, jint i);

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_myapplication_CVector_intAt(JNIEnv *env, jclass clazz, jlong ptr, jint i);
#endif //MY_APPLICATION_JNIVECTOR_H

