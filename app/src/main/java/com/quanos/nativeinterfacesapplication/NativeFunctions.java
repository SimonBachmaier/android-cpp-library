package com.quanos.nativeinterfacesapplication;

public class NativeFunctions {

    private static final NativeFunctions instance = new NativeFunctions();

    private NativeFunctions() {}
    public static NativeFunctions getInstance() { return instance; }

    public native String stringFromJNI(String arg1) throws IllegalArgumentException;
}
