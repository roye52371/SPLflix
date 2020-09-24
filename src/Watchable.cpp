//
// Created by royek on 20/11/2019.
//

#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/User.h"

Watchable::Watchable(long id, int length, const std::vector<std::string> &tags):id(id),length(length), tags()
{
    for (unsigned int i = 0; i < tags.size(); ++i) {
       this->tags.push_back(tags[i]);
    }
}

Watchable::Watchable(const Watchable &other):id(other.id), length(other.length), tags() {
    for (unsigned int i = 0; i < other.tags.size(); ++i) {
        this->tags.push_back(other.tags[i]);
    }
}

long Watchable::getId() { return id; }
int Watchable::getLength() { return length; }
std::vector<std::string>& Watchable::getTags() { return tags; }

Watchable::~Watchable() {}


//EPISODE
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags):Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode), nextEpisodeId(id+1) {}

Episode::Episode(const Episode &other): Watchable(other), seriesName(other.seriesName), season(other.season), episode(other.episode), nextEpisodeId(other.nextEpisodeId) {}

Watchable* Episode::getNextWatchable(Session &s) const {
    std::vector<Watchable*> history = s.getUser().get_history();
    Watchable *lastWatch = history.at(history.size() - 1);
    if(lastWatch->isEpisode()) {
        unsigned int nextEpisodeId = ((Episode*)lastWatch)->getNextEpisodeId() - 1;
        std::vector<Watchable*> content = s.getContent();
        if(nextEpisodeId < content.size()) {
            if (content.at(nextEpisodeId)->isEpisode()) {
                if (((Episode *) content.at(nextEpisodeId))->seriesName == ((Episode *) lastWatch)->seriesName)
                    return s.getContent().at(nextEpisodeId);// returning next episode to watch' from whole content that given
            }
        }
        else
            return nullptr;
    }
    return s.getUser().getRecommendation(s);
}

std::string Episode::toString() const { return seriesName + "  S0" + to_string(season) + "E0" + to_string(episode); }

bool Episode::isEpisode() const { return true; }

long Episode::getNextEpisodeId() { return nextEpisodeId; }

Episode* Episode::clone() { return new Episode(*this); }


//MOVIE
Movie::Movie(long id, const std::string &name, int length, const std::vector <std::string> &tags): Watchable(id, length, tags), name(name) {}

Movie::Movie(const Movie &other): Watchable(other), name(other.name) {}

std::string Movie::toString() const { return name; }

Watchable* Movie::getNextWatchable(Session & s) const {
    return s.getUser().getRecommendation(s);
}

bool Movie::isEpisode() const { return false; }

Movie* Movie::clone() { return new Movie(*this); }
