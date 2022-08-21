package com.example.myapplication;

public
class Shop {
    public static native long newShop(int x, int y, String shopName, int rating, float avgCost);

    public static native String getName(long objId);

    public static native int getX(long objId);

    public static native int getY(long objId);

    public static native int getRating(long objId);

    public static native int getAvgCost(long objId);
}
