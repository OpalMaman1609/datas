#include "Movie.h"

int Movie::getId() {
        return m_movieRank->m_id;
    }

double Movie::getRate() {
    return m_movieRank->m_rate;
}

int Movie::getViews() {
    return m_movieRank->getViews();
}

bool Movie::getIsVip() {
    return m_isVip;
}

Genre Movie::getGenre() {
    return m_genre;
}

MovieRank* Movie::getMovieRank() {
    return m_movieRank;
}
void Movie::watch() {
    m_movieRank->incrementViews();
}

void Movie::rate(int rate) {
    m_movieRank->setRate(rate);
}

Movie::Movie(int id, int views, bool isVip, Genre genre): m_genre(genre), m_isVip(isVip) {
    m_movieRank = new MovieRank(movieId, views);
}

Movie::Movie(const Movie& other) {
    this->m_isVip = other.m_isVip;
    this->m_genre = other.m_genre;
    this->m_movieRank = new MovieRank(other.m_movieRank->getId(), other.m_movieRank->getId());
}

Movie::~Movie() {
    delete m_movieRank;
}