package com.sbacs.phoneapp;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.app.TaskStackBuilder;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.support.v4.app.NotificationCompat;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.sbacs.phoneapp.HMAC.HMACHelper;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by Richard on 2017-03-10.
 */

public class NotificationService extends Service {

    private int user_id;
    private String auth;

    private Timer pollTimer;
    private RequestQueue pollQueue;

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        user_id = intent.getIntExtra(UserActivity.USER_ID, -1);
        auth = new String(intent.getByteArrayExtra(UserActivity.USER_NFC_AUTH));

        return START_REDELIVER_INTENT;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        pollQueue = Volley.newRequestQueue(this);
        pollTimer = new Timer(true);
        pollTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                String url = getResources().getString(R.string.sbacs_url) +
                        getResources().getString(R.string.notification_endpoint) + "?user_id=" + user_id;
                final StringRequest request = new StringRequest(Request.Method.GET, url, notificationListener(),
                        errorListener()){
                    @Override
                    public Map<String, String> getHeaders() throws AuthFailureError {
                        return HMACHelper.GetHMACHeaders(user_id, "", auth);
                    }
                };
                pollQueue.add(request);
            }
        }, 5000, 5000);
    }

    private Response.Listener<String> notificationListener() {
        return new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                JSONArray notifications;
                try {
                    notifications = new JSONArray(response);
                } catch (Exception e) {
                    return;
                }
                generateNotification(notifications);
            }
        };
    }

    private Response.ErrorListener errorListener() {
        return new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
            }
        };
    }

    private void generateNotification(JSONArray notifications) {
        NotificationCompat.Builder builder;
        if (notifications.length() < 1) {
            return;
        } else if (notifications.length() == 1) {
            try {
                JSONObject notification = notifications.getJSONObject(0);

                builder = new NotificationCompat.Builder(this)
                        .setSmallIcon(R.mipmap.ic_launcher)
                        .setContentTitle("SBACS update")
                        .setContentText("New registration with " +
                                notification.getString("BelongsTo"));
            } catch (JSONException e) {
                return;
            }
        } else {
            builder = new NotificationCompat.Builder(this)
                    .setSmallIcon(R.mipmap.ic_launcher)
                    .setContentTitle("SBACS update")
                    .setContentText("New registrations available");
        }
        builder.setAutoCancel(true);

        Intent intent = new Intent(this, RegistrationActivity.class);
        intent.putExtra(UserActivity.USER_ID, user_id);
        intent.putExtra(UserActivity.HMAC_AUTH, auth.getBytes());
        TaskStackBuilder stackBuilder = TaskStackBuilder.create(this);
        stackBuilder.addParentStack(MainActivity.class);
        stackBuilder.addNextIntent(intent);
        PendingIntent resultPendingIntent =
                stackBuilder.getPendingIntent(
                        0,
                        PendingIntent.FLAG_UPDATE_CURRENT
                );
        builder.setContentIntent(resultPendingIntent);
        NotificationManager mNotificationManager =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        mNotificationManager.notify(0, builder.build());
    }

}
