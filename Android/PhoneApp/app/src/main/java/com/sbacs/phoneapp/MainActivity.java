package com.sbacs.phoneapp;

import android.content.Context;
import android.content.Intent;
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
    public final static String USER_ID = "com.SBACS.app.USER_ID";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    private Response.ErrorListener genericErrorListener(final TextView view) {
        return new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                view.setText(error.getMessage());
            }
        };
    }

    public void login(View view) {
        TextView results = (TextView) findViewById(R.id.result_message);
        String user_name = ((TextView) findViewById(R.id.user_name)).getText().toString();

        RequestQueue queue = Volley.newRequestQueue(this);
        String url = getResources().getString(R.string.sbacs_url) +
                getResources().getString(R.string.user_table_name) +
                "?name=" + user_name;
        final Context currentContext = this;
        Response.Listener<String> loginResponseListener = new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                int user_id;
                try {
                    // This probably shouldn't be hardcoded!
                    user_id = Integer.parseInt(response.split("\\s+")[0]);
                } catch (NumberFormatException e) {
                    user_id = -1;
                }

                Intent loginIntent = new Intent(currentContext, UserActivity.class);
                loginIntent.putExtra(USER_ID, user_id);
                startActivity(loginIntent);
            }
        };
        StringRequest request = new StringRequest(Request.Method.GET, url, loginResponseListener,
                genericErrorListener(results));
        queue.add(request);
    }
}
