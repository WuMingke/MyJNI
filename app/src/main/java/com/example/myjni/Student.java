package com.example.myjni;

import android.util.Log;

import androidx.annotation.NonNull;

public class Student {

    public static final String TAG = "MINGKE";
    public int id;
    public String name;

    public int getId() {
        Log.i(TAG, "java getId: " + id);
        return id;
    }

    public void setId(int id) {
        Log.i(TAG, "java setId: " + id);
        this.id = id;
    }

    public String getName() {
        Log.i(TAG, "java getName: " + name);
        return name;
    }

    public void setName(String name) {
        Log.i(TAG, "java setName: " + name);
        this.name = name;
    }

    public static void showInfo(String info) {
        Log.i(TAG, "java showInfo: " + info);
    }

    @NonNull
    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder("Student:");
        return builder
                .append(" id = ")
                .append(id)
                .append(" , name = ")
                .append(name)
                .toString();
    }
}
