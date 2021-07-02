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

extern "C" JNIEXPORT jint JNICALL
Java_com_quanos_nativeinterfacesapplication_NativeFunctions_AddOne(
        JNIEnv *env, jobject thiz, jint x) {
    return cpl::AddOne(x);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_quanos_nativeinterfacesapplication_NativeFunctions_HelloMessage(
        JNIEnv *env, jobject thiz, jstring from) {
    if (from == NULL) {
        jclass exception = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(exception, "Passed argument is null."); // Error Message
        return NULL;
    }

    jboolean isCopy;
    std::string cppArg1 = env->GetStringUTFChars(from, &isCopy);

    /**
     * Use library BEGIN
     */

    std::string hello = cpl::HelloMessage(cppArg1);

    /**
     * Use library END
     */

    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_quanos_nativeinterfacesapplication_NativeFunctions_openDatabaseConnection(
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

    std::string* err;
    cpl::Database& db = cpl::Database::GetInstance();
    if (db.CreateConnection(cppArg1, err) == false)
        return env->NewStringUTF(err->c_str());
    /**
     * Use library END
     */

    return env->NewStringUTF("");
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_quanos_nativeinterfacesapplication_NativeFunctions_closeDatabaseConnection(
        JNIEnv *env, jobject thiz) {
    std::string err;
    cpl::Database& db = cpl::Database::GetInstance();
    if (db.CloseConnection() == false)
        err = "Error closing database.";

    return env->NewStringUTF(err.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_quanos_nativeinterfacesapplication_NativeFunctions_setupTestData(
        JNIEnv *env, jobject thiz) {
    cpl::Database& db = cpl::Database::GetInstance();
    db.SetupTestData();
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_quanos_nativeinterfacesapplication_NativeFunctions_getAllUsers(JNIEnv *env, jobject thiz) {
    cpl::Database& db = cpl::Database::GetInstance();
    std::vector<cpl::User> users;
    db.GetAllUsers(&users);

    jmethodID cnstrctr;
    jclass c = env->FindClass("com/quanos/nativeinterfacesapplication/User");
    if (c == 0) {
        printf("Find Class Failed.\n");
    }else{
        printf("Found class.\n");
    }

    cnstrctr = env->GetMethodID(c, "<init>", "(ILjava/lang/String;)V");
    if (cnstrctr == 0) {
        printf("Find method Failed.\n");
    }else {
        printf("Found method.\n");
    }

    jobjectArray userArray = env->NewObjectArray(users.size(), c, 0);
    int index = 0;
    for (cpl::User user: users) {
        jobject userObject = env->NewObject(c, cnstrctr, user.id, env->NewStringUTF(user.name.c_str()));
        env->SetObjectArrayElement(userArray, index, userObject);
        index += 1;
    }
    return userArray;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_quanos_nativeinterfacesapplication_NativeFunctions_getAllArticles(JNIEnv *env, jobject thiz) {
    cpl::Database& db = cpl::Database::GetInstance();
    std::vector<cpl::Article> articles;
    db.GetAllArticles(&articles);

    jmethodID cnstrctr;
    jclass c = env->FindClass("com/quanos/nativeinterfacesapplication/Article");
    if (c == 0) {
        printf("Find Class Failed.\n");
    }else{
        printf("Found class.\n");
    }

    cnstrctr = env->GetMethodID(c, "<init>", "(IILjava/lang/String;Ljava/lang/String;)V");
    if (cnstrctr == 0) {
        printf("Find method Failed.\n");
    }else {
        printf("Found method.\n");
    }

    jobjectArray articleArray = env->NewObjectArray(articles.size(), c, 0);
    int index = 0;
    for (cpl::Article article: articles) {
        jobject articleObject = env->NewObject(
                c,
                cnstrctr,
                article.id,
                article.author_id,
                env->NewStringUTF(article.headline.c_str()),
                env->NewStringUTF(article.content.c_str())
                );
        env->SetObjectArrayElement(articleArray, index, articleObject);
        index += 1;
    }
    return articleArray;
}