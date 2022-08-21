package com.example.myapplication;

/**
 * An object that reads an item from a C++ vector. The back end is associated with a C++ vector of type jint
 */
public class CVector {
    /**
     * Pointer to the C++ object corresponding to the current object
     */
    private long objId;

    /**
     * create a new object in java.and bind to a new c++ backend object;
     */
    public CVector() {
        initialize();
    }

    /**
     * create a new object in java.and bind to a given c++ backend object;
     */
    public CVector(long objId) {
        this.objId = objId;
        initialize();
    }

    /**
     * create a new  object in java.and bind to a new c++ backend object;
     */
    protected native void initialize();

    /**
     * release the memory used by this object in c++
     */
    public native void finalize();

    /**
     * get the count of shop in this manager
     */
    public native int size();

    /**
     * Gets the item at the specified index in the list
     */
    public native long get(int i);

    /**
     * Gets the item at the specified index in the list,treat it as an integer
     */
    public native int getInt(int i);

    /**
     * Gets the item at the specified index in the list,treat it as an integer
     */
    public static native int intAt(long ptr, int i);

    public long getObjId() {
        return objId;
    }

}

