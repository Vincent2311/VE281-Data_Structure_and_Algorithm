package com.example.myapplication.ui.main;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.myapplication.ShopManager;
import com.example.myapplication.databinding.FragmentShopListBinding;

import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;

/**
 * determines the content of the shop list in LIST page
 *
 */
public class ShopFragment extends Fragment {
    private MyShopAdapter adapter;
    private ShopManager shopManager;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        shopManager = ShopManager.getInstance();
        adapter = new MyShopAdapter(shopManager);

        FragmentShopListBinding binding = FragmentShopListBinding.inflate(getLayoutInflater());
        binding.list.setLayoutManager(new LinearLayoutManager(getContext()));
        binding.list.setAdapter(adapter);

        // Sort the shops by Avg Cost
        binding.listOrderByAvgCost.setOnClickListener(view -> {
            shopManager.orderByPrice();
            adapter.notifyDataSetChanged();
        });

        // Sort the shops by Rating
        binding.listOrderByRating.setOnClickListener(view -> {
            shopManager.orderByRating();
            adapter.notifyDataSetChanged();
        });

        return binding.getRoot();
    }
}