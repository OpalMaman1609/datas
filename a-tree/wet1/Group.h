
#ifndef GROUP_H_
#define GROUP_H_
#include "AvlTree.h"
#include "Constants.h"

class User;


class Group {
	private:
	int groupId;
    int numUsers = 0;
    //Counts how many VIP members are in the group
	int isVip = 0;
    //Sums the individual views of the users by genere
	int usersViewsLog[numGenres];
    //Counts the num of movies watched by the group by genre
    int groupMoviesCount[numGenres];
    //Sums the number of views - +=movies*numUsers
    int groupViewsCount[numGenres];
    //Tree of pointers to users
	AvlTree<int,User *> groupUsersTree;


	public:

	Group() = default;
    ~Group() = default;
	Group(int groupId) : groupId(groupId) {
		for (int i = 0; i<numGenres; i++)
		{
			usersViewsLog[i]=0;
            groupMoviesCount[i]=0;
            groupViewsCount[i]=0;
		}
	}
	friend class streaming_database;
} 
;

#endif