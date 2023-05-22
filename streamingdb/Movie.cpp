#include "Movie.h"

int Movie::getId() const {
        return this->getMovieRank().getId();
    }

double Movie::getRate() const {
    return this->getMovieRank().getRating();
}

int Movie::getViews() const {
    return this->getMovieRank().getViews();
}

bool Movie::getIsVip() const {
    return m_isVip;
}

Genre Movie::getGenre() const {
    return m_genre;
}

MovieRank Movie::getMovieRank() const {
    return m_movieRank;
}
void Movie::watch() {
    m_movieRank.incrementViews();
}

void Movie::watch(int numViews) {
    m_movieRank.m_views += numViews;
}

void Movie::rate(int rate) {
    m_movieRank.setRate(rate);
}

Movie::Movie(int id, int views, bool isVip, Genre genre): m_isVip(isVip), m_genre(genre) {
    m_movieRank = MovieRank(id, views);
}

Movie::Movie(const Movie& other) {
    this->m_isVip = other.m_isVip;
    this->m_genre = other.m_genre;
    this->m_movieRank = MovieRank(other.getMovieRank().getId(), other.getMovieRank().getViews());
    this->m_movieRank.m_rating = other.m_movieRank.m_rating;
    this->m_movieRank.m_numRates = other.m_movieRank.m_numRates;
}

Movie::~Movie() = default;