#include <jni.h>
#include <string>

#include "inc/cross-platform-library.h"

bool checkExc(JNIEnv *env) {
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe(); // writes to logcat
        env->ExceptionClear();
        return true;
    }
    return false;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_quanos_nativeinterfacesapplication_NativeFunctions_stringFromJNI(
        JNIEnv *env, jobject thiz, jstring arg1) {
    if (arg1 == NULL) {
        jclass exception = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(exception, "Passed argument is null."); // Error Message
        return NULL;
    }

    jboolean isCopy;
    std::string cppArg1 = env->GetStringUTFChars(arg1, &isCopy);

    /**
     * Use library BEGIN
     */

    std::string hello = cpl::HelloMessage(cppArg1);

    /**
     * Use library END
     */

    return env->NewStringUTF(hello.c_str());
}