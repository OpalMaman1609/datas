
#ifndef RATUV2_COSTUMER_H
#define RATUV2_COSTUMER_H


class Costumer
{
    int m_costumerId;
    int m_phone;
    bool m_isMember;
    double m_expenses;
public:
    Costumer() = default;
    Costumer(int c_id, int phone):m_costumerId(c_id),m_phone(phone),m_isMember(false),m_expenses(0){}
    ~Costumer() = default;
    int getCostumerPhone();
    bool getIfMember();
    void changeToMember();
    void addToExpenses(double price);
    double getExpenses();
    void deleteExpenses();

};
#endif //RATUV2_COSTUMER_H
