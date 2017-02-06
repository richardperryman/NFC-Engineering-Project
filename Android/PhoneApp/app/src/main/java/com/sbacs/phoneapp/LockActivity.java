package com.sbacs.phoneapp;

import android.content.Context;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import com.sbacs.phoneapp.HMAC.HMACHelper;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class LockActivity extends AppCompatActivity {

    private int user_id;
    private String auth;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lock);

        //Toolbar toolbar = (Toolbar) findViewById(R.id.lock_toolbar);
        //setSupportActionBar(toolbar);

        user_id = getIntent().getIntExtra(UserActivity.USER_ID, -1);
        auth = getIntent().getStringExtra(UserActivity.HMAC_AUTH);

        final ListView view = (ListView) findViewById(R.id.lock_list);
        List<Lock> locksList = new ArrayList<>();
        view.setAdapter(new LockAdapter(this, locksList));

        RequestQueue queue = Volley.newRequestQueue(this);
        String url = getResources().getString(R.string.sbacs_url) +
                getResources().getString(R.string.registration_table_name) +
                "?user_id=" + user_id;
        JsonArrayRequest request = new JsonArrayRequest(url, registrationResponseListener(),
                genericErrorListener())
                {
                    @Override
                    public Map<String, String> getHeaders() throws AuthFailureError {
                        return HMACHelper.GetHMACHeaders(user_id, "", auth);
                    }
                };
        queue.add(request);

    }

    private Response.Listener<JSONArray> registrationResponseListener() {
        final Context currentContext = this;
        ListView view = (ListView) findViewById(R.id.lock_list);
        final LockAdapter adapter = (LockAdapter) view.getAdapter();
        return new Response.Listener<JSONArray>() {
            @Override
            public void onResponse(JSONArray response) {
                adapter.clear();
                if (response == null || response.length() < 1) {
                    // Todo better handling
                    return;
                }
                RequestQueue queue = Volley.newRequestQueue(currentContext);
                Response.Listener<JSONArray> lockResponseListener = new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        if (response.length() < 1) {
                            // Todo better error logging
                            return;
                        }
                        try {
                            JSONObject result = response.getJSONObject(0);
                            int id = result.getInt("Lock_Id");
                            String name = result.getString("Name");
                            String owner = result.getString("BelongsTo");
                            Lock lock = new Lock(id, name, owner);
                            adapter.add(lock);
                        } catch (JSONException e) {
                            // Todo better error handling
                        }
                    }
                };

                for (int i=0; i<response.length(); i++) {
                    JSONObject registration;
                    try {
                        registration = response.getJSONObject(i);
                    } catch(JSONException e) {
                        continue;
                    }
                    int lock_id;
                    try {
                        // Probably shouldn't be hardcoded
                        lock_id = registration.getInt("Lock_Id");
                    } catch (NumberFormatException e) {
                        // Should probably handle this better
                        continue;
                    } catch (JSONException e) {
                        continue;
                    }
                    String url = getResources().getString(R.string.sbacs_url) +
                            getResources().getString(R.string.lock_table_name) +
                            "?lock_id=" + lock_id;
                    JsonArrayRequest request = new JsonArrayRequest(url, lockResponseListener,
                            genericErrorListener()){
                                @Override
                                public Map<String, String> getHeaders() throws AuthFailureError {
                                    return HMACHelper.GetHMACHeaders(user_id, "", auth);
                                }
                            };
                    queue.add(request);
                }
            }
        };
    }

    private Response.ErrorListener genericErrorListener() {
        return new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
            }
        };
    }

}
