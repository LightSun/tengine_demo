package com.heaven7.android.tenginestudy;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;

import com.heaven7.core.util.PermissionHelper;

public class MainActivity extends AppCompatActivity {

    private final PermissionHelper mHelper = new PermissionHelper(this);

    static {
        System.loadLibrary("opencv_java4");
        System.loadLibrary("tengine-lite");
        System.loadLibrary("tengine_demo");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mHelper.startRequestPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, 1, new PermissionHelper.ICallback() {
            @Override
            public void onRequestPermissionResult(String s, int i, boolean b) {
                if(b){
                    System.out.println("request write SD ok");
                }
            }
            @Override
            public boolean handlePermissionHadRefused(String s, int i, Runnable runnable) {
                return false;
            }
        });
    }

    public static native int runMain(String[] args);
}
