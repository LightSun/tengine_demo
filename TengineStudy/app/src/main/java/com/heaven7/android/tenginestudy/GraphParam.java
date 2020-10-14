package com.heaven7.android.tenginestudy;

public class GraphParam{

    public static final int OPENPOSE_TYPE_MP1     = 1;
    public static final int OPENPOSE_TYPE_COCO    = 2;
    public static final int OPENPOSE_TYPE_BODY25  = 3;

    public static final int TENGINE_CLUSTER_ALL = 0;
    public static final int TENGINE_CLUSTER_BIG = 1;
    public static final int TENGINE_CLUSTER_MEDIUM = 2;
    public static final int TENGINE_CLUSTER_LITTLE = 3;

    public static final int TENGINE_MODE_FP32        = 0;
    public static final int TENGINE_MODE_FP16        = 1;
    public static final int TENGINE_MODE_HYBRID_INT8 = 2;
    public static final int TENGINE_MODE_UINT8       = 3;
    public static final int TENGINE_MODE_INT8        = 4;

    private long ptr;
/*
    private int n;
    private int c;
    private int h;
    private int w;
*/

    public GraphParam() {
        this.ptr = nCreate();
    }
    public long getNativePointer(){
        return ptr;
    }
    public void destroyNative(){
        if(ptr != 0){
            nRelease(ptr);
            ptr = 0;
        }
    }
    @Override
    protected void finalize() throws Throwable {
        destroyNative();
        super.finalize();
    }

    public int getN() {
        return ngetN(getNativePointer());
    }

    public void setN(int n) {
       nsetN(getNativePointer(), n);
    }

    public int getC() {
        return ngetC(getNativePointer());
    }

    public void setC(int c) {
        nsetC(getNativePointer(), c);
    }

    public int getH() {
        return ngetH(getNativePointer());
    }

    public void setH(int h) {
        nsetH(getNativePointer(), h);
    }

    public int getW() {
        return ngetW(getNativePointer());
    }

    public void setW(int w) {
        nsetW(getNativePointer(), w);
    }

    public void setOutputDir(String outputDir){
        nsetOutputDir(getNativePointer(), outputDir);
    }
    public void setStype(int type){
        nsetStype(getNativePointer(), type);
    }
    public void setUnitSize(int unitSize){
        nsetUnitSize(getNativePointer(), unitSize);
    }
    public void setThreadCount(int n){
        nsetThreadCount(getNativePointer(), n);
    }
    public void setCluster(int val){
        nsetCluster(getNativePointer(), val);
    }
    public void setPrecision(int val){
        switch (val){
            case TENGINE_MODE_FP16:
                setUnitSize(2);
                break;
            case TENGINE_MODE_FP32:
                setUnitSize(4);
                break;
            case TENGINE_MODE_HYBRID_INT8:
            case TENGINE_MODE_UINT8:
            case TENGINE_MODE_INT8:
                setUnitSize(1);
                break;
        }
        nsetPrecision(getNativePointer(), val);
    }
    //-------------------------------------------------
    private native void nsetPrecision(long ptr, int n);
    private native void nsetCluster(long ptr, int n);
    private native void nsetThreadCount(long ptr, int n);
    private native void nsetUnitSize(long ptr, int unitSize);
    private native void nsetStype(long ptr, int type);
    private native void nsetOutputDir(long ptr, String dir);

    private static native long nCreate();
    private static native void nRelease(long ptr);

    private native int ngetN(long ptr);
    private native void nsetN(long ptr, int n);
    private native int ngetC(long ptr);
    private native void nsetC(long ptr, int c);

    private native int ngetH(long ptr);
    private native void nsetH(long ptr, int h);
    private native int ngetW(long ptr);
    private native void nsetW(long ptr, int w);
}
