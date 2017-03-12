package com.sbacs.phoneapp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

public class UserActivity extends AppCompatActivity {
    public final static String USER_ID = "com.SBACS.app.USER_ID";
    public final static String HMAC_AUTH = "com.SBACS.app.HMAC_AUTH";
    public final static String USER_NFC_AUTH = "com.SBACS.app.NFC_AUTHENTICATOR";

    private int user_id;
    private byte[] auth;

    private BroadcastReceiver receiver;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user);

        Intent intent = getIntent();
        user_id = intent.getIntExtra(MainActivity.USER_ID, -1);
        auth = intent.getByteArrayExtra(MainActivity.HMAC_AUTH);

        // Doesn't fail if service isn't running (returns false)
        stopService(new Intent(this, SBACSNFCService.class));
        stopService(new Intent(this, NotificationService.class));

        Intent serviceIntent = new Intent(this, SBACSNFCService.class);
        serviceIntent.putExtra(UserActivity.USER_NFC_AUTH, auth);
        startService(serviceIntent);

        Intent notificationIntent = new Intent(this, NotificationService.class);
        notificationIntent.putExtra(UserActivity.USER_NFC_AUTH, auth);
        notificationIntent.putExtra(UserActivity.USER_ID, user_id);
        startService(notificationIntent);

        final ListView view = (ListView) findViewById(R.id.list_results);
        List<String> resultsList = new ArrayList<>();
        view.setAdapter(new ArrayAdapter<>(this, R.layout.adapter_text_view, resultsList));

        receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                ((ArrayAdapter<String>)view.getAdapter()).add(intent.getStringExtra(SBACSNFCService.SBACS_MESSAGE));
            }
        };
    }

    @Override
    protected void onStart() {
        super.onStart();
        LocalBroadcastManager.getInstance(this).registerReceiver(receiver, new IntentFilter(SBACSNFCService.SBACS_NOTIFICATION));
    }

    public void logout(View view) {
        stopService(new Intent(this, SBACSNFCService.class));
        stopService(new Intent(this, NotificationService.class));

        Intent logoutIntent = new Intent(this, MainActivity.class);
        startActivity(logoutIntent);
    }

    public void manageLocks(View view) {
        Intent lockIntent = new Intent(this, LockActivity.class);
        lockIntent.putExtra(USER_ID, user_id);
        lockIntent.putExtra(HMAC_AUTH, auth);
        startActivity(lockIntent);
    }

    public void manageRegistrations(View view) {
        Intent registrationIntent = new Intent(this, RegistrationActivity.class);
        registrationIntent.putExtra(USER_ID, user_id);
        registrationIntent.putExtra(HMAC_AUTH, auth);
        startActivity(registrationIntent);
    }
}
