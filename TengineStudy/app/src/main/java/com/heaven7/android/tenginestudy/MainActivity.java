package com.heaven7.android.tenginestudy;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;

import com.heaven7.core.util.PermissionHelper;
import com.heaven7.core.util.Toaster;
import com.heaven7.java.base.util.FileUtils;
import com.heaven7.java.pc.schedulers.Schedulers;

import java.util.ArrayList;
import java.util.List;

/**
 * coo: 12.5 s
 * body25: 4.5s
 */
public class MainActivity extends AppCompatActivity {

    private static final String DST_PATH = Environment.getExternalStorageDirectory() + "/tengine_demos";
    private final PermissionHelper mHelper = new PermissionHelper(this);

    static {
        System.loadLibrary("c++_shared"); //opencv need c++ shared
        System.loadLibrary("opencv_java4");
        System.loadLibrary("tengine-lite");
        System.loadLibrary("tengine_demo");
    }
    public static native int runMain(int argc, String prefix, String[] args);

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
                AssetsUtils.copyAll(getApplicationContext(), "openpose", DST_PATH);
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
                list.add(DST_PATH + "/landmark");
                //list.add("test"); //getopt 不能以"-" 开头
               // list.add("-m");
                list.add(DST_PATH + "/landmark/landmark.tmfile");
               // list.add("-i");
                list.add(DST_PATH + "/landmark/2.jpg");
               // list.add("-r");
                list.add("1");
               // list.add("-t");
                list.add("1");
                int result = runMain(4, "xx",list.toArray(new String[list.size()]));
                System.out.println("onClickTestLandmark: result = " + result);
            }
        });
    }

    public void onClickTestOpenpose(View view) {
        Schedulers.io().newWorker().schedule(new Runnable() {
            @Override
            public void run() {
                String file = Environment.getExternalStorageDirectory() +  "/DCIM/Camera/IMG_20200927_101845.jpg";
                // -m models/landmark.tmfile -i images/mobileface02.jpg -r 1 -t 1
                int count = 1;
                for (int i = 1 ; i < count + 1; i ++){
                    file = DST_PATH + "/openpose/4" +".jpg";
                    long s = System.currentTimeMillis();
                    List<String> list = new ArrayList<>();
                    list.add(DST_PATH + "/openpose");
                    //list.add(DST_PATH + "/openpose/self_openpose_coco.tmfile");
                    list.add(DST_PATH + "/openpose/openpose_body25.tmfile");
                    list.add(file);
                    int result = runMain(4, "/" + FileUtils.getFileName(file), list.toArray(new String[list.size()]));
                    System.out.println("onClickTestOpenpose: result = " + result);
                    System.out.println("cost time = " + (System.currentTimeMillis() - s));
                }
            }
        });
    }
}
