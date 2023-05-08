#ifndef USER_H_
#define USER_H_
#include "Group.h"
#include "Constants.h"

class User {
private:
    int userId;
    bool isVip;
    //only views from user_watch or a remomved group
    int viewsLog[numGenres];
    //Points to the user's group if exists
    Group *group = nullptr;
    //Saves the movies watched record of the group prior joining it
    int snapGroup[numGenres];

    //group pointer

public:
    User(int userId, bool isVip) : userId(userId), isVip(isVip) {
        for (int i = 0; i<numGenres; i++)
        {
            viewsLog[i]=0;
            snapGroup[i]=0;
        }
    }
    friend class streaming_database;
}
;



#endif // USER_H_