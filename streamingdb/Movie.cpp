#include "Movie.h"

int Movie::getId() const {
        return m_movieRank->getId();
    }

double Movie::getRate() const {
    return m_movieRank->getRating();
}

int Movie::getViews() const {
    return m_movieRank->getViews();
}

bool Movie::getIsVip() const {
    return m_isVip;
}

Genre Movie::getGenre() const {
    return m_genre;
}

MovieRank Movie::getMovieRank() const {
    return *m_movieRank;
}
void Movie::watch() {
    m_movieRank->incrementViews();
}

void Movie::rate(int rate) {
    m_movieRank->setRate(rate);
}

Movie::Movie(int id, int views, bool isVip, Genre genre): m_isVip(isVip), m_genre(genre) {
    m_movieRank = new MovieRank(id, views);
}

Movie::Movie(const Movie& other) {
    this->m_isVip = other.m_isVip;
    this->m_genre = other.m_genre;
    this->m_movieRank = new MovieRank(other.m_movieRank->getId(), other.m_movieRank->getViews());
}

Movie::~Movie() {
    delete m_movieRank;
}