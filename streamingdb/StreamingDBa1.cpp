#include "StreamingDBa1.h"

streaming_database::streaming_database()
{
    users = AvlTree<int,User>();
    m_groups = AvlTree<int,Group>();
    m_idMovieTree = new AvlTree<int, Movie*>();

    // for each genre, creates a tree
    for (int i = 0; i < numGenres; i++) {
        m_genreTreesArray[i] = new AvlTree<MovieRank, Movie*>();
        m_maxViewsPerGenre[i] = nullptr;
        m_movieCountByGenre[i] = 0;
    }
}

streaming_database::~streaming_database()
{
    for (int i = 0; i < numGenres; i++) {
        delete m_genreTreesArray[i];
        delete m_maxViewsPerGenre[i];
    }
}

void streaming_database::updateTopRatedMoviePerGenre(const Movie& movie) {
    Pair<MovieRank, int>* currentMax = this->m_maxViewsPerGenre[(int)movie.getGenre()];
    if (currentMax == nullptr) {
        m_maxViewsPerGenre[(int) movie.getGenre()] = new Pair<MovieRank, int>();
        m_maxViewsPerGenre[(int) movie.getGenre()]->key = movie.getMovieRank();
        m_maxViewsPerGenre[(int) movie.getGenre()]->value = movie.getId();
    }

    // other max already exist
    else if (currentMax->key < movie.getMovieRank()) {
        currentMax->key = movie.getMovieRank();
        currentMax->value = movie.getId();
    }
}

void streaming_database::deleteMovieFromGenresTreeArray(Genre genre, const MovieRank& key) {
    // deletes from the correct genre, and in addition updates the all genres (NONE) tree as well
    int movieGenre = (int) genre;
    m_genreTreesArray[movieGenre]->Delete(key);
    m_genreTreesArray[(int)Genre::NONE]->Delete(key);
}

void streaming_database::insertMovieToGenresTreeArray(Genre genre, Movie& movie) {
    // inserts to the correct genre, and in addition updates the all genres (NONE) tree as well
    int movieGenre = (int) genre;
    m_genreTreesArray[movieGenre]->Insert(movie.getMovieRank(), &movie);
    m_genreTreesArray[(int)Genre::NONE]->Insert(movie.getMovieRank(), &movie);
}


StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    // validate input
    if (movieId <= 0 || genre == Genre::NONE || views < 0) {
        return StatusType::INVALID_INPUT;
    }

    if (this->m_idMovieTree->Find(movieId) != nullptr) {
        return StatusType::FAILURE;
    }

    Movie* movie = nullptr;
    try {
        movie = new Movie(movieId, views, vipOnly, genre);
    } catch (std::bad_alloc &e) {
        return StatusType::ALLOCATION_ERROR;
    }

    m_idMovieTree->Insert(movieId, movie);
    this->insertMovieToGenresTreeArray(genre, *movie);
    insertMovieToGenresTreeArray(genre, *movie);

    // updates number of movies in correct genre & all movies count
    m_movieCountByGenre[(int)genre]++;
    m_movieCountByGenre[(int)Genre::NONE]++;

    // if necessary, updates most viewed movie in category
    this->updateTopRatedMoviePerGenre(*movie);

	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
    if (movieId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, Movie*>* movieNode = this->m_idMovieTree->Find(movieId);
    if (movieNode == nullptr) {
        return StatusType::FAILURE;
    }

    Movie movie = *(movieNode->value);
    // removes the movie from general tree
    m_idMovieTree->Delete(movieId);

    int movieGenre = (int)movieNode->value->getGenre();
    // removes the tree from his genre tree
    this->deleteMovieFromGenresTreeArray(movieNode->value->getGenre(), movie.getMovieRank());

    // if movie is current max, needs to find in the same genre
    if (m_maxViewsPerGenre[movieGenre] != nullptr && m_maxViewsPerGenre[movieGenre]->value == movieId) {
        delete m_maxViewsPerGenre[movieGenre];
        this->updateTopRatedMoviePerGenre(movie);
    }

    // updates the max views for genre & general
    m_movieCountByGenre[movieGenre]--;
    m_movieCountByGenre[(int)Genre::NONE]--;

	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
    if (userId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    User userToAdd = User(userId,isVip);
    return streaming_database::users.Insert(userId,userToAdd);

}

StatusType streaming_database::remove_user(int userId)
{
    if (userId <= 0){
        return StatusType::INVALID_INPUT;
    }
    AvlNode<int, User> *user = users.Find(userId);
    if (!user)
    {
        return StatusType::FAILURE;
    }
    if (user->value.isVip){
        user->value.group->isVip-=1;
    }
    //delete users view from individual views and delete group watches of user(one view for each movie he attanded)
    for (int i = 0; i < numGenres; i++)
    {
        user->value.group->usersViewsLog[i]-= user->value.viewsLog[i];
        user->value.group->groupViewsCount[i]-=(user->value.group->groupMoviesCount[i])-(user->value.snapGroup[i]);
    }
    user->value.group->numUsers--;
    user->value.group->groupUsersTree.Delete(userId);
    users.Delete(userId);

    return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
    if (groupId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    Group groupToAdd = Group(groupId);
    return streaming_database::m_groups.Insert(groupId,groupToAdd);
}

StatusType streaming_database::remove_group(int groupId)
{
    if (groupId <= 0){
        return StatusType::INVALID_INPUT;
    }
    AvlNode<int, Group> *group = m_groups.Find(groupId);
    if (!group)
    {
        return StatusType::FAILURE;
    }
    AvlNode<int, User *> *user = findMinNode(group->value.groupUsersTree.root);
    while (user !=nullptr)
    {
        //Adding the group views to each user as user views before the group is deleted
        for (int i = 0; i < numGenres; i++)
        {
            user->value->viewsLog[i]+= (group->value.groupMoviesCount[i])-(user->value->snapGroup[i]);
        }
        user->value->group=nullptr;
        user = group->value.groupUsersTree.Next_InOrder(user);
    }
    return m_groups.Delete(groupId);
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    if (userId <= 0 || groupId <= 0){
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, User> *user = users.Find(userId);
    AvlNode<int, Group> *group = m_groups.Find(groupId);
    //If group/user is not found Find returns nullptr
    if (!user || !group || (user->value.group)) {
        return StatusType::FAILURE;
    }
    StatusType status = group->value.groupUsersTree.Insert(user->key,&user->value);
    if (status!=StatusType::SUCCESS){
        return status;
    }
    //Add a pointer to the group from user
    user->value.group = &group->value;
    for (int i = 0; i < numGenres; i++)
    {
        user->value.snapGroup[i]=group->value.groupMoviesCount[i];
        group->value.usersViewsLog[i]+=user->value.viewsLog[i];
    }
    if (user->value.isVip){
        group->value.isVip+=1;
    }
    group->value.numUsers++;
    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    if (userId <= 0 || movieId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, User>* userNode = users.Find(userId);
    AvlNode<int, Movie*>* movieNode = m_idMovieTree->Find(movieId);

    if (userNode == nullptr || movieNode == nullptr || (movieNode->value->getIsVip() && !userNode->value.isVip)) {
        return StatusType::FAILURE;
    }

    // updates the genre tree (sorted by rank, should be removed and added with new rank)
    int movieGenre = (int)movieNode->value->getGenre();
    Movie movie = *(movieNode->value);
    this->deleteMovieFromGenresTreeArray(movieNode->value->getGenre(), movieNode->value->getMovieRank());
    movie.watch();
    this->insertMovieToGenresTreeArray(movieNode->value->getGenre(), movie);

    this->updateTopRatedMoviePerGenre(movie);

    // updates the user views
    userNode->value.viewsLog[movieGenre]++;
    userNode->value.viewsLog[(int)Genre::NONE]++;

    if (userNode->value.group != nullptr) {
        userNode->value.group->usersViewsLog[movieGenre]++;
        userNode->value.group->usersViewsLog[(int)Genre::NONE]++;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
	if (movieId <= 0 || groupId <= 0){
		return StatusType::INVALID_INPUT;
	}
	AvlNode<int, Movie*> *movieNode = m_idMovieTree->Find(movieId);
	AvlNode<int, Group> *group = m_groups.Find(groupId);
	//If group/user is not found Find returns nullptr
	if (!movieNode || !group || (group->value.numUsers == 0)) {
		return StatusType::FAILURE;
	}

	if ( movieNode->value->getIsVip() && group->value.isVip == 0) {
		return StatusType::FAILURE;
	}
    int movieGenre = (int)movieNode->value->getGenre();
    // updated group's number of group watch instances
	group->value.groupMoviesCount[movieGenre]++;
	group->value.groupMoviesCount[(int)Genre::NONE]++;

    // updates number of views as a group
	group->value.groupViewsCount[movieGenre] += group->value.numUsers;
	group->value.groupViewsCount[(int)Genre::NONE] += group->value.numUsers;
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    return output_t<int>(m_movieCountByGenre[(int)genre]);
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    if (output == nullptr) {
        return StatusType::INVALID_INPUT;
    }

    int genreInt = (int) genre;
    int movieCount = m_movieCountByGenre[genreInt];
    if (movieCount == 0) {
        return StatusType::FAILURE;
    }

    MovieRank* movieRanks = new MovieRank[movieCount];

    m_genreTreesArray[genreInt]->ReverseInOrder(movieRanks);

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
    int genreInt = (int) genre;
    int numViews = user.viewsLog[genreInt];
    if (user.group != nullptr) {
        // adds movies that user watched with the group
        numViews += user.group->groupMoviesCount[genreInt] - user.snapGroup[genreInt];
    }

	return output_t<int>(numViews);
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if (userId <= 0 || movieId <= 0 || rating < 0 || rating > 100) {
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, Movie*>* movieNode = m_idMovieTree->Find(movieId);
    AvlNode<int, User>* userNode = users.Find(userId);
    if (movieNode == nullptr || userNode == nullptr || (movieNode->value->getIsVip() && !userNode->value.isVip)) {
        return StatusType::FAILURE;
    }

    movieNode->value->rate(rating);
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	if (groupId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    AvlNode<int, Group>* groupNode = m_groups.Find(groupId);

    if (groupNode == nullptr || groupNode->value.numUsers == 0) {
        return output_t<int>(StatusType::FAILURE);
    }

    int max = 0;
    int genreMax = 0;
    for (int i = 0; i < numGenres; i++) {
        int genreViews = groupNode->value.usersViewsLog[i] + groupNode->value.groupViewsCount[i];
        if (genreViews > max) {
            genreMax = i;
            max = genreViews;
        }
    }

    return m_maxViewsPerGenre[genreMax]->value;
}


