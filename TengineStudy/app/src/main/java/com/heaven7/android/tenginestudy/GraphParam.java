package com.heaven7.android.tenginestudy;

public class GraphParam{

    private long ptr;
/*

    private int n;
    private int c;
    private int h;
    private int w;
*/

    public GraphParam() {
        this.ptr = _nCreate();
    }
    public long getNativePointer(){
        return ptr;
    }
    public void destroyNative(){
        if(ptr != 0){
            _nRelease(ptr);
            ptr = 0;
        }
    }
    @Override
    protected void finalize() throws Throwable {
        destroyNative();
        super.finalize();
    }

    public int getN() {
        return _getN(getNativePointer());
    }

    public void setN(int n) {
       _setN(getNativePointer(), n);
    }

    public int getC() {
        return _getC(getNativePointer());
    }

    public void setC(int c) {
        _setC(getNativePointer(), c);
    }

    public int getH() {
        return _getH(getNativePointer());
    }

    public void setH(int h) {
        _setH(getNativePointer(), h);
    }

    public int getW() {
        return _getW(getNativePointer());
    }

    public void setW(int w) {
        _setW(getNativePointer(), w);
    }

    private static native long _nCreate();
    private static native void  _nRelease(long ptr);

    public native int _getN(long ptr);
    public native void _setN(long ptr, int n);
    public native int _getC(long ptr);
    public native void _setC(long ptr, int c);

    public native int _getH(long ptr);
    public native void _setH(long ptr, int h);
    public native int _getW(long ptr);
    public native void _setW(long ptr, int w);
}
