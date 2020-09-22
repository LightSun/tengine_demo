package com.heaven7.android.tenginestudy;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;

import com.heaven7.core.util.PermissionHelper;
import com.heaven7.core.util.Toaster;
import com.heaven7.java.pc.schedulers.Schedulers;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private static final String DST_PATH = Environment.getExternalStorageDirectory() + "/tengine_demos";
    private final PermissionHelper mHelper = new PermissionHelper(this);

    static {
        System.loadLibrary("c++_shared"); //opencv need c++ shared
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
                    copyAssets();
                }
            }
            @Override
            public boolean handlePermissionHadRefused(String s, int i, Runnable runnable) {
                return false;
            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        mHelper.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    private void copyAssets() {
        Schedulers.io().newWorker().schedule(new Runnable() {
            @Override
            public void run() {
                AssetsUtils.copyAll(getApplicationContext(), "landmark", DST_PATH);
                System.out.println("copyAssets done");
                Toaster.show(getApplicationContext(), "copyAssets done");
            }
        });
    }
    public void onClickTestLandmark(View view) {
        Schedulers.io().newWorker().schedule(new Runnable() {
            @Override
            public void run() {
                // -m models/landmark.tmfile -i images/mobileface02.jpg -r 1 -t 1
                List<String> list = new ArrayList<>();
                list.add(DST_PATH + "/landmark/out.jpg");
                //list.add("test"); //getopt 不能以"-" 开头
               // list.add("-m");
                list.add(DST_PATH + "/landmark/landmark.tmfile");
               // list.add("-i");
                list.add(DST_PATH + "/landmark/2.jpg");
               // list.add("-r");
                list.add("1");
               // list.add("-t");
                list.add("1");
                int result = runMain(4, list.toArray(new String[list.size()]));
                System.out.println("onClickTestLandmark: result = " + result);
            }
        });
    }

    public static native int runMain(int argc,String[] args);
}