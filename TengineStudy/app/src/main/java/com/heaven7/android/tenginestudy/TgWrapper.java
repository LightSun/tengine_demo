package com.heaven7.android.tenginestudy;

import android.graphics.Bitmap;

public final class TgWrapper {

    public static final int TYPE_OPENPOSE = 1;
    private static boolean sInited; //for currently tengine can't init twice

    private final int type;
    private long ptr;

    public TgWrapper(int type) {
        this.type = type;
        this.ptr = nCreate(type);
    }
    public long getNativePointer(){
        return ptr;
    }
    public void destroyNative(){
        if(ptr != 0){
            nRelease(ptr, type);
            ptr = 0;
        }
    }
    @Override
    protected void finalize() throws Throwable {
        destroyNative();
        super.finalize();
    }
    public static void initEngine(){
        if(!sInited){
            sInited = true;
            ninitEngine();
        }
    }
    public static void destroyEngine(){
        nDestroyEngine();
    }
    public void destroy(){
        ndestroy(getNativePointer());
    }
    public void createGraph(String format, String moduleFile){
        ncreateGraph(getNativePointer(), format, moduleFile);
    }
    public boolean getInputTensor(int input_node_idx, int tensor_idx){
        return ngetInputTensor(getNativePointer(), input_node_idx, tensor_idx);
    }
    public void setTensorShape(GraphParam param){
        nsetTensorShape(getNativePointer(), param.getNativePointer());
    }
    public boolean getOutputTensor(int input_node_idx, int tensor_idx){
        return ngetOutputTensor(getNativePointer(), input_node_idx, tensor_idx);
    }
    public void setInputBuffer(Bitmap bitmap, String id){
        nsetInputBuffer(getNativePointer(), bitmap, id);
    }
    public void setInputBuffer(String imgFile, String id){
        nsetInputBuffer2(getNativePointer(), imgFile, id);
    }
    public void setInputBuffer(byte[] data, int width, int height, String id) {
        nsetInputBuffer3(getNativePointer(), data, width, height, id);
    }
    public void preRunGraph(){
        npreRunGraph(getNativePointer());
    }
    public boolean runGraph(int repeatCount, boolean block){
        return nrunGraph(getNativePointer(), repeatCount, block);
    }
    public void postProcess(){
        npostProcess(getNativePointer());
    }
    public void postRunGraph(){
        npostRunGraph(getNativePointer());
    }
    private native boolean ngetInputTensor(long ptr, int input_node_idx, int tensor_idx);
    private native boolean ngetOutputTensor(long ptr, int input_node_idx, int tensor_idx);
    private native void nsetTensorShape(long ptr, long gpPtr);
    private native void ncreateGraph(long ptr, String format, String moduleFile);
    private native void nsetInputBuffer(long ptr, Bitmap bitmap, String id);
    private native void nsetInputBuffer2(long ptr, String imgFile, String id);
    private native void nsetInputBuffer3(long ptr, byte[] data, int width, int height, String id);
    private native boolean nrunGraph(long ptr, int repeatCount, boolean block);

    private static native void ninitEngine();
    private static native void nDestroyEngine();
    private native void ndestroy(long ptr);
    private native void npreRunGraph(long ptr);
    private native void npostProcess(long ptr);
    private native void npostRunGraph(long ptr);
    private static native long nCreate(int type);
    private static native void nRelease(long ptr, int type);
}
