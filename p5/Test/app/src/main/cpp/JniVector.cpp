#include "JniVector.h"
#include <vector>

using namespace std;

/***/
vector<void *> *getVectorPointer(JNIEnv *env, jobject thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(clazz, "objId", "J");
    jlong ptr = env->GetLongField(thiz, fid);
    return reinterpret_cast<vector<void *> *>(ptr);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_myapplication_CVector_get(JNIEnv *env, jobject thiz, jint i) {
    vector<void *> *vec = getVectorPointer(env, thiz);
    if (vec == nullptr) return -1;
    if (vec->size() <= i) return -2;
    return reinterpret_cast<jlong>(vec->at(i));
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_myapplication_CVector_size(JNIEnv *env, jobject thiz) {
    vector<void *> *vec = getVectorPointer(env, thiz);
    if (vec == nullptr) return -1;
    return vec->size();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_CVector_finalize(JNIEnv *env, jobject thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(clazz, "objId", "J");
    jlong ptr = env->GetLongField(thiz, fid);
    (delete ((vector<void *> *) ptr));

    //reset pointer,Avoid repeated releases
    env->SetLongField(thiz, fid, (jlong) 0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_CVector_initialize(JNIEnv *env, jobject thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(clazz, "objId", "J");
    jlong ptr = env->GetLongField(thiz, fid);

    if (ptr == NULL) {
        vector<void *> *vec = new vector<void *>();
        ptr = reinterpret_cast<jlong>(vec);
        env->SetLongField(thiz, fid, ptr);
    }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_myapplication_CVector_getInt(JNIEnv *env, jobject thiz, jint i) {
    jclass clazz = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(clazz, "objId", "J");
    jlong ptr = env->GetLongField(thiz, fid);
    vector<jint> *vec = reinterpret_cast<vector<jint> *>(ptr);
    if (vec == nullptr) return -1;
    if (vec->size() <= i) return -2;
    return reinterpret_cast<jint>(vec->at(i));
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_myapplication_CVector_intAt(JNIEnv *env, jclass clazz, jlong ptr, jint i) {
    vector<jint> *vec = reinterpret_cast<vector<jint> *>(ptr);
    if (vec == nullptr) return -1;
    if (vec->size() <= i) return -2;
    return reinterpret_cast<jint>(vec->at(i));
}