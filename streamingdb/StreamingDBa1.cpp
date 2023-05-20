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

    //add the none max it is not handeled, also in remove movie user watch group watch
    Pair<MovieRank, int>* currentMax = this->m_maxViewsPerGenre[(int)movie.getGenre()];
    Pair<MovieRank, int>* currentMaxNone = this->m_maxViewsPerGenre[(int)Genre::NONE];
    if (currentMax == nullptr) {
        m_maxViewsPerGenre[(int) movie.getGenre()] = new Pair<MovieRank, int>();
        m_maxViewsPerGenre[(int) movie.getGenre()]->m_key = movie.getMovieRank();
        m_maxViewsPerGenre[(int) movie.getGenre()]->m_value = movie.getId();
    }

    // other max already exist
    else if (currentMax->m_key < movie.getMovieRank()) {
        currentMax->m_key = movie.getMovieRank();
        currentMax->m_value = movie.getId();
    }
}

void streaming_database::deleteMovieFromGenresTreeArray(Genre genre, const MovieRank& key) {
    // deletes from the correct genre, and in addition updates the all genres (NONE) tree as well
    int movieGenre = (int) genre;
    m_genreTreesArray[(int)Genre::NONE]->Delete(key);
    m_genreTreesArray[movieGenre]->Delete(key);
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

    if (this->m_idMovieTree->FindByKey(movieId) != nullptr) {
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

    AvlNode<int, Movie*>* movieNode = this->m_idMovieTree->FindByKey(movieId);
    if (movieNode == nullptr) {
        return StatusType::FAILURE;
    }

    // removes the movie from general tree
    int movieGenre = (int)movieNode->m_value->getGenre();

    AvlNode<MovieRank, Movie*>* movieGenreNode = m_genreTreesArray[movieGenre]->FindByKey(
        movieNode->m_value->getMovieRank()
        );
    

    // if movie is current max, needs to find in the same genre
    if (m_maxViewsPerGenre[movieGenre] != nullptr && m_maxViewsPerGenre[movieGenre]->m_value == movieId) {
        delete m_maxViewsPerGenre[movieGenre];
        m_maxViewsPerGenre[movieGenre] = nullptr;
        if (movieGenreNode->m_left_son != nullptr) {
            this->updateTopRatedMoviePerGenre(*(movieGenreNode->m_left_son->m_value));
        } else if (movieGenreNode->m_parent != nullptr) {
            this->updateTopRatedMoviePerGenre(*(movieGenreNode->m_parent->m_value));
        }
    }

    // removes the tree from his genre tree
    this->deleteMovieFromGenresTreeArray(movieNode->m_value->getGenre(), movieNode->m_value->getMovieRank());

    // updates the max views for genre & general
    m_movieCountByGenre[movieGenre]--;
    m_movieCountByGenre[(int)Genre::NONE]--;
    m_idMovieTree->Delete(movieId);
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
    if (userId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, User>* userNode = users.FindByKey(userId);
    if (userNode) {
        return StatusType::FAILURE;
    }

    User userToAdd = User(userId,isVip);
    return streaming_database::users.Insert(userId,userToAdd);

}

StatusType streaming_database::remove_user(int userId)
{
    if (userId <= 0){
        return StatusType::INVALID_INPUT;
    }
    AvlNode<int, User> *user = users.FindByKey(userId);
    if (!user)
    {
        return StatusType::FAILURE;
    }
    if (user->m_value.isVip && user->m_value.group){
        user->m_value.group->isVip-=1;
    }
    //delete users view from individual views and delete group watches of user(one view for each movie he attanded)
    if (user->m_value.group) {
    for (int i = 0; i < numGenres; i++)
    {
        user->m_value.group->usersViewsLog[i]-= user->m_value.viewsLog[i];
        user->m_value.group->groupViewsCount[i]-=(user->m_value.group->groupMoviesCount[i])-(user->m_value.snapGroup[i]);
    }
    user->m_value.group->numUsers--;
    user->m_value.group->groupUsersTree.Delete(userId);
    }
    users.Delete(userId);

    return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
    if (groupId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, Group> *group = m_groups.FindByKey(groupId);
    if (group)
    {
        return StatusType::FAILURE;
    }

    Group groupToAdd = Group(groupId);
    return streaming_database::m_groups.Insert(groupId,groupToAdd);
}

StatusType streaming_database::remove_group(int groupId)
{
    if (groupId <= 0){
        return StatusType::INVALID_INPUT;
    }
    AvlNode<int, Group> *group = m_groups.FindByKey(groupId);
    if (!group)
    {
        return StatusType::FAILURE;
    }

    if (group->m_value.numUsers>0){
    AvlNode<int, User *> *user = findMinNode(group->m_value.groupUsersTree.m_root);
    while (user !=nullptr)
    {
        //Adding the group views to each user as user views before the group is deleted
        for (int i = 0; i < numGenres; i++)
        {
            user->m_value->viewsLog[i]+= (group->m_value.groupMoviesCount[i])-(user->m_value->snapGroup[i]);
        }
        user->m_value->group=nullptr;
        user = group->m_value.groupUsersTree.NextInOrder(user);
        }
    }

    return m_groups.Delete(groupId);
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    if (userId <= 0 || groupId <= 0){
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, User> *user = users.FindByKey(userId);
    AvlNode<int, Group> *group = m_groups.FindByKey(groupId);
    //If group/user is not found Find returns nullptr
    if (!user || !group || (user->m_value.group)) {
        return StatusType::FAILURE;
    }
    StatusType status = group->m_value.groupUsersTree.Insert(user->m_key,&user->m_value);
    if (status!=StatusType::SUCCESS){
        return status;
    }
    //Add a pointer to the group from user
    user->m_value.group = &group->m_value;
    for (int i = 0; i < numGenres; i++)
    {
        user->m_value.snapGroup[i]=group->m_value.groupMoviesCount[i];
        group->m_value.usersViewsLog[i]+=user->m_value.viewsLog[i];
    }
    if (user->m_value.isVip){
        group->m_value.isVip+=1;
    }
    group->m_value.numUsers++;
    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    if (userId <= 0 || movieId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AvlNode<int, User>* userNode = users.FindByKey(userId);
    AvlNode<int, Movie*>* movieNode = m_idMovieTree->FindByKey(movieId);

    if (userNode == nullptr || movieNode == nullptr || (movieNode->m_value->getIsVip() && !userNode->m_value.isVip)) {
        return StatusType::FAILURE;
    }

    // updates the genre tree (sorted by rank, should be removed and added with new rank)
    int movieGenre = (int)movieNode->m_value->getGenre();
    
    this->deleteMovieFromGenresTreeArray(movieNode->m_value->getGenre(), movieNode->m_value->getMovieRank());
    movieNode->m_value->watch();
    this->insertMovieToGenresTreeArray(movieNode->m_value->getGenre(), *(movieNode->m_value));

    this->updateTopRatedMoviePerGenre(*(movieNode->m_value));

    // updates the user views
    userNode->m_value.viewsLog[movieGenre]++;
    userNode->m_value.viewsLog[(int)Genre::NONE]++;

    if (userNode->m_value.group != nullptr) {
        userNode->m_value.group->usersViewsLog[movieGenre]++;
        userNode->m_value.group->usersViewsLog[(int)Genre::NONE]++;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
	if (movieId <= 0 || groupId <= 0){
		return StatusType::INVALID_INPUT;
	}
	AvlNode<int, Movie*> *movieNode = m_idMovieTree->FindByKey(movieId);
	AvlNode<int, Group> *group = m_groups.FindByKey(groupId);
	//If group/user is not found Find returns nullptr
	if (!movieNode || !group || (group->m_value.numUsers == 0)) {
		return StatusType::FAILURE;
	}

	if ( movieNode->m_value->getIsVip() && group->m_value.isVip == 0) {
		return StatusType::FAILURE;
	}
    int movieGenre = (int)movieNode->m_value->getGenre();
    // updated group's number of group watch instances
	group->m_value.groupMoviesCount[movieGenre]++;
	group->m_value.groupMoviesCount[(int)Genre::NONE]++;

    // updates number of views as a group
	group->m_value.groupViewsCount[movieGenre] += group->m_value.numUsers;
	group->m_value.groupViewsCount[(int)Genre::NONE] += group->m_value.numUsers;

    // remove & insert to tree, in order to place
    this->deleteMovieFromGenresTreeArray(movieNode->m_value->getGenre(), movieNode->m_value->getMovieRank());
    movieNode->m_value->watch(group->m_value.numUsers);
    this->insertMovieToGenresTreeArray(movieNode->m_value->getGenre(), *(movieNode->m_value));

    this->updateTopRatedMoviePerGenre(*(movieNode->m_value));

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
        movieRanks++;
    }

    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
    if (userId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    AvlNode<int, User>* userNode = users.FindByKey(userId);
    if (userNode == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }

    User user = userNode->m_value;
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

    AvlNode<int, Movie*>* movieNode = m_idMovieTree->FindByKey(movieId);
    AvlNode<int, User>* userNode = users.FindByKey(userId);
    if (movieNode == nullptr || userNode == nullptr || (movieNode->m_value->getIsVip() && !userNode->m_value.isVip)) {
        return StatusType::FAILURE;
    }

    // remove & insert to tree, in order to place
    this->deleteMovieFromGenresTreeArray(movieNode->m_value->getGenre(), movieNode->m_value->getMovieRank());
    movieNode->m_value->rate(rating);
    this->insertMovieToGenresTreeArray(movieNode->m_value->getGenre(), *(movieNode->m_value));

    // after rate, should update max ranked in genre may have changed
    int genreInt = (int)movieNode->m_value->getGenre();
    if (m_maxViewsPerGenre[genreInt] != nullptr) {
        delete m_maxViewsPerGenre[genreInt];
        m_maxViewsPerGenre[genreInt] = nullptr;

    }

    // FindMax cannot return max
    AvlNode<MovieRank, Movie*>* movieGenreNode = m_genreTreesArray[genreInt]->FindByKey(
        m_genreTreesArray[genreInt]->FindMax()->m_key
        );

    
    this->updateTopRatedMoviePerGenre(*(movieGenreNode->m_value));
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	if (groupId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    AvlNode<int, Group>* groupNode = m_groups.FindByKey(groupId);

    if (groupNode == nullptr || groupNode->m_value.numUsers == 0) {
        return output_t<int>(StatusType::FAILURE);
    }

    int max = 0;
    int genreMax = 0;
    // exculde the NONE genre, it is not relevant for this task
    for (int i = 0; i < numGenres - 1; i++) {
        int genreViews = groupNode->m_value.usersViewsLog[i] + groupNode->m_value.groupViewsCount[i];
        if (genreViews > max) {
            genreMax = i;
            max = genreViews;
        }
    }
        if (!m_maxViewsPerGenre[genreMax]) {
            return output_t<int>(StatusType::FAILURE);
        }
        
    return output_t<int>(m_maxViewsPerGenre[genreMax]->m_value);
}


