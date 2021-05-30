#pragma once

jint JNI_saveCurrent(JNIEnv *env, jobject thiz, jstring tag);

jboolean JNI_loadSelected(JNIEnv *env, jobject thiz, jstring tag);

jobjectArray JNI_loadAccountData(JNIEnv *env, jobject thiz);

jint JNI_deleteSelected(JNIEnv *env, jobject thiz, jobjectArray tags);

jboolean JNI_newAccount(JNIEnv *env, jobject thiz);
