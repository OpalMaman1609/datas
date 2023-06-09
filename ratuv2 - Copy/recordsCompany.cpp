#include "recordsCompany.h"


RecordsCompany::RecordsCompany():m_costumers(), m_members(){}

RecordsCompany::~RecordsCompany(){}

StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records){
    return StatusType::SUCCESS;
}

StatusType RecordsCompany::addCostumer(int c_id, int phone){
    if (c_id < 0 || phone < 0){
        return StatusType::INVALID_INPUT;
    }
    Costumer toAdd = Costumer(c_id,phone);
    return m_costumers.insert_HT(c_id,toAdd);
}

Output_t<int> RecordsCompany::getPhone(int c_id){
     if (c_id < 0 ){
        return Output_t<int>(StatusType::INVALID_INPUT);
    }
    AvlNode<int, Costumer> *findCostumer = m_costumers.find_HT(c_id);
    if (!findCostumer){
        return Output_t<int>(StatusType::DOESNT_EXISTS);
    }
    return Output_t<int>(findCostumer->m_value.getCostumerPhone());
}
StatusType RecordsCompany::makeMember(int c_id){
     if (c_id < 0){
        return StatusType::INVALID_INPUT;
    }
    AvlNode<int, Costumer> *findCostumer = m_costumers.find_HT(c_id);
    if (!findCostumer){
        return StatusType::DOESNT_EXISTS;
    }
    RankNode<int, Costumer*> *findMember = m_members.find(c_id);
    if (findMember){
        return StatusType::ALREADY_EXISTS;
    }
    findCostumer->m_value.changeToMember();
    StatusType status = m_members.insert(c_id, &(findCostumer->m_value));
    if (status == StatusType::SUCCESS){
        RankNode<int, Costumer*> *member = m_members.find(c_id);
        member->privateExtra -= m_members.sumExtra(c_id);
    }
    return status;
}


Output_t<bool> RecordsCompany::isMember(int c_id){
    if (c_id < 0 ){
        return Output_t<bool>(StatusType::INVALID_INPUT);
    }
    AvlNode<int, Costumer> *findCostumer = m_costumers.find_HT(c_id);
    if (!findCostumer){
        return Output_t<bool>(StatusType::DOESNT_EXISTS);
    }
    return Output_t<bool>(findCostumer->m_value.getIfMember());
}

// StatusType RecordsCompany::buyRecord(int c_id, int r_id);
StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double  amount){
        if (c_id1 < 0 || c_id2 < 0 || c_id2 < c_id1 || amount <= 0){
            return StatusType::INVALID_INPUT;
        }
        m_members.addToExtra (c_id2, amount);
        RankNode<int, Costumer*> *member = m_members.find(c_id1);
        if (!member->left_son){
            return StatusType::SUCCESS;
        }
        RankNode<int, Costumer*> *closestMember = findMaxNode(member->left_son);
        m_members.addToExtra(closestMember->key, (-amount));
        return StatusType::SUCCESS;
}

Output_t<double> RecordsCompany::getExpenses(int c_id){
    if (c_id < 0 ){
        return Output_t<double>(StatusType::INVALID_INPUT);
    }
    RankNode<int, Costumer*> *member = m_members.find(c_id);
    if (!member){
        return Output_t<double>(StatusType::DOESNT_EXISTS);
    }
    double expenses = member->value->getExpenses() - m_members.sumExtra(c_id);
    return Output_t<double>(expenses);
}


int main(){
    RecordsCompany *test_obj = new RecordsCompany();
    StatusType status = test_obj->addCostumer(1,503);
    Output_t<int> phone = test_obj->getPhone(1);
    status = test_obj->makeMember(1);
    Output_t<bool> is = test_obj->isMember(1);
    test_obj->addCostumer(8,503);
    status = test_obj->makeMember(8);
    test_obj->addCostumer(12,503);
    status = test_obj->makeMember(12);
    test_obj->addCostumer(4,503);
    status = test_obj->makeMember(4);
    test_obj->addCostumer(2,503);
    status = test_obj->makeMember(2);
    test_obj->addCostumer(10,503);
    status = test_obj->makeMember(10);
    test_obj->addCostumer(6,503);
    status = test_obj->makeMember(6);
    test_obj->addCostumer(14,503);
    status = test_obj->makeMember(14);
    test_obj->addCostumer(1,503);
    status = test_obj->makeMember(1);
    test_obj->addCostumer(5,503);
    status = test_obj->makeMember(5);
    test_obj->addCostumer(9,503);
    status = test_obj->makeMember(9);
    test_obj->addCostumer(13,503);
    status = test_obj->makeMember(13);
    test_obj->addCostumer(3,503);
    status = test_obj->makeMember(3);
    test_obj->addCostumer(7,503);
    status = test_obj->makeMember(7);
    test_obj->addCostumer(11,503);
    status = test_obj->makeMember(11);
    test_obj->addPrize(2,3,5);

    delete test_obj;
    return 0;
}





// void RecordsCompany::addToExtra(int c_id, double  amount){
// RankNode<int, Costumer*> *root = m_members.root;
//     bool isRightSequnce = false;
//     while (root != NULL) {
//         if (c_id > root->key){
//             if (isRightSequnce == false){
//                 root->extra += amount;
//                 isRightSequnce = true;
//             }
//             root = root->right_son;
//         }
//         else if (c_id < root->key){
//             if (isRightSequnce == true){
//                 root->extra -= amount;
//                 isRightSequnce = false;
//             }
//             root = root->left_son;
//             }
//         else {
//             if (isRightSequnce == false){
//                 root->extra += amount;
//             }
//             if (root->right_son){
//                 root->right_son->extra -= amount;
//             }
//             return; 
//         }
//     }
//     return;
// }

// int RecordsCompany::sumExtra(int c_id, bool withPrivate){
// RankNode<int, Costumer*> *root = m_members.root;
//     bool isRightSequnce = false;
//     int sum = 0;
//     while (root != NULL) {
//         if (c_id > root->key){
//             sum += root->extra;
//             root = root->right_son;
//         }
//         else if (c_id < root->key){
//             sum += root->extra;
//             root = root->left_son;
//         }
//         else {
//             if (withPrivate){
//                 sum += root->privateExtra;
//             }
//             sum += root->extra;
//             return sum; 
//         }
//     }
//     return 0;
// }


// Output_t<double> RecordsCompany::getExpenses(int c_id);
// StatusType RecordsCompany::putOnTop(int r_id1, int r_id2);
// StatusType RecordsCompany::getPlace(int r_id, int *column, int *hight);
