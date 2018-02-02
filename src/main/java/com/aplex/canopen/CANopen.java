package com.aplex.canopen;

import android.util.Log;

/**
 * Created by aplex on 2018/1/29.
 */

public class CANopen {
    private static final String TAG = "CANopen";

    static {
        Log.d(TAG, "static1");
        System.loadLibrary("Canopen-lib");
        Log.d(TAG, "static2");
    }
//    public native String stringFromJNI();
//    public native String excuteUtils(int argc, String[] argv);
    public native String canopenUtils(String[] cmd);

}
