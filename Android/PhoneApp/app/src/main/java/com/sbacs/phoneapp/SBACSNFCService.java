package com.sbacs.phoneapp;

import android.content.Intent;
import android.nfc.cardemulation.HostApduService;
import android.os.Bundle;
import android.support.v4.content.LocalBroadcastManager;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class SBACSNFCService extends HostApduService {
    public final static String SBACS_NOTIFICATION = "com.SBACS.service.NOTIFICATION";
    public final static String SBACS_MESSAGE = "com.SBACS.service.MESSAGE";

    private byte[] data_message;
    private int bytes_sent = 0;

    private LocalBroadcastManager broadcaster;

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        data_message = intent.getByteArrayExtra(UserActivity.USER_NFC_AUTH);

        return START_REDELIVER_INTENT;
    }

    @Override
    public void onCreate() {
        // Can be used to send data to the app
        broadcaster = LocalBroadcastManager.getInstance(this);
    }

    @Override
    public byte[] processCommandApdu(byte[] apdu, Bundle extras) {
        // If this becomes expensive use another thread and use sendResponseApdu()

        Intent notifyIntent = new Intent(SBACS_NOTIFICATION);
        notifyIntent.putExtra(SBACS_MESSAGE, String.valueOf(bytes_sent));
        broadcaster.sendBroadcast(notifyIntent);

        int length = apdu[apdu.length - 1];
        if (length == 1) {
            bytes_sent = 0;
            return new byte[] {0x00};
        } else if (length < 0) {
            // Casting to int does sign extension, quick fix is to add 256 since we want positive values
            length = 256 + length;
        }

        byte[] result = Arrays.copyOfRange(data_message, Math.min(bytes_sent, data_message.length),
                Math.min(bytes_sent + length, data_message.length));
        bytes_sent += result.length;

        return result;
    }

    @Override
    public void onDeactivated(int reason) {
        bytes_sent = 0;
    }

    private void sendDebugString(String message) {
        Intent notifyIntent = new Intent(SBACS_NOTIFICATION);
        notifyIntent.putExtra(SBACS_MESSAGE, message);
        broadcaster.sendBroadcast(notifyIntent);
    }
}