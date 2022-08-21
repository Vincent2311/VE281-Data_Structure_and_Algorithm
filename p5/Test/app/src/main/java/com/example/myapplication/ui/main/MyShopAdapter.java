package com.example.myapplication.ui.main;

import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.widget.TextView;

import com.example.myapplication.Shop;
import com.example.myapplication.ShopManager;
import com.example.myapplication.databinding.FragmentShopBinding;

import androidx.recyclerview.widget.RecyclerView;

/**
 *
 * determines the layout of the LIST page
 *
 */
public class MyShopAdapter extends RecyclerView.Adapter<MyShopAdapter.ViewHolder> {

    private ShopManager mValues;
    static int rating = 0;
    static int cost = 0;
    static int count = 0;

    public MyShopAdapter(ShopManager values) {
        mValues = values;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return new ViewHolder(FragmentShopBinding.inflate(LayoutInflater.from(parent.getContext()), parent, false));
    }

    @Override
    public void onBindViewHolder(final ViewHolder holder, int position) {
        // set the column name to be "Shop", "Location", "Avg Cost" and "Rating"
        if (position == 0) {
            holder.shopNameInList.setText("Shop");
            holder.locInList.setText("Location");
            holder.ratingInList.setText("Rating");
            holder.avgCostInList.setText("Avg Cost");
        }
        // list the factors of each shop corresponds to the column name
        else {
            long ptr = mValues.get(position - 1);
            holder.ptr = ptr;
            holder.shopNameInList.setText(Shop.getName(ptr));
            holder.locInList.setText(String.format("(%d,%d)", Shop.getX(ptr), Shop.getY(ptr)));
            int rate = Shop.getRating(ptr);
            holder.ratingInList.setText(rate + "");
            rating = rating + rate;
            int cst = Shop.getAvgCost(ptr);
            holder.avgCostInList.setText(cst + "");
            cost = cost + cst;
            count += 1;
        }
    }


    @Override
    public int getItemCount() {
        return mValues.shopCount() + 1;
    }

    public class ViewHolder extends RecyclerView.ViewHolder {
        public final TextView shopNameInList;
        public final TextView locInList;
        public final TextView ratingInList;
        public final TextView avgCostInList;
        public long ptr;

        public ViewHolder(FragmentShopBinding binding) {
            super(binding.getRoot());
            shopNameInList = binding.shopNameInList;
            locInList = binding.locInList;
            ratingInList = binding.ratingInList;
            avgCostInList = binding.avgCostInList;
        }
    }
}