package com.devilist.glrender.pngrender;

import android.view.Surface;

/**
 * Created by zengpu on 2018/10/25
 */
public class PngPreviewController0 {

    public native void init(String pngPath);

    public native void setSurface(Surface surface);

    public native void resetSize(int width, int height);

    public native void stop();

}
