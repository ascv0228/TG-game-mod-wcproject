#pragma once

void JNI_Changes(JNIEnv *env, jclass clazz, jobject obj, jint feature,
             jint value, jboolean boolean, jstring str);

jstring JNI_Export(JNIEnv *env, jclass clazz);