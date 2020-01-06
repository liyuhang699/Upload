//
// Created by 86182 on 2019/12/10.
//

#ifndef BIGINT_BIGINTEGER_H
#define BIGINT_BIGINTEGER_H

#include <cstring>
#include <iostream>
#include <cmath>
#include <string>
class bigInteger{

    char *data= nullptr;
    long long size;
    int sign;
public:
    friend bool operator>(const bigInteger &b1,const bigInteger &b2);
    friend bool operator==(const bigInteger &b1,const bigInteger &b2);
    friend bool operator!=(const bigInteger &b1,const bigInteger &b2);
    friend bool operator<(const bigInteger &b1,const bigInteger &b2);
    friend bool operator<=(const bigInteger &b1,const bigInteger &b2);
    friend bool operator>=(const bigInteger &b1,const bigInteger &b2);
    friend std::ostream& operator<<(std::ostream &os,const bigInteger &b2);
    //friend std::istream& operator>>(std::istream &is,const bigInteger &b2);
    friend bigInteger operator+(const bigInteger &b1,const bigInteger &b2);
    friend bigInteger operator-(const bigInteger &b1,const bigInteger &b2);
    friend bigInteger operator*(const bigInteger &b1,const bigInteger &b2);
    friend bigInteger operator/(const bigInteger &b1,const bigInteger &b2);
    friend bigInteger operator%(const bigInteger &b1,const bigInteger &b2);
    bigInteger operator+=(const bigInteger &b1)
    {
        *this=*this+b1;
        return *this;
    }
    bigInteger operator-=(const bigInteger &b1)
    {
        *this=*this-b1;
        return *this;
    }
    bigInteger operator*=(const bigInteger &b1)
    {
        *this=*this*b1;
        return *this;
    }
    bigInteger operator/=(const bigInteger &b1)
    {
        *this=*this/b1;
        return *this;
    }

    bigInteger(char *a);
    bigInteger();
    bigInteger(bigInteger const &b2);
    bigInteger& operator=(const bigInteger &b2);
    ~bigInteger();

    explicit operator double() const
    {
        double tmpFloat=0;
        //tmpFloat == atof(data);
        for(long long i=0;i<size;i++)
        {
            tmpFloat+=(data[i]-'0')*pow(10,size-1-i);
        }
        if(sign==-1)
            tmpFloat=-tmpFloat;
        //float f=float(tmpFloat);
        return tmpFloat;
    }
    explicit operator bool() const
    {
        bool B;
        if (data[0]=='0' && size==1)B=false;
        else B=true;
        return B;
    }
    explicit operator std::string() const
    {
        std::string ans;
        if(sign == -1) ans += '-';
        for(int i=0;i<size;i++)
            ans+=data[i];
        return ans;
    }
};



#endif //BIGINT_BIGINTEGER_H

