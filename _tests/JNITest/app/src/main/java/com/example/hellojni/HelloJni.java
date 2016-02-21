package com.example.hellojni;

import android.app.Activity;
import android.os.Bundle;

public class HelloJni extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        performTest();
    }

    public native void performTest();

    static {
        System.loadLibrary("hello-jni");
    }
}
