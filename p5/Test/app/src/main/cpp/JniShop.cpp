#include "JniShop.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_myapplication_Shop_newShop(JNIEnv *env, jclass clazz, jint x, jint y,
                                            jstring shop_name, jint rating, jfloat avg_cost) {
    return reinterpret_cast<jlong>(new JniShop(x, y, jstring2string(env, shop_name), rating,
                                               avg_cost));
}

extern "C"
JNIEXPORT jint
JNICALL
Java_com_example_myapplication_Shop_getAvgCost(JNIEnv *env, jclass clazz, jlong obj_id) {
    JniShop *cShop = reinterpret_cast<JniShop *>(obj_id);
    if (cShop == nullptr) return -1;
    return cShop->avgCost;
}

extern "C"
JNIEXPORT jint
JNICALL
Java_com_example_myapplication_Shop_getRating(JNIEnv *env, jclass clazz, jlong obj_id) {
    JniShop *cShop = reinterpret_cast<JniShop *>(obj_id);
    if (cShop == nullptr) return -1;
    return cShop->rating;
}

extern "C"
JNIEXPORT jint
JNICALL
Java_com_example_myapplication_Shop_getY(JNIEnv *env, jclass clazz, jlong obj_id) {
    JniShop *cShop = reinterpret_cast<JniShop *>(obj_id);
    if (cShop == nullptr) return -1;
    return cShop->y;
}

extern "C"
JNIEXPORT jint
JNICALL
Java_com_example_myapplication_Shop_getX(JNIEnv *env, jclass clazz, jlong obj_id) {
    JniShop *cShop = reinterpret_cast<JniShop *>(obj_id);
    if (cShop == nullptr) return -1;
    return cShop->x;
}

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_myapplication_Shop_getName(JNIEnv *env, jclass clazz, jlong obj_id) {
    JniShop *cShop = reinterpret_cast<JniShop *>(obj_id);
    return string2jstring(env, cShop->shopName);
}