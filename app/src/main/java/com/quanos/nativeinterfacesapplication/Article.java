package com.quanos.nativeinterfacesapplication;

public class Article {
    int id;
    int authorId;
    String headline;
    String content;

    public Article(int id, int authorId, String headline, String content) {
        this.id = id;
        this.authorId = authorId;
        this.headline = headline;
        this.content = content;
    }
}
