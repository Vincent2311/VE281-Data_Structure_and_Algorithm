#include <jni.h>
#include <string>
#include "string"
#include "native-lib.h"
#ifndef MY_APPLICATION_CSHOP_H
#define MY_APPLICATION_CSHOP_H
using namespace std;

class JniShop {
public:
    jint x;
    jint y;
    char *shopName;
    jint rating;
    jfloat avgCost;

    JniShop(jint x, jint y, char *shopName, jint rating, jfloat avgCost) :
            x(x), y(y), shopName(shopName), rating(rating), avgCost(avgCost) {

    }
};

/**
 * get the average cost of a certain shop, whose pointer is obj_id
 */
extern "C"
JNIEXPORT jint
JNICALL
Java_com_example_myapplication_Shop_getAvgCost(JNIEnv *env, jclass clazz, jlong obj_id);

/**
 * get the rating of a certain shop, whose pointer is obj_id
 */
extern "C"
JNIEXPORT jint
JNICALL
Java_com_example_myapplication_Shop_getRating(JNIEnv *env, jclass clazz, jlong obj_id);

/**
 * get y coordinate of a certain shop, whose pointer is obj_id
 */
extern "C"
JNIEXPORT jint
JNICALL
Java_com_example_myapplication_Shop_getY(JNIEnv *env, jclass clazz, jlong obj_id);

/**
 * get x coordinate of a certain shop, whose pointer is obj_id
 */
extern "C"
JNIEXPORT jint
JNICALL
Java_com_example_myapplication_Shop_getX(JNIEnv *env, jclass clazz, jlong obj_id);

/**
 * get the name of a certain shop, whose pointer is obj_id
 */
extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_myapplication_Shop_getName(JNIEnv *env, jclass clazz, jlong obj_id);

/**
 * create a new shop with certain factors
 */
extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_myapplication_Shop_newShop(JNIEnv *env, jclass clazz, jint x, jint y,
                                            jstring shop_name, jint rating, jfloat avg_cost);
#endif //MY_APPLICATION_CSHOP_H
