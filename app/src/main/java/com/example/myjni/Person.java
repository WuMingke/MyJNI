package com.example.myjni;

import android.util.Log;

public class Person {
    public Student student;

    public void setStudent(Student student) {
        Log.i("MINGKE", "java setStudent: " + student.toString());
        this.student = student;
    }

    public static void putStudent(Student student) {
        Log.i("MINGKE", "java static putStudent: " + student.toString());
    }
}
