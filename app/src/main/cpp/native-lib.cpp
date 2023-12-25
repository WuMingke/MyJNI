#include <jni.h>
#include <string>
#include <android/log.h>

#define TAG "MINGKE"
#define LOGE(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

extern "C" // 采用C标准，禁止函数重载，并且函数编译采用C的方式
// 因为C/C++最终都是调用C的JNINativeInterface
JNIEXPORT // 标记该方法可以被外部调用
jstring   // java <--> native 相互转换用的【翻译官】
JNICALL   // 代表是JNI标记
Java_com_example_myjni_MainActivity_stringFromJNI( //
        JNIEnv *env,  // JNI的桥梁，所有的JNI操作都是通过它
        jobject /* this */) {  // 谁调用函数谁就是 jobject
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_changeName(JNIEnv *env, jobject thiz) {
    // 获取class
    jclass j_cls = env->GetObjectClass(thiz);
    // 获取属性
    jfieldID j_fid = env->GetFieldID(j_cls, "name", "Ljava/lang/String;");
    //
    jstring j_str = static_cast<jstring>(env->GetObjectField(thiz, j_fid));
    char *c_str = const_cast<char *>(env->GetStringUTFChars(j_str, nullptr));
    LOGE("native : %s", c_str);

    // 修改
    jstring res = env->NewStringUTF("111");
    env->SetObjectField(thiz, j_fid, res);
}

