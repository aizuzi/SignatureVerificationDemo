#include <jni.h>
#include <string>
#include"valid.cpp"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_aizuzi_verificationdemo_MainActivity_getSignaturesSha1(
        JNIEnv *env,
        jobject,
        jobject contextObject) {

    return env->NewStringUTF(app_sha1);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_aizuzi_verificationdemo_MainActivity_checkSha1(
        JNIEnv *env,
        jobject,
        jobject contextObject) {

char *sha1 = getSha1(env,contextObject);

jboolean result = checkValidity(env,sha1);

return result;
}