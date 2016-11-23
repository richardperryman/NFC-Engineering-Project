package com.sbacs.phoneapp;

import android.nfc.cardemulation.HostApduService;
import android.os.Bundle;

public class SBACSNFCService extends HostApduService {
    @Override
    public byte[] processCommandApdu(byte[] apdu, Bundle extras) {
        // If this becomes expensive use another thread and use sendResponseApdu()
        return "Hi Jess!".getBytes();
    }
    @Override
    public void onDeactivated(int reason) {
        // Clean up?
    }
}
