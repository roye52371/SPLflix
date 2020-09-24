//
// Created by royek on 20/11/2019.
//

#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/User.h"

User::User(const std::string &name): history(), name(name) {}

User::User(const User &other, std::string name): history(), name(name) { copy(other); }

User::User(const User &other): User(other, other.name) { }

User& User::operator=(const User &other) {
    if(this == &other){
        return *this;
    }
    clear();
    this->name = other.name;
    copy(other);
    return *this;
}

User::User(User &&other): history(), name() {
    copy(other);
    other.name = "";
    other.clear();
}

User& User::operator=(User &&other) {
    if(this==&other){
        return *this;
    }
    clear();
    this->name = other.name;
    copy(other);
    other.name = "";
    other.clear();
    return *this;
}

void User::addToHistory(Watchable *watch) {
    history.push_back(watch->clone());
}

std::vector<Watchable*> User::get_history() const { return history; }
std::string User::getName() { return name; }

void User::copy(const User &other) {
    std::vector<Watchable*> otherHistory = other.get_history();
    for (unsigned int i = 0; i < otherHistory.size(); ++i) {
        history.push_back(otherHistory.at(i)->clone());
    }
}

void User::clear() {
    for (unsigned int i = 0; i < history.size(); ++i) {
        delete history[i];
    }
    history.clear();
}

User::~User() { clear(); }


//GENRE RECOMMENDER USER
GenreRecommenderUser::GenreRecommenderUser(const std::string &name): User(name) {}

GenreRecommenderUser::GenreRecommenderUser(const User &other, std::string name): User(other, name) {}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    User &activeUser = s.getUser();
    std::vector<Watchable*> content= s.getContent();
    std::vector<Watchable*> history= activeUser.get_history();
    std::unordered_map<std::string,int> genreMap;
    for (unsigned int i = 0; i < content.size(); ++i) {
        std::vector<std::string> tags = content.at(i)->getTags();
        for (unsigned int j = 0; j < tags.size(); ++j) {
            genreMap.insert({tags.at(j), 0});
        }
    }
    for (unsigned int i = 0; i < history.size(); ++i) {
        std::vector<std::string> tags = history.at(i)->getTags();
        for (unsigned int j = 0; j < tags.size(); ++j) {
                genreMap.at(tags.at(j))++;// increasing num of mofaimm of the current tag
        }
    }

    std::string popularTag = getPopularTag(genreMap); // taking the best tag checking lexicographic order
    while(popularTag != "") {
        for (unsigned int k = 0; k < content.size(); ++k) {
            std::vector<std::string> tags = content.at(k)->getTags();
            for (unsigned int i = 0; i < tags.size(); ++i) {
                if (tags.at(i) == popularTag) {
                    bool flag = false;
                    for (unsigned int j = 0; (j < history.size()) & (!flag); ++j) {
                        if (history.at(j)->toString() == content.at(k)->toString())
                            flag = true;
                    }
                    if (!flag)
                        return content.at(k);
                }
            }
        }
        genreMap.erase(popularTag);
        popularTag = getPopularTag(genreMap);
    }
    return nullptr;// if NO matching content to genre algorithm has found

}

std::string GenreRecommenderUser::getPopularTag(std::unordered_map<std::string, int> genreMap) {
    int max = 0;
    for (auto l = genreMap.begin(); l != genreMap.end() ; ++l) {
        if(l -> second > max)
            max = l -> second ;
    }
    std::vector<std::string> popularTags;
    for (auto l = genreMap.begin(); l != genreMap.end() ; ++l) {
        if(l -> second == max)
            popularTags.push_back(l -> first);
    }
    std::string popularTag = lexicographicTag(popularTags);// taking the best tag checking lexicographic order
    return popularTag;
}

std::string GenreRecommenderUser::lexicographicTag(std::vector<std::string> &tags) {
    if(tags.size() > 0) {
        std::string popularTag = tags.at(0);
        for (unsigned int i = 1; i < tags.size(); ++i) {
            if (tags.at(i).compare(popularTag) < 0)
                popularTag = tags.at(i);
        }
        return popularTag;
    }
    else
        return "";
}

std::string GenreRecommenderUser::typeOfUser() const { return "gen"; }

GenreRecommenderUser* GenreRecommenderUser::clone() { return new GenreRecommenderUser(*this); }


//LENGTH RECOMMENDER USER
LengthRecommenderUser::LengthRecommenderUser(const std::string &name): User(name) {}

LengthRecommenderUser::LengthRecommenderUser(const User &other, std::string name): User(other, name) {}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    User &activeUser = s.getUser();
    std::vector<Watchable*> history= activeUser.get_history();
    int sumLength = 0;
    for (unsigned int i = 0; i < history.size(); ++i)
        sumLength += history.at(i)->getLength();
    double averageLength= sumLength/history.size();
    std::vector<Watchable*> content= s.getContent();
    Watchable *recWatch;
    bool found = false;
    for (unsigned int i = 0; (i < content.size()) & (!found); ++i) {
        bool flag = false;
        for (unsigned int j = 0; (j < history.size()) & (!flag); ++j) {
            if (history.at(j)->toString() == content.at(i)->toString())
                flag = true;
        }
        if(!flag) {
            recWatch = content.at(i);
            found = true;
        }
    }
    if(!found)
        return nullptr;
    for (unsigned int j = 1; j < content.size(); ++j) {
        if(abs(recWatch->getLength() - averageLength) >  abs(content.at(j)->getLength() - averageLength)) {
            bool flag = false;
            for (unsigned int i = 0; (i < history.size()) & (!flag); ++i)
                if(history.at(i)->toString() == content.at(j)->toString())
                    flag = true;
            if(!flag)
                recWatch = content.at(j);
        }
    }
    return recWatch;
}

std::string LengthRecommenderUser::typeOfUser() const { return "len"; }

LengthRecommenderUser* LengthRecommenderUser::clone() { return new LengthRecommenderUser(*this); }


//RERUN RECOMMENDER USER
RerunRecommenderUser::RerunRecommenderUser(const std::string &name): User(name), historyIndex(0) {}

RerunRecommenderUser::RerunRecommenderUser(const User &other, std::string name): User(other, name), historyIndex(((RerunRecommenderUser&)other).historyIndex) {}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    User &activeUser = s.getUser();
    std::vector<Watchable*> history= activeUser.get_history();
    Watchable* watch = history.at(historyIndex % history.size());
    historyIndex++;
    return  watch;
}

std::string RerunRecommenderUser::typeOfUser() const { return "rer"; }

RerunRecommenderUser* RerunRecommenderUser::clone() { return new RerunRecommenderUser(*this); }

