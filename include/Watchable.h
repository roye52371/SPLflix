
#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>

class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    Watchable(const Watchable &other);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual bool isEpisode() const = 0;
    virtual Watchable* clone() = 0;
    long getId();
    int getLength();
    std::vector<std::string> &getTags();
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    Movie(const Movie &other);//copy constructor
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual bool isEpisode() const;
    Movie* clone();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    Episode(const Episode &other);//copy constructor
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual bool isEpisode() const;
    long getNextEpisodeId();
    Episode* clone();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif