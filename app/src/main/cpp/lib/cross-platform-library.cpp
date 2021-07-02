#include "cross-platform-library.h"

std::string cpl::HelloMessage(std::string from) {
    return "Hello from C++ and " + from;
}

int cpl::AddOne(int x) {
    return x + 1;
}

bool cpl::Database::CreateConnection(std::string db_path, std::string* err) {
    if (connected_ == true) {
        if (db_path == db_path)
            return connected_;
        else
            CloseConnection();
    }

    this->db_path_ = db_path;

    int result = sqlite3_open_v2(
            this->db_path_.c_str(),
            &connection_,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
            NULL
    );

    if (result == SQLITE_OK) {
        connected_ = true;
    } else {
        connected_ = false;
        *err = sqlite3_errmsg(connection_);
        sqlite3_close_v2(connection_);
    }
    return connected_;
}

bool cpl::Database::CloseConnection() {
    if (connected_) {
        if (sqlite3_close_v2(connection_) == SQLITE_OK)
            connected_ = false;
    }
    return !connected_;
}

void cpl::Database::CreateUserTable() {
    if (connected_ == false) return;

    sqlite3_exec(
            connection_,
            "CREATE TABLE IF NOT EXISTS USER (" \
        "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
        "NAME TEXT NOT NULL" \
        ");",
            nullptr,
            nullptr,
            nullptr
    );
}

void cpl::Database::CreateArticleTable() {
    if (connected_ == false) return;

    sqlite3_exec(
            connection_,
            "CREATE TABLE IF NOT EXISTS ARTICLE (" \
        "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
        "AUTHOR_ID INTEGER NOT NULL," \
        "HEADLINE INTEGER NOT NULL," \
        "CONTENT INTEGER NOT NULL" \
        ");",
            nullptr,
            nullptr,
            nullptr
    );
}

int cpl::Database::InsertUser(User user) {
    if (connected_ == false) return -1;

    sqlite3_exec(
            connection_,
            ("INSERT INTO USER ('NAME') VALUES ('" + user.name + "');").c_str(),
            nullptr,
            nullptr,
            nullptr
    );

    return sqlite3_last_insert_rowid(connection_);
}

int cpl::Database::InsertArticle(Article article) {
    if (connected_ == false) return -1;

    sqlite3_exec(
            connection_,
            ("INSERT INTO ARTICLE " \
        "('AUTHOR_ID', 'HEADLINE', 'CONTENT') VALUES " \
        "('" + std::to_string(article.author_id) + "', '" + article.headline + "', '" + article.content + "');").c_str(),
            nullptr,
            nullptr,
            nullptr
    );

    return sqlite3_last_insert_rowid(connection_);
}

void cpl::Database::GetUser(int id, User* user) {
    if (connected_ == false) return;

    sqlite3_exec(
            connection_,
            ("SELECT * FROM USER WHERE ID = " + std::to_string(id) + ";").c_str(),
            [] (void* ctx, int argc, char **argv, char **columnName) -> int
            {
                for(int i = 0; i < argc; i++) {
                    static_cast<User*>(ctx)->id = std::stoi(argv[0]);
                    static_cast<User*>(ctx)->name = argv[1];
                }
                return 0;
            },
            user,
            nullptr
    );
}

void cpl::Database::GetArticle(int id, Article* article) {
    if (connected_ == false) return;

    sqlite3_exec(
            connection_,
            ("SELECT * FROM ARTICLE WHERE ID = " + std::to_string(id) + ";").c_str(),
            [] (void* ctx, int argc, char **argv, char **columnName) -> int
            {
                for(int i = 0; i < argc; i++) {
                    static_cast<Article*>(ctx)->id = std::stoi(argv[0]);
                    static_cast<Article*>(ctx)->author_id = std::stoi(argv[1]);
                    static_cast<Article*>(ctx)->headline = argv[2];
                    static_cast<Article*>(ctx)->content = argv[3];
                }
                return 0;
            },
            article,
            nullptr
    );
}

void cpl::Database::GetAllUsers(std::vector<User>* users) {
    if (connected_ == false) return;

    sqlite3_exec(
            connection_,
            "SELECT * FROM USER;",
            [] (void* ctx, int argc, char **argv, char **columnName) -> int
            {
                static_cast<std::vector<User>*>(ctx)->push_back(User(
                        std::stoi(argv[0]),
                        argv[1]
                ));
                return 0;
            },
            users,
            nullptr
    );
}

void cpl::Database::GetAllArticles(std::vector<Article>* articles) {
    if (connected_ == false) return;

    sqlite3_exec(
            connection_,
            "SELECT * FROM ARTICLE;",
            [] (void* ctx, int argc, char **argv, char **columnName) -> int
            {
                static_cast<std::vector<Article>*>(ctx)->push_back(Article(
                        std::stoi(argv[0]),
                        std::stoi(argv[1]),
                        argv[2],
                        argv[3]
                ));
                return 0;
            },
            articles,
            nullptr
    );
}

void cpl::Database::SetupTestData() {
    if (connected_ == false) return;

    sqlite3_exec(connection_, "DROP TABLE IF EXISTS USER;", nullptr, nullptr, nullptr);
    CreateUserTable();
    int idUser1 = InsertUser(User("User Nr.1"));
    int idUser2 = InsertUser(User("User Nr.2"));

    sqlite3_exec(connection_, "DROP TABLE IF EXISTS ARTICLE;", nullptr, nullptr, nullptr);
    CreateArticleTable();
    InsertArticle(Article(idUser1, "Headline 1 by User1", "Lorem Ipsum dolor sit amet."));
    InsertArticle(Article(idUser1, "Headline 2 by User1", "Lorem Ipsum dolor sit amet."));
    InsertArticle(Article(idUser1, "Headline 3 by User1", "Lorem Ipsum dolor sit amet."));
    InsertArticle(Article(idUser2, "Headline 4 by User2", "Lorem Ipsum dolor sit amet."));
    InsertArticle(Article(idUser2, "Headline 5 by User2", "Lorem Ipsum dolor sit amet."));

    std::vector<User> users;
    GetAllUsers(&users);
    std::vector<Article> articles;
    GetAllArticles(&articles);
}

cpl::User::User(std::string name) : name{name} {}
cpl::User::User(int id, std::string name) : id{id}, name{name} {}

cpl::Article::Article(int author_id, std::string headline, std::string content) :
        author_id{author_id},
        headline{headline},
        content{content}
{}
cpl::Article::Article(int id, int author_id, std::string headline, std::string content) :
        id{id},
        author_id{author_id},
        headline{headline},
        content{content}
{}