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



        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        NativeFunctions nativeFunctions = NativeFunctions.getInstance();

        tv.setText(nativeFunctions.HelloMessage("Java"));

        Button showArticlesButton = findViewById(R.id.showArticlesButton);
        showArticlesButton.setOnClickListener(v -> {
            // Run test
            long t1 = new Date().getTime();
            int testNumber = 0;
            for (int i = 0; i < 10000000; i++) {
                testNumber = nativeFunctions.AddOne(testNumber);
            }
            long t2 = new Date().getTime();
            tv.setText("Test took " + (t2-t1) + "ms");

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
        });
    }
}