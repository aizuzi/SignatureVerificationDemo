#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <malloc.h>
#include <string>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "dedao" ,__VA_ARGS__) // 定义LOGE类型

// 花指令
std::string reverseString(std::string s) {
    int len = s.length();
    int mid = len / 2;
    for (int i = 0; i < mid; i++) {
        int t = len - i - 1;
        s[i] ^= s[t];
        s[t] ^= s[i];
        s[i] ^= s[t];
    }
    return s;
}

const char *app_signature =
        "签名的SHA1值，通过以下方法获得："
        "1. 通过命令：keytool -list -rfc -keystore your.keystore"
        "2. 并用BASE64 解密";

bool sign_sha1_verify(JNIEnv *env, jclass clz, jobject context_object) {
    jclass context_class = env->GetObjectClass(context_object);

    jmethodID methodId = env->GetMethodID(context_class, "getPackageManager",
                                          "()Landroid/content/pm/PackageManager;");
    jobject package_manager_object = env->CallObjectMethod(context_object, methodId);
    if (package_manager_object == NULL) {
        LOGE("getPackageManager() Failed!");
        return false;
    }

    methodId = env->GetMethodID(context_class, "getPackageName", "()Ljava/lang/String;");
    jstring package_name_string = (jstring) env->CallObjectMethod(context_object, methodId);
    if (package_name_string == NULL) {
        LOGE("getPackageName() Failed!");
        return false;
    }
    env->DeleteLocalRef(context_class);

    jclass pack_manager_class = env->GetObjectClass(package_manager_object);
    methodId = env->GetMethodID(pack_manager_class, "getPackageInfo",
                                "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    env->DeleteLocalRef(pack_manager_class);
    jobject package_info_object = env->CallObjectMethod(package_manager_object, methodId,
                                                        package_name_string, 0x40);
    if (package_info_object == NULL) {
        LOGE("getPackageInfo() Failed!");
        return false;
    }
    env->DeleteLocalRef(package_manager_object);

    jclass package_info_class = env->GetObjectClass(package_info_object);
    jfieldID fieldId = env->GetFieldID(package_info_class, "signatures",
                                       "[Landroid/content/pm/Signature;");
    env->DeleteLocalRef(package_info_class);
    jobjectArray signature_object_array = (jobjectArray) env->GetObjectField(package_info_object,
                                                                             fieldId);
    if (signature_object_array == NULL) {
        LOGE("PackageInfo.signatures[] is null");
        return false;
    }
    jobject signature_object = env->GetObjectArrayElement(signature_object_array, 0);
    env->DeleteLocalRef(package_info_object);

    //Signature.toByteArray()
    jclass signature_class = env->GetObjectClass(signature_object);
    methodId = env->GetMethodID(signature_class, "toCharsString", "()Ljava/lang/String;");
    env->DeleteLocalRef(signature_class);
    jstring signature_jstirng = (jstring) env->CallObjectMethod(signature_object, methodId);

    const char *sign = env->GetStringUTFChars(signature_jstirng, NULL);

    if (strcmp(sign, app_signature) == 0) {
        LOGE("验证通过");
        return true;
    }
    return false;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_haoshiditu_keysosafe_ApiKeyGeneractor_keyFromJNI(JNIEnv *env, jclass type, jobject context) {

    if (sign_sha1_verify(env, type, context)) {
        // 只有个别有用，花指令，加大破解难度
        std::string key_1 = "1234";
        std::string key_2 = "5678";
        std::string key_3 = "^re231x&%%h";
        std::string key_4 = "9c^4123x&%%h";
        std::string key_5 = "AA$#@!x&%%h";
        std::string key_6 = "823vv%$#@x&%%h";
        std::string key_7 = "64vvc！#@￥h44";
        char *real_sha1 = (char *) malloc(300 * sizeof(char));
        strcat(strcat(real_sha1, key_1.c_str()),
               key_2.c_str());

        // 用自己写的加密，避免库函数
        std::string need = reverseString(real_sha1);
        return env->NewStringUTF(need.c_str());
    }
    return NULL;
}
