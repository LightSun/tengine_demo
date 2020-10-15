package com.heaven7.android.tenginestudy;

import android.Manifest;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.WorkerThread;
import androidx.appcompat.app.AppCompatActivity;

import com.heaven7.core.util.Logger;
import com.heaven7.core.util.PermissionHelper;
import com.heaven7.java.pc.schedulers.Schedulers;

import java.io.File;
import java.util.List;

public class OpenposeCameraAc extends AppCompatActivity {

    private static final String TAG = "OpenposeCameraAc";
   // private static final String OPENPOSE_PREVIEW_DIR = MainActivity.DST_PATH + "/openpose/pre";
    private static final String OPENPOSE_OUT_DIR = MainActivity.DST_PATH + "/openpose/out";

    SurfaceView mSurfaceView;

    private final PermissionHelper mHelper = new PermissionHelper(this);
    private final TgWrapper mWrapper = new TgWrapper(TgWrapper.TYPE_OPENPOSE);
    private final GraphParam mGraphParam = new GraphParam();
    private InferenceQueueManager mInferM;

    private int mScreenWidth,mScreenHeight;

    private Camera.CameraInfo mCameraInfo;
    private Camera mCamera;
    private Camera.Parameters mParameters;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        mScreenWidth = dm.widthPixels;
        mScreenHeight = dm.heightPixels;

        mInferM = new InferenceQueueManager(mWrapper);
        setContentView(R.layout.ac_openpose_camera);

        mSurfaceView = findViewById(R.id.surfaceView);
        initCameraEvent();
    }

    @Override
    protected void onDestroy() {
        mCamera.release();
        mInferM.cancel();
        mWrapper.destroy();
        mWrapper.destroyNative();
        mGraphParam.destroyNative();
        super.onDestroy();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        mHelper.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    private void initCameraEvent() {
        mSurfaceView.getHolder().setKeepScreenOn(true);
        mSurfaceView.getHolder().addCallback(new SurfaceCallback());
        mSurfaceView.getHolder().setFormat(PixelFormat.TRANSPARENT);
    }
    private void reqPermission(Camera.CameraInfo info, int cameraId, SurfaceHolder holder){
        mHelper.startRequestPermission(new String[]{Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE},
                new int[]{1, 2}, new PermissionHelper.ICallback() {
                    @Override
                    public void onRequestPermissionResult(String s, int i, boolean b) {
                        if(b){
                            startPreview(info, cameraId, holder);
                        }else {
                            System.err.println("permission not required. camera / sd");
                        }
                    }
                    @Override
                    public boolean handlePermissionHadRefused(String s, int i, Runnable runnable) {
                        return false;
                    }
                });
    }
    public void startPreview(Camera.CameraInfo info, int cameraId, SurfaceHolder holder) {
        try {

            mCameraInfo = info;

            mCamera = Camera.open(cameraId);
            mCamera.setPreviewDisplay(holder);
            mCamera.setDisplayOrientation(90); // 设置显示的方向，这里手机是竖直为正向90度，可以自己写个方法来根据屏幕旋转情况获取到相应的角度
            {
                mParameters = mCamera.getParameters();

                // PictureSize 获取支持显示的尺寸 因为支持的显示尺寸是和设备有关，所以需要获取设备支持的尺寸列表
                // 另外因为是预览画面是全屏显示，所以显示效果也和屏幕的分辨率也有关系，为了最好的适应屏幕，建议选取
                // 与屏幕最接近的宽高比的尺寸
                List<Camera.Size> listPictureSizes = mParameters
                        .getSupportedPictureSizes();

                Camera.Size sizeOptimalPicture = Utils.getOptimalSize(mScreenWidth,
                        mScreenHeight, listPictureSizes, (double) mScreenWidth
                                / mScreenHeight);
                mParameters.setPictureSize(sizeOptimalPicture.width,
                        sizeOptimalPicture.height);

                // PreviewSize
                List<Camera.Size> ListPreviewSizes = mParameters
                        .getSupportedPreviewSizes();

                Camera.Size sizeOptimalPreview = Utils.getOptimalSize(
                        sizeOptimalPicture.width, sizeOptimalPicture.height,
                        ListPreviewSizes, (double) sizeOptimalPicture.width
                                / sizeOptimalPicture.height);
                mParameters.setPreviewSize(sizeOptimalPreview.width,
                        sizeOptimalPreview.height);

                // 这里就是有的设备不支持Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE 连续自动对焦
                List<String> lstFocusModels = mParameters
                        .getSupportedFocusModes();
                for (String str : lstFocusModels) {
                    if (str.equals(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE)) {
                        mParameters
                                .setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
                        break;
                    }
                }
                mCamera.setParameters(mParameters);
            }
            //setPreviewCallback();
            mCamera.startPreview();
            Schedulers.io().newWorker().schedule(new Runnable() {
                @Override
                public void run() {
                    setupParameters();
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    @WorkerThread
    private void setupParameters(){
        new File(OPENPOSE_OUT_DIR).mkdirs();

        long start = System.currentTimeMillis();
        TgWrapper.initEngine();
        Logger.d(TAG, "setupParameters", "init cost time = " + (System.currentTimeMillis() - start));
        start = System.currentTimeMillis();
        //prepare param
        mGraphParam.setOutputDir(OPENPOSE_OUT_DIR);
        mGraphParam.setUnitSize(4);
        mGraphParam.setN(1);
        mGraphParam.setC(3);
        mGraphParam.setW(368);
        mGraphParam.setH(368);
        mGraphParam.setStype(GraphParam.OPENPOSE_TYPE_BODY25);

        mWrapper.createGraph("tengine", MainActivity.DST_PATH + "/openpose/openpose_body25.tmfile");
        mWrapper.getInputTensor(0, 0);
        mWrapper.setTensorShape(mGraphParam);
        Logger.d(TAG, "setupParameters", "createGraph cost time = " + (System.currentTimeMillis() - start));
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                setPreviewCallback();
            }
        });
    }
    private void setPreviewCallback() {
        mCamera.setPreviewCallback(new Camera.PreviewCallback() {
            @Override
            public void onPreviewFrame(byte[] data, Camera camera) {
                Camera.Size size = camera.getParameters().getPreviewSize();
                mInferM.putItem(new InferenceQueueManager.Item(data, size.width, size.height,  System.currentTimeMillis()+""));
            }
        });
    }
    private class SurfaceCallback implements SurfaceHolder.Callback{
        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            int nNum = Camera.getNumberOfCameras();
            if (nNum == 0) {
                return;
            }
            for (int i = 0; i < nNum; i++) {
                Camera.CameraInfo info = new Camera.CameraInfo();
                Camera.getCameraInfo(i, info);
                if (info.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
                    reqPermission(info, i, holder);
                    mCameraInfo = info;
                    return;
                }
            }
        }
        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            if (mCamera == null) {
                return;
            }
            mCamera.autoFocus(new Camera.AutoFocusCallback() {
                @Override
                public void onAutoFocus(boolean success, Camera camera) {
                    if (success) {
                        mCamera.cancelAutoFocus();
                        Log.d("surfaceChange","autofocus success");
                    }
                }
            });
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            if (mCamera != null) {
                mCamera.stopPreview();
                mCamera.release();
                mCamera = null;
                mCameraInfo = null;
            }
        }
    }

}
