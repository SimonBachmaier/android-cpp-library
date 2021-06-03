#ifndef CROSS_PLATFORM_LIBRARY_H
#define CROSS_PLATFORM_LIBRARY_H

#include "../../../../../../../../../../../Users/Simon/AppData/Local/Android/Sdk/ndk/21.1.6352462/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/c++/v1/string"
#include "../../../../../../../../../../../Users/Simon/AppData/Local/Android/Sdk/ndk/21.1.6352462/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/c++/v1/vector"
#include "../../../../../../../../../../../Users/Simon/AppData/Local/Android/Sdk/ndk/21.1.6352462/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/c++/v1/iostream"

#include "sqlite3.h"

namespace cpl {
    std::string HelloMessage(std::string from);

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

        // std::string TestConnection();
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