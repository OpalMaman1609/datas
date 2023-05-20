#include "wet1util.h"
#include "MovieRank.h"

class Movie {
private:
    MovieRank* m_movieRank;
    bool m_isVip;
    Genre m_genre;
public:
    Movie()=default;
    Movie(int id, int views, bool isVip, Genre genre);
    Movie(const Movie& other);
    virtual ~Movie();
    int getId() const;
    int getViews() const;
    double getRate() const;
    MovieRank getMovieRank() const;
    bool getIsVip() const;
    Genre getGenre() const;
    void watch();
    void watch(int numViews);
    void rate(int rate);
};