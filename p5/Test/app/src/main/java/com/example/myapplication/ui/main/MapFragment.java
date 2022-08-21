package com.example.myapplication.ui.main;

import android.graphics.Point;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import com.example.myapplication.CVector;
import com.example.myapplication.Shop;
import com.example.myapplication.ShopManager;
import com.example.myapplication.databinding.FragmentMapBinding;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

/**
 *
 * determines the functionality of buttons in the MAP page, interacts with the user
 * calls the functions in MyMapRender.java to show the shops or draw the paths
 *
 */
public class MapFragment extends Fragment {
    private ShopManager shopManager;
    FragmentMapBinding binding;
    Point myLocation = null;

    public MapFragment(ShopManager shopManager) {
        this.shopManager = shopManager;
    }

    private void releaseIfNeeded() {
        if (binding.myMap.getShops().getObjId() == shopManager.vector()) return;
        binding.myMap.getShops().finalize();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        binding = FragmentMapBinding.inflate(getLayoutInflater());
        binding.myMap.setShops(new CVector(ShopManager.getInstance().vector()));
        binding.myMap.setMapEdges(shopManager.getEdgesV());
        binding.myMap.setMapPoints(shopManager.getRoadsV());

        binding.mapResetMap.setOnClickListener(view -> {
            releaseIfNeeded();
            binding.myMap.setShops(new CVector(ShopManager.getInstance().vector()));

        });
        binding.mapResetCenter.setOnClickListener(view -> binding.myMap.setOffset(0, 0));
        binding.mapShowCenter.setOnClickListener(view -> binding.myMap.setShowCenter(!binding.myMap.isShowCenter()));
        binding.mapAsStart.setOnClickListener(view -> myLocation = binding.myMap.getCenter());
        binding.mapFindNeighbors.setOnClickListener(view -> {
            if (myLocation == null) {
                Toast.makeText(getContext(), "Must pick one point by button pick center", Toast.LENGTH_LONG).show();
                return;
            }
            releaseIfNeeded();
            int distance = Integer.parseInt(binding.mapFindDistance.getText().toString());
            binding.myMap.setShops(new CVector(shopManager.findByLocation(myLocation.x, myLocation.y, distance)));
        });

        binding.mapShortestP2p.setOnClickListener(view -> {
            if (myLocation == null) {
                Toast.makeText(getContext(), "Must pick one point by button pick center", Toast.LENGTH_LONG).show();
                return;
            }
            String shopName = binding.mapShopName.getText().toString();
            long shop = shopManager.findByName(shopName);
            if (shop == 0) {
                Toast.makeText(getContext(), String.format("no shop named %s!", shopName), Toast.LENGTH_LONG).show();
                return;
            }
            //find the most nearby neighbor
            CVector path = new CVector(shopManager.planningPath(findMapPoint(myLocation.x, myLocation.y), findMapPoint(Shop.getX(shop), Shop.getY(shop))));
            binding.myMap.setPath(path);
        });
        return binding.getRoot();
    }

    private int findMapPoint(int x, int y) {
        CVector vector = shopManager.getRoadsV();
        int minDistance = Integer.MAX_VALUE;
        int minDistanceIndex = -1;
        for (int i = 0; i < vector.size(); i++) {
            long ptr = vector.get(i);
            int xi = CVector.intAt(ptr, 0);
            int yi = CVector.intAt(ptr, 1);
            int distance = Math.abs(x - xi) + Math.abs(y - yi);
            if (distance < minDistance) {
                minDistance = distance;
                minDistanceIndex = i;
            }
        }
        return minDistanceIndex;
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }
}