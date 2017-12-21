# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#
#http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)
LOCAL_CPP_EXTENSION := .cpp
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/ffmpeg \
#$(LOCAL_PATH)/libs
		 
LOCAL_LDLIBS := -llog -lz -L $(LOCAL_PATH)/libs -lffmpeg -lm -L$(SYSROOT)/usr/lib -llog -ljnigraphics
LOCAL_MODULE :=rtspclient
LOCAL_SRC_FILES := RtspClient.cpp FFmpeg.cpp FFmpeg.h

include $(BUILD_SHARED_LIBRARY)