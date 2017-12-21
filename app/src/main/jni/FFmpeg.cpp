/*
 * FFmpeg.cpp
 *
 *  Created on: 2014年2月25日
 *      Author: ny
 */

#include "FFmpeg.h"

FFmpeg::FFmpeg() {
	pCodecCtx = NULL;
	videoStream = -1;

}

FFmpeg::~FFmpeg() {
	sws_freeContext(pSwsCtx);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
}

int FFmpeg::initial(char * url, JNIEnv * e) {
	int err;
	env = e;
	rtspURL = url;
	AVCodec *pCodec;
	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	pFrame = avcodec_alloc_frame();
	err = avformat_open_input(&pFormatCtx, rtspURL, NULL, NULL);
	if (err < 0) {
		printf("Can not open this file");
		return -1;
	}
	if (av_find_stream_info(pFormatCtx) < 0) {
		printf("Unable to get stream info");
		return -1;
	}
	int i = 0;
	videoStream = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	}
	if (videoStream == -1) {
		printf("Unable to find video stream");
		return -1;
	}
	pCodecCtx = pFormatCtx->streams[videoStream]->codec;

	width = pCodecCtx->width;
	height = pCodecCtx->height;
	avpicture_alloc(&picture, PIX_FMT_RGB24, pCodecCtx->width,
			pCodecCtx->height);
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	pSwsCtx = sws_getContext(width, height, PIX_FMT_YUV420P, width, height,
			PIX_FMT_RGB24, SWS_BICUBIC, 0, 0, 0);

	if (pCodec == NULL) {
		printf("Unsupported codec");
		return -1;
	}
	printf("video size : width=%d height=%d \n", pCodecCtx->width,
			pCodecCtx->height);
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		printf("Unable to open codec");
		return -1;
	}
	printf("initial successfully");

	return 0;
}

void FFmpeg::fillPicture(AndroidBitmapInfo* info, void *pixels,
		AVPicture *rgbPicture) {
	uint8_t *frameLine;

	int yy;
	for (yy = 0; yy < info->height; yy++) {
		uint8_t* line = (uint8_t*) pixels;
		frameLine = (uint8_t *) rgbPicture->data[0] + (yy * rgbPicture->linesize[0]);

		int xx;
		for (xx = 0; xx < info->width; xx++) {
			int out_offset = xx * 4;
			int in_offset = xx * 3;

			line[out_offset] = frameLine[in_offset];
			line[out_offset + 1] = frameLine[in_offset + 1];
			line[out_offset + 2] = frameLine[in_offset + 2];
			line[out_offset + 3] = 0xff; //主要是A值
		}
		pixels = (char*) pixels + info->stride;
	}
}

int FFmpeg::h264Decodec(jobject & bitmap) {
	int frameFinished = 0;
	AndroidBitmapInfo info;
	void * pixels;
	int ret = -1;
	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
		LOGE("AndroidBitmap_getInfo() failed ! error");
		//return -1;
	}
	while (av_read_frame(pFormatCtx, &packet) >= 0) {
		if (packet.stream_index == videoStream) {
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
			if (frameFinished) {
				LOGI("***************ffmpeg decodec*******************\n");
				int rs = sws_scale(pSwsCtx,
						(const uint8_t* const *) pFrame->data, pFrame->linesize,
						0, height, picture.data, picture.linesize);

				if (rs == -1) {
					LOGE(
							"__________Can open to change to des imag_____________e\n");
					return -1;
				}
				if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels))
						< 0) {
					LOGE("AndroidBitmap_lockPixels() failed ! error");
					//return -1;
				}
				//pixels = picture.data[0] + info.stride;
				fillPicture(&info,pixels,&picture);
				AndroidBitmap_unlockPixels(env, bitmap);
			}
		}
	}
	return 1;

}
