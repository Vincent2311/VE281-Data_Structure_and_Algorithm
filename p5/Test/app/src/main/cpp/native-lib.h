#include <jni.h>
#include <string>

using namespace std;

#ifndef NATIVELIB_H
#define NATIVELIB_H

/**
 * convert jstring to string
 * @param env
 * @param jstr: a jstring type string
 * @return string type
 */
char *jstring2string(JNIEnv *env, jstring jstr);

/**
 * convert string to jstring
 * @param env
 * @param str: a string
 * @return jstring type
 */
jstring string2jstring(JNIEnv *env, char *str);

#endif