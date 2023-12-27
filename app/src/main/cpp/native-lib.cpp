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


// 动态注册 开始
void dynamicMethod01(JNIEnv *env, jobject thiz) {
    LOGI("动态注册的函数 dynamicMethod01 ...");
}

void dynamicMethod02(JNIEnv *env, jobject thiz, jstring str) {
    LOGI("动态注册的函数 dynamicMethod02 ... value:%s ", env->GetStringUTFChars(str, nullptr));
}


JavaVM *javaVm = nullptr;
const char *mainActivityClassName = "com/example/myjni/MainActivity";
const char *javaMethod1 = "dynamicRegister";
const char *javaMethod2 = "dynamicRegister2";
static const JNINativeMethod jniNativeMethod[] = {
        {javaMethod1, "()V",                   (void *) (dynamicMethod01)},
        {javaMethod2, "(Ljava/lang/String;)V", (void *) (dynamicMethod02)},
};

// System.loadLibrary("myjni") 默认会调用 JNI_OnLoad
// 相当于重写 JNI_OnLoad
JNIEXPORT jint JNI_OnLoad(JavaVM *javaVm, void *) {

    ::javaVm = javaVm; // this.javaVm = javaVm;

    JNIEnv *jniEnv = nullptr;
    int result = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);
    if (result != JNI_OK) { // result == 0 成功
        return JNI_ERR; // -1 崩溃
    }

    LOGI("JNI_OnLoad start ...");


    jclass mainActivityClass = jniEnv->FindClass(mainActivityClassName);

    jniEnv->RegisterNatives(mainActivityClass, jniNativeMethod,
                            sizeof(jniNativeMethod) / sizeof(JNINativeMethod));

    LOGI("JNI_OnLoad end ...");

    return JNI_VERSION_1_6;
}

// 动态注册 结束


class MyContext {
public:
    JNIEnv *jniEnv = nullptr;
    jobject instance = nullptr;
};

void *myThreadTaskAction(void *pVoid) { // 异步线程
    LOGI("native thread run ...");


    JNIEnv *jniEnv = nullptr;
    jint attachResult = ::javaVm->AttachCurrentThread(&jniEnv, nullptr); // 新建 jniEnv 给子线程用
    if (attachResult != JNI_OK) {
        return 0;
    }

    MyContext *myContext = static_cast<MyContext *>(pVoid);


//    jclass mainActivityClass2 = jniEnv->FindClass(mainActivityClassName); // 这种方式找不到，只能GetObjectClass新建
    jclass mainActivityClass = jniEnv->GetObjectClass(myContext->instance);
    jmethodID updateActivityUI = jniEnv->GetMethodID(mainActivityClass,
                                                     "updateActivityUI", "()V");

    jniEnv->CallVoidMethod(myContext->instance, updateActivityUI);

    ::javaVm->DetachCurrentThread(); // 需要detach

    return 0;
}

// JNIEnv *env 不能跨越线程，【安卓进程只有一个JavaVM，是全局的，可跨越线程】
// jobject thiz 不能跨越线程，不能跨越函数【解决方式：默认是局部引用，提升为全局引用】
// JavaVM 可以跨越线程，可以跨越函数
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_nativeThread(JNIEnv *env, jobject thiz) { // 主线程
//    pthread_t pid;
//    pthread_create(&pid, nullptr, myThreadTaskAction, nullptr);
//    pthread_join(pid, nullptr);

    MyContext *myContext = new MyContext();
//    myContext->jniEnv = env;
//    myContext->instance = thiz;
    myContext->instance = env->NewGlobalRef(thiz);//提升为全局引用

    // 子线程
    pthread_t pid;
    pthread_create(&pid, nullptr, myThreadTaskAction, myContext);
    pthread_join(pid, nullptr);

    // 主线程
//    jclass mainActivityClass = env->FindClass(mainActivityClassName);
//    jmethodID updateActivityUI = env->GetMethodID(mainActivityClass,
//                                                  "updateActivityUI", "()V");
//    env->CallVoidMethod(thiz, updateActivityUI);


    /**
     * 1 JavaVM 全局，绑定当前进程，只有一个地址
     * 2 JNIEnv 绑定线程，
     * 3 jobject 谁调用JNI函数，谁的实例就会给jobject
     */
}


// 调用 NDK 函数 qsort
int compare(const jint *n1, const jint *n2) {
    return *n1 - *n2;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_sort(JNIEnv *env, jobject thiz, jintArray array) {

    jint *intArray = env->GetIntArrayElements(array, nullptr);
    int length = env->GetArrayLength(array);
    qsort(intArray, length, sizeof(int),
          reinterpret_cast<int (*)(const void *, const void *)>(compare));
    env->ReleaseIntArrayElements(array, intArray, 0);
}

// 非静态缓存
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_localCache(JNIEnv *env, jclass clazz, jstring str) {
    jfieldID f_id = nullptr;
    if (f_id == nullptr) {
        f_id = env->GetStaticFieldID(clazz, "name1", "Ljava/lang/String;");
    } else {
        LOGI("空的");
    }

    env->SetStaticObjectField(clazz, f_id, str);
    f_id = nullptr;
}
// 静态缓存1 在构造函数中调用
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_initStaticCache(JNIEnv *env, jclass clazz) {

}
// 静态缓存2 使用
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_staticCache(JNIEnv *env, jclass clazz, jstring str) {
}
// 静态缓存3 清理
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myjni_MainActivity_clearStaticCache(JNIEnv *env, jclass clazz) {
}