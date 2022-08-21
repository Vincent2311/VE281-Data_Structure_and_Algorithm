package com.example.myapplication;

import android.widget.ArrayAdapter;
import android.widget.Spinner;

import java.util.List;
import java.util.Random;
import java.util.ArrayList;

public class ShopManager {
    private static ShopManager instance;

    static {
        System.loadLibrary("myapplication");
        instance = new ShopManager();
//        final List<String> list = new ArrayList<String>();

        // initialize the shops with randomly generated integers
        Random random = new Random();
        for (int i = 100; i < 500; i += 100) {
            for (int j = 100; j < 500; j += 100) {
                int x = i + random.nextInt(100);
                int y = j + random.nextInt(100);
                instance.addShop(Shop.newShop(x, y, "test" + x + "," + y, random.nextInt(10), random.nextInt(1000)));
                //list.add("test" + x + "," + y);
            }
      }
//        ArrayAdapter<String> adp1 = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1,list);
//        adp1.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
//        Spinner sItems = (Spinner) findViewById(R.id.spinner1);
//        sItems.setAdapter(adp1);
    }

    public static ShopManager getInstance() {
        return instance;
    }

    /**
     * pointer to the C++ object corresponding to the current object
     */
    private long objId;

    /**
     * create a new object in java.and bind to a new c++ backend object;
     */
    public ShopManager() {
        initialize();
    }

    /**
     * create a new object in java.and bind to a given c++ backend object;
     */
    public ShopManager(int objId) {
        this.objId = objId;
        initialize();
    }

    /**
     * add a shop to this manager
     */
    public native void addShop(long ptr);

    /**
     * create a new  object in java.and bind to a new c++ backend object;
     */
    protected native void initialize();

    /**
     * release the memory used by this object in c++
     */
    protected native void finalize();

    /**
     * get the count of shop in this manager,The order of the shops in the ShopManager can be changed by calling the orderByPrice and orderByRating methods
     */
    public native int shopCount();

    /**
     * get the item at the specified index in the list,The order of the shops in the ShopManager can be changed by calling the orderByPrice and orderByRating methods
     */
    public native long get(int i);

    /**
     * find shop by shopName
     *
     * @return return a pointer to the find shop,may be null
     */
    public native long findByName(String name);

    /**
     * order shops by price,This will adjust the current vector
     */
    public native void orderByPrice();

    /**
     * order shops by rating,This will adjust the current vector
     */
    public native void orderByRating();

    /**
     * find shop nearby the given location
     */
    public native long findByLocation(int x, int y, int distance);

    /**
     * plan a path from a given location to a given store
     */
    public native long planningPath(int a, int b);

    /**
     * get a pointer to a vector whose shops are in the order they were originally added to
     */
    public native long vector();

    /**
     * get all points on load
     * inner vector : [x,y]
     */
    public native long getRoads();

    /**
     * get all edges in map
     * inner vector : [pointA,pointB,distance]
     */
    public native long getEdges();

    public CVector getRoadsV() {
        return new CVector(getRoads());
    }

    public CVector getEdgesV() {
        return new CVector(getEdges());
    }

}
