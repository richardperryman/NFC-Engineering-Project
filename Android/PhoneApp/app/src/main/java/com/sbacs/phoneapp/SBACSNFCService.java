package com.sbacs.phoneapp;

import android.nfc.cardemulation.HostApduService;
import android.os.Bundle;
import java.util.Arrays;

public class SBACSNFCService extends HostApduService {
    private final static byte[] data_message = "Hi Jess!".getBytes();
    private final static byte[] success_message = new byte[]{0};
    private final static byte sentinel_byte = 0x00;
    @Override
    public byte[] processCommandApdu(byte[] apdu, Bundle extras) {
        // If this becomes expensive use another thread and use sendResponseApdu()
        if (apdu.length == 0) {
            return data_message;
        } else if (apdu[0] == sentinel_byte) {
            byte[] check = Arrays.copyOfRange(apdu, 1, data_message.length);
            if (check.equals(data_message)) {
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
