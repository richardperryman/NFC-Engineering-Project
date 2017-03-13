package com.sbacs.phoneapp.Model;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by Richard on 2017-03-11.
 */

public class Registration {
    private int id;
    private int identity_id;
    private int lock_id;
    private int use_count;
    private boolean is_new;
    private String identityName;
    private String lockName;

    public Registration(int id, int identity_id, int lock_id, int use_count, boolean is_new) {
        this.id = id;
        this.identity_id = identity_id;
        this.lock_id = lock_id;
        this.use_count = use_count;
        this.is_new = is_new;
    }

    public Registration(JSONObject registration) throws JSONException {
        id = registration.getInt("Reg_Id");
        identity_id = registration.getInt("Identity_Id");
        lock_id = registration.getInt("Lock_Id");
        is_new = false;
    }

    public int getId() {
        return id;
    }

    public int getIdentity_id() {
        return identity_id;
    }

    public int getLock_id() {
        return lock_id;
    }

    public int getUse_count() {
        return use_count;
    }

    public boolean is_new() {
        return is_new;
    }

    public String getIdentityName() {
        return identityName;
    }

    public String getLockName() {
        return lockName;
    }

    public void setIdentityName(String name) {
        identityName = name;
    }

    public void setLockName(String name) {
        lockName = name;
    }
}
