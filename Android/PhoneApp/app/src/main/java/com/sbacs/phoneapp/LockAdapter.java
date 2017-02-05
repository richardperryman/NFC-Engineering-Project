package com.sbacs.phoneapp;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Richard on 2017-01-23.
 */

public class LockAdapter extends ArrayAdapter<Lock> {

    public LockAdapter(Context context, List<Lock> users) {
        super(context, 0, users);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        Lock lock = getItem(position);

        if (convertView == null) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.adapter_lock_view, parent, false);
        }

        TextView nameView = (TextView) convertView.findViewById(R.id.lock_name);
        TextView ownerView = (TextView) convertView.findViewById(R.id.lock_owner);

        nameView.setText(lock.getName());
        ownerView.setText("From: " + lock.getOwner());

        return convertView;
    }

}
