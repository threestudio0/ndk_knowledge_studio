#include <jni.h>
#include <string>
#include <unistd.h>
#include <pthread.h>

struct NativeWorkerArgs
{
    jint id;
    jint iterations;
};

static jmethodID gOnNativeMessage = NULL;
static JavaVM* gVm = NULL;
static jobject gObj = NULL;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    gVm = vm;
    return JNI_VERSION_1_6;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_threestudio_ndk_1knowledge_1studio_MainActivity_nativeWorker(JNIEnv *env, jobject obj,
                                                                      jint id, jint iterations) {

    for(jint i = 0; i < iterations; i++)
    {
        char message[26];
        sprintf(message,"Worker %d: Iteration %d", id,i);
        jstring  messageString = env->NewStringUTF(message);
        env->CallVoidMethod(obj,gOnNativeMessage,messageString);
        if(NULL != env->ExceptionOccurred()){
            break;
        }
        sleep(1);
    }


}


static void* nativeWorkerThread (void* args)
{
    JNIEnv *env = NULL;
    if( 0 == gVm->AttachCurrentThread(&env,NULL))
    {
        NativeWorkerArgs* nativeWorkerArgs = (NativeWorkerArgs*) args;
        Java_com_threestudio_ndk_1knowledge_1studio_MainActivity_nativeWorker(env,
                                                                              gObj,
                                                                              nativeWorkerArgs->id,
                                                                              nativeWorkerArgs->iterations);
        delete nativeWorkerArgs;
        gVm->DetachCurrentThread();
    }


    return (void*) 1;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_threestudio_ndk_1knowledge_1studio_MainActivity_nativeInit(JNIEnv *env, jobject instance) {

    if(NULL == gObj)
    {
        gObj = env->NewGlobalRef(instance);
        if(NULL == gObj)
        {
            goto exit;
        }
    }


    if(NULL == gOnNativeMessage){
        jclass clazz = env->GetObjectClass(instance);
        gOnNativeMessage = env->GetMethodID(clazz,
                "onNativeMessage",
                "(Ljava/lang/String;)V");
    }
    if(NULL == gOnNativeMessage){
        jclass exceptiosnClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptiosnClazz,"Unable to find method");
    }

    exit:
    return;


}

extern "C"
JNIEXPORT void JNICALL
Java_com_threestudio_ndk_1knowledge_1studio_MainActivity_nativeFree(JNIEnv *env, jobject instance) {

    if(NULL == gObj)
    {
        env->DeleteGlobalRef(gObj);
        gObj = NULL;
    }

}


extern "C"
JNIEXPORT void JNICALL
Java_com_threestudio_ndk_1knowledge_1studio_MainActivity_posixThreads(JNIEnv *env, jobject instance,
                                                                      jint Threads,
                                                                      jint iterations) {
    for(jint i=0;i<Threads;i++)
    {
        NativeWorkerArgs* nativeWorkerArgs = new NativeWorkerArgs();
        nativeWorkerArgs->id = i;
        nativeWorkerArgs->iterations = iterations;

        pthread_t thread;
        int result = pthread_create(
                &thread,
                NULL,
                nativeWorkerThread,
                (void*)nativeWorkerArgs);
        if(0 != result)
        {
            jclass exceptionClazz = env->FindClass(
                    "java/lang/RuntimeException"
                    );
            env->ThrowNew(exceptionClazz,"Unable to create thread");
        }
    }




}