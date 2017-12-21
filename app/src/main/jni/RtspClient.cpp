/*
 * RtspClient.cpp
 *
 *  Created on: 2014-3-16
 *      Author: ny
 */
#include <jni.h>
#include <android/log.h>
#define  LOG_TAG    "jniTest"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C" {

#include "ffmpeg/libavcodec/avcodec.h"
#include "ffmpeg/libavformat/avformat.h"
#include "ffmpeg/libswscale/swscale.h"
#include "FFmpeg.h"

}
const char * rtspURL;
FFmpeg * ffmpeg;
extern "C" {

void Java_com_ny_rtspclient_VideoDisplay_initialWithUrl(JNIEnv *env,
		jobject thisz, jstring url) {
	rtspURL = env->GetStringUTFChars(url, NULL);
	LOGI("%s", rtspURL);
	ffmpeg = new FFmpeg();
	ffmpeg->initial((char *) rtspURL, env);
	//调用java的方法，设置bitmap的wdith和height
	/**
	 *public void setBitmapSize(int width, int height) {
	 *    Log.i(TAG, "setsize");
	 *    mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
	 *}
	 *调用这个方法之后bitmpa！=null，绘图线程就会启动
	 */
	jclass cls = env->GetObjectClass(thisz);
	jmethodID mid = env->GetMethodID(cls, "setBitmapSize", "(II)V"); //调用java的方法
	env->CallVoidMethod(thisz, mid, (int) ffmpeg->width, (int) ffmpeg->height);
}

void Java_com_ny_rtspclient_VideoDisplay_play(JNIEnv *env, jobject thisz,
		jobject bitmap) {

	ffmpeg->h264Decodec(bitmap);
}
}

