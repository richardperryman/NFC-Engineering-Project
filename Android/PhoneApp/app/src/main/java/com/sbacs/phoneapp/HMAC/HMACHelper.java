package com.sbacs.phoneapp.HMAC;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.KeySpec;

import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

/**
 * Created by Richard on 2017-02-05.
 */

public class HMACHelper {
    private static final int iterations = 100000;
    private static final int secret_length = 255 * 8; // Secret length is in bits

    public static String QueryString(int userId, String body, String key) {
        String secret = SecretString(body, key);
        try {
            return "hmac-user=" + userId + "&hmac-content=" + URLEncoder.encode(secret, "UTF-16");
        } catch (UnsupportedEncodingException e) {
            return "";
        }
    }

    public static String SecretString(String body, String key){
        // Should probably add code to force UTF-16
        SecretKeyFactory skf;
        KeySpec spec;
        SecretKey secret;
        try {
            skf = SecretKeyFactory.getInstance("PBKDF2WithHMACSHA256");
        } catch (NoSuchAlgorithmException e) {
            return "";
        }

        spec = new PBEKeySpec(body.toCharArray(), key.getBytes(), iterations, secret_length);

        try {
            secret = skf.generateSecret(spec);
        } catch (InvalidKeySpecException e) {
            return "";
        }

        return new String(secret.getEncoded());
    }
}
