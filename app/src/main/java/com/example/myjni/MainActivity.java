package com.example.myjni;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Looper;
import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.myjni.databinding.ActivityMainBinding;

import java.io.Serializable;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'myjni' library on application startup.
    static {
        System.loadLibrary("myjni");
    }

    private ActivityMainBinding binding;

    public String name = "mingke";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        /****************************************************/
        // 调用JNI
        changeName();
        tv.setText(name);

//        int[] ints = {1, 2, 3};
//        String[] strings = {"aa", "bb", "cc"};
//        testArrayAction(100, "test information", ints, strings);
//        for (int i : ints) {
//            Log.i("MINGKE", "onCreate: " + i);
//        }


        Student student = new Student();
        student.setId(11);
        student.setName("erk");
        putObject(student, "erkuai");
        Log.i("MINGKE", "onCreate: student:" + student.toString());

        insertObject();


        Parcel parcel = Parcel.obtain();
        parcel.writeInt(2);
        parcel.writeString("");

    }

    /**
     * A native method that is implemented by the 'myjni' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void changeName();

    public native void testArrayAction(int count, String testInfo, int[] ints, String[] strs);

    public native void putObject(Student student, String str); // 传递引用类型，传递对象

    public native void insertObject();

    public native void testQuote(); // 测试 局部引用 全局引用

    public native void delQuote(); // 手动释放全局引用

    /**
     * 以上 都属于 静态注册，
     * 缺点：
     * 1 JNI函数名非常长
     * 2 捆绑上层 包名+类名
     * 3 运行期才会去匹配JNI函数，性能上低于动态注册
     * 优点：
     * 开发简单
     */

    public native void dynamicRegister();

    public native void dynamicRegister2(String str);


    public native void nativeThread();

    public native void sort(int[] array);

    public static String name1 = "erkuai";

    public static native void localCache(String name); // 普通局部缓存


    {
        initStaticCache(); // 静态缓存在构造函数中初始化
    }

    public static native void initStaticCache(); // 初始化静态缓存

    public static native void staticCache(String name);

    public static native void clearStaticCache(); // 清除化静态缓存

    public static native void exception(); // 异常

    public static native void exceptionNativeToJava() throws NoSuchFieldError; // native 抛给 java ,抛出来可以接收

    public native void exceptionJavaToNative(); // java 抛给 native



    public void testFun(View view) {
//        testQuote();
//        delQuote();

//        dynamicRegister();
//        dynamicRegister2("erkuai");

//        nativeThread();
//        int[] ints = {0, -1, 3, 4};
//        sort(ints);
//        for (int anInt : ints) {
//            Log.i("MINGKE", "ndk qsort : " + anInt);
//        }

//        localCache("newerkuai");
//        Log.i("MINGKE", "activity localCache: name1 = " + name1);
//        staticCache("static new erkuai");
//        Log.i("MINGKE", "activity staticCache: name1 = " + name1);

//        exception();
//        try {
//            exceptionNativeToJava();
//        } catch (NoSuchFieldError e) {
//            Log.i("MINGKE", "exceptionNativeToJava 的异常: " + e.toString());
//        }
        exceptionJavaToNative();
    }


    // 被native调用的java方法
    public void updateActivityUI() {
        if (Looper.getMainLooper() == Looper.myLooper()) {
            new AlertDialog.Builder(this)
                    .setTitle("UI")
                    .setMessage("updateActivityUI Activity UI ...")
                    .setPositiveButton("i know", null)
                    .show();
        } else {
            runOnUiThread(() -> new AlertDialog.Builder(MainActivity.this)
                    .setTitle("UI")
                    .setMessage("child thread updateActivityUI Activity UI ...")
                    .setPositiveButton("i know", null)
                    .show());
        }

    }

    public void show() throws Exception {
        throw new NullPointerException("exception from java");
    }

}