package com.quanos.nativeinterfacesapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.widget.Button;
import android.widget.TextView;

import java.time.LocalDateTime;
import java.util.Date;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView helloMessage = findViewById(R.id.sample_text);
        NativeFunctions nativeFunctions = NativeFunctions.getInstance();
        helloMessage.setText(nativeFunctions.HelloMessage("Java"));

        Button showArticlesButton = findViewById(R.id.showArticlesButton);
        showArticlesButton.setOnClickListener(v -> ShowArticlesAndRunTest(helloMessage, nativeFunctions));
    }

    private void ShowArticlesAndRunTest(TextView tv, NativeFunctions nativeFunctions) {
        // Run function call performance test
        int iterations = 10000000;
        int testNumber = 0;
        long t1, t2;

        t1 = new Date().getTime();
        for (int i = 0; i < iterations; i++) {
            testNumber = nativeFunctions.AddOne(testNumber);
        }
        t2 = new Date().getTime();
        long cppTime = t2 - t1;

        testNumber = 0;
        t1 = new Date().getTime();
        for (int i = 0; i < iterations; i++) {
            testNumber = AddOne(testNumber);
        }
        t2 = new Date().getTime();
        long javaTime = t2 - t1;

        long timeDiff = cppTime - javaTime;
        tv.setText("C++ test took " + cppTime + "ms. Java test took " + javaTime + "ms. C++ was " + timeDiff + "ms " + (timeDiff > 0 ? "slower." : "faster."));

        /**
         * Connect to database and read data
         */
        System.out.println("--------------------------------------------------------------");
        System.out.println(getFilesDir().getAbsolutePath() + "/android_test.db");
        System.out.println(nativeFunctions.openDatabaseConnection(getFilesDir().getAbsolutePath() + "/android_test.db"));
        nativeFunctions.setupTestData();
        User[] users = nativeFunctions.getAllUsers();
        Article[] articles = nativeFunctions.getAllArticles();

        StringBuilder output = new StringBuilder();
        for (Article article: articles) {
            for (User user: users) {
                if (article.authorId == user.id) {
                    output.append("----- Article -----\n");
                    output.append("id: " + article.id + "\n");
                    output.append("headline: " + article.headline + "\n");
                    output.append("content: " + article.content + "\n");
                    output.append("authorId: " + article.authorId + "\n");
                    output.append("authorName: " + user.name + "\n");
                }
            }
        }
        output.append("-------------------");
        System.out.println(output);
        System.out.println(nativeFunctions.closeDatabaseConnection());

        TextView articlesTextView = findViewById(R.id.articlesTextView);
        articlesTextView.setMovementMethod(new ScrollingMovementMethod());
        articlesTextView.setText(output);
    }

    private int AddOne(int x) {
        return x + 1;
    }
}