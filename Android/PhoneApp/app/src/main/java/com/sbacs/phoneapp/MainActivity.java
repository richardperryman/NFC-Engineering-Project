package com.sbacs.phoneapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    private Response.Listener<String> genericResponseListener(final TextView view) {
        return new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                view.setText(response);
            }
        };
    }

    private Response.ErrorListener genericErrorListener(final TextView view) {
        return new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                view.setText(error.getMessage());
            }
        };
    }

    public void pingServer(View view) {
        TextView results = (TextView) findViewById(R.id.result_message);
        RequestQueue queue = Volley.newRequestQueue(this);
        String url = getResources().getText(R.string.sbacs_url).toString();
        StringRequest request = new StringRequest(Request.Method.GET, url, genericResponseListener(results),
                genericErrorListener(results));
        queue.add(request);
    }

    public void getUsers(View view) {
        TextView results = (TextView) findViewById(R.id.result_message);
        RequestQueue queue = Volley.newRequestQueue(this);
        String url = getResources().getText(R.string.sbacs_url).toString() +
                getResources().getText(R.string.user_table_name).toString();
        StringRequest request = new StringRequest(Request.Method.GET, url, genericResponseListener(results),
                genericErrorListener(results));
        queue.add(request);
    }

    public void getLocks(View view) {
        TextView results = (TextView) findViewById(R.id.result_message);
        RequestQueue queue = Volley.newRequestQueue(this);
        String url = getResources().getText(R.string.sbacs_url).toString() +
                getResources().getText(R.string.lock_table_name).toString();
        StringRequest request = new StringRequest(Request.Method.GET, url, genericResponseListener(results),
                genericErrorListener(results));
        queue.add(request);
    }
}
