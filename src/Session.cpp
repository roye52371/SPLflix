//
// Created by royek on 20/11
// /2019.
//

#include "../include/Session.h"
#include "../include/Watchable.h"
#include "fstream"
#include "../include/json.hpp"
#include "../include/User.h"
#include "../include/Action.h"

using namespace std;

Session::Session(const std::string &configFilePath): content(), actionsLog(), userMap(), activeUser(), act() {
    using json= nlohmann::json;
    std::ifstream config(configFilePath);
    json stream;
    config >> stream;
    long idCounter(1);
    int movieIndex(0);
    while(stream["movies"][movieIndex] != nullptr) {
        string name = stream["movies"][movieIndex]["name"];
        int length = stream["movies"][movieIndex]["length"];
        int tagsIndex(0);
        vector<string> vTags;
        while(stream["movies"][movieIndex]["tags"][tagsIndex] != nullptr) {
            vTags.push_back(stream["movies"][movieIndex]["tags"][tagsIndex]);
            tagsIndex++;
        }
        content.push_back(new Movie(idCounter, name, length, vTags));
        idCounter++;
        movieIndex++;
    }
    int tvIndex(0);
    while(stream["tv_series"][tvIndex] != nullptr) {
        string name = stream["tv_series"][tvIndex]["name"];
        int epLength = stream["tv_series"][tvIndex]["episode_length"];
        int tagsIndex(0);
        vector<string> vTags;
        while(stream["tv_series"][tvIndex]["tags"][tagsIndex] != nullptr) {
            vTags.push_back(stream["tv_series"][tvIndex]["tags"][tagsIndex]);
            tagsIndex++;
        }
        int seasonIndex(0);
        while(stream["tv_series"][tvIndex]["seasons"][seasonIndex] != nullptr) {
            int episodes = stream["tv_series"][tvIndex]["seasons"][seasonIndex];
            for (int i = 1; i <= episodes; ++i) {
                content.push_back(new Episode(idCounter, name, epLength, seasonIndex + 1, i, vTags));
                idCounter++;
            }
            seasonIndex++;
        }
        tvIndex++;
    }

    this->activeUser = new LengthRecommenderUser("default");
    userMap.insert({"default", activeUser});

}

Session::Session(const Session &other): content(), actionsLog(), userMap(), activeUser(), act(other.act) {
    copy(other);
}

Session& Session::operator=(const Session &other) {
    if(this==&other){
        return *this;
    }
    clear();
    this->act = other.act;
    copy(other);
    return *this;
}

Session::Session(Session &&other):  content(), actionsLog(), userMap(), activeUser(other.activeUser), act(other.act) {
    copy(other);
    other.setActiveUser(nullptr);
    other.setAct("");
    other.clear();
}

Session& Session::operator=(Session &&other) {
    if(this==&other){
        return *this;
    }
    clear();
    act = other.act;
    copy(other);
    other.setActiveUser(nullptr);
    other.setAct("");
    other.clear();
    return *this;
}

void Session::start() {
    cout<<"SPLFLIX is now on!"<<endl;
    std::string action;
    getline(cin,action);
    BaseAction *a;
    while(action != "exit") {
        std::string ac = splitAction(action);
        if(ac=="createuser") {
            int spaces = numOfSpaces(action);
            if(spaces == 2) {
                this->act = action;
                a = new CreateUser();
                a->act(*this);
                delete a;
            }
        }
        else if(ac=="changeuser") {
            int spaces = numOfSpaces(action);
            if(spaces == 1) {
                this->act = action;
                a = new ChangeActiveUser();
                a->act(*this);
                delete a;
            }
        }
        else if(ac=="deleteuser") {
            int spaces = numOfSpaces(action);
            if(spaces == 1) {
                this->act = action;
                a = new DeleteUser();
                a->act(*this);
                delete a;
            }
        }
        else if(ac=="dupuser") {
            int spaces = numOfSpaces(action);
            if(spaces == 2) {
                this->act = action;
                a = new DuplicateUser();
                a->act(*this);
                delete a;
            }
        }
        else if(ac=="watch") {
            int spaces = numOfSpaces(action);
            if(spaces == 1) {
                this->act = action;
                a = new Watch();
                a->act(*this);
                delete a;
            }
        }
        else if(ac=="watchhist") {
            int spaces = numOfSpaces(action);
            if(spaces == 0) {
                this->act = action;
                a = new PrintWatchHistory();
                a->act(*this);
                delete a;
            }
        }
        else if(ac=="content") {
            int spaces = numOfSpaces(action);
            if(spaces == 0) {
                this->act = action;
                a = new PrintContentList();
                a->act(*this);
                delete a;
            }
        }
        else if(ac=="log") {
            int spaces = numOfSpaces(action);
            if(spaces == 0) {
                this->act = action;
                a = new PrintActionsLog();
                a->act(*this);
                delete a;
            }
        }

        getline(cin,action);
    }
}

std::string Session::splitAction(std::string action) {
    if(action.find(" ") < action.size())
        return action.substr(0, action.find(" "));
    return action;
}

int Session::numOfSpaces(std::string action) {
    int spaces = 0;
    for (unsigned int i = 0; i < action.length(); i++) {
        if (action.at(i) == ' ')
            spaces++;
    }
    return spaces;
}

User& Session::getUser() { return *activeUser; }
const std::string& Session::getAct() { return  act; }
std::unordered_map<std::string,User*>& Session::getUserMap() { return userMap; }
std::vector<BaseAction*>& Session::getActionsLog() { return actionsLog; }
std::vector<Watchable*>& Session::getContent() { return content; }
void Session::setActiveUser(User *activeUser) { this->activeUser=activeUser;}
void Session::setAct(std::string act) { this->act = act; }

void Session::copy(const Session &other) {
    for (unsigned int i = 0; i < other.content.size(); ++i) {
        content.push_back(other.content.at(i)->clone());
    }
    for (unsigned int j = 0; j < other.actionsLog.size(); ++j) {
        actionsLog.push_back(other.actionsLog.at(j)->clone());
    }
    for (auto l = other.userMap.begin(); l != other.userMap.end() ; ++l) {
        userMap.insert({l -> first, l -> second -> clone()});
    }
    activeUser = userMap.find(other.activeUser->getName()) -> second;
}

void Session::clear() {
    for (unsigned int i = 0; i <content.size() ; i=i+1) {
        delete content[i];
    }
    for (unsigned int j = 0; j < actionsLog.size(); ++j) {
        delete actionsLog[j];
    }
    for (auto l = userMap.begin(); l != userMap.end() ; ++l) {
        delete l -> second;
    }
    content.clear();
    actionsLog.clear();
    userMap.clear();
}

Session::~Session() { clear(); }