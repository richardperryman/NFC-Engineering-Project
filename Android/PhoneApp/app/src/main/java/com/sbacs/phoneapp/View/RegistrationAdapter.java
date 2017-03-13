package com.sbacs.phoneapp.View;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.sbacs.phoneapp.Model.Registration;
import com.sbacs.phoneapp.R;

import java.util.List;

/**
 * Created by Richard on 2017-03-11.
 */

public class RegistrationAdapter extends ArrayAdapter<Registration> {
    public RegistrationAdapter(Context context, List<Registration> registrations) {
        super(context, 0, registrations);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        Registration reg = getItem(position);

        if (convertView == null) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.adapter_registration_view, parent, false);
        }

        TextView identityView = (TextView) convertView.findViewById(R.id.identity_name);
        TextView lockView = (TextView) convertView.findViewById(R.id.lock_name_and_owner);

        identityView .setText(reg.getIdentityName());
        lockView.setText(reg.getLockName());

        return convertView;
    }
}
