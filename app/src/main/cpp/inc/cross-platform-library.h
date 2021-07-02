#ifndef CROSS_PLATFORM_LIBRARY_H
#define CROSS_PLATFORM_LIBRARY_H

#include <string>
#include <vector>
#include <iostream>

#include "sqlite3.h"

namespace cpl {
    std::string HelloMessage(std::string from);
    int AddOne(int x);

    class Database;
    class User;
    class Article;
};

class cpl::Database {
private:
    std::string db_path_;
    sqlite3* connection_;
    bool connected_;

    Database() {};

public:
    Database(Database const&) = delete;
    void operator=(Database const&) = delete;
    static Database& GetInstance()
    {
        static Database instance;
        return instance;
    }

    bool CreateConnection(std::string db_path, std::string* err);
    bool CloseConnection();

    void SetupTestData();

    void CreateUserTable();
    int InsertUser(User user);
    void GetUser(int id, User* user);
    void GetAllUsers(std::vector<User>* users);

    void CreateArticleTable();
    int InsertArticle(Article article);
    void GetArticle(int article_id, Article* article);
    void GetAllArticles(std::vector<Article>* articles);
};

class cpl::User {
public:
    int id;
    std::string name;

    User(std::string name);
    User(int id, std::string name);
};

class cpl::Article {
public:
    int id;
    int author_id;
    std::string headline;
    std::string content;

    Article(int author_id, std::string headline, std::string content);
    Article(int id, int author_id, std::string headline, std::string content);
};

#endif /* CROSS_PLATFORM_LIBRARY_H */