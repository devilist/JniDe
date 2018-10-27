package com.devilist.demo;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.RelativeLayout;

import com.devilist.glrender.R;
import com.devilist.glrender.pngrender.PngPreviewController0;

/**
 * Created by zengpu on 2018/10/26
 */
public class PngPreview0Activity extends AppCompatActivity implements SurfaceHolder.Callback {

    static {
        System.loadLibrary("pngrender0");
    }

    public static void start(Context context, String pngPath) {
        Intent starter = new Intent(context, PngPreview0Activity.class);
        starter.putExtra("pngPath", pngPath);
        context.startActivity(starter);
    }

    private RelativeLayout mRootView;
    private SurfaceView mSurfaceView;

    private PngPreviewController0 mController;

    private String pngPath = "/sdcard/z/render0.png";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_png_preview0);
        pngPath = getIntent().getStringExtra("pngPath");
        initView();
    }

    private void initView() {
        mRootView = findViewById(R.id.root);
        mSurfaceView = new SurfaceView(this);
        SurfaceHolder holder = mSurfaceView.getHolder();
        holder.addCallback(this);
        holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

        mRootView.addView(mSurfaceView);
        mSurfaceView.getLayoutParams().width = getWindowManager().getDefaultDisplay().getWidth();
        mSurfaceView.getLayoutParams().height = getWindowManager().getDefaultDisplay().getHeight();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        mController = new PngPreviewController0();
        mController.init(pngPath);
        mController.setSurface(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        mController.resetSize(width, height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    @Override
    protected void onStop() {
        super.onStop();

        if (null != mController) {
            mController.stop();
            mController = null;
        }
    }
}
