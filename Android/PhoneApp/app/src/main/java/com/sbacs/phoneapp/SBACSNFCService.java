package com.sbacs.phoneapp;

import android.content.Intent;
import android.nfc.cardemulation.HostApduService;
import android.os.Bundle;
import android.support.v4.content.LocalBroadcastManager;

import java.util.Arrays;

public class SBACSNFCService extends HostApduService {
    public final static String SBACS_NOTIFICATION = "com.SBACS.service.NOTIFICATION";
    public final static String SBACS_MESSAGE = "com.SBACS.service.MESSAGE";

    private final static byte[] data_message = "Hi Jess!".getBytes();
    private final static byte[] success_message = new byte[]{0};
    private final static byte sentinel_byte = 0x00;

    private LocalBroadcastManager broadcaster;

    @Override
    public void onCreate() {
        broadcaster = LocalBroadcastManager.getInstance(this);
        // Just for testing
        Intent notifyIntent = new Intent(SBACS_NOTIFICATION);
        notifyIntent.putExtra(SBACS_MESSAGE, "Test message");
        broadcaster.sendBroadcast(notifyIntent);
    }

    @Override
    public byte[] processCommandApdu(byte[] apdu, Bundle extras) {
        // If this becomes expensive use another thread and use sendResponseApdu()

        Intent notifyIntent = new Intent(SBACS_NOTIFICATION);
        notifyIntent.putExtra(SBACS_MESSAGE, new String(apdu));
        broadcaster.sendBroadcast(notifyIntent);

        if (apdu.length == 0) {
            return data_message;
        } else if (apdu[0] == sentinel_byte) {
            byte[] check = Arrays.copyOfRange(apdu, 1, data_message.length);
            if (Arrays.equals(check, data_message)) {
                return success_message;
            } else {
                return data_message;
            }
        } else {
            return data_message;
        }
    }
    @Override
    public void onDeactivated(int reason) {
        // Clean up?
    }
}
