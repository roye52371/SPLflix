//
// Created by royek on 20/11/2019.
//

#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/User.h"

BaseAction::BaseAction(): errorMsg(), status() {
    status = PENDING;
}

ActionStatus BaseAction::getStatus() const { return status; }

void BaseAction::complete()  { this->status=COMPLETED; }

void BaseAction::error(const std::string &errorMsg) {
    this->status=ERROR;
    this->errorMsg= errorMsg;
}

std::string BaseAction::getErrorMsg() const { return errorMsg; }

BaseAction::~BaseAction() { }


//CHANGE ACTIVE USER
ChangeActiveUser::ChangeActiveUser(): BaseAction() {}

void ChangeActiveUser::act(Session &sess) {
    std::string act = sess.getAct();
    std::string userName = act.substr(act.find(" ") + 1, act.length()- 1);
    if(sess.getUserMap().find(userName) != sess.getUserMap().end()){
        sess.setActiveUser(sess.getUserMap().at(userName));
        complete();
    }
    else
        error("username does not exist");

    sess.getActionsLog().push_back(this->clone());
}

std::string ChangeActiveUser::toString() const {
    if(getStatus()==ERROR)
        return "ERROR: " + getErrorMsg();
    else
        return "ChangeActiveUser COMPLETED";
}

ChangeActiveUser* ChangeActiveUser::clone() { return new ChangeActiveUser(*this); }


//CREATE USER
CreateUser::CreateUser(): BaseAction() {}

void CreateUser::act(Session &sess) {
    std::string act = sess.getAct();
    act = act.substr(act.find(" ") + 1, act.length()- 1);
    std::string userName = act.substr(0, act.find(" "));
    std::string rec = act.substr(act.find(" ") + 1, act.length() - 1);
    if((rec != "len") & (rec != "rer") & (rec != "gen"))
        error("recommendation algorithm is invalid");
    else if(sess.getUserMap().find(userName) != sess.getUserMap().end())
        error("username is already in use");
    else {
        if(rec == "len")
            sess.getUserMap().insert({userName, new LengthRecommenderUser(userName)});
        else if(rec == "rer")
            sess.getUserMap().insert({userName, new RerunRecommenderUser(userName)});
        else if(rec == "gen")
            sess.getUserMap().insert({userName, new GenreRecommenderUser(userName)});
        complete();
    }
    sess.getActionsLog().push_back(this->clone());
}

std::string CreateUser::toString() const {
    if(getStatus()==ERROR)
        return "ERROR: " + getErrorMsg();
    else
        return "CreateUser COMPLETED";
}

CreateUser* CreateUser::clone() { return new CreateUser(*this);}


//DELETE USER
DeleteUser::DeleteUser(): BaseAction() {}

void DeleteUser::act(Session &sess) {
    std::string act = sess.getAct();
    std::string userName = act.substr(act.find(" ") + 1, act.length()- 1);
    if(sess.getUserMap().find(userName) != sess.getUserMap().end()) {
        delete(sess.getUserMap().at(userName));
        sess.getUserMap().erase(userName);
        complete();
    }
    else
        error("username does not exist");

    sess.getActionsLog().push_back(this->clone());
}

std::string DeleteUser::toString() const {
    if(getStatus()==ERROR)
        return "ERROR: " + getErrorMsg();
    else
        return "DeleteUser COMPLETED";
}

DeleteUser* DeleteUser::clone() { return new DeleteUser(*this);}


//DUPLICATE USER
DuplicateUser::DuplicateUser(): BaseAction() {}

void DuplicateUser::act(Session &sess) {
    std::string act = sess.getAct();
    act = act.substr(act.find(" ") + 1, act.length()- 1);
    std::string orgUserName = act.substr(0, act.find(" "));
    std::string newUserName = act.substr(act.find(" ") + 1, act.length() - 1);
    if(sess.getUserMap().find(orgUserName) == sess.getUserMap().end())
        error("username does not exist");
    else if(sess.getUserMap().find(newUserName) != sess.getUserMap().end())
        error("username is already in use");
    else {
        User *orgUser = sess.getUserMap().at(orgUserName);
        if(orgUser->typeOfUser()=="len")
            sess.getUserMap().insert({newUserName, new LengthRecommenderUser(*orgUser, newUserName)});
        else if(orgUser->typeOfUser()=="rer")
            sess.getUserMap().insert({newUserName, new RerunRecommenderUser(*orgUser, newUserName)});
        else if(orgUser->typeOfUser()=="gen")
            sess.getUserMap().insert({newUserName, new GenreRecommenderUser(*orgUser, newUserName)});
        complete();
    }

    sess.getActionsLog().push_back(this->clone());
}

