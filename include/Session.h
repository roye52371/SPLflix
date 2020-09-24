
#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
using namespace std;
class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    Session(const Session &other);//copy constructor
    Session& operator=(const Session &other);//assignment operator
    Session(Session &&other);// move constructor
    Session& operator=(Session&& other);//move assignment operator
    void copy(const Session &other);
    void start();
    void clear();
    ~Session();
    User& getUser();
    const std::string& getAct();
    std::unordered_map<std::string,User*>& getUserMap();
    std::vector<BaseAction*> &getActionsLog();
    std::vector<Watchable*> &getContent();
    void setActiveUser(User *activeUser);
    void setAct(std::string act);
    std::string splitAction(std::string action);
    int numOfSpaces(std::string action);
    private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string act;
};
#endif