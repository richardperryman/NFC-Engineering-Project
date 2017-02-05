package com.sbacs.phoneapp;

/**
 * Created by Richard on 2017-01-23.
 */

public class Lock {
    private int id;
    private String name;
    private String owner;

    public Lock(int id, String name, String owner) {
        this.id = id;
        this.name = name;
        this.owner = owner;
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getOwner() {
        return owner;
    }

}
