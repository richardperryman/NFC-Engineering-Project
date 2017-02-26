package com.sbacs.phoneapp;

import android.app.DownloadManager;
import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

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

import java.util.Arrays;

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
        final TextView results = (TextView) findViewById(R.id.result_message);
        final String user_name = ((TextView) findViewById(R.id.user_name)).getText().toString();
        final String password = ((TextView) findViewById(R.id.password)).getText().toString();

        RequestQueue queue = Volley.newRequestQueue(this);
        String url = getResources().getString(R.string.sbacs_url) +
                getResources().getString(R.string.login_endpoint);
        final Context currentContext = this;
        Response.Listener<String> loginResponseListener = new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                int user_id;
                byte[] auth;
                try {
                    JSONObject responseObject = new JSONObject(response);
                    String user = responseObject.getString("user");
                    JSONArray authValues = responseObject.getJSONObject("key").getJSONArray("data");
                    auth = new byte[authValues.length()];
                    for(int i = 0; i < authValues.length(); i++) {
                        auth[i] = (byte) authValues.getInt(i);
                    }
                    //TODO Constants
                    user_id = Integer.parseInt(user);
                } catch (NumberFormatException e) {
                    user_id = -1;
                    auth = new byte[]{};
                } catch (JSONException e) {
                    // TODO better handling
                    user_id = -1;
                    auth = new byte[]{};
                }

                if (user_id > -1 && auth.length > 0) {
                    Intent loginIntent = new Intent(currentContext, UserActivity.class);
                    loginIntent.putExtra(USER_ID, user_id);
                    loginIntent.putExtra(HMAC_AUTH, auth);
                    startActivity(loginIntent);
                } else {
                    results.setText("An error occurred with response:\n" + response);
                }
            }
        };
        StringRequest request = new StringRequest(Request.Method.POST, url, loginResponseListener,
                genericErrorListener(results))
        {
            @Override
            public byte[] getBody() {
                return HMACHelper.HMACLoginBody(user_name, password);
            }
        };
        queue.add(request);
    }
}
