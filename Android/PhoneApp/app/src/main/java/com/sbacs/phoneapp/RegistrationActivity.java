package com.sbacs.phoneapp;

import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ListView;

import com.android.volley.AuthFailureError;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.sbacs.phoneapp.HMAC.HMACHelper;
import com.sbacs.phoneapp.Model.Lock;
import com.sbacs.phoneapp.Model.Registration;
import com.sbacs.phoneapp.View.LockAdapter;
import com.sbacs.phoneapp.View.RegistrationAdapter;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class RegistrationActivity extends AppCompatActivity {

    private int user_id;
    private String auth;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registration);

        user_id = getIntent().getIntExtra(UserActivity.USER_ID, -1);
        auth = new String(getIntent().getByteArrayExtra(UserActivity.HMAC_AUTH));

        final ListView view = (ListView) findViewById(R.id.registration_list);
        List<Registration> registrationList = new ArrayList<>();
        view.setAdapter(new RegistrationAdapter(this, registrationList));

        RequestQueue queue = Volley.newRequestQueue(this);
        String url = getResources().getString(R.string.sbacs_url) +
                getResources().getString(R.string.registration_table_name) +
                "?user_id=" + user_id;
        StringRequest request = new StringRequest(url, registrationResponseListener(),
                genericErrorListener())
        {
            @Override
            public Map<String, String> getHeaders() throws AuthFailureError {
                return HMACHelper.GetHMACHeaders(user_id, "", auth);
            }
        };
        queue.add(request);

        // Clear any pending notifications about new registrations
        NotificationManager mNotificationManager =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        mNotificationManager.cancel(0);


    }

    public void back(View view) {
        Intent intent = new Intent(this, UserActivity.class);
        intent.putExtra(MainActivity.USER_ID, user_id);
        intent.putExtra(MainActivity.HMAC_AUTH, auth.getBytes());
        startActivity(intent);
    }

    private Response.Listener<String> registrationResponseListener() {
        return new Response.Listener<String>() {
            @Override
            public void onResponse (String response) {
                JSONArray registrations;
                try {
                    registrations = new JSONArray(response);
                } catch (JSONException e) {
                    return;
                }

                ListView view = (ListView) findViewById(R.id.registration_list);
                final RegistrationAdapter adapter = (RegistrationAdapter) view.getAdapter();

                for (int i = 0; i < registrations.length(); i++) {
                    Registration registration;
                    try {
                        JSONObject reg = registrations.getJSONObject(i);
                        registration = new Registration(reg);
                    } catch (JSONException e) {
                        continue;
                    }
                    adapter.add(registration);
                }
            }
        };
    }

    private Response.ErrorListener genericErrorListener() {
        return new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                int foo = 5;
            }
        };
    }
}
