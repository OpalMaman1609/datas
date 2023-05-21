#ifndef DATAS_GROUP_H
#define DATAS_GROUP_H

#include "Constants.h"
#include "AvlTree.h"


class User;

template<class Key, class Value, bool SHOULD_FREE>
class AvlNode;



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
    AvlTree<int,User *, false> groupUsersTree;


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


#endif //DATAS_GROUP_H
