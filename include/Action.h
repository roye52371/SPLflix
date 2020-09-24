
#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone() = 0;
    virtual ~BaseAction();
protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    CreateUser();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    CreateUser* clone();
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    ChangeActiveUser* clone();
};

class DeleteUser : public BaseAction {
public:
    DeleteUser();
    virtual void act(Session & sess);
    virtual std::string toString() const;
    DeleteUser* clone();
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser();
    virtual void act(Session & sess);
    virtual std::string toString() const;
    DuplicateUser* clone();
};

class PrintContentList : public BaseAction {
public:
    PrintContentList();
    virtual void act (Session& sess);
    virtual std::string toString() const;
    PrintContentList* clone();
};

class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
    virtual void act (Session& sess);
    virtual std::string toString() const;
    PrintWatchHistory* clone();
};


class Watch : public BaseAction {
public:
    Watch();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    Watch* clone();
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    PrintActionsLog* clone();
};

class Exit : public BaseAction {
public:
    Exit();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    Exit* clone();
};
#endif