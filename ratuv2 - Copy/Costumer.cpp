
#include "Costumer.h"

int Costumer::getCostumerPhone(){
    return m_phone;
}

bool Costumer::getIfMember(){
    return m_isMember;
}

void Costumer::changeToMember(){
    m_isMember = true;
    return;
}

void Costumer::addToExpencese(double price){
    m_expenses += price;
    return;
}

double Costumer::getExpenses(){
    return m_expenses;
}