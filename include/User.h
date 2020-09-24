
#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    User(const User &other, std::string name);
    User(const User &other);//copy constructor
    User& operator=(const User &other);//assignment operator
    User(User &&other);// move constructor
    User& operator=(User&& other);//move assignment operator
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual std::string typeOfUser() const = 0;
    std::string getName();
    std::vector<Watchable*> get_history() const;
    void addToHistory(Watchable* watch);
    virtual User* clone() = 0;
    void copy(const User &other);
    void clear();
    virtual ~User();
protected:
    std::vector<Watchable*> history;
private:
    std::string name;
};

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    LengthRecommenderUser(const User &other, std::string name);//copy constructor
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string typeOfUser() const;
    LengthRecommenderUser* clone();
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser(const User &other, std::string name);//copy constructor
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string typeOfUser() const;
    RerunRecommenderUser* clone();
private:
    int historyIndex;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    GenreRecommenderUser(const User &other, std::string name);//copy constructor
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string typeOfUser() const;
    std::string getPopularTag(std::unordered_map<std::string,int> genreMap);
    std::string lexicographicTag(std::vector<std::string> &tags);
    GenreRecommenderUser* clone();
private:
};

#endif