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
}