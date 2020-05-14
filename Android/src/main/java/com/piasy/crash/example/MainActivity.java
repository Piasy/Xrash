package com.piasy.crash.example;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("crash");
    }

    private static native void testCrash();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        findViewById(R.id.crash).setOnClickListener(v -> {
            Log.e("XXX", "before testJni");
            testCrash();
            Log.e("XXX", "after testJni");
        });
    }
}
