#pragma once

void JNI_LoadSounds(JNIEnv *env, jclass clazz, jstring directory);

jstring JNI_Title(JNIEnv *env, jobject thiz);

jstring JNI_Heading(JNIEnv *env, jobject thiz);

jstring JNI_Icon(JNIEnv *env, jobject thiz);

jstring JNI_IconWebViewData(JNIEnv *env, jobject thiz);

jobjectArray JNI_getFeatureList(JNIEnv *env, jobject activityObject);