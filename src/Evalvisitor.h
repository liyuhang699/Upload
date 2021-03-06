#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H


#include "Python3BaseVisitor.h"
#include <cstring>
#include <string>
#include <cstdio>
#include "bigInteger.h"
#include <cmath>
#include <map>
#include <iomanip>
#include <algorithm>
#include <stack>
#include <stdlib.h>

class EvalVisitor: public Python3BaseVisitor {
    std::map<std::string,antlrcpp::Any> globa_quality; 
    std::map<std::string,antlrcpp::Any> quality; 
    std::stack<std::map<std::string,antlrcpp::Any>> level;
    int lev = 0;
    std::map<std::string,Python3Parser::ParametersContext*> functylist;
    std::map<std::string,Python3Parser::SuiteContext*> funcsutie;
    antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {  
        //???????
        //std::cout<<"hit Funcdef"<<std::endl;
        //lev += 1;
        //Python3Parser::ParametersContext *tylist = ctx -> parameters();
        //Python3Parser::SuiteContext *todo = ctx -> suite();
        std::string funcname = ctx -> NAME() -> toString();
        //std::cout<<"tostring"<<std::endl;
        functylist[funcname] = ctx -> parameters();
        //std::cout<<"funlist"<<std::endl;
        funcsutie[funcname] = ctx -> suite();
        //std::cout<<"complete"<<std::endl;
        return 0;
        //level.push(quality);
        //quality.clear();
        //std::cout<<"fundef1"<<std::endl;
        //antlrcpp::Any tylist = visit(ctx -> parameters());
        //std::cout<<"fundef2"<<std::endl;
        //antlrcpp::Any todo = visit(ctx -> suite());
        //std::cout<<"fundef3"<<std::endl;
        //lev -=1;
        //quality = level.top();
        //if( todo.is<std::string>() && todo.as<std::string>() != "return" 
        // || !todo.is<std::string>() && !todo.is<int>()) 
        //{
        //    return todo;
        //}else{
        //    std::cout<<"return 0 func"<<std::endl;
        //    return 0;
        //}     
    }

    antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        if(ctx -> typedargslist()){
             return visit(ctx -> typedargslist());
        }
       else return nullptr;
    }

    antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
        //std::cout<<"hit Typedargslist"<<std::endl;
        int Nna = 1,Nte = 0;
        if(ctx -> tfpdef(0)) Nna = ctx -> tfpdef().size();
        if(ctx -> test(0)) Nte = ctx -> test().size();
        //std::cout<<"hh"<<std::endl;
        antlrcpp::Any na,va;
        std::string Na;
        std::vector<std::string> Name;
        for(int i = 0;i < Nna;i++){
            //std::cout<<"h0"<<std::endl;
            na = visit(ctx -> tfpdef(i));
            //std::cout<<"h1"<<std::endl;
            //if(na.is<std::string>()) std::cout<<"["<<na.as<std::string>()<<std::endl;
            Na = '8' + na.as<std::string>();
            //std::cout<<"h2"<<std::endl;
            Name.push_back(Na);
            if(i >= Nna - Nte){
                quality[Name[i]] = visit(ctx -> test(i - Nna + Nte));
                //std::cout<<"morenzhi"<<std::endl;
            }
            //std::cout<<"canshu"<<std::endl;
        }    
        return Name;
    }

    antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
        return ctx -> NAME() -> toString();
    }

    antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override {
        //std::cout<<"hit stmt"<<std::endl;
        //if(ctx -> simple_stmt())
        //    return visit(ctx -> simple_stmt());
        //else 
        //    return visit(ctx -> compound_stmt());
        return visitChildren(ctx);
    }

    antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
        //std::cout<<"hit simple stmt"<<std::endl;
        return visit(ctx -> small_stmt());
    }

    antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
        //std::cout<<"hit small stmt"<<std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
        //std::cout<<"hit expr stmt"<<std::endl;
        bool f;
        if(ctx -> ASSIGN(0))
        {  
            if(lev != 0){
                if(quality.empty())
                {quality = globa_quality;}//如果尚无变量，取全部全局变量
                f = true;//说明这是函数内部
            }else f = false;
            //std::cout<<"("<<f<<")"<<std::endl;
            int n = ctx ->testlist().size();
            //std::cout<<"["<<n<<"]";
            antlrcpp::Any Tmp1 = visit(ctx -> testlist(n-1)),tmp1,tmp2,T1,tmp1in;
            int num1 = Tmp1.as<std::vector<antlrcpp::Any>>().size(),num2;
            antlrcpp::Any T[num1];
           for(int j = 0;j < num1;j ++){
                tmp1 = Tmp1.as<std::vector<antlrcpp::Any>>()[j];
                if(!f){//在函数外部
                    if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"')
                        T[j] = globa_quality[tmp1.as<std::string>()];
                    else T[j] = tmp1;
                }else{//在函数内部
                    if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                        //std::cout<<"hanshuneibu"<<std::endl;
                        tmp1in = '8' + tmp1.as<std::string>();//函数内部的储存
                        if(quality.find(tmp1in.as<std::string>()) != quality.end()){
                            tmp1 = tmp1in;//如果函数内部有，取函数内部的变量名
                        }
                        T[j] = quality[tmp1.as<std::string>()];
                    } else T[j] = tmp1;  
                }   
            }
            //此时T中存的是值

            for(int i = n-2;i >= 0;i--){
                
               antlrcpp::Any Tmp2 = visit(ctx -> testlist(i));
               num2 = Tmp2.as<std::vector<antlrcpp::Any>>().size();
               if(num2 != num1){std::cerr<<"Grammer error";}
               
               for(int j=0;j < num1;j++){ 
                   tmp2 = Tmp2.as<std::vector<antlrcpp::Any>>()[j];
                   
                   if(tmp2.is<std::string>() || tmp2.as<std::string>()[0] != '"'){ 
                       if(f){
                           tmp2 = '8' + tmp2.as<std::string>();//如果在函数内部，但是又不能修改全局变量，此处只能是函数内变量，加上标记
                           quality[tmp2.as<std::string>()] = T[j];
                       }else{//处于全局，直接建在全局
                           globa_quality[tmp2.as<std::string>()] = T[j];
                       }
                   }else std::cerr <<"Grammer error";
                   //std::cout<<"["<<num1<<","<<num2<<"]";
                   //std::cout<<tmp1.as<std::string>();
                   //std::cout<<tmp2.as<std::string>();
                   
                   /*if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"')//说明这里是一个变量
                   {
                       T[j] = quality[tmp1.as<std::string>()];
                      if(!tmp2.is<std::string>() || tmp2.as<std::string>()[0] == '"') std::cerr<<"Gramer error"<<std::endl;//不能把一个变量赋给一个常量
                      else {
                          /*if(f) {//不在函数内
                              tmp2 = "8" + tmp2.as<std::string>();//做出全局变量的标记，由于变量不能以数字开头，因此这里的变量独一无二
                              tmp1 = "8" + tmp1.as<std::string>();
                              globa_quality[tmp2.as<std::string>()] = globa_quality[tmp1.as<std::string>()];
                          }else{
                              if(quality.find(tmp1.as<std::string>()) == quality.end()){
                                  T1 = "8" + tmp1.as<std::string>();
                                  if(quality.find(T1.as<std::string>()) == quality.end()){
                                      std::cerr<<"Grammer error";
                                  }else{//是一个全局变量！
                                      quality[tmp2.as<std::string>()] = quality[T1.as<std::string>()];
                                  }
                              }else{*/
                                  //quality[tmp2.as<std::string>()] = quality[tmp1.as<std::string>()];
                             /* }
                          }*/
                      /*}
                   }
                   else//tmp1是常量
                   {
                      if(!tmp2.is<std::string>() || tmp2.as<std::string>()[0] == '"') std::cerr<<"Gramer error"<<std::endl;//不能把一个常量赋给一个常量
                      else {
                          /*if(!f){//不在函数内
                              tmp2 = "8" + tmp2.as<std::string>();
                              globa_quality[tmp2.as<std::string>()] = tmp1;
                          }else{*/
                              //quality[tmp2.as<std::string>()] = tmp1;
                          //}
                      /*}
                   }*/
               }
                   //Tmp1 = Tmp2;
                   //num1 = num2;
                //tmp2 = visit(ctx -> testlist(i));
            }
        }
        else if(ctx -> augassign())
        {
            //ADD_ASSIGN : '+=';SUB_ASSIGN : '-=';MULT_ASSIGN : '*=';
            //DIV_ASSIGN : '/=';MOD_ASSIGN : '%=';IDIV_ASSIGN : '//=';
            antlrcpp::Any op = visit(ctx -> augassign());
            antlrcpp::Any Tmp = visit(ctx -> testlist(0)),tmp1;
            antlrcpp::Any tmp2 = visit(ctx -> testlist(1));
            if(Tmp.as<std::vector<antlrcpp::Any>>().size() != 1)std::cerr<<"Grammer error";
            Tmp = Tmp.as<std::vector<antlrcpp::Any>>()[0];
            tmp2 = tmp2.as<std::vector<antlrcpp::Any>>()[0];
            if(!Tmp.is<std::string>() || Tmp.as<std::string>()[0] == '"'){
                std::cerr<<"Grammer error";
            }else{
                if(lev != 0) Tmp = '8' + Tmp.as<std::string>();
            }
            /*if(lev != 0){
                if(quality.empty())
                {quality = globa_quality;}//如果尚无变量，取全部全局变量
                f = true;//说明这是函数内部
            }else f = false;*/

            if(tmp2.is<std::string>() && tmp2.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + tmp2.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        tmp2 = t2;
                    }
                    tmp2 = quality[tmp2.as<std::string>()];
                }else{//在全局
                    tmp2 = globa_quality[tmp2.as<std::string>()];
                }
            }
            if(lev == 0)  tmp1 = globa_quality[Tmp.as<std::string>()];
            else tmp1 = quality[Tmp.as<std::string>()];
            if(op.as<int>() == 1 || op.as<int>() == 2){
                bool flag;
                if(op.as<int>() == 1) flag = true;
                else flag = false;
                if(tmp1.is<bigInteger>())//此前运算结果是bigint
               {
                   if(tmp2.is<bigInteger>())
                   {
                       if(flag) tmp1 = tmp1.as<bigInteger>() + tmp2.as<bigInteger>();
                       else tmp1 = tmp1.as<bigInteger>() - tmp2.as<bigInteger>();
                   }
                   else if(tmp2.is<double>())
                   {
                       if(flag) tmp1 = (double)tmp1.as<bigInteger>() + tmp2.as<double>();
                       else tmp1 = (double)tmp1.as<bigInteger>() - tmp2.as<double>();
                   }
                   else if(tmp2.is<bool>()){
                        if(tmp2.as<bool>()) {
                           bigInteger t("1");
                           if(flag)  tmp1 = tmp1.as<bigInteger>() + t;
                           else   tmp1 = tmp1.as<bigInteger>() - t;
                        }
                   }
                   else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
               } 
               else if(tmp1.is<double>()){
                   if(tmp2.is<bigInteger>()){
                       if(flag) tmp1 = tmp1.as<double>() + (double)tmp2.as<bigInteger>();
                       else tmp1 = tmp1.as<double>() - (double)tmp2.as<bigInteger>();
                   } 
                   else if(tmp2.is<double>()){
                       if(flag) tmp1 = tmp1.as<double>() + tmp2.as<double>();
                       else tmp1 = tmp1.as<double>() - tmp2.as<double>();
                   } 
                   else if(tmp2.is<bool>()){
                        if(tmp2.as<bool>()) {
                           if(flag)  tmp1 = tmp1.as<double>() + 1;
                           else   tmp1 = tmp1.as<double>() - 1;
                        }
                   }
                   else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
               }
               else if(tmp1.is<bool>()){
                   if(tmp2.is<bigInteger>()){
                       if(tmp1.as<bool>()){
                           bigInteger t("1");
                           if(flag) tmp1 = t + tmp2.as<bigInteger>();
                           else tmp1 = t - tmp2.as<bigInteger>();
                       }
                       else{
                           bigInteger t("0");
                           if(flag) tmp1 = t + tmp2.as<bigInteger>();
                           else tmp1 = t - tmp2.as<bigInteger>();
                       }
                   } 
                   else if(tmp2.is<double>()){
                       if(tmp1.as<bool>()){
                           if(flag) tmp1 = 1 + tmp2.as<double>();
                           else tmp1 = 1 - tmp2.as<double>();
                       }
                       else{
                           if(flag) tmp1 = 0 + tmp2.as<double>();
                           else tmp1 = 0 - tmp2.as<double>();
                       }
                   } 
                   else if(tmp2.is<bool>()){
                       bigInteger t1,t2;
                        if(tmp1.as<bool>()) {
                            bigInteger t("1");
                            t1 = t;
                        }
                        else{
                            bigInteger t("0");
                            t1 = t;
                        }
                        if(tmp2.as<bool>()) {
                            bigInteger t("1");
                            t2 = t;
                        }
                        else{
                            bigInteger t("0");
                            t2 = t;
                        } 
                        if(flag)tmp1 = t1 + t2;
                        else tmp1 = t1-t2;
                   }
                   else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
               }
               else if(tmp1.is<std::string>()){
                   if(tmp2.is<std::string>()){
                       std::string tmp3 = tmp2.as<std::string>();
                       tmp3.erase(0,1);
                       if(flag)
                       tmp1.as<std::string>() += tmp3;
                       else std::cerr<<"Grammer error";
                   }
                   else std::cerr<<"Grammer error";
               }
            }else if(op.as<int>() == 3){
                if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = tmp1.as<bigInteger>() * tmp2.as<bigInteger>();
                        }
                        else if(tmp2.is<double>()){
                            tmp1 = (double)tmp1.as<bigInteger>() * tmp2.as<double>();
                        }
                        else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                bigInteger t("0");
                                tmp1 = t;
                            }
                        }
                        else if(tmp2.is<std::string>()){
                            std::string t,tmp3;
                            bigInteger h("0"),l("1");
                            tmp3 = tmp2.as<std::string>();
                            tmp3.erase(0,1);
                            t += '"';
                            for(;h < tmp1.as<bigInteger>();h =h + l){
                                t += tmp3;
                            }
                            tmp1 = t;
                        }
                    }
                    else if(tmp1.is<double>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = tmp1.as<double>() * (double)tmp2.as<bigInteger>();
                        }
                        else if(tmp2.is<double>()){
                            tmp1 = tmp1.as<double>() * tmp2.as<double>();
                        }
                        else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                tmp1 = (double)0;
                            }
                        }
                        else if(tmp2.is<std::string>())std::cerr<<"Grammer error";
                    }
                    else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<bool>()){
                                tmp1 = tmp2.as<bigInteger>();
                            }else{
                                bigInteger t("0");
                                tmp1 = t;
                            }
                        }
                        else if(tmp2.is<double>()){
                            if(tmp1.as<bool>()){
                                tmp1 = tmp2.as<double>();
                            }else{
                                tmp1 = (double)0;
                            }
                        }
                        else if(tmp2.is<bool>()){
                            if(tmp1.as<bool>() && tmp2.as<bool>()){
                                bigInteger t("1");
                                tmp1 = t;
                            }else{
                                bigInteger t("0");
                                tmp1 = t;
                            }
                        }
                        else if(tmp2.is<std::string>()){
                            if(tmp1.as<bool>()){
                                tmp1 = tmp2.as<std::string>();
                            }else{
                                std::string t;
                                tmp1 = t;
                            }
                        }
                    }
                    else if(tmp1.is<std::string>()){
                        if(tmp2.is<bigInteger>()){
                            std::string t,tmp3;
                            bigInteger h("0"),l("1");
                            tmp3 = tmp1.as<std::string>();
                            tmp3.erase(0,1);
                            t += '"';
                            for(;h < tmp2.as<bigInteger>();h += l){
                                t += tmp3;
                            }
                            tmp1 = t;
                        }
                        else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                std::string t;
                                t += '"';
                                tmp1 = t;
                            }
                        }
                        else std::cerr<<"Grammer error";
                    }
            }else if(op.as<int>() == 4){
                if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = (double)((double)tmp1.as<bigInteger>() / (double)tmp2.as<bigInteger>());
                        }else if(tmp2.is<double>()){
                            tmp1 = (double)tmp1.as<bigInteger>() / tmp2.as<double>();
                        }else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                std::cerr<<"Grammer error";
                            }else{
                                tmp1 = (double)tmp1.as<bigInteger>();
                            }
                        }else if(tmp2.is<std::string>()){
                            std::cerr<<"Grammer error";
                        }  
                    }
                    else if(tmp1.is<double>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = tmp1.as<double>() / (double)tmp2.as<bigInteger>();
                        }else if(tmp2.is<double>()){
                            tmp1 = tmp1.as<double>() / tmp2.as<double>();
                        }else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                std::cerr<<"Grammer error";
                            }
                        }else if(tmp2.is<std::string>())std::cerr<<"Grammer error";
                    }
                    else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = (double)tmp1.as<bool>() / (double)tmp2.as<bigInteger>();
                        }else if(tmp2.is<double>()){
                            tmp1 = (double)tmp1.as<bool>() / tmp2.as<double>();
                        }else if(tmp2.is<bool>()){
                            tmp1 = (double)tmp1.as<bool>() / (double)tmp2.as<bool>();
                        }else if(tmp2.is<std::string>()){
                            std::cerr<<"Grammer error";
                        }
                    }
                    else if(tmp1.is<std::string>()){
                        std::cerr<<"Grammer error";
                    }
            }else if(op.as<int>() == 5){
                if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = tmp1.as<bigInteger>() % tmp2.as<bigInteger>();
                        }else if(tmp2.is<double>()){
                            std::cerr<<"undefined behavior";
                        }else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()) std::cerr<<"Grammer error";
                            else {
                                bigInteger t("1");
                                tmp1 = tmp1.as<bigInteger>() % t;
                            }
                        }else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                    }
                    else if(tmp1.is<double>()){
                        std::cerr<<"undefined behavior";
                    }
                    else if(tmp1.is<bool>()){
                        if(tmp1.as<bool>()){
                            if(tmp2.is<bigInteger>()){
                                bigInteger t("1");
                                tmp1 = t % tmp2.as<bigInteger>();
                            }else if(tmp2.is<double>()){
                                std::cerr<<"undefined behavior";
                            }else if(tmp2.is<bool>()){
                                if(tmp2.as<bool>()){
                                    bigInteger t("0");
                                    tmp1 = t;
                                }else{
                                    std::cerr<<"Grammer error";
                                }
                            }else if(tmp2.is<std::string>()){
                                std::cerr<<"Grammer error";
                            }
                        }else{
                            if(tmp2.is<bigInteger>()){
                                bigInteger t("0");
                                tmp1 = t % tmp2.as<bigInteger>();
                            }else if(tmp2.is<double>()){
                                std::cerr<<"undefined behavior";
                            }else if(tmp2.is<bool>()){
                                if(tmp2.as<bool>()){
                                    bigInteger t("0");
                                    tmp1 = t;
                                }else{
                                    std::cerr<<"Grammer error";
                                }
                            }else if(tmp2.is<std::string>()){
                                std::cerr<<"Grammer error";
                            }
                        }
                    }else if(tmp1.is<std::string>()){
                        std::cerr<<"Grammer error";
                    }
            }else if(op.as<int>() == 6){
                if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = tmp1.as<bigInteger>() / tmp2.as<bigInteger>();
                        }else if(tmp2.is<double>()){
                            std::cerr<<"undefined behavior";
                        }else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                std::cerr<<"Grammer error";
                            }
                        }else if(tmp2.is<std::string>()){
                            std::cerr<<"Grammer error";
                        }
                    }
                    else if(tmp1.is<double>()){
                        std::cerr<<"undefined behavior";
                    }
                    else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<bool>()){
                                bigInteger t("1");
                                tmp1 = t / tmp2.as<bigInteger>();
                            }else{
                                bigInteger t("0");
                                tmp1 = t / tmp2.as<bigInteger>();
                            }
                            
                        }else if(tmp2.is<double>()){
                            std::cerr<<"undefined behavior";
                        }else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                std::cerr<<"Grammer error";
                            }else{
                                if(tmp1.as<bool>()){
                                    bigInteger t("1");
                                    tmp1 = t;
                                }else{
                                    bigInteger t("0");
                                    tmp1 = t;
                                }
                            }
                        }else if(tmp2.is<std::string>()){
                            std::cerr<<"Grammer error";
                        }
                    }
                    else if(tmp1.is<std::string>()){
                        std::cerr<<"Grammer error";
                    }
            }
            if(lev == 0 ) globa_quality[Tmp.as<std::string>()] = tmp1;
            else  quality[Tmp.as<std::string>()] = tmp1;
        }
        return visitChildren(ctx);
    }

    antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override {
        //ADD_ASSIGN : '+=';SUB_ASSIGN : '-=';MULT_ASSIGN : '*=';
        //DIV_ASSIGN : '/=';MOD_ASSIGN : '%=';IDIV_ASSIGN : '//=';
        if(ctx -> ADD_ASSIGN()){
            return 1;
        }else if(ctx -> SUB_ASSIGN()){
            return 2;
        }else if(ctx -> MULT_ASSIGN()){
            return 3;
        }else if(ctx -> DIV_ASSIGN()){
            return 4;
        }else if(ctx -> MOD_ASSIGN()){
            return 5;
        }else if(ctx -> IDIV_ASSIGN()){
            return 6;
        }
    }

    antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
        //std::cout<<"hit flow stmt"<<std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
        std::string s;
        s += "break";
        return s;
        //return visitChildren(ctx);
    }

    antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
        std::string s;
        s += "continue";
        return s;
        //return visitChildren(ctx);
    }

    antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
        if(ctx -> testlist()){
            std::vector <antlrcpp::Any> ans;
            std::string s = "return";
            ans.push_back(s);
            antlrcpp::Any A = visit(ctx -> testlist());
            A = A.as<std::vector<antlrcpp::Any>>()[0];
            //std::cout<<A.as<std::string>()<<"]"<<std::endl;
            ans.push_back(A);
            return ans;
        }else{
            std::string s = "return";
            return s;
        }
    }

    antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
        //std::cout<<"hit Compound stmt"<<std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
        //std::cout<<"hit if"<<std::endl;
        int n = ctx -> test().size();
        for(int i = 0;i < n;i++){
            antlrcpp::Any con = visit(ctx -> test(i));
            if(con.is<std::vector<antlrcpp::Any>>()){
               con = con.as<std::vector<antlrcpp::Any>>()[0];
            }
            if(con.is<std::string>() && con.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + con.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        con = t2;
                    }
                    con = quality[con.as<std::string>()];
                }else{//在全局
                    con = globa_quality[con.as<std::string>()];
                }
            }
            if(con.is<bigInteger>()){
                bigInteger t0("0");
                if(con.as<bigInteger>() == t0){
                   con = false;
                }else con = true;
            }else if(con.is<double>()){
                con = (bool)con.as<double>();
            }else if(con.is<std::string>()){
                std::string s;
                s += '"';
                if(con.as<std::string>() == s){
                   con = false;
                }else con = true;
            }
            if(con.as<bool>()){
                return visit(ctx -> suite(i));
            }
        }
        if(ctx -> ELSE()){
            return visit(ctx -> suite(n));
        }else{
            return 0;
        }
        //return visitChildren(ctx);//
    }

    antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
        //std::cout<<"hit while"<<std::endl;
        antlrcpp::Any con = visit(ctx -> test()),tmp;
        if(con.is<std::vector<antlrcpp::Any>>()){
            con = con.as<std::vector<antlrcpp::Any>>()[0];
        }
        if(con.is<std::string>() && con.as<std::string>()[0] != '"'){
            if(lev != 0){
                std::string t2 = '8' + con.as<std::string>();
                if(quality.find(t2) != quality.end()){//如果内部有
                    con = t2;
                }
                con = quality[con.as<std::string>()];
            }else{//在全局
                con = globa_quality[con.as<std::string>()];
            }
        }
        if(con.is<bigInteger>()){
            bigInteger t0("0");
            if(con.as<bigInteger>() == t0){
                con = false;
            }else con = true;
        }else if(con.is<double>()){
            con = (bool)con.as<double>();
        }else if(con.is<std::string>()){
            std::string s;
            s += '"';
            if(con.as<std::string>() == s){
                con = false;
            }else con = true;
        }
        //std::cout<<con.as<bool>()<<std::endl;
        //int j=0;
        while(con.as<bool>()){
            tmp = visit(ctx -> suite());
            if(tmp.is<std::string>()){
                if(tmp.as<std::string>() == "break") break;
            }
            //std::cout<<"  H   "<<std::endl;
            con = visit(ctx -> test());
            if(con.is<std::vector<antlrcpp::Any>>()){
               con = con.as<std::vector<antlrcpp::Any>>()[0];
            }
            if(con.is<std::string>() && con.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + con.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        con = t2;
                    }
                    con = quality[con.as<std::string>()];
                }else{//在全局
                    con = globa_quality[con.as<std::string>()];
                }
            }
            if(con.is<bigInteger>()){
               bigInteger t0("0");
               if(con.as<bigInteger>() == t0){
                   con = false;
                }else con = true;
            }else if(con.is<double>()){
               con = (bool)con.as<double>();
            }else if(con.is<std::string>()){
               std::string s;
               s += '"';
               if(con.as<std::string>() == s){
                   con = false;
                }else con = true;
            }
            //j ++;
            //std::cout<<"["<<j<<"]";
        }
        return 0;
        //return visitChildren(ctx);//
    }

    antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
        //std::cout<<"in suite"<<std::endl;
        //std::cout<<"["<<lev<<"]"<<std::endl;
        if(ctx -> stmt(0) != nullptr){
            antlrcpp::Any tmp;
            //std::cout<<"stmt1"<<std::endl;
            int n = ctx -> stmt().size();
            for(int i = 0;i < n;i++){
                tmp = visit(ctx -> stmt(i));
                //std::cout<<"stmt2"<<std::endl;
                if(tmp.is<std::string>()){
                    //std::cout<<"string"<<std::endl;
                    if(tmp.as<std::string>() == "continue"){
                       std::string ans = "continue";
                       return ans;
                    }else if(tmp.as<std::string>() == "break"){
                       std::string ans = "break";
                       return ans;
                    }else if(tmp.as<std::string>() == "return"){
                        //std::cout<<"R"<<std::endl;
                        std::string ans = "return";
                        return ans;
                    }
                }else if(tmp.is<std::vector<antlrcpp::Any>>()){
                    //std::cout<<"vector"<<std::endl;
                    if(tmp.as<std::vector<antlrcpp::Any>>()[0].is<std::string>()){
                        if(tmp.as<std::vector<antlrcpp::Any>>()[0].as<std::string>() == "return"){
                            //std::cout<<tmp.as<std::vector<antlrcpp::Any>>()[1].as<std::string>()<<std::endl;
                            return tmp;
                        }
                    }
                    
                } 
                //std::cout<<"for"<<std::endl;   
            }
            //std::cout<<"return0"<<std::endl;
            return 0;
        }else if(ctx -> simple_stmt()){
            //std::cout<<"simple stmt"<<std::endl;
            antlrcpp::Any tmp;
            tmp = visit(ctx -> simple_stmt());
                if(tmp.is<std::string>()){
                    if(tmp.as<std::string>() == "continue"){
                       std::string ans = "continue";
                       return ans;
                    }else if(tmp.as<std::string>() == "break"){
                       std::string ans = "break";
                       return ans;
                    }
                }else if(tmp.is<std::vector<antlrcpp::Any>>()){
                    //std::cout<<"vector"<<std::endl;
                   if(tmp.as<std::vector<antlrcpp::Any>>()[0].is<std::string>()){
                        if(tmp.as<std::vector<antlrcpp::Any>>()[0].as<std::string>() == "return"){
                           //std::cout<<"Return"<<std::endl;
                            return tmp.as<std::vector<antlrcpp::Any>>()[1];
                        }
                    }
                }    
            return 0;
        }
    }

    antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {
        if(ctx -> OR(0)){
            int n = ctx -> and_test().size();
            antlrcpp::Any tmp1 = visit(ctx -> and_test(0)),tmp2;
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + tmp1.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        tmp1 = t2;
                    }
                    tmp1 = quality[tmp1.as<std::string>()];
                }else{//在全局
                    tmp1 = globa_quality[tmp1.as<std::string>()];
                }
                //tmp1 = quality[tmp1.as<std::string>()];
            }
            if(tmp1.as<bool>()) return true;
            else {
                //std::cout<<"or";
                for(int i = 1;i < n;i++){
                    tmp2 = visit(ctx -> and_test(i));
                    if(tmp2.is<std::string>() && tmp2.as<std::string>()[0] != '"'){
                        if(lev != 0){
                            std::string t2 = '8' + tmp2.as<std::string>();
                            if(quality.find(t2) != quality.end()){//如果内部有
                                tmp2 = t2;
                            }
                        tmp2 = quality[tmp2.as<std::string>()];
                        }else{//在全局
                            tmp2 = globa_quality[tmp2.as<std::string>()];
                        }
                    }
                    //std::cout<<"[or]";
                    if(tmp2.as<bool>())
                    {return true;}
                    
                }
                return false; 
            }
        }else{
            return visit(ctx -> and_test(0));
        }
    }

    antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
        if(ctx -> AND(0)){
            int n = ctx -> not_test().size();
            antlrcpp::Any tmp1 = visit(ctx -> not_test(0)),tmp2;
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + tmp1.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        tmp1 = t2;
                    }
                    tmp1 = quality[tmp1.as<std::string>()];
                }else{//在全局
                    tmp1 = globa_quality[tmp1.as<std::string>()];
                }
            }
            if(!tmp1.as<bool>()) return false;
            else {
                for(int i = 1;i < n;i++){
                    tmp2 = visit(ctx -> not_test(i));
                    if(tmp2.is<std::string>() && tmp2.as<std::string>()[0] != '"'){
                        if(lev != 0){
                            std::string t2 = '8' + tmp2.as<std::string>();
                            if(quality.find(t2) != quality.end()){//如果内部有
                                tmp2 = t2;
                            }
                             tmp2 = quality[tmp2.as<std::string>()];
                        }else{//在全局
                             tmp2 = globa_quality[tmp2.as<std::string>()];
                        }
                    }
                    //std::cout<<"[and]";
                    if(!tmp2.as<bool>())
                    {return false;}
                    
                } 
                return true;
            }
        }else{
            return visit(ctx -> not_test(0));
        }
    }

    antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
        if(ctx -> NOT()){
            antlrcpp::Any tmp1 = visit(ctx -> not_test());
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + tmp1.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        tmp1 = t2;
                    }
                    tmp1 = quality[tmp1.as<std::string>()];
                }else{//在全局
                    tmp1 = globa_quality[tmp1.as<std::string>()];
                }
            }
            if(tmp1.as<bool>()){
                return false;
            }else{
                return true;
            }
        }else{
            return visit(ctx -> comparison());
        }
    }

    antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
        if(ctx -> comp_op(0)){
            //1 <     2 >     3 ==    4 >=    5 <=    6 !=
            bool ans,Ans = true;
            int n = ctx -> arith_expr().size();
            antlrcpp::Any tmp1 = visit(ctx -> arith_expr(0)),tmp2,op;
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + tmp1.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        tmp1 = t2;
                    }
                    tmp1 = quality[tmp1.as<std::string>()];
                }else{//在全局
                    tmp1 = globa_quality[tmp1.as<std::string>()];
                }
            }
            for(int i = 1;i < n;i++){
                tmp2 = visit(ctx -> arith_expr(i));
                if(tmp2.is<std::string>() && tmp2.as<std::string>()[0] != '"'){
                    if(lev != 0){
                        std::string t2 = '8' + tmp2.as<std::string>();
                        if(quality.find(t2) != quality.end()){//如果内部有
                            tmp2 = t2;
                        }
                        tmp2 = quality[tmp2.as<std::string>()];
                    }else{//在全局
                        tmp2 = globa_quality[tmp2.as<std::string>()];
                    }
                }
                //读入tmp2
                op = visit(ctx -> comp_op(i-1));
                //std::cout<<"["<<op.as<int>()<<"]";
                if( op.as<int>() == 1 )//<
                {
                    if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<bigInteger>() < tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if((double)tmp1.as<bigInteger>() < tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            bigInteger t2;
                            if(tmp2.as<bool>()){
                                bigInteger t("1");
                                t2 = t;
                            }else {
                                bigInteger t("0");
                                t2 = t;
                            }
                            if(tmp1.as<bigInteger>() < t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }
                    else if(tmp1.is<double>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<double>() < (double)tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if(tmp1.as<double>() < tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            double t2;
                            if(tmp2.as<bool>()){
                                t2 = 1.0;
                            }else {
                                t2 = 0.0;
                            }
                            if(tmp1.as<double>() < t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }else if(tmp1.is<std::string>()){
                        if(tmp2.is<std::string>()){
                            if(tmp1.as<std::string>() < tmp2.as<std::string>()) ans = true;
                            else ans = false;
                        }
                        else std::cerr<<"Grammer error";
                    }else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            bigInteger t1;
                            if(tmp1.as<bool>()){
                                bigInteger t("1");
                                t1 = t;
                            }else {
                                bigInteger t("0");
                                t1 = t;
                            }
                            if(t1 < tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            double t1;
                            if(tmp1.as<bool>()) t1 = 1.0;
                            else t1 = 0.0;
                            if(t1 < tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            if(tmp1.as<bool>() < tmp2.as<bool>()) ans = true;
                            else ans = false;
                        }
                    }
                } else if( op.as<int>() == 2)//>
                {
                    if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<bigInteger>() > tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if((double)tmp1.as<bigInteger>() > tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            bigInteger t2;
                            if(tmp2.as<bool>()){
                                bigInteger t("1");
                                t2 = t;
                            }else {
                                bigInteger t("0");
                                t2 = t;
                            }
                            if(tmp1.as<bigInteger>() > t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }
                    else if(tmp1.is<double>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<double>() > (double)tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if(tmp1.as<double>() > tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            double t2;
                            if(tmp2.as<bool>()){
                                t2 = 1.0;
                            }else {
                                t2 = 0.0;
                            }
                            if(tmp1.as<double>() > t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }else if(tmp1.is<std::string>()){
                        if(tmp2.is<std::string>()){
                            if(tmp1.as<std::string>() > tmp2.as<std::string>()) ans = true;
                            else ans = false;
                        }
                        else std::cerr<<"Grammer error";
                    }else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            bigInteger t1;
                            if(tmp1.as<bool>()){
                                bigInteger t("1");
                                t1 = t;
                            }else {
                                bigInteger t("0");
                                t1 = t;
                            }
                            if(t1 > tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            double t1;
                            if(tmp1.as<bool>()) t1 = 1.0;
                            else t1 = 0.0;
                            if(t1 > tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            if(tmp1.as<bool>() > tmp2.as<bool>()) ans = true;
                            else ans = false;
                        }
                    }
                } else if( op.as<int>() == 3)//==
                {
                    if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<bigInteger>() == tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if((double)tmp1.as<bigInteger>() == tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) ans = false;
                        else if(tmp2.is<bool>()){
                            bigInteger t2;
                            if(tmp2.as<bool>()){
                                bigInteger t("1");
                                t2 = t;
                            }else {
                                bigInteger t("0");
                                t2 = t;
                            }
                            if(tmp1.as<bigInteger>() == t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }
                    else if(tmp1.is<double>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<double>() == (double)tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if(tmp1.as<double>() == tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) ans = false;
                        else if(tmp2.is<bool>()){
                            double t2;
                            if(tmp2.as<bool>()){
                                t2 = 1.0;
                            }else {
                                t2 = 0.0;
                            }
                            if(tmp1.as<double>() == t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }else if(tmp1.is<std::string>()){
                        if(tmp2.is<std::string>()){
                            if(tmp1.as<std::string>() == tmp2.as<std::string>()) ans = true;
                            else ans = false;
                        }
                        else ans = false;
                    }else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            bigInteger t1;
                            if(tmp1.as<bool>()){
                                bigInteger t("1");
                                t1 = t;
                            }else {
                                bigInteger t("0");
                                t1 = t;
                            }
                            if(t1 == tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            double t1;
                            if(tmp1.as<bool>()) t1 = 1.0;
                            else t1 = 0.0;
                            if(t1 == tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) ans = false;
                        else if(tmp2.is<bool>()){
                            if(tmp1.as<bool>() == tmp2.as<bool>()) ans = true;
                            else ans = false;
                        }
                    }
                } else if( op.as<int>() == 4)//>=
                {
                     if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<bigInteger>() >= tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if((double)tmp1.as<bigInteger>() >= tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            bigInteger t2;
                            if(tmp2.as<bool>()){
                                bigInteger t("1");
                                t2 = t;
                            }else {
                                bigInteger t("0");
                                t2 = t;
                            }
                            if(tmp1.as<bigInteger>() >= t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }
                    else if(tmp1.is<double>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<double>() >= (double)tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if(tmp1.as<double>() >= tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            double t2;
                            if(tmp2.as<bool>()){
                                t2 = 1.0;
                            }else {
                                t2 = 0.0;
                            }
                            if(tmp1.as<double>() >= t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }else if(tmp1.is<std::string>()){
                        if(tmp2.is<std::string>()){
                            if(tmp1.as<std::string>() >= tmp2.as<std::string>()) ans = true;
                            else ans = false;
                        }
                        else std::cerr<<"Grammer error";
                    }else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            bigInteger t1;
                            if(tmp1.as<bool>()){
                                bigInteger t("1");
                                t1 = t;
                            }else {
                                bigInteger t("0");
                                t1 = t;
                            }
                            if(t1 >= tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            double t1;
                            if(tmp1.as<bool>()) t1 = 1.0;
                            else t1 = 0.0;
                            if(t1 >= tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            if(tmp1.as<bool>() >= tmp2.as<bool>()) ans = true;
                            else ans = false;
                        }
                    }
                } else if( op.as<int>() == 5)//<=
                {
                    if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<bigInteger>() <= tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if((double)tmp1.as<bigInteger>() <= tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            bigInteger t2;
                            if(tmp2.as<bool>()){
                                bigInteger t("1");
                                t2 = t;
                            }else {
                                bigInteger t("0");
                                t2 = t;
                            }
                            if(tmp1.as<bigInteger>() <= t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }
                    else if(tmp1.is<double>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<double>() <= (double)tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if(tmp1.as<double>() <= tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            double t2;
                            if(tmp2.as<bool>()){
                                t2 = 1.0;
                            }else {
                                t2 = 0.0;
                            }
                            if(tmp1.as<double>() <= t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }else if(tmp1.is<std::string>()){
                        if(tmp2.is<std::string>()){
                            if(tmp1.as<std::string>() <= tmp2.as<std::string>()) ans = true;
                            else ans = false;
                        }
                        else std::cerr<<"Grammer error";
                    }else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            bigInteger t1;
                            if(tmp1.as<bool>()){
                                bigInteger t("1");
                                t1 = t;
                            }else {
                                bigInteger t("0");
                                t1 = t;
                            }
                            if(t1 <= tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            double t1;
                            if(tmp1.as<bool>()) t1 = 1.0;
                            else t1 = 0.0;
                            if(t1 <= tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                        else if(tmp2.is<bool>()){
                            if(tmp1.as<bool>() <= tmp2.as<bool>()) ans = true;
                            else ans = false;
                        }
                    }
                } else if( op.as<int>() == 6)//!=
                {
                    if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<bigInteger>() != tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if((double)tmp1.as<bigInteger>() != tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) ans = true;
                        else if(tmp2.is<bool>()){
                            bigInteger t2;
                            if(tmp2.as<bool>()){
                                bigInteger t("1");
                                t2 = t;
                            }else {
                                bigInteger t("0");
                                t2 = t;
                            }
                            if(tmp1.as<bigInteger>() != t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }
                    else if(tmp1.is<double>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<double>() != (double)tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            if(tmp1.as<double>() != tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) ans = true;
                        else if(tmp2.is<bool>()){
                            double t2;
                            if(tmp2.as<bool>()){
                                t2 = 1.0;
                            }else {
                                t2 = 0.0;
                            }
                            if(tmp1.as<double>() != t2){
                                ans = true; 
                            }else ans = false;
                        }
                    }else if(tmp1.is<std::string>()){
                        if(tmp2.is<std::string>()){
                            if(tmp1.as<std::string>() != tmp2.as<std::string>()) ans = true;
                            else ans = false;
                        }
                        else ans = true;
                    }else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            bigInteger t1;
                            if(tmp1.as<bool>()){
                                bigInteger t("1");
                                t1 = t;
                            }else {
                                bigInteger t("0");
                                t1 = t;
                            }
                            if(t1 != tmp2.as<bigInteger>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<double>()){
                            double t1;
                            if(tmp1.as<bool>()) t1 = 1.0;
                            else t1 = 0.0;
                            if(t1 != tmp2.as<double>()){
                                ans = true;
                            } else ans = false;
                        } else if(tmp2.is<std::string>()) ans = true;
                        else if(tmp2.is<bool>()){
                            if(tmp1.as<bool>() != tmp2.as<bool>()) ans = true;
                            else ans = false;
                        }
                    }
                }
                //将tmp2赋给tmp1，作为下次运算的左运算符
                //测试短路
                //std::cout<<"["<<"h"<<"]";
                tmp1 = tmp2;
                //短路
                if(!ans){
                    Ans = false;
                    break;
                }
            }
            return Ans;
        }
        else {
            return visit(ctx -> arith_expr(0));
        }
        
    }

    antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override {
        if(ctx -> LESS_THAN()){
            int op = 1;
            return op;
        } else if(ctx -> GREATER_THAN()){
            int op = 2;
            return op;
        } else if(ctx -> EQUALS()){
            int op = 3;
            return op;
        } else if(ctx -> GT_EQ()){
            int op = 4;
            return op;
        } else if(ctx -> LT_EQ()){
            int op = 5;
            return op;
        } else if(ctx -> NOT_EQ_2()){
            int op = 6;
            return op;
        }
    }

    antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
        int n = ctx -> term().size();
        if(n == 1)
        return visit(ctx -> term(0));
        else{
            bool flag;//true 为加法，false 为减法；
            antlrcpp::Any tmp1 = visit(ctx -> term(0));
            antlrcpp::Any tmp2,Tmp2;
            int addnow = 0,minusnow = 0;
            size_t atmp = 0,mtmp = 0;
            int addN = ctx -> ADD().size(),minusN = ctx -> MINUS().size();
            //std::cout<<"["<<addN<<","<<minusN<<"]";
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + tmp1.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        tmp1 = t2;
                    }
                    tmp1 = quality[tmp1.as<std::string>()];
                }else{//在全局
                    tmp1 = globa_quality[tmp1.as<std::string>()];
                }
            }
            //std::cout<<"{"<<n<<"}";
            for(int i = 1;i<n;i++){
                //获取tmp2
                Tmp2 = visit(ctx -> term(i));
                if(Tmp2.is<std::string>() && Tmp2.as<std::string>()[0] != '"'){
                   if(lev != 0){
                        std::string t2 = '8' + Tmp2.as<std::string>();
                        if(quality.find(t2) != quality.end()){//如果内部有
                            Tmp2 = t2;
                        }
                        tmp2 = quality[Tmp2.as<std::string>()];
                    }else{//在全局
                        tmp2 = globa_quality[Tmp2.as<std::string>()];
                    }
                }
                else tmp2 = Tmp2;
                //if(tmp2.is<bigInteger>())std::cout<<"["<<tmp2.as<bigInteger>()<<"]";
                //else if(tmp2.is<bool>())std::cout<<"["<<tmp2.as<bool>()<<"]";
                //else if(tmp2.is<double>())std::cout<<"["<<tmp2.as<double>()<<"]";
                //获取此次运算符号
                if(addN !=0 && minusN != 0){
                    //if(addnow > addN || minusnow > minusN) std::cerr << "Grammer error";
                    if(addN != 0 && addnow < addN)atmp = ctx -> ADD(addnow) -> getSymbol() -> getTokenIndex();
                    if(minusN != 0 && minusnow < minusN)mtmp = ctx -> MINUS(minusnow) -> getSymbol() -> getTokenIndex();
                    //std::cout<<"#"<<atmp<<","<<mtmp<<"#";
                    if(addN != addnow && minusN != minusnow){
                        if(atmp < mtmp){
                           flag = true;
                           addnow += 1;
                        }
                        else if(atmp > mtmp){
                           flag = false;
                           minusnow +=1;
                        }
                        else std::cerr<<"runtime error";
                    }
                    else if(addN == addnow) flag = false;
                    else flag = true;
                }
                else if(addN == 0)
                flag = false;
                else flag = true;
                
                //std::cout<<"("<<flag<<")";
                
                //开始运算，此时tmp1,tmp2里存下是两个any，flag里存下此次运算的符号
                /*运算规则
                合法运算 bigint +- bigint double +- double bool +- bool 
                         bool +- bigint   double +- bool
                         bigint +- double //自动转化为double，最少保留1位小数
                         string + string //拼接
                非法运算 bigint +- string   double +- string
                         string - string   string +- bool   
                */
               if(tmp1.is<bigInteger>())//此前运算结果是bigint
               {
                   if(tmp2.is<bigInteger>())
                   {
                       if(flag) tmp1 = tmp1.as<bigInteger>() + tmp2.as<bigInteger>();
                       else tmp1 = tmp1.as<bigInteger>() - tmp2.as<bigInteger>();
                   }
                   else if(tmp2.is<double>())
                   {
                       if(flag) tmp1 = (double)tmp1.as<bigInteger>() + tmp2.as<double>();
                       else tmp1 = (double)tmp1.as<bigInteger>() - tmp2.as<double>();
                   }
                   else if(tmp2.is<bool>()){
                        if(tmp2.as<bool>()) {
                           bigInteger t("1");
                           if(flag)  tmp1 = tmp1.as<bigInteger>() + t;
                           else   tmp1 = tmp1.as<bigInteger>() - t;
                        }
                   }
                   else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
               } 
               else if(tmp1.is<double>()){
                   if(tmp2.is<bigInteger>()){
                       if(flag) tmp1 = tmp1.as<double>() + (double)tmp2.as<bigInteger>();
                       else tmp1 = tmp1.as<double>() - (double)tmp2.as<bigInteger>();
                   } 
                   else if(tmp2.is<double>()){
                       if(flag) tmp1 = tmp1.as<double>() + tmp2.as<double>();
                       else tmp1 = tmp1.as<double>() - tmp2.as<double>();
                   } 
                   else if(tmp2.is<bool>()){
                        if(tmp2.as<bool>()) {
                           if(flag)  tmp1 = tmp1.as<double>() + 1;
                           else   tmp1 = tmp1.as<double>() - 1;
                        }
                   }
                   else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
               }
               else if(tmp1.is<bool>()){
                   if(tmp2.is<bigInteger>()){
                       if(tmp1.as<bool>()){
                           bigInteger t("1");
                           if(flag) tmp1 = t + tmp2.as<bigInteger>();
                           else tmp1 = t - tmp2.as<bigInteger>();
                       }
                       else{
                           bigInteger t("0");
                           if(flag) tmp1 = t + tmp2.as<bigInteger>();
                           else tmp1 = t - tmp2.as<bigInteger>();
                       }
                   } 
                   else if(tmp2.is<double>()){
                       if(tmp1.as<bool>()){
                           if(flag) tmp1 = 1 + tmp2.as<double>();
                           else tmp1 = 1 - tmp2.as<double>();
                       }
                       else{
                           if(flag) tmp1 = 0 + tmp2.as<double>();
                           else tmp1 = 0 - tmp2.as<double>();
                       }
                   } 
                   else if(tmp2.is<bool>()){
                       bigInteger t1,t2;
                        if(tmp1.as<bool>()) {
                            bigInteger t("1");
                            t1 = t;
                        }
                        else{
                            bigInteger t("0");
                            t1 = t;
                        }
                        if(tmp2.as<bool>()) {
                            bigInteger t("1");
                            t2 = t;
                        }
                        else{
                            bigInteger t("0");
                            t2 = t;
                        } 
                        if(flag)tmp1 = t1 + t2;
                        else tmp1 = t1-t2;
                   }
                   else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
               }
               else if(tmp1.is<std::string>()){
                   if(tmp2.is<std::string>()){
                       std::string tmp3 = tmp2.as<std::string>();
                       tmp3.erase(0,1);
                       if(flag)
                       tmp1.as<std::string>() += tmp3;
                       else std::cerr<<"Grammer error";
                   }
                   else std::cerr<<"Grammer error";
               }
            }
            return tmp1;
        }
    }

    antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
        int n = ctx -> factor().size();

        if(n == 1) return visit(ctx -> factor(0));
        else{
            antlrcpp::Any tmp1 = visit(ctx -> factor(0));
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + tmp1.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        tmp1 = t2;
                    }
                    tmp1 = quality[tmp1.as<std::string>()];
                }else{//在全局
                    tmp1 = globa_quality[tmp1.as<std::string>()];
                }
            }

            antlrcpp::Any tmp2,Tmp2;
            int starN = ctx -> STAR().size();//标记1乘法
            int divN = ctx -> DIV().size();//标记2除法
            int modN = ctx -> MOD().size();//标记3取模
            int idivN = ctx -> IDIV().size();//标记4整除
            //std::cout<<"("<<starN<<" "<<divN<<" "<<modN<<" "<<idivN<<")";
            int symbol = 0;
            bool flag1 = true,flag2 = true,flag3 = true,flag4 = true;
            size_t startmp=0,divtmp=0,modtmp=0,idivtmp=0;
            int starnow=0,divnow=0,modnow=0,idivnow=0;

            for(int i=1;i < n;i++){
                Tmp2 = visit(ctx -> factor(i));
                if(Tmp2.is<std::string>() && Tmp2.as<std::string>()[0] != '"'){
                   if(lev != 0){
                        std::string t2 = '8' + Tmp2.as<std::string>();
                        if(quality.find(t2) != quality.end()){//如果内部有
                            Tmp2 = t2;
                        }
                        tmp2 = quality[Tmp2.as<std::string>()];
                    }else{//在全局
                        tmp2 = globa_quality[Tmp2.as<std::string>()];
                    }
                }
                else tmp2 = Tmp2;
                
                if(flag1 && starN != 0 && starnow < starN)
                startmp = ctx -> STAR(starnow) -> getSymbol() -> getTokenIndex();
                else flag1 = false;
                if(flag2 && divN != 0 && divnow < divN)
                divtmp = ctx -> DIV(divnow) -> getSymbol() -> getTokenIndex();
                else flag2 = false;
                if(flag3 && modN != 0 && modnow < modN)
                modtmp = ctx -> MOD(modnow) -> getSymbol() -> getTokenIndex();
                else flag3 = false;
                if(flag4 && idivN != 0 && idivnow < idivN)
                idivtmp = ctx -> IDIV(idivnow) -> getSymbol() -> getTokenIndex();
                else flag4 = false;
                //std::cout<<"["<<startmp<<" "<<divtmp<<" "<<modtmp<<" "<<idivtmp<<"]";
                size_t S[4]={startmp,divtmp,modtmp,idivtmp};
                std::sort(S,S+4);
                for(int j =0;j<4;j++){
                    if(S[j] == startmp && flag1) {symbol = 1; starnow++; break;}
                    else if(S[j] == divtmp && flag2) {symbol = 2; divnow++; break;}
                    else if(S[j] == modtmp && flag3) {symbol = 3; modnow++; break;}
                    else if(S[j] == idivtmp && flag4) {symbol = 4; idivnow++; break;}
                }
                //std::cout<<"{"<<symbol<<"}";
                if(symbol == 1){
                    if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = tmp1.as<bigInteger>() * tmp2.as<bigInteger>();
                        }
                        else if(tmp2.is<double>()){
                            tmp1 = (double)tmp1.as<bigInteger>() * tmp2.as<double>();
                        }
                        else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                bigInteger t("0");
                                tmp1 = t;
                            }
                        }
                        else if(tmp2.is<std::string>()){
                            std::string t,tmp3;
                            bigInteger h("0"),l("1");
                            tmp3 = tmp2.as<std::string>();
                            tmp3.erase(0,1);
                            t += '"';
                            for(;h < tmp1.as<bigInteger>();h =h + l){
                                t += tmp3;
                            }
                            tmp1 = t;
                        }
                    }
                    else if(tmp1.is<double>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = tmp1.as<double>() * (double)tmp2.as<bigInteger>();
                        }
                        else if(tmp2.is<double>()){
                            tmp1 = tmp1.as<double>() * tmp2.as<double>();
                        }
                        else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                tmp1 = (double)0;
                            }
                        }
                        else if(tmp2.is<std::string>())std::cerr<<"Grammer error";
                    }
                    else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<bool>()){
                                tmp1 = tmp2.as<bigInteger>();
                            }else{
                                bigInteger t("0");
                                tmp1 = t;
                            }
                        }
                        else if(tmp2.is<double>()){
                            if(tmp1.as<bool>()){
                                tmp1 = tmp2.as<double>();
                            }else{
                                tmp1 = (double)0;
                            }
                        }
                        else if(tmp2.is<bool>()){
                            if(tmp1.as<bool>() && tmp2.as<bool>()){
                                bigInteger t("1");
                                tmp1 = t;
                            }else{
                                bigInteger t("0");
                                tmp1 = t;
                            }
                        }
                        else if(tmp2.is<std::string>()){
                            if(tmp1.as<bool>()){
                                tmp1 = tmp2.as<std::string>();
                            }else{
                                std::string t;
                                tmp1 = t;
                            }
                        }
                    }
                    else if(tmp1.is<std::string>()){
                        if(tmp2.is<bigInteger>()){
                            std::string t,tmp3;
                            bigInteger h("0"),l("1");
                            tmp3 = tmp1.as<std::string>();
                            tmp3.erase(0,1);
                            t += '"';
                            for(;h < tmp2.as<bigInteger>();h += l){
                                t += tmp3;
                            }
                            tmp1 = t;
                        }
                        else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                std::string t;
                                t += '"';
                                tmp1 = t;
                            }
                        }
                        else std::cerr<<"Grammer error";
                    }
                }
                else if(symbol == 2){
                    if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = (double)((double)tmp1.as<bigInteger>() / (double)tmp2.as<bigInteger>());
                        }else if(tmp2.is<double>()){
                            tmp1 = (double)tmp1.as<bigInteger>() / tmp2.as<double>();
                        }else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                std::cerr<<"Grammer error";
                            }else{
                                tmp1 = (double)tmp1.as<bigInteger>();
                            }
                        }else if(tmp2.is<std::string>()){
                            std::cerr<<"Grammer error";
                        }  
                    }
                    else if(tmp1.is<double>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = tmp1.as<double>() / (double)tmp2.as<bigInteger>();
                        }else if(tmp2.is<double>()){
                            tmp1 = tmp1.as<double>() / tmp2.as<double>();
                        }else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                std::cerr<<"Grammer error";
                            }
                        }else if(tmp2.is<std::string>())std::cerr<<"Grammer error";
                    }
                    else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = (double)tmp1.as<bool>() / (double)tmp2.as<bigInteger>();
                        }else if(tmp2.is<double>()){
                            tmp1 = (double)tmp1.as<bool>() / tmp2.as<double>();
                        }else if(tmp2.is<bool>()){
                            tmp1 = (double)tmp1.as<bool>() / (double)tmp2.as<bool>();
                        }else if(tmp2.is<std::string>()){
                            std::cerr<<"Grammer error";
                        }
                    }
                    else if(tmp1.is<std::string>()){
                        std::cerr<<"Grammer error";
                    }
                }
                else if(symbol == 3){//取模运算
                    if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = tmp1.as<bigInteger>() % tmp2.as<bigInteger>();
                        }else if(tmp2.is<double>()){
                            std::cerr<<"undefined behavior";
                        }else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()) std::cerr<<"Grammer error";
                            else {
                                bigInteger t("1");
                                tmp1 = tmp1.as<bigInteger>() % t;
                            }
                        }else if(tmp2.is<std::string>()) std::cerr<<"Grammer error";
                    }
                    else if(tmp1.is<double>()){
                        std::cerr<<"undefined behavior";
                    }
                    else if(tmp1.is<bool>()){
                        if(tmp1.as<bool>()){
                            if(tmp2.is<bigInteger>()){
                                bigInteger t("1");
                                tmp1 = t % tmp2.as<bigInteger>();
                            }else if(tmp2.is<double>()){
                                std::cerr<<"undefined behavior";
                            }else if(tmp2.is<bool>()){
                                if(tmp2.as<bool>()){
                                    bigInteger t("0");
                                    tmp1 = t;
                                }else{
                                    std::cerr<<"Grammer error";
                                }
                            }else if(tmp2.is<std::string>()){
                                std::cerr<<"Grammer error";
                            }
                        }else{
                            if(tmp2.is<bigInteger>()){
                                bigInteger t("0");
                                tmp1 = t % tmp2.as<bigInteger>();
                            }else if(tmp2.is<double>()){
                                std::cerr<<"undefined behavior";
                            }else if(tmp2.is<bool>()){
                                if(tmp2.as<bool>()){
                                    bigInteger t("0");
                                    tmp1 = t;
                                }else{
                                    std::cerr<<"Grammer error";
                                }
                            }else if(tmp2.is<std::string>()){
                                std::cerr<<"Grammer error";
                            }
                        }
                    }else if(tmp1.is<std::string>()){
                        std::cerr<<"Grammer error";
                    }
                }
                else if(symbol == 4){
                    if(tmp1.is<bigInteger>()){
                        if(tmp2.is<bigInteger>()){
                            tmp1 = tmp1.as<bigInteger>() / tmp2.as<bigInteger>();
                        }else if(tmp2.is<double>()){
                            std::cerr<<"undefined behavior";
                        }else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                std::cerr<<"Grammer error";
                            }
                        }else if(tmp2.is<std::string>()){
                            std::cerr<<"Grammer error";
                        }
                    }
                    else if(tmp1.is<double>()){
                        std::cerr<<"undefined behavior";
                    }
                    else if(tmp1.is<bool>()){
                        if(tmp2.is<bigInteger>()){
                            if(tmp1.as<bool>()){
                                bigInteger t("1");
                                tmp1 = t / tmp2.as<bigInteger>();
                            }else{
                                bigInteger t("0");
                                tmp1 = t / tmp2.as<bigInteger>();
                            }
                            
                        }else if(tmp2.is<double>()){
                            std::cerr<<"undefined behavior";
                        }else if(tmp2.is<bool>()){
                            if(!tmp2.as<bool>()){
                                std::cerr<<"Grammer error";
                            }else{
                                if(tmp1.as<bool>()){
                                    bigInteger t("1");
                                    tmp1 = t;
                                }else{
                                    bigInteger t("0");
                                    tmp1 = t;
                                }
                            }
                        }else if(tmp2.is<std::string>()){
                            std::cerr<<"Grammer error";
                        }
                    }
                    else if(tmp1.is<std::string>()){
                        std::cerr<<"Grammer error";
                    }
                }
            }
            return tmp1;
        }
    }

    antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override {
        if(ctx -> MINUS()){
            //std::cout<<"h";
            antlrcpp::Any tmp = visit(ctx -> factor());
            if(tmp.is<std::string>() && tmp.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + tmp.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        tmp = t2;
                    }
                    tmp = quality[tmp.as<std::string>()];
                }else{//在全局
                    tmp = globa_quality[tmp.as<std::string>()];
                }
            }
            if(tmp.is<bigInteger>()){
                bigInteger t("0");
                tmp = t - tmp.as<bigInteger>();
            }else if(tmp.is<double>()){
                tmp = -tmp.as<double>();
            }else if(tmp.is<bool>()){
                if(tmp.as<bool>()){
                    bigInteger t("-1");
                    tmp = t;
                }else{
                    bigInteger t("0");
                    tmp = t;
                }
            }else if(tmp.is<std::string>()){
                std::cerr<<"Grammer error";
            }
            return tmp;
        }
        else if(ctx -> ADD()){
            antlrcpp::Any tmp = visit(ctx -> factor());
            if(tmp.is<std::string>() && tmp.as<std::string>()[0] != '"'){
                if(lev != 0){
                    std::string t2 = '8' + tmp.as<std::string>();
                    if(quality.find(t2) != quality.end()){//如果内部有
                        tmp = t2;
                    }
                    tmp = quality[tmp.as<std::string>()];
                }else{//在全局
                    tmp = globa_quality[tmp.as<std::string>()];
                }
            }
            if(tmp.is<bool>()){
                if(tmp.as<bool>()){
                    bigInteger t("1");
                    tmp = t;
                }else{
                    bigInteger t("0");
                    tmp = t;
                }
            }else if(tmp.is<std::string>()){
                std::cerr<<"Grammer error";
            }

            return tmp;
        }
        else{
            return visit(ctx -> atom_expr());
        }
        
    }

    antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
        //std::cout<<"    visit atom_expr    ";
        //std::cout<<"hit atom expr"<<std::endl;
        if(ctx -> trailer())
        {
            antlrcpp::Any tmp0,tmp1,tmp2;
            
            tmp0 = visit(ctx -> trailer());
            //if(tmp0.is<std::vector<antlrcpp::Any>>())
            int num = tmp0.as<std::vector<antlrcpp::Any>>().size();
            //std::cout<<"{"<<num<<"}";
            tmp2 = visit(ctx -> atom()); 
            if(tmp2.as<std::string>() == "print"){//输出函数
               for(int b = 0;b < num;b++){
                   tmp1 = tmp0.as<std::vector<antlrcpp::Any>>()[b];
                   if(tmp1.is<std::string>()){//为一个字符串
                    if(tmp1.as<std::string>()[0]=='"'){
                        tmp1.as<std::string>().erase(0,1);
                        std::cout<<tmp1.as<std::string>();
                    }
                    else{//为一个变量
                        if(lev != 0){//在函数内部
                            std::string t2 = '8' + tmp1.as<std::string>();
                            if(quality.find(t2) != quality.end()){//如果内部有
                                tmp1 = t2;
                            }
                            tmp1 = quality[tmp1.as<std::string>()];
                        }else{//在全局
                            tmp1 = globa_quality[tmp1.as<std::string>()];
                        }
                        if(tmp1.is<std::string>()){
                            std::string out = tmp1.as<std::string>();
                            out.erase(0,1);
                            std::cout<<out;
                        }
                        else if(tmp1.is<bigInteger>()){
                            std::cout<<tmp1.as<bigInteger>();
                        }
                        else if(tmp1.is<double>()){
                            //printf("%.6lf",quality.at(tmp1.as<std::string>()).as<double>());
                            std::cout<<std::fixed<<std::setprecision(6)<<tmp1.as<double>();
                        } 
                        else if(tmp1.is<bool>()){
                            if(tmp1.as<bool>())
                            std::cout<<"True";
                            else std::cout<<"False";
                        } 
                        else if(tmp1.is<int>()){
                               std::cout<<"None";
                        }
                    }
                }
                else if(tmp1.is<bigInteger>())//为一个整数
                std::cout<<tmp1.as<bigInteger>();
                else if(tmp1.is<double>())//为一个浮点数
                //printf("%.6lf",quality.at(tmp1.as<std::string>()).as<double>());
                std::cout<<std::fixed<<std::setprecision(6)<<tmp1.as<double>();
                else if(tmp1.is<bool>()){//为一个bool
                    if(tmp1.as<bool>())
                    std::cout<<"True";
                    else
                    std::cout<<"False";
                }
                else if(tmp1.is<int>())//为none
                std::cout<<"None";
                else if(tmp1.is<short>()){
                    //do nothing
                }

                std::cout<<" ";
              }
              std::cout<<std::endl;
              return visit(ctx -> atom()); 
            } else if(tmp2.as<std::string>() == "int"){
                if(num != 1) std::cerr<<"Grammer";
                tmp1 = tmp0.as<std::vector<antlrcpp::Any>>()[0];
                if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                    if(lev != 0){
                        std::string t2 = '8' + tmp1.as<std::string>();
                        if(quality.find(t2) != quality.end()){//如果内部有
                            tmp1 = t2;
                        }
                        tmp1 = quality[tmp1.as<std::string>()];
                    }else{//在全局
                        tmp1 = globa_quality[tmp1.as<std::string>()];
                    }
                }
                if(tmp1.is<double>()){
                    bool flag;
                    int s1 = (int)tmp1.as<double>();
                    //std::cout<<"{"<<s1<<"}";
                    std::string s2;
                    if(s1 < 0){
                        s1 = -s1;
                        flag = false;
                    }else{
                        flag = true;
                    }
                    do{
                        s2 += s1 % 10 + '0';
                        s1 = s1 / 10;
                    }while(s1 != 0);
                    if(!flag) s2 += "-";
                    //std::cout<<"["<<s2<<"]";
                    int l = s2.length();
                    char bi[l+1];
                    for(int h = 0;h < l;h++){
                        bi[h] = s2[l-h-1];
                    }
                    bi[l] = '\0';
                    bigInteger t(bi);
                    return t;
                }else if(tmp1.is<bool>()){
                    bigInteger t1;
                    if(tmp1.as<bool>()){
                        bigInteger t("1");
                        t1 = t;
                    }else {
                        bigInteger t("0");
                        t1 = t;
                    }
                    return t1;
                }else if(tmp1.is<std::string>()){
                    int n = tmp1.as<std::string>().size();
                    char a[n];
                    for(int i = 1;i < n;i++){
                        a[i-1] = tmp1.as<std::string>()[i];
                    }
                    a[n-1] = '\0';
                    bigInteger t(a);
                    return t;
                }
                else if(tmp1.is<bigInteger>())return tmp1;
            }else if(tmp2.as<std::string>() == "float"){
                if(num != 1) std::cerr<<"Grammer";
                tmp1 = tmp0.as<std::vector<antlrcpp::Any>>()[0];
                if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                    if(lev != 0){
                        std::string t2 = '8' + tmp1.as<std::string>();
                        if(quality.find(t2) != quality.end()){//如果内部有
                            tmp1 = t2;
                        }
                        tmp1 = quality[tmp1.as<std::string>()];
                    }else{//在全局
                        tmp1 = globa_quality[tmp1.as<std::string>()];
                    }
                }
                if(tmp1.is<bigInteger>()){
                    double t = (double)tmp1.as<bigInteger>();
                    return t;
                }else if(tmp1.is<bool>()){
                    double t1;
                    if(tmp1.as<bool>()){
                        t1 = 1.0;
                    }else {
                        t1 = 0.0;
                    }
                    return t1;
                }else if(tmp1.is<std::string>()){
                    //std::cout<<"string"<<std::endl;stdlib.h
                    tmp1.as<std::string>().erase(0,1);//tmp.erase(len-1,1);
                    double t = atof(tmp1.as<std::string>().c_str());
                    return t;
                    //return 0;
                }
                else if(tmp1.is<double>())return tmp1;
            }else if(tmp2.as<std::string>() == "str"){
                if(num != 1) std::cerr<<"Grammer";
                tmp1 = tmp0.as<std::vector<antlrcpp::Any>>()[0];
                if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                    if(lev != 0){
                        std::string t2 = '8' + tmp1.as<std::string>();
                        if(quality.find(t2) != quality.end()){//如果内部有
                            tmp1 = t2;
                        }
                        tmp1 = quality[tmp1.as<std::string>()];
                    }else{//在全局
                        tmp1 = globa_quality[tmp1.as<std::string>()];
                    }
                }
                if(tmp1.is<bigInteger>()){
                    std::string s;
                    s += '"';
                    std::string s2 = (std::string)tmp1.as<bigInteger>();
                    s += s2;
                    return s;
                }else if(tmp1.is<bool>()){
                    std::string s1;
                    s1 += '"';
                    if(tmp1.as<bool>()){
                        s1 += "True";
                    }else {
                        s1 += "False";
                    }
                    return s1;
                }else if(tmp1.is<double>()){
                    std::string s;
                    s += '"';
                    std::string s2 = std::to_string(tmp1.as<double>());
                    s += s2;
                    return s;
                }else if(tmp1.is<std::string>()) return tmp1;
            }else if(tmp2.as<std::string>() == "bool"){
                if(num != 1) std::cerr<<"Grammer";
                tmp1 = tmp0.as<std::vector<antlrcpp::Any>>()[0];
                if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                    if(lev != 0){
                        std::string t2 = '8' + tmp1.as<std::string>();
                        if(quality.find(t2) != quality.end()){//如果内部有
                            tmp1 = t2;
                        }
                        tmp1 = quality[tmp1.as<std::string>()];
                    }else{//在全局
                        tmp1 = globa_quality[tmp1.as<std::string>()];
                    }
                }
                if(tmp1.is<bigInteger>()){
                    bool b;
                    bigInteger t("0");
                    if(tmp1.as<bigInteger>() == t){
                        b = false;
                    }else b = true;
                    return b;
                }else if(tmp1.is<double>()){
                    bool b;
                    if(tmp1.as<double>() == 0.0){
                        b = false;
                    }else b = true;
                    return b;
                }else if(tmp1.is<std::string>()){
                    bool b;
                    std::string s;
                    s += '"';
                    if(tmp1.as<std::string>() == s){
                        b = false;
                    }else b = true;
                    return b;
                }else if(tmp1.is<bool>())return tmp1;
            }else if(functylist.find(tmp2.as<std::string>()) != functylist.end()) 
            {
                //std::cout<<"func"<<std::endl;
                lev += 1;
                std::map<std::string,antlrcpp::Any> tmpquality;
                if(lev > 1) {level.push(quality); tmpquality = quality;}
                else {level.push(globa_quality); tmpquality = globa_quality;}
                quality.clear();
                quality = globa_quality;
                Python3Parser::ParametersContext* list = functylist[tmp2.as<std::string>()];
                //anltrcpp::Any ret = visit(
                    //std::cout<<"list"<<std::endl;
                Python3Parser::SuiteContext* todo = funcsutie[tmp2.as<std::string>()];
                //std::cout<<"sut"<<std::endl;
                if(list -> typedargslist()){
                    //std::cout<<"youcanshu"<<std::endl;
                    antlrcpp::Any vlist = visit(list -> typedargslist());
                    //std::cout<<"wanchengfangwen"<<std::endl;
                    for(int j = 0;j < num;j++){
                        antlrcpp::Any va = tmp0.as<std::vector<antlrcpp::Any>>()[j];
                        //std::cout<<"["<<va.as<std::string>()<<std::endl;
                        std::string Nam = vlist.as<std::vector<std::string>>()[j];
                        //std::cout<<"{"<<Nam<<std::endl;
                        if(va.is<std::string>() && va.as<std::string>()[0] != '"'){
                            if(lev > 1){//在函数内部
                                std::string v2 = '8' + va.as<std::string>();
                                //std::cout<<"("<<v2<<std::endl;
                                if(tmpquality.find(v2) != tmpquality.end()){//如果内部有
                                    va = v2;
                                }
                                va = tmpquality[va.as<std::string>()];
                            }else{//在全局
                                 //std::cout<<"quanju"<<std::endl;
                                 va = tmpquality[va.as<std::string>()];
                                 /*if(va.is<bigInteger>()) std::cout<<"1"<<std::endl;
                                 else if(va.is<int>())std::cout<<"2"<<std::endl;
                                 else if(va.is<double>())std::cout<<"3"<<std::endl;
                                 else if(va.is<std::string>())std::cout<<"5"<<std::endl;
                                 else if(va.is<bool>())std::cout<<"6"<<std::endl;*/
                            }   
                        }
                        quality[Nam] = va;
                    }
                }
                antlrcpp::Any vtodo = visit(todo);
                //std::cout<<"hhh"<<std::endl;
                if(vtodo.is<std::vector<antlrcpp::Any>>()){
                    vtodo = vtodo.as<std::vector<antlrcpp::Any>>()[1];
                    if(vtodo.is<std::string>() && vtodo.as<std::string>()[0] != '"'){
                        if(lev != 0){
                            //std::cout<<vtodo.as<std::string>()<<std::endl;
                            std::string t2 = '8' + vtodo.as<std::string>();
                            if(quality.find(t2) != quality.end()){//如果内部有
                                vtodo = t2;
                            }
                            vtodo = quality[vtodo.as<std::string>()];
                        }else{//在全局
                            vtodo = globa_quality[vtodo.as<std::string>()];
                        }
                    }
                    quality = level.top();
                    level.pop();
                    lev -=1;
                    return vtodo;
                }else{
                    quality = level.top();
                    level.pop();
                    lev -=1;
                    return 0;
                }
            }  
        }
        return visit(ctx -> atom());
    }

    antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
        if(ctx -> arglist())
        return visit(ctx -> arglist());
        else{
            std::vector <antlrcpp::Any> fina;
            short a = 0;
            fina.push_back(a);
            return fina;
        } 
    }

    antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override {
        //std::cout<<"    visit atom    ";
        if(ctx -> OPEN_PAREN()){
            return visit(ctx -> test());
        }
        if(ctx -> NAME()) {
            std::string tmp;
            tmp = ctx -> NAME() -> toString();
            //std::cout<<"["<<tmp<<"]";
            return tmp;
        }
        else if(ctx -> STRING(0)){
            int size = ctx ->STRING().size();
            std::string Tmp;
            Tmp+='"';
            for(int i=0;i<size;i++){
                std::string tmp;
                tmp = ctx -> STRING(i) -> toString();
                int len = tmp.length();
                tmp.erase(len-1,1);
                tmp.erase(0,1);
                //return tmp;
                Tmp+=tmp;
            }
            return Tmp;
        }
        else if(ctx -> NUMBER()){
            std::string tmp;
            tmp = ctx -> NUMBER() -> toString();
            bool flag = false;
            int len = tmp.length(),lenint,lenfloat; 
            for(int i=0;i<len;i++){
                if(tmp[i]=='.'){
                    flag = true;
                    lenint = i;
                    break;
                }
            }
            if(!flag){
               char now[len+1];
               for(int i = 0;i <len;i++){
                  now[i] = tmp[i];
               }
               now[len] = '\0';
               bigInteger S(now);
               return S;
            }
            else{
                char nowint[lenint+1];
                char nowfloat[len-lenint];
                for(int i = 0;i <lenint;i++){
                    nowint[i] = tmp[i];
                }
                nowint[lenint]='\0';
                for(int i = lenint + 1;i<len;i++){
                    nowfloat[i-lenint-1] = tmp[i];
                }
                nowfloat[len-lenint-1]='\0';
                bigInteger intS(nowint),floatS(nowfloat);
                //std::cout<<"intS="<<intS<<std::endl;
                //std::cout<<"floatS="<<floatS<<std::endl;
                double fS = (double)floatS/pow(10,len-lenint-1);
                //std::cout<<"fS="<<fS<<std::endl;
                double IS = (double)intS;
                //std::cout<<"IS="<<IS<<std::endl;
                double ans = fS + IS;
                //std::cout<<"ans="<<ans<<std::endl;
                return ans;
            }
            
        }
        else if(ctx -> TRUE()){
            bool ans = true;
            return ans;
        }
        else if(ctx -> FALSE()){
            bool ans = false;
            return ans;
        }
        else if(ctx -> NONE()){
            int tmp = 0;
            return tmp;
        }
    }

    antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override {
       
        int num = ctx -> test().size();
        std::vector <antlrcpp::Any> fina;
        for(int i = 0;i < num;i++)
        { 
            antlrcpp::Any tmp = visit(ctx -> test(i));
            fina.push_back(tmp);
        }
        return fina;
    }

    antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
        //std::cout<<"    visit arglist    ";
        int n = ctx -> argument().size();
        //std::cout<<"["<<n<<"]";
        std::vector <antlrcpp::Any> fina;
        for(int i = 0;i < n;i++){
            antlrcpp::Any tmp = visit(ctx -> argument(i));
            if(!tmp.is<int>())  fina.push_back(tmp);
            else break;    
        } 
        return fina;
    }

    antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
        if(ctx -> ASSIGN()){
            antlrcpp::Any va = visit(ctx -> test());
            std::string na = ctx -> NAME() -> toString();
            na = '8' + na;
            quality[na] = va;
            return 0;
        }else return visitChildren(ctx);
    }

//todo:override all methods of Python3BaseVisitor
};


#endif //PYTHON_INTERPRETER_EVALVISITOR_H

