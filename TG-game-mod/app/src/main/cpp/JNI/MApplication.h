#pragma once

void JNI_attachBaseContext(JNIEnv *env, jobject thiz, jobject context);

jobject JNI_invoke(JNIEnv *env, jobject thiz, jobject proxy, jobject method,
               jobjectArray args);