package com.sbacs.phoneapp;

import android.app.ActivityManager;
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

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

public class UserActivity extends AppCompatActivity {
    public final static String USER_NFC_AUTH = "com.SBACS.app.NFC_AUTHENTICATOR";

    private int user_id;

    private BroadcastReceiver receiver;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user);

        Intent intent = getIntent();
        user_id = intent.getIntExtra(MainActivity.USER_ID, -1);

        RequestQueue queue = Volley.newRequestQueue(this);
        // HORRIBLY WRONG
        String url = getResources().getString(R.string.sbacs_url) +
                getResources().getString(R.string.auth_table_name) +
                "?user_id=" + user_id;

        JsonArrayRequest request = new JsonArrayRequest(url, serviceResponseListener(),
                genericErrorListener());
        queue.add(request);

        final ListView view = (ListView) findViewById(R.id.list_results);
        List<String> resultsList = new ArrayList<>();
        resultsList.add("Nothing to see here");
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

    private Response.ErrorListener genericErrorListener() {
        return new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                // This is pretty bad because of asynchronicity of other calls
                ListView view = (ListView) findViewById(R.id.list_results);
                ArrayAdapter<String> adapter = (ArrayAdapter<String>) view.getAdapter();
                adapter.clear();
                adapter.add(error.getMessage());
            }
        };
    }

    private Response.Listener<String> registrationResponseListener() {
        final Context currentContext = this;
        ListView view = (ListView) findViewById(R.id.list_results);
        final ArrayAdapter<String> adapter = (ArrayAdapter<String>) view.getAdapter();
        return new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                adapter.clear();
                if (response == null || response.equals("")) {
                    adapter.add("You don't have any locks");
                    return;
                }
                String[] registrations = response.split("\n");
                RequestQueue queue = Volley.newRequestQueue(currentContext);
                Response.Listener<String> lockResponseListener = new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        adapter.add(response);
                    }
                };

                for (String registration : registrations) {
                    int lock_id;
                    try {
                        // Probably shouldn't be hardcoded
                        lock_id = Integer.parseInt(registration.split("\\s+")[1]);
                    } catch (NumberFormatException e) {
                        // Should probably handle this better
                        continue;
                    }
                    String url = getResources().getString(R.string.sbacs_url) +
                            getResources().getString(R.string.lock_table_name) +
                            "?lock_id=" + lock_id;
                    StringRequest request = new StringRequest(Request.Method.GET, url, lockResponseListener,
                            genericErrorListener());
                    queue.add(request);
                }
            }
        };
    }

    private Response.Listener<JSONArray> serviceResponseListener() {
        final Context currentContext = this;
        return new Response.Listener<JSONArray>() {
            @Override
            public void onResponse(JSONArray response) {
                ListView view = (ListView) findViewById(R.id.list_results);
                ArrayAdapter<String> adapter = (ArrayAdapter<String>) view.getAdapter();
                // This currently ties the service to this window being open... this should probably not be the case
                Intent serviceIntent = new Intent(currentContext, SBACSNFCService.class);
                for (int i=0; i<response.length(); i++) {
                    try {
                        JSONObject auth = response.getJSONObject(i);
                        // TODO move these things to constants
                        if ("password".equalsIgnoreCase((String) auth.get("AuthType"))) {
                            String key = (String) auth.get("AuthKey");
                            serviceIntent.putExtra(UserActivity.USER_NFC_AUTH, key);
                            currentContext.startService(serviceIntent);
                            break;

                        }
                    } catch (JSONException e) {
                        // TODO better error handling
                    }
                }

            }
        };
    }

    public void getLocks(View view) {
        RequestQueue queue = Volley.newRequestQueue(this);
        String url = getResources().getString(R.string.sbacs_url) +
                getResources().getString(R.string.registration_table_name) +
                "?user_id=" + user_id;
        StringRequest request = new StringRequest(Request.Method.GET, url, registrationResponseListener(),
                genericErrorListener());
        queue.add(request);
    }
}