std::string DuplicateUser::toString() const {
    if(getStatus()==ERROR)
        return "ERROR: " + getErrorMsg();
    else
        return "DuplicateUser COMPLETED";
}

DuplicateUser* DuplicateUser::clone() { return new DuplicateUser(*this);}


//EXIT
Exit::Exit(): BaseAction() {}

void Exit::act(Session &sess) {
    sess.getActionsLog().push_back(this->clone());
    complete();
}

std::string Exit::toString() const { return "Exit COMPLETED"; }

Exit* Exit::clone() { return new Exit(*this); }


//PRINT ACTIONS LOG
PrintActionsLog::PrintActionsLog(): BaseAction() {}

void PrintActionsLog::act(Session &sess) {
    std::vector<BaseAction*> actionLog= sess.getActionsLog();
    for (int i = actionLog.size()-1; i >= 0 ; --i) {
        cout<<actionLog.at(i)->toString()<<endl;
    }
    sess.getActionsLog().push_back(this->clone());
    complete();
}

std::string PrintActionsLog::toString() const { return "PrintActionsLog COMPLETED"; }

PrintActionsLog* PrintActionsLog::clone() { return new PrintActionsLog(*this);}


//PRINT CONTENT LIST
PrintContentList::PrintContentList(): BaseAction() {}

void PrintContentList::act(Session &sess) {
    std::vector<Watchable*> content= sess.getContent();
    for (unsigned int i = 0; i <content.size() ; ++i) {
        std::string watchable;
        int length=content.at(i)->getLength();
        watchable = to_string(i+1) + ". " + content.at(i)->toString() + " " + to_string(length) + " mintues [";
        std::vector<std:: string> tags=content.at(i)->getTags();
        for (unsigned int j = 0; j < tags.size()-1; ++j) {
            watchable += tags.at(j)+ ", ";
        }
        watchable+= tags.at(tags.size()-1)+"]";
        cout<<watchable<<endl;
    }
    sess.getActionsLog().push_back(this->clone());
    complete();
}

std::string PrintContentList::toString() const { return "PrintContentList COMPLETED"; }

PrintContentList* PrintContentList::clone() { return new PrintContentList(*this);}


//PRINT WATCH HISTORY
PrintWatchHistory::PrintWatchHistory(): BaseAction() {}

void PrintWatchHistory::act(Session &sess) {
    User &activeUser=sess.getUser();
    std::vector<Watchable*> contentHistory= activeUser.get_history();
    cout<<"Watch history for " + activeUser.getName() <<endl;
    for (unsigned int i = 0; i <contentHistory.size() ; ++i) {
        std::string watchable;
        watchable = to_string(i+1) + ". " + contentHistory.at(i)->toString();
        cout<<watchable<<endl;
    }
    sess.getActionsLog().push_back(this->clone());
    complete();
}

std::string PrintWatchHistory::toString() const { return "PrintWatchHistory COMPLETED"; }

PrintWatchHistory* PrintWatchHistory::clone() { return new PrintWatchHistory(*this);}


//WATCH
Watch::Watch(): BaseAction() {}

void Watch::act(Session &sess) {
    std::string act = sess.getAct();
    int watchIndex = std::stoi(act.substr(act.find(" ") + 1, act.length()- 1));//stoi converting string that represnting number to int
    watchIndex--;
    std::vector<Watchable*> content= sess.getContent();
    Watchable *watch= content.at(watchIndex);
    bool flag = true;
    while(flag)
    {
        cout<<"Watching " + (*watch).toString()<<endl;
        sess.getUser().addToHistory(sess.getContent().at(watchIndex));//adding to history the newest watch
        sess.getActionsLog().push_back(this->clone());
        Watchable *recWatch= watch->getNextWatchable(sess);
        if(recWatch != nullptr) {
            cout << "We recommend watching " + recWatch->toString() + ", continue watching? [y/n]" << endl;
            std::string answer;
            cin >> answer;
            if (answer != "y")
                flag = false;
            else {
                watch = recWatch;
                watchIndex = recWatch->getId() - 1;
            }
        }
        else {
            cout << "We are Sorry, Matching Content Was Not Found" << endl;
            flag = false;
        }
    }
    complete();
}

std::string Watch::toString() const { return "Watch COMPLETED"; }

Watch* Watch::clone() { return new Watch(*this);}