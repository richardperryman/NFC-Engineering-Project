package com.sbacs.phoneapp;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.Volley;

public class MainActivity extends AppCompatActivity {
    public final static String USER_ID = "com.SBACS.app.USER_ID";
    public final static String HMAC_AUTH = "com.SBACS.app.HMAC_AUTH";

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
        Response.Listener<JSONObject> loginResponseListener = new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                int user_id;
                String auth = "";
                try {
                    String user = response.getString("user");
                    auth = response.getString("key");
                    //TODO Constants
                    user_id = Integer.parseInt(user);
                } catch (NumberFormatException e) {
                    user_id = -1;
                } catch (JSONException e) {
                    // TODO better handling
                    user_id = -1;
                }

                Intent loginIntent = new Intent(currentContext, UserActivity.class);
                loginIntent.putExtra(USER_ID, user_id);
                loginIntent.putExtra(HMAC_AUTH, auth);
                startActivity(loginIntent);
            }
        };
        JsonObjectRequest  request = new JsonObjectRequest (Request.Method.GET, url, loginResponseListener,
                genericErrorListener(results));
        queue.add(request);
    }
}
