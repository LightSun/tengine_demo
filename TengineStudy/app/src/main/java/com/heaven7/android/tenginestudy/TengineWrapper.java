package com.heaven7.android.tenginestudy;

import android.graphics.Bitmap;

public final class TengineWrapper {

    public static void initTengine(){
        _initTengine();
    }
    public static void destroyTengine(){
        _destroyTengine();
    }
    public static void initModule(String moduleFile){
        _initModule(moduleFile);
    }
    public static void prepareGraph(GraphParam gp){
        _prepareGraph(gp.getNativePointer());
    }
    public static void setInputBuffer(Bitmap bitmap){
        _setInputBuffer(bitmap);
    }

    private native static void _setInputBuffer(Bitmap bitmap);
    private native static void _prepareGraph(long gpPtr);

    private native static void _initTengine();
    private native static void _destroyTengine();
    private native static void _initModule(String moduleFile);


}
