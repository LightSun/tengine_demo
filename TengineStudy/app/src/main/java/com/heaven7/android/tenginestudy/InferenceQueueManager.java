package com.heaven7.android.tenginestudy;

import com.heaven7.core.util.Logger;
import com.heaven7.java.pc.Consumer;
import com.heaven7.java.pc.pm.DirectProductManager;
import com.heaven7.java.pc.producers.PipeProducer;
import com.heaven7.java.pc.schedulers.Schedulers;

import java.util.concurrent.atomic.AtomicInteger;

public class InferenceQueueManager implements Consumer<InferenceQueueManager.Item> {

    private static final String TAG = InferenceQueueManager.class.getSimpleName();
    private final AtomicInteger mIdIndex = new AtomicInteger();
    private final TgWrapper mWrapper;
    private final PipeProducer<Item> mProducer;

    public InferenceQueueManager(TgWrapper mWrapper) {
        this.mWrapper = mWrapper;
        this.mProducer = new PipeProducer<>();

        DirectProductManager<Item> mPm = new DirectProductManager<>(mProducer);
        mPm.setScheduler(Schedulers.io());
        mPm.open(this);
    }

    public void putItem(Item item){
        if(mProducer.isOpened() && mProducer.getLeftProductSize() < 10){
            mProducer.getPipe().addProduct(item);
        }else {
            Logger.d(TAG, "putItem","Item is to many. ignore");
        }
    }

    public void cancel(){
        if(mProducer.isOpened()){
            mProducer.getPipe().close();
            mProducer.close();
        }
    }

    @Override
    public void onStart(Runnable next) {
        next.run();
    }
    @Override
    public void onConsume(Item item, Runnable next) {
        long start = System.currentTimeMillis();
        mWrapper.setInputBuffer(item.data, item.width, item.height,  item.id);

        mWrapper.preRunGraph();
        mWrapper.runGraph(1, true);
        mWrapper.getOutputTensor(0, 0);
        mWrapper.postProcess();
        mWrapper.postRunGraph();
        Logger.d(TAG, "onConsume","inference ("+ mIdIndex.incrementAndGet()
                +") cost time: " + (System.currentTimeMillis() - start)
                + " ,left itemSize = " + mProducer.getLeftProductSize());
        next.run();
    }
    @Override
    public void onEnd() {

    }
    public static class Item{
        byte[] data;
        int width;
        int height;
        String id;

        public Item(byte[] data, int width, int height, String id) {
            this.data = data;
            this.width = width;
            this.height = height;
            this.id = id;
        }
    }
}
