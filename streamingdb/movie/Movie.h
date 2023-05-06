#include "../utils/wet1util.h"
#include "../movierank/MovieRank.h"

class Movie {
private:
    MovieRank* m_movieRank;
    bool m_isVip;
    Genre m_genre;
public:
    Movie(int id, int views, bool isVip, Genre genre);
    Movie(const Movie& other);
    virtual ~Movie();
    int getId();
    int getViews();
    double getRate();
    MovieRank getMovieRank();
    bool getIsVip();
    Genre getGenre();
    void watch();
    void rate(int rate);
};