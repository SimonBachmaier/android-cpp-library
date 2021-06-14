package com.quanos.nativeinterfacesapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.widget.Button;
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

        tv.setText(test.stringFromJNI("Java"));


        Button showArticlesButton = findViewById(R.id.showArticlesButton);
        showArticlesButton.setOnClickListener(v -> {
            System.out.println("--------------------------------------------------------------");
            System.out.println(getFilesDir().getAbsolutePath() + "/android_test.db");
            System.out.println(test.openDatabaseConnection(getFilesDir().getAbsolutePath() + "/android_test.db"));
            test.setupTestData();
            User[] users = test.getAllUsers();
            Article[] articles = test.getAllArticles();

            StringBuilder output = new StringBuilder();
            for (Article article: articles) {
                for (User user: users) {
                    if (article.authorId == user.id) {
                        output.append("----- Article -----\n");
                        output.append("id: " + article.id + "\n");
                        output.append("content: " + article.content + "\n");
                        output.append("headline: " + article.headline + "\n");
                        output.append("authorId: " + article.authorId + "\n");
                        output.append("authorName: " + user.name + "\n");
                    }
                }
            }
            output.append("-------------------");
            System.out.println(output);
            System.out.println(test.closeDatabaseConnection());

            TextView articlesTextView = findViewById(R.id.articlesTextView);
            articlesTextView.setMovementMethod(new ScrollingMovementMethod());
            articlesTextView.setText(output);
        });
    }
}