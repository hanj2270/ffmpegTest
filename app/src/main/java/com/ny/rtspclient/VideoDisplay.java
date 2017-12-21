package com.ny.rtspclient;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

public class VideoDisplay extends SurfaceView implements Callback {
	private Bitmap bitmap;
	private Matrix matrix;
	private SurfaceHolder sfh;
	private int width = 0;
	private int height = 0;
	public native void initialWithUrl(String url);
	public native void play( Bitmap bitmap);
	
	public VideoDisplay(Context context) {
		super(context);
		sfh = this.getHolder();
		sfh.addCallback(this);
		matrix=new Matrix();
		bitmap = Bitmap.createBitmap(640, 480, Bitmap.Config.ARGB_8888);
		Log.i("SUr", "begin");
	}

	@Override
	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
		width = arg2;
		height = arg3;
	}

	@Override
	public void surfaceCreated(SurfaceHolder arg0) {
		Log.i("SUr", "play before");
		new Thread(new Runnable() {

			@Override
			public void run() {
				Log.i("SUr", "play");
				initialWithUrl(MainActivity.RTSPURL);
				play(bitmap);
			}
		}).start();
		new Thread(new Runnable() {

			@Override
			public void run() {
				while (true) {
					if ((bitmap != null)) {
						// System.out.println("begin");
						Canvas canvas = sfh.lockCanvas(null);
						Paint paint = new Paint();
						paint.setAntiAlias(true);
						paint.setStyle(Style.FILL);
						int mWidth = bitmap.getWidth();
						int mHeight = bitmap.getHeight();
						matrix.reset();
						matrix.setScale((float) width / mWidth, (float) height
								/ mHeight);
						canvas.drawBitmap(bitmap, matrix, paint);
						sfh.unlockCanvasAndPost(canvas);
					}
				}
			}
		}).start();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {

	}

	public void setBitmapSize(int width, int height) {
		Log.i("Sur", "setsize");
		bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
	}

	static {
		System.loadLibrary("rtspclient");
	}
}
