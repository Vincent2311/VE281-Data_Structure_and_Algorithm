package com.example.myapplication.ui.main;

import android.content.Context;

import com.example.myapplication.R;
import com.example.myapplication.ShopManager;

import androidx.annotation.Nullable;
import androidx.annotation.StringRes;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentPagerAdapter;

/**
 * A [FragmentPagerAdapter] that returns a fragment corresponding to one of the sections
 *
 */
public class SectionsPagerAdapter extends FragmentPagerAdapter {

    @StringRes
    private static final int[] TAB_TITLES = new int[]{R.string.tab_text_shops, R.string.tab_text_find, R.string.tab_text_map};
    private final Context mContext;

    public SectionsPagerAdapter(Context context, FragmentManager fm) {
        super(fm);
        mContext = context;
    }

    @Override
    public Fragment getItem(int position) {
        switch (position) {
            // enters LIST page
            case 0:
                return new ShopFragment();
            // enters FIND page
            case 1:
                return new FindFragment();
            // enters MAP page either when you click the third button at the top or in the default case
            case 2:
            default:
                return new MapFragment(ShopManager.getInstance());
        }
    }

    @Nullable
    @Override
    public CharSequence getPageTitle(int position) {
        return mContext.getResources().getString(TAB_TITLES[position]);
    }

    @Override
    public int getCount() {
        return TAB_TITLES.length;
    }
}