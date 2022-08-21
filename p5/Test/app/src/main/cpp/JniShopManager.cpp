#include "JniShopManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ShopManager::addShop(JniShop *shop) {
    innerVector->push_back(shop);
    hashTable->insert(shop->shopName, shop);
    if (kdtree == nullptr) {
        vector<pair<tuple<jint, jint>, JniShop *>> v;
        v.push_back(pair<tuple<jint, jint>, JniShop *>(make_tuple(shop->x, shop->y), shop));
        kdtree = new KDTree<tuple<jint, jint>, JniShop *>(v);
    } else {
        kdtree->insert(make_tuple(shop->x, shop->y), shop);
    }
}

jint ShopManager::shopCount() {
    return innerVector->size();
}

JniShop *ShopManager::get(jint i) {
    if (i >= innerVector->size())
        return reinterpret_cast<JniShop *>(-2L);
    return innerVector->at(i);
}

JniShop *ShopManager::findByName(string name) {
    auto find = hashTable->find(name);
    if (find == hashTable->end())
        return 0;
    return find->second;
}

struct _LIBCPP_TEMPLATE_VIS lessPrice : binary_function<JniShop *, JniShop *, bool> {
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY bool operator()(const JniShop *__x, const JniShop *__y) const {
        return __x->avgCost < __y->avgCost;
    }
};

void ShopManager::orderByPrice() {
    quick_sort_inplace(*innerVector, lessPrice());
}

struct _LIBCPP_TEMPLATE_VIS GreaterRating : binary_function<JniShop *, JniShop *, bool> {
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY bool operator()(const JniShop *__x, const JniShop *__y) const {
        return __x->rating > __y->rating;
    }
};

void ShopManager::orderByRating() {
    quick_sort_inplace(*innerVector, GreaterRating());
}

long ShopManager::findByLocation(jint x, jint y, jint distance) {
    std::tuple <jint, jint> location;
    location = make_tuple(x, y);
    vector<JniShop *> *shops = kdtree->Range_Search(location, distance);
    return reinterpret_cast<jlong>(shops);
}

vector<jint> *ShopManager::planningPath(jint a, jint b) {
    return shortestP2pMap->distance(a, b);
}

ShopManager::ShopManager() {
    hashTable = new HashTable<string, JniShop *>();
    innerVector = new vector<JniShop *>();
    shortestP2pMap = new ShortestP2P();
    kdtree = nullptr;
    mapPoints = new vector<vector<jint> *>();
    // 5 * 5 road
    for (jint i = 100; i < 600; i += 100) {
        for (jint j = 100; j < 600; j += 100) {
            auto p = new vector<jint>();
            p->push_back(i);//x
            p->push_back(j);//y
            mapPoints->push_back(p);
        }
    }
    mapEdges = new vector<vector<jint> *>();
    for (jint i = 0; i < 25; ++i) {
        vector<jint> *p;
        if (i < 24 && i % 5 != 4) {
            p = new vector<jint>();
            p->push_back(i);//a
            p->push_back(i + 1);//b
            p->push_back(random() % 80 + 10);//distance
            mapEdges->push_back(p);
        }
        if (i + 5 <= 24) {
            p = new vector<jint>();
            p->push_back(i);//a
            p->push_back(i + 5);//b
            p->push_back(random() % 80 + 10);//distance
            mapEdges->push_back(p);
        }
    }
    shortestP2pMap->setGraph(25, mapEdges);
}

vector<JniShop *> *ShopManager::getInnerVector() {
    return innerVector;
}

vector<vector<jint> *> *ShopManager::getRoads() {
    return mapPoints;
}

vector<vector<jint> *> *ShopManager::getEdges() {
    return mapEdges;
}
//////////////////////////////////////////////////////////////////////////////////////

ShopManager *GetShopManager(JNIEnv *env, jobject thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(clazz, "objId", "J");
    jlong ptr = env->GetLongField(thiz, fid);
    return reinterpret_cast<ShopManager *>(ptr);
}

extern "C" JNIEXPORT jlong
JNICALL
Java_com_example_myapplication_ShopManager_get(JNIEnv *env, jobject thiz, jint i) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    return reinterpret_cast<jlong>(shopManager->get(i));
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ShopManager_finalize(JNIEnv *env, jobject thiz) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    (delete shopManager);
}
extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ShopManager_initialize(JNIEnv *env, jobject thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    jfieldID fid = env->GetFieldID(clazz, "objId", "J");
    jlong ptr = env->GetLongField(thiz, fid);

    if (ptr == NULL) {
        ShopManager *shopManager = new ShopManager();
        ptr = reinterpret_cast<jlong>(shopManager);
        env->SetLongField(thiz, fid, ptr);
    }
}
extern "C" JNIEXPORT jint
JNICALL
Java_com_example_myapplication_ShopManager_shopCount(JNIEnv *env, jobject thiz) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    return shopManager->shopCount();
}
extern "C" JNIEXPORT jlong
JNICALL
Java_com_example_myapplication_ShopManager_planningPath(JNIEnv *env, jobject thiz,
                                                        jint a, jint b) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    auto v = shopManager->planningPath(a, b);
    return reinterpret_cast<jlong>(v);
}
extern "C" JNIEXPORT jlong
JNICALL
Java_com_example_myapplication_ShopManager_findByLocation(JNIEnv *env, jobject thiz,
                                                          jint x, jint y, jint distance) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    return shopManager->findByLocation(x, y, distance);
}
extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ShopManager_orderByRating(JNIEnv *env, jobject thiz) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    shopManager->orderByRating();
}
extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ShopManager_orderByPrice(JNIEnv *env, jobject thiz) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    shopManager->orderByPrice();
}

extern "C" JNIEXPORT jlong
JNICALL
Java_com_example_myapplication_ShopManager_findByName(JNIEnv *env, jobject thiz, jstring name) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    return reinterpret_cast<jlong>(shopManager->findByName(jstring2string(env, name)));
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ShopManager_addShop(JNIEnv *env, jobject thiz, jlong ptr) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    shopManager->addShop(reinterpret_cast<JniShop *>(ptr));
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_example_myapplication_ShopManager_vector(JNIEnv *env, jobject thiz) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    return reinterpret_cast<jlong>(shopManager->getInnerVector());
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_myapplication_ShopManager_getRoads(JNIEnv *env, jobject thiz) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    return reinterpret_cast<jlong>(shopManager->getRoads());
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_myapplication_ShopManager_getEdges(JNIEnv *env, jobject thiz) {
    ShopManager *shopManager = GetShopManager(env, thiz);
    return reinterpret_cast<jlong>(shopManager->getEdges());
}