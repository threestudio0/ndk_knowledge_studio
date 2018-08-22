#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_threestudio_ndk_1knowledge_1studio_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_threestudio_ndk_1knowledge_1studio_MainActivity_nativeInit(JNIEnv *env, jobject instance) {

    // TODO

}

extern "C"
JNIEXPORT void JNICALL
Java_com_threestudio_ndk_1knowledge_1studio_MainActivity_nativeFree(JNIEnv *env, jobject instance) {

    // TODO

}

extern "C"
JNIEXPORT void JNICALL
Java_com_threestudio_ndk_1knowledge_1studio_MainActivity_nativeWorker(JNIEnv *env, jobject instance,
                                                                      jint id, jint iterations) {

    // TODO

}