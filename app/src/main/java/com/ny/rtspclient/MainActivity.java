package com.ny.rtspclient;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity {
//	public static String RTSPURL=tsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov
	public static String RTSPURL="rtsp://47.96.145.189:554/cuizhou";
	private EditText text_rtsp;
	private Button btn_play,btn_cancle;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// 去除title
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_main);
		text_rtsp=(EditText)findViewById(R.id.rtspurl);
		btn_play=(Button)findViewById(R.id.btn_play);
		btn_cancle=(Button)findViewById(R.id.btn_cancle);
		btn_play.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
//				RTSPURL=text_rtsp.getText().toString();
				Intent i = new Intent(MainActivity.this, VideoActivity.class);
				startActivity(i);
				finish();
			}
		});
		
		btn_cancle.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				finish();
			}
		});
		
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
