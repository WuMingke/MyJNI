package com.example.myjni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.myjni.databinding.ActivityMainBinding;

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


    public void testFun(View view) {
//        testQuote();
        delQuote();
    }
}