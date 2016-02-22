package com.quint;

import android.util.Log;

public class Example
{
    static final String TAG = "Example";

    // CONSTRUCTORS:

    Example()
    {
        Log.i(TAG, "Default constructor.");
    }

    Example(int x, float y)
    {
        Log.i(TAG, "Constructor with primitive types: int=" + x + ", float=" + y + ".");
    }

    Example(String str)
    {
        Log.i(TAG, "Constructor with default types: String=" + str + ".");
    }

    Example(Example other)
    {
        Log.i(TAG, "Constructor with the same type: Example=" + other + ".");
    }

    // STATIC METHODS:

    static void static1()
    {
        Log.i(TAG, "Static method without arguments.");
    }

    static void static2(int x, boolean y)
    {
        Log.i(TAG, "Static method with two arguments: int=" + x + ", boolean=" + y + ".");
    }

    static void static3(Example other)
    {
        Log.i(TAG, "Static method with the object parameter: " + other + ".");
    }

    static long static4(long x, long y)
    {
        return x + y;
    }

    static Example static5(String s)
    {
        return new Example(s);
    }

    // INSTANCE METHODS:

    void instance1()
    {
        Log.i(TAG, "Instance void method without arguments.");
    }

    void instance2(String s)
    {
        Log.i(TAG, "Instance void method with string argument: " + s + ".");
    }

    int instance3()
    {
        return 777;
    }

    String instance4(String s)
    {
        return s + "x" + s;
    }

    Example instance5()
    {
        return new Example();
    }

    // STATIC NATIVE METHODS

    public static void callNativeStaticMethodLater()
    {
        Log.i(TAG, "Registered delayed callback.");

        new java.util.Timer().schedule(
                new java.util.TimerTask() {
                    @Override
                    public void run() {
                        staticNativeMethod();
                    }
                },
                2000
        );
    }

    public native static void staticNativeMethod();

    // INSTANCE NATIVE METHODS

    public void testNativeMethod()
    {
        Log.i(TAG, "calling native method #1...");
        native1();
        Log.i(TAG, "calling native method #2 (3.1 + 0.0415): " + native2(3.1, 0.0415));
        Log.i(TAG, "calling native method #3 (...X...): " + native3("kanojo"));
        Log.i(TAG, "calling native method #4 (777): " + native4().instance3());
        Log.i(TAG, "calling native method #5...");
        native5(this);
    }

    public native void native1();
    public native double native2(double d1, double d2);
    public native String native3(String s);
    public native Example native4();
    public native void native5(Example e);
}