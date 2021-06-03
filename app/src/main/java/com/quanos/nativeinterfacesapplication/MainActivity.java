package com.quanos.nativeinterfacesapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        NativeFunctions test = NativeFunctions.getInstance();

        try {
            System.out.println("------------> Try");
            tv.setText(test.stringFromJNI(null));
            wait(5000);
        }
        catch (Exception e) {
            tv.setText(e.getMessage());
            System.out.println("------------> Catch");
            System.out.println("Error in JNI function call: " + e.getMessage());
        }

        tv.setText(test.stringFromJNI("Java"));
    }
}