#include <jni.h>
#include <string>
#include "hashtable.hpp"
#include "kdtree.hpp"
#include "sort.hpp"
#include "shortestP2P.hpp"
#include "JniShop.h"
#include "native-lib.h"

#ifndef MY_APPLICATION_CSHOPMANAGER_H
#define MY_APPLICATION_CSHOPMANAGER_H
#define T0(t) std::get<0>(t)
#define T1(t) std::get<1>(t)

class ShopManager {
private:
    HashTable<string, JniShop *> *hashTable;
    vector<JniShop *> *innerVector;
    KDTree<tuple<jint, jint>, JniShop *> *kdtree;
    ShortestP2P *shortestP2pMap;
    vector<vector<jint> *> *mapPoints;
    vector<vector<jint> *> *mapEdges;
public:
    ShopManager();

public:
    /**
     * get the count of shop in this manager
     */
    jint shopCount();

    /**
     * get the item at the specified index in the list
     */
    JniShop *get(jint i);

    /**
     * find shop by shopName
     *
     * @return return a pointer to the find shop,may be null
     */
    JniShop *findByName(string name);

    /**
     * order shops by price,This will adjust the current vector
     */
    void orderByPrice();

    /**
     * order shops by rating,This will adjust the current vector
     */
    void orderByRating();

    /**
     * find shop nearby the given location
     *
     * @return return a vector
     */
    long findByLocation(jint x, jint y, jint distance);

    /**
     * get all points on load
     * inner vector : [x,y]
     *
     * @return return a vector
     * */
    vector<vector<jint> *> *getRoads();

    /**
     * get all edges in map
     * inner vector : [pointA,pointB,distance]
     *
     * @return return a vector
     * */
    vector<vector<jint> *> *getEdges();

    /**
     * Plan a path from a given location to a given store
     *
     * @return return a vector
     */
    vector<jint> *planningPath(jint a, jint b);

    /**
     * add a shop to this manager
     * */
    void addShop(JniShop *shop);

    /**
     * Get the Inner Vector object
     *
     * @return return a vector
     */
    vector<JniShop *> *getInnerVector();


};

extern "C" JNIEXPORT jlong
JNICALL
Java_com_example_myapplication_ShopManager_get(JNIEnv *env, jobject thiz, jint i);

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ShopManager_finalize(JNIEnv *env, jobject thiz);

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ShopManager_initialize(JNIEnv *env, jobject thiz);

extern "C" JNIEXPORT jint
JNICALL
Java_com_example_myapplication_ShopManager_shopCount(JNIEnv *env, jobject thiz);

extern "C" JNIEXPORT jlong
JNICALL
Java_com_example_myapplication_ShopManager_planningPath(JNIEnv *env, jobject thiz,
                                                        jint a, jint b);
extern "C" JNIEXPORT jlong
JNICALL
Java_com_example_myapplication_ShopManager_findByLocation(JNIEnv *env, jobject thiz,
                                                          jint x, jint y, jint distance);
extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ShopManager_orderByRating(JNIEnv *env, jobject thiz);

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ShopManager_orderByPrice(JNIEnv *env, jobject thiz);

extern "C" JNIEXPORT jlong
JNICALL
Java_com_example_myapplication_ShopManager_findByName(JNIEnv *env, jobject thiz, jstring name);

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ShopManager_addShop(JNIEnv *env, jobject thiz, jlong ptr);

extern "C" JNIEXPORT jlong JNICALL
Java_com_example_myapplication_ShopManager_vector(JNIEnv *env, jobject thiz);

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_myapplication_ShopManager_getRoads(JNIEnv *env, jobject thiz);

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_myapplication_ShopManager_getEdges(JNIEnv *env, jobject thiz);
#endif // MY_APPLICATION_CSHOPMANAGER_H

