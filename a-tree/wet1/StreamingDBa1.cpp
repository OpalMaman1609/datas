#include "StreamingDBa1.h"




streaming_database::streaming_database()
{
	streaming_database::users = AvlTree<int,User>();
	streaming_database::groups = AvlTree<int,Group>();

}

streaming_database::~streaming_database()
{}


StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
	// TODO: Your code goes here
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
	//talk to roee about status
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
	if (groupId <= 0) {
		return StatusType::INVALID_INPUT;
	}
	Group groupToAdd = Group(groupId);
	return streaming_database::groups.Insert(groupId,groupToAdd);
}

StatusType streaming_database::remove_group(int groupId)
{
	if (groupId <= 0){
		return StatusType::INVALID_INPUT;
	}
	AvlNode<int, Group> *group = groups.Find(groupId);
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
	return 	groups.Delete(groupId);
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
	if (userId <= 0 || groupId <= 0){
		return StatusType::INVALID_INPUT;
	}

	AvlNode<int, User> *user = users.Find(userId);
	AvlNode<int, Group> *group = groups.Find(groupId);
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
	// TODO: Your code goes here
    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{/*
	if (userId <= 0 || groupId <= 0){
		return StatusType::INVALID_INPUT;
	}

	AvlNode<int, User> *movie = movies.Find(movieId);
	AvlNode<int, Group> *group = groups.Find(groupId);
	//If group/user is not found Find returns nullptr
	if (!user || !group || (group->value.numUsers==0)) {
		return StatusType::FAILURE;
	}
    //increase views in movie after roee is finished
	if ( 1 || group->value.isVip=0 & 1) { // add condition about movie vip/not
		return StatusType::FAILURE;
	}
	group->value.groupMoviesCount[1]++; //in brackets put movie.genre
	group->value.groupMoviesCount[StatusType::NONE];
	group->value.groupMoviesCount[1]+=group->value.numUsers;//in brackets put movie.genre
	group->value.groupViewsCount[StatusType::NONE]+=group->value.numUsers;*/
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    // TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    // TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
	// TODO: Your code goes here
	return 2008;
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    // TODO: Your code goes here
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}


