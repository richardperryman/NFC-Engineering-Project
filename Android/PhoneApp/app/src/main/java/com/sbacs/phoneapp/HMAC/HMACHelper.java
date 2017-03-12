package com.sbacs.phoneapp.HMAC;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.KeySpec;
import java.util.HashMap;
import java.util.Map;

import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

/**
 * Created by Richard on 2017-02-05.
 */

public class HMACHelper {
    private static final int iterations = 10000;
    private static final int secret_length = 255 * 8; // Secret length is in bits

    public static Map<String, String> GetHMACHeaders(int userId, String body, String key) {
        String secret;
        Map <String, String> result = new HashMap<String, String>();

        /*
        try {
            secret = URLEncoder.encode(SecretString(body, key), "UTF-16");
        } catch (UnsupportedEncodingException e) {
            secret = "";
        }
        */

        result.put("hmac-user", "10outta13");
        /*result.put("hmac-user", "" + userId);
        result.put("hmac-content", secret);
        /**/
        return result;
    }

    public static byte[] HMACLoginBody(String username, String password) {
        JSONObject body = new JSONObject();
        try {
            body.put("username", username);
            body.put("password", password);
        } catch (JSONException e) {
            // Uhhh do something?
        }

        return body.toString().getBytes();
    }

    private static String SecretString(String body, String key){
        // Should probably add code to force UTF-16
        SecretKeyFactory skf;
        KeySpec spec;
        SecretKey secret;
        try {
            // Not working anymore...?
            skf = SecretKeyFactory.getInstance("PBKDF2withHmacSHA256");
        } catch (NoSuchAlgorithmException e) {
            return "";
        }

        //char[] pw = body.length() > 0 ? body.toCharArray() : new char[] {'1'};
        spec = new PBEKeySpec(body.toCharArray(), key.getBytes(), iterations, secret_length);

        try {
            secret = skf.generateSecret(spec);
        } catch (InvalidKeySpecException e) {
            return "";
        } catch (Exception e) {
            e.printStackTrace();
            Object foo = e.getStackTrace();
            return "wtf";
        }

        return new String(secret.getEncoded());
    }
}
