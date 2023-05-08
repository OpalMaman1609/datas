#ifndef DATAS_MOVIERANK_H
#define DATAS_MOVIERANK_H

#define NOT_RATED 0.0

class MovieRank {
private:
    int m_movieId;
    int m_views;
    double m_rating;
    int m_numRates;
public:
    MovieRank();
    MovieRank(int movieId, int views);
    int getId();
    int getViews();
    double getRating();
    void setRate(int rate);
    void incrementViews();
    bool operator<(const MovieRank& other);
    bool operator>(const MovieRank& other);
    bool operator==(const MovieRank& other);
};


#endif //DATAS_MOVIERANK_H
