#include "MovieRank.h"

MovieRank::MovieRank(int movieId, int views): m_movieId(movieId), m_views(views), m_rating(NOT_RATED), m_numRates(0) {};
MovieRank::MovieRank() {};

int MovieRank::getId() {
    return m_movieId;
}

double MovieRank::getRating() {
    return m_rating;
}

int MovieRank::getViews() {
    return m_views;
}

void MovieRank::setRate(int rate) {
    m_rating = (m_rating * m_numRates + rate) / (m_numRates + 1);
    m_numRates++;
}

void MovieRank::incrementViews() {
    m_views++;
}

// a < b

bool MovieRank::operator<(const MovieRank &other) {
    // rank logic, returns if current rank smaller than other
    if (this->m_rating < other.m_rating) {
        return true;
    } else if (this->m_rating > other.m_rating) {
        return false;
    }

    // rating is equal in both cases{
    if (this->m_views < other.m_views) {
        return true;
    } else if (this->m_views > other.m_views) {
        return false;
    }

    // rating & views number is equal, will determine by movieId
    return this->m_movieId > other.m_movieId;
}

bool MovieRank::operator>(const MovieRank &other) {
    // values here cannot be equal
    return !(*this < other);
}

bool MovieRank::operator==(const MovieRank& other) {
    return m_movieId == other.m_movieId;
}