#include <jni.h>
#include <string>
#include <android/log.h>

#define TAG "MINGKE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

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
//    LOGI("native : %s", c_str);

    // 修改
    jstring res = env->NewStringUTF("111");
    env->SetObjectField(thiz, j_fid, res);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_testArrayAction(JNIEnv *env, jobject thiz, jint count,
                                                    jstring test_info, jintArray ints,
                                                    jobjectArray strs) {
    int rInt = count;
    LOGI("native int: %d", rInt);

    const char *info = env->GetStringUTFChars(test_info, nullptr);
    LOGI("native int: %s", info);

    jint *jintArray = env->GetIntArrayElements(ints, nullptr); // 到JNI层
    jsize size = env->GetArrayLength(ints);
    for (int i = 0; i < size; ++i) {
        *(jintArray + i) += 10; // 到 C 层
        LOGI("native int[], %d", *jintArray + i);
    }
    // 0:刷新数据，并释放C++层数据
    env->ReleaseIntArrayElements(ints, jintArray, 0); // 通知改变了Java层

    jsize strLength = env->GetArrayLength(strs);
    for (int i = 0; i < strLength; ++i) {
        jstring jstr = static_cast<jstring>(env->GetObjectArrayElement(strs, i)); // 到JNI层
        const char *strC = env->GetStringUTFChars(jstr, nullptr);  // 到 C 层
        LOGI("native strs[], %s", strC);

        // 释放jstring
        env->ReleaseStringUTFChars(jstr, strC);
    }

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_putObject(JNIEnv *env, jobject thiz, jobject student,
                                              jstring str) {
    const char *strChar = env->GetStringUTFChars(str, nullptr);
    LOGI("native str:%s", strChar);
    env->ReleaseStringUTFChars(str, strChar); // 对象要释放

    jclass studentClass = env->GetObjectClass(student);
    jmethodID setName = env->GetMethodID(studentClass, "setName", "(Ljava/lang/String;)V");
    jmethodID getName = env->GetMethodID(studentClass, "getName", "()Ljava/lang/String;");
    jmethodID showInfo = env->GetStaticMethodID(studentClass, "showInfo",
                                                "(Ljava/lang/String;)V"); // static method
    // 1
    jstring value = env->NewStringUTF("AAA");
    env->CallVoidMethod(student, setName, value);
    // 2
    jstring getNameRes = static_cast<jstring>(env->CallObjectMethod(student, getName));
    const char *strC = env->GetStringUTFChars(getNameRes, nullptr);
    LOGI("native getName:%s", strC);
    env->ReleaseStringUTFChars(getNameRes, strC);
    // 3
    jstring jstringValue = env->NewStringUTF("hello erkuai");
    env->CallStaticVoidMethod(studentClass, showInfo, jstringValue);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_insertObject(JNIEnv *env, jobject thiz) {
    // 通过 包名+类名 的方式获取 Student class
    const char *studentStr = "com/example/myjni/Student";
    jclass studentClass = env->FindClass(studentStr);
    // 创建实例
    jobject student = env->AllocObject(studentClass); // 只实例化，不会调用对象的构造函数
    jmethodID setName = env->GetMethodID(studentClass, "setName", "(Ljava/lang/String;)V");
    jstring value = env->NewStringUTF("eeeee");
    env->CallVoidMethod(student, setName, value);

//    env->NewObject() // 实例化，并调用构造函数


    /*操作Person*/
    const char *personStr = "com/example/myjni/Person";
    jclass personClass = env->FindClass(personStr);
    jobject person = env->AllocObject(personClass);
    jmethodID setStudent = env->GetMethodID(personClass, "setStudent",
                                            "(Lcom/example/myjni/Student;)V");
    env->CallVoidMethod(person, setStudent, student);

    // 释放
    env->DeleteLocalRef(studentClass);
    env->DeleteLocalRef(personClass);
    env->DeleteLocalRef(student);
    env->DeleteLocalRef(person);

//    env->GetStringUTFChars();
//    env->ReleaseStringUTFChars();

    // 以上都是 局部引用，其实可以不手动释放，
}


// 这依然是个 局部引用，JNI函数结束，会释放局部引用，但是还不等于null，是个悬空指针
jclass dogClass;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_testQuote(JNIEnv *env, jobject thiz) {
    if (nullptr == dogClass) {
        const char *dogStr = "com/example/myjni/Dog";
//        dogClass = env->FindClass(dogStr); // 局部引用

        jclass temp = env->FindClass(dogStr);
        dogClass = static_cast<jclass>(env->NewGlobalRef(temp)); // 提升为 全局引用
        env->DeleteLocalRef(temp);
    }
    jmethodID init = env->GetMethodID(dogClass, "<init>", "()V");
    jobject dog = env->NewObject(dogClass, init);

    env->DeleteLocalRef(dog);
    env->DeleteLocalRef(dogClass);

//    dogClass = nullptr; // 这里要加上，避免悬空指针，或升级为 全局引用 == 》不主动释放，需要手动释放
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_delQuote(JNIEnv *env, jobject thiz) {
    // 手动释放全局引用
    if (nullptr != dogClass) {
        env->DeleteLocalRef(dogClass);
        dogClass = nullptr;
    }
}

// 声明全局变量和函数
extern int age;

extern void show();