#include "StreamingDBa1.h"

streaming_database::streaming_database()
{
    users = AvlTree<int,User>();
    groups = AvlTree<int,Group>();
    m_idMovieTree = new AvlTree<int, Movie*>();

    // for each genre, creates a tree
    for (int i = Genre::COMEDY; i <= Genre::NONE; i++) {
        m_genreTreesArray[i] = new AvlTree<MovieRank, Movie*>();
        m_maxViewsPerGenre[i] = nullptr;
        m_movieCountByGenre = 0;
    }
}

streaming_database::~streaming_database()
{
    for (int i = 0; i <= Genre::NONE; i++) {
        delete m_genreTreesArray[i];
        delete m_maxViewsPerGenre[i];
    }
}

void streaming_database::updateTopRatedMoviePerGenre(const Movie* movie) {
    Pair<int, int>* currentMax = this->m_maxViewsPerGenre[movie->getGenre()];
    if (currentMax == nullptr) {
        m_maxViewsPerGenre[movie->getGenre()] = new Pair<movie->getMovieRank().getViews(), movie->getId()>();
    }

    // other max already exist
    if (currentMax->key < movie->getMovieRank()) {
        currentMax->key = movie->getMovieRank();
        currentMax->value = movie->getId();
    }
}

void streaming_database::deleteMovieFromGenresTreeArray(Genre genre, const MovieRank& key) {
    // deletes from the correct genre, and in addition updates the all genres (NONE) tree as well
    m_genreTreesArray[movieGenre]->Delete(*(movieNode->value->getMovieRank()));
    m_genreTreesArray[Genre::NONE]->Delete(*(movieNode->value->getMovieRank()));
}

void streaming_database::insertMovieToGenresTreeArray(Genre genre, const Movie* movie) {
    // inserts to the correct genre, and in addition updates the all genres (NONE) tree as well
    m_genreTreesArray[genre]->Insert(*(movie->getMovieRank()), movie);
    m_genreTreesArray[Genre::NONE]->Insert(*(movie->getMovieRank()), movie);
}


StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    // validate input
    if (movieId <= 0 || genre == Genre::NONE || views < 0) {
        return StatusType::INVALID_INPUT;
    }

    if (idMovieTree.Find(movieId) != nullptr) {
        return StatusType::FAILURE;
    }

    Movie* movie = nullptr;
    try {
        movie = new Movie(movieId, views, vipOnly, genre);
    } catch (std::bad_alloc &e) {
        return StatusType::ALLOCATION_ERROR;
    }

    m_idMovieTree.Insert(movieId, movie);
    this->insertMovieToGenresTreeArray(genre, movie);
    insertMovieToGenresTreeArray(genre)

    // updates number of movies in correct genre & all movies count
    m_movieCountByGenre[genre]++;
    m_movieCountByGenre[Genre::NONE]++;

    // if necessary, updates most viewed movie in category
    this->updateTopRatedMoviePerGenre(movie);

	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
    if (movieId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, Movie*>* movieNode = idMovieTree.Find(movieId);
    if (movieNode == nullptr) {
        return StatusType::FAILURE;
    }

    Movie movie = *(movieNode->value);
    // removes the movie from general tree
    m_idMovieTree->Delete(movieId);

    Genre movieGenre = movieNode->value->getGenre();
    // removes the tree from his genre tree
    this->deleteMovieFromGenresTreeArray(movieGenre, *(movie.getMovieRank()));

    // if movie is current max, needs to find in the same genre
    if (m_maxViewsPerGenre[movieGenre] != nullptr && m_maxViewsPerGenre[movieGenre]->value == movieId) {
        delete m_maxViewsPerGenre[movieGenre];
        this->updateTopRatedMoviePerGenre(movie);
    }

    // updates the max views for genre & general
    m_movieCountByGenre[genre]--;
    m_movieCountByGenre[Genre::NONE]--;

	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
	// TODO: Your code goes here
    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    if (userId <= 0 || movieId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, User>* userNode = users.Find(userId);
    AvlNode<int, Movie*>* movieNode = m_idMovieTree->Find(movieId);

    if (userNode == nullptr || movieNode == nullptr || movieNode->value->getIsVip() && !userNode->value.isVip) {
        return StatusType::FAILURE;
    }

    // updates the genre tree (sorted by rank, should be removed and added with new rank)
    Genre movieGenre = movieNode->value->getGenre();
    Movie movie = *(movieNode->value);
    this->deleteMovieFromGenresTreeArray(movieGenre, *(movieNode->value->getMovieRank()));
    movie.watch();
    this->insertMovieToGenresTreeArray(movieGenre, &movie);

    this->updateTopRatedMoviePerGenre(&movie);

    // updates the user views
    // TODO: ask opal if its OK, maybe viewsLog should point to group's
    userNode->value.viewsLog[movieGenre]++;
    userNode->value.viewsLog[Genre::NONE]++;

    if (userNode->value.group != nullptr) {
        userNode->value.group->usersViewsLog[movieGenre]++;
        userNode->value.group->usersViewsLog[Genre::NONE]++;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    return output_t<int>(m_movieCountByGenre[genre]);
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    if (output == nullptr) {
        return StatusType::INVALID_INPUT;
    }

    int movieCount = m_movieCountByGenre[genre];
    if (movieCount == 0) {
        return StatusType::FAILURE;
    }

    MovieRank* movieRanks = new MovieRank[movieCount];

    m_genreTreesArray[genre]->ReverseInOrder(movieRanks);

    output = new int[movieCount];
    for (int i = 0; i < movieCount; i++) {
        output[i] = movieRanks->getId();
    }

    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
    if (userId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    AvlNode<int, User>* userNode = users.Find(userId);
    if (userNode == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }

    User user = userNode->value;
    int numViews = user.viewsLog[genre];
    if (user.group != nullptr) {
        // adds movies that user watched with the group
        numViews += user.group->groupMoviesCount[genre] - user.snapGroup[genre];
    }

	return output_t(numViews);
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if (userId <= 0 || movieId <= 0 || rating < 0 || rating > 100) {
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, Movie*>* movieNode = m_idMovieTree->Find(movieId);
    AvlNode<int, User>* userNode = users.Find(userId);
    if (movieNode == nullptr || userNode == nullptr || movieNode->value->getIsVip() && !userNode->value.isVip) {
        return StatusType::FAILURE;
    }

    movieNode->value->rate(rating);
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}


