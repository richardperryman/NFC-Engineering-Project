package com.sbacs.phoneapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.ViewGroup;
import android.widget.TextView;

public class UserActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user);

        Intent intent = getIntent();
        TextView tempView = new TextView(this);
        tempView.setText(intent.getStringExtra(MainActivity.EXTRA_MESSAGE));
        ((ViewGroup) findViewById(R.id.activity_user)).addView(tempView);
    }
}
