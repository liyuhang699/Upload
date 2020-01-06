//
// Created by 86182 on 2019/12/10.
//

#include "bigInteger.h"

bigInteger::bigInteger(char *a) {
    //cout<<"gouzao1";
    size=strlen(a);

    long long i;
    if(a[0]=='-')
    {
        data=new char [size+5];
        for(i=1;i<size;i++)
            data[i-1]=a[i];
        --size;
        sign=-1;
        data[size]='\0';
    }
    else
    {
        data=new char [size+5];
        for(i=0;i<size;i++)
            data[i]=a[i];
        sign=1;
        data[size]='\0';
    }
    //std::cout<<"{"<<size<<"}";
    //if(data[size]=='\0')std::cout<<"H";
    //std::cout<<data<<" ";
}
bigInteger::bigInteger(){sign=1;size=0;data=new char[size];data[0]='0';}
bigInteger::bigInteger(bigInteger const &b2) {
    size=b2.size;
    data=new char [size+5];
    for(long long i=0;i<size;i++)
    data[i]=b2.data[i];
    sign=b2.sign;
}
bigInteger::~bigInteger() {delete []data;}

bool operator>(const bigInteger &b1,const bigInteger &b2){
    //cout<<"<1<";
    if(b1.sign>b2.sign)return true;
    if(b1.sign<b2.sign)return false;
    long long i;
    if(b1.sign>0)
    {
        if(b1.size>b2.size)return true;
        if(b1.size<b2.size)return false;
        //if(b1.size==b2.size)
        for(i=0;i<b1.size;i++)
        {
            if(b1.data[i]>b2.data[i])return true;
            else if(b1.data[i]<b2.data[i])return false;
        }
        return false;
    }
    else if(b1.sign<0)
    {
        if(b1.size>b2.size)return false;
        if(b1.size<b2.size)return true;
        //if(b1.size==b2.size)
        for(i=0;i<b1.size;i++)
        {
            if(b1.data[i]>b2.data[i])return false;
            else if(b1.data[i]<b2.data[i])return true;
        }
        return false;
    }

}
bool operator==(const bigInteger &b1,const bigInteger &b2)
{
    if(b1.size!=b2.size)return false;
    if(b1.sign!=b2.sign)return false;
    long long i;
    for(i=0;i<b1.size;i++)
    {
        if(b1.data[i]!=b2.data[i])
            return false;
    }
    return true;
}
bool operator!=(const bigInteger &b1,const bigInteger &b2)
{return !(b1==b2);}
bool operator<(const bigInteger &b1,const bigInteger &b2)
{
    if(b1.sign<b2.sign)return true;
    if(b1.sign>b2.sign)return false;
    long long i;
    if(b1.sign>0)
    {
        if(b1.size<b2.size)return true;
        if(b1.size>b2.size)return false;
        //if(b1.size==b2.size)
        for(i=0;i<b1.size;i++)
        {
            if(b1.data[i]<b2.data[i])return true;
            else if(b1.data[i]>b2.data[i])return false;
        }
        return false;
    }
    else if(b1.sign<0)
    {
        if(b1.size<b2.size)return false;
        if(b1.size>b2.size)return true;
        //if(b1.size==b2.size)
        for(i=0;i<b1.size;i++)
        {
            if(b1.data[i]<b2.data[i])return false;
            else if(b1.data[i]>b2.data[i])return true;
        }
        return false;
    }
}
bool operator>=(const bigInteger &b1,const bigInteger &b2)
{
    return !(b1<b2);
}
bool operator<=(const bigInteger &b1,const bigInteger &b2)
{
    return !(b1>b2);
}
std::ostream& operator<<(std::ostream &os,const bigInteger &b2)
{
    //std::cout<<"{"<<b2.size<<"}";
    //if(b2.data[b2.size]=='\0')std::cout<<"h";
    b2.data[b2.size]='\0';
    if (b2.sign==-1)
    os<<"-";
    //for(long long i=0;b2.data[i]!='\0';i++)
    os<<b2.data;
    return os;
}
/*std::istream& operator>>(std::istream &is,bigInteger &b2)
{
    char *a;
    is>>a;
    delete []b2.data;
    b2.size=strlen(a);
    long long i;
    if(a[0]=='-')
    {
        b2.data=new char [b2.size+5];
        for(i=1;i<b2.size;i++)
            b2.data[i-1]=a[i];
        --b2.size;
        b2.sign=-1;
        b2.data[b2.size]='\0';
    }
    else
    {
        b2.data=new char [b2.size+5];
        for(i=0;i<b2.size;i++)
            b2.data[i]=a[i];
        b2.sign=1;
        b2.data[b2.size]='\0';
    }
    return is;
}*/
bigInteger &bigInteger::operator=(const bigInteger &b2)
{
    if(this==&b2)return *this;
    delete []data;
    size=b2.size;
    data=new char[size+5];
    for(long long i=0;i<size;i++)
        data[i]=b2.data[i];
    sign=b2.sign;
    return *this;
}
bigInteger operator+(const bigInteger &b1,const bigInteger &b2)
{
    if(b1.sign < b2.sign){bigInteger tmp1=b1;tmp1.sign=1;bigInteger tmp=b2 - tmp1;return tmp;}
    if(b1.sign > b2.sign){bigInteger tmp1=b2;tmp1.sign=1;bigInteger tmp=b1 - tmp1;return tmp;}//判断是否需要调用减法
    long long i,n1=b1.size,n2=b2.size,N = (n1>n2?n1:n2);//加法
    long long B1[N],B2[N];//两个加数
    //将字符串逆序储存为整型数组，短的一个要补0
    for(i = 0;i < n1;i++)
        B1[n1-i-1] = b1.data[i] - '0';
    if(n1 != N)//补0
        for(;i < N;i++)
            B1[i]=0;
    for(i = 0;i < n2;i++)
        B2[n2 - i - 1] = b2.data[i] - '0';
    if(n2 != N)//补0
        for(;i < N;i++)
           B2[i]=0;
    long long t,ans[N+1];//t一个储存中间值的临时变量，ans中存放相加所得答案
    bool ece=false;//记录是否进位
    for(i = 0;i <= N;i++)ans[i] = 0;//初始化ans
    for (i = 0;i < N;i++)//计算过程
    {
        t=B1[i]+B2[i];
        if(ece)t++;
        if(t>=10) ece = true;
        else ece = false;
        ans[i] = t % 10;
    }
    if(ece)
    ans[i]=1;
    else {ans[i]=0;N--;}
    char a[N+2];
    for(i = 0;i <= N;i++)
        a[i] = ans[N-i]+'0';//将ans转化为字符数组，存放在a中
    a[N+1]='\0';
    bigInteger tmp(a);//用a新建一个bigInteger
    tmp.sign = b1.sign;
    tmp.size = N+1;
    return tmp;
}
bigInteger operator-(const bigInteger &b1,const bigInteger &b2)
{
    if(b1.sign>b2.sign){bigInteger tmp1=b2;tmp1.sign=1;bigInteger tmp=b1+tmp1;return tmp;}
    if(b1.sign<b2.sign){bigInteger tmp1=b2;tmp1.sign=-1;bigInteger tmp=b1+tmp1;return tmp;}
    long long Sign,n1,n2,Size;
    char *m1,*m2;
    if(b1.sign>0)//为正数
    {
        if(b1>b2) {
            Sign=1;
            m1=b1.data,m2=b2.data;//m1中存被减数；m2存减数
            n1=b1.size,n2=b2.size;
        }
        else {
            Sign=-1;
            m1=b2.data,m2=b1.data;
            n1=b2.size,n2=b1.size;
        }
    }
    if(b1.sign<0)//为负数
    {
        if(b1>b2){
            Sign=1;
            m1=b2.data,m2=b1.data;
            n1=b2.size,n2=b1.size;
        }
        else{
            Sign=-1;
            m1=b1.data,m2=b2.data;
            n1=b1.size,n2=b2.size;
        }
    }
    long long i,N = (n1>n2?n1:n2);
    long long B1[N],B2[N];
    for(i = 0;i < n1;i++)
        B1[n1-i-1] = m1[i] - '0';
    if(n1 != N)//补0
        for(;i < N;i++)
            B1[i]=0;
    for(i = 0;i < n2;i++)
        B2[n2 - i - 1] = m2[i] - '0';
    if(n2 != N)//补0
        for(;i < N;i++)
            B2[i]=0;
    long long ans[N];
    bool ece=false;
    for(i = 0;i < N;i++)
        ans[i]=0;
    //std::cout<<B1[n1]<<":"<<B2[n2];
    for (i = 0;i < N;i++)
    {
        if(ece)
        {
            if(B1[i]-1>=B2[i])
            {
                ans[i]=B1[i]-1-B2[i];
                ece=false;
            } else{
                ans[i]=B1[i]-B2[i]+9;
                ece=true;
            }
        } else{
            if(B1[i]>=B2[i])
            {
                ans[i]=B1[i]-B2[i];
                ece=false;
            } else{
                ans[i]=B1[i]-B2[i]+10;
                ece=true;
            }
        }
        //std::cout<<i<<":"<<ans[i]<<":"<<ece[i+1]<<"]";
    }
    for(i=N-1;i>=0;i--)
    {
        if(ans[i]!=0)
            break;
    }
    if(i<0) return bigInteger("0");
    char a[i+2];
    Size=i+1;
    for(;i>=0;i--)
    {
        a[Size-i-1]=ans[i]+'0';
    }
    a[Size]='\0';
    bigInteger tmp (a);
    tmp.size=Size;
    tmp.sign=Sign;
    return tmp;
}
bigInteger operator*(const bigInteger &b1,const bigInteger &b2)
{
    long long n1=b1.size,n2=b2.size;
    long long Sign=(b1.sign==b2.sign?1:-1),i,j;
    long long ans[n1+n2];
    long long B1[n1],B2[n2];
    for(i = 0;i < n1;i++)
        B1[n1 - i - 1] = b1.data[i] - '0';
    for(i = 0;i < n2;i++)
        B2[n2 - i - 1] = b2.data[i] - '0';
    //std::cout<<"b1=";for(i = 0;i < n1;i++)std::cout<<B1[i];std::cout<<"b2=";for(i = 0;i < n2;i++)std::cout<<B2[i];
    for(i=0;i<n1+n2;i++)
        ans[i]=0;
    for(i=0;i<n1;i++)
    {
        for(j=0;j<n2;j++)
        {
            ans[j+i]+=B1[i]*B2[j];
            //std::cout<<i+j<<"["<<ans[j+i]<<"]";
        }
    }
    for(i=0;i<n1+n2;i++)
    {
        if(ans[i]>=10)
        {
            ans[i+1]+=ans[i]/10;
            ans[i]=ans[i]%10;
            //std::cout<<"ans["<<i<<"]="<<ans[i]<<",ans["<<i+1<<"]="<<ans[i+1]<<",";
        }

    }
    for(i=n1+n2-1;i>=0;i--)
    {
        if(ans[i]!=0)
            break;
    }
    if(i<0) return bigInteger("0");
    //std::cout<<"/"<<i<<"/";
    char a[i+2];
    long long Size=i+1;
    //std::cout<<"{"<<Size<<"}";

    for(;i>=0;i--)
    {
        a[Size-i-1]=ans[i]+'0';
        //std::cout<<"("<<a[Size-i-1]<<")";
    }
    a[Size]='\0';
    //std::cout<<"{"<<strlen(a)<<"}";
    bigInteger tmp (a);
    tmp.sign=Sign;
    return tmp;
}
bigInteger operator/(const bigInteger &b1,const bigInteger &b2)
{
    //if(b1.data[0]=='0' && b1.size==1){bigInteger tmp("0");return tmp;}
    long long Sign=(b1.sign==b2.sign?1:-1);
    long long i,j,ans[b1.size];
    long long n1=b1.size,n2=b2.size;
    bigInteger tmp1=b1;
    tmp1.sign=1;
    //if(tmp1<b2){bigInteger tmp("0");return tmp;}
    for(i=0;i<b1.size;i++)
        ans[i]=0;//初始化
    for(i=n1-1;i>=n2-1;i--)
    {
        //std::cout<<"h";
        char B2[i+2];
        for(j=0;j<n2;j++)
            B2[j]=b2.data[j];
        for(;j<=i;j++)
            B2[j]='0';
        B2[i+1]='\0';//此时的B2完成补零
        bigInteger tmp2(B2);
        //std::cout<<tmp2<<" ";
        while(tmp2<=tmp1)
        {
            tmp1=tmp1-tmp2;
            ans[i-n2+1]++;
        }
    }

    for(i=n1-1;i>=0;i--)
    {
        if(ans[i]!=0)
            break;
    }
    if(i<0 && Sign==1) return bigInteger("0");
    if(i<0 && Sign==-1) return bigInteger("-1");
    char a[i+2];
    long long Size=i+1;
    for(;i>=0;i--)
    {
        a[Size-i-1]=ans[i]+'0';
    }
    a[Size]='\0';
    bigInteger tmp (a);
    tmp.size=Size;
    tmp.sign=Sign;
    //std::cout<<tmp1<<"|";
    bigInteger t ("1");
    if(tmp1!="0" && Sign==-1)tmp=tmp-t;
    return tmp;
}
bigInteger operator%(const bigInteger &b1,const bigInteger &b2)
{
    bigInteger tmp;
    tmp=b1-(b1/b2)*b2;
    return tmp;
}
