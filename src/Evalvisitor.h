#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H


#include "Python3BaseVisitor.h"
#include <cstring>
#include <string>
#include "bigInteger.h"
#include <cmath>
#include <map>
#include <iomanip>
#include <algorithm>


class EvalVisitor: public Python3BaseVisitor {
    std::map<std::string,antlrcpp::Any> quality; 
    antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
        //std::cout<<"hit expr stmt"<<std::endl;
        if(ctx -> ASSIGN(0))
        {  
            int n = ctx ->testlist().size();
            //std::cout<<"["<<n<<"]";
            antlrcpp::Any Tmp1 = visit(ctx -> testlist(n-1)),tmp1,tmp2;
            int num1 = Tmp1.as<std::vector<antlrcpp::Any>>().size(),num2;
            for(int i = n-2;i >= 0;i--){
               antlrcpp::Any Tmp2 = visit(ctx -> testlist(i));
               num2 = Tmp2.as<std::vector<antlrcpp::Any>>().size();
               if(num2 != num1){std::cerr<<"Gramer error";}
               for(int j=0;j < num1;j++){
                   tmp1 = Tmp1.as<std::vector<antlrcpp::Any>>()[j];
                   tmp2 = Tmp2.as<std::vector<antlrcpp::Any>>()[j];
                   //std::cout<<"["<<num1<<","<<num2<<"]";
                   //std::cout<<tmp1.as<std::string>();
                   //std::cout<<tmp2.as<std::string>();
                   
                   if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"')//说明这里是一个变量
                   {  
                      if(!tmp2.is<std::string>() || tmp2.as<std::string>()[0] == '"') std::cerr<<"Gramer error"<<std::endl;//不能把一个变量赋给一个常量
                      else {
                          quality[tmp2.as<std::string>()] = quality[tmp1.as<std::string>()];
                          //std::cout<<quality[tmp2.as<std::string>()].as<bigInteger>();
                      }
                   }
                   else
                   {
                      if(!tmp2.is<std::string>() || tmp2.as<std::string>()[0] == '"') std::cerr<<"Gramer error"<<std::endl;//不能把一个常量赋给一个常量
                      else {
                          quality[tmp2.as<std::string>()] = tmp1;
                          //std::cout<<quality[tmp2.as<std::string>()].as<bigInteger>();
                      }
                   }
               }
                   Tmp1 = Tmp2;
                   num1 = num2;
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
            }
            if(tmp2.is<std::string>() && tmp2.as<std::string>()[0] != '"'){
                tmp2 = quality[tmp2.as<std::string>()];
            }
            tmp1 = quality[Tmp.as<std::string>()];
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
            quality[Tmp.as<std::string>()] = tmp1;
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
        return visitChildren(ctx);
    }

    antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {
        if(ctx -> OR(0)){
            int n = ctx -> and_test().size();
            antlrcpp::Any tmp1 = visit(ctx -> and_test(0)),tmp2;
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                tmp1 = quality[tmp1.as<std::string>()];
            }
            if(tmp1.as<bool>()) return true;
            else {
                //std::cout<<"or";
                for(int i = 1;i < n;i++){
                    tmp2 = visit(ctx -> and_test(i));
                    if(tmp2.is<std::string>() && tmp2.as<std::string>()[0] != '"'){
                        tmp2 = quality[tmp2.as<std::string>()];
                    }
                    //std::cout<<"[or]";
                    if(tmp2.as<bool>())
                    {return true;}
                    
                }
                return false; 
            }
        }else{
            return visitChildren(ctx);
        }
    }

    antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
        if(ctx -> AND(0)){
            int n = ctx -> not_test().size();
            antlrcpp::Any tmp1 = visit(ctx -> not_test(0)),tmp2;
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                tmp1 = quality[tmp1.as<std::string>()];
            }
            if(!tmp1.as<bool>()) return false;
            else {
                for(int i = 1;i < n;i++){
                    tmp2 = visit(ctx -> not_test(i));
                    if(tmp2.is<std::string>() && tmp2.as<std::string>()[0] != '"'){
                        tmp2 = quality[tmp2.as<std::string>()];
                    }
                    //std::cout<<"[and]";
                    if(!tmp2.as<bool>())
                    {return false;}
                    
                } 
                return true;
            }
        }else{
            return visitChildren(ctx);
        }
    }

    antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
        if(ctx -> NOT()){
            antlrcpp::Any tmp1 = visit(ctx -> not_test());
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                tmp1 = quality[tmp1.as<std::string>()];
            }
            if(tmp1.as<bool>()){
                return false;
            }else{
                return true;
            }
        }else{
            return visitChildren(ctx);
        }
    }

    antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
        if(ctx -> comp_op(0)){
            //1 <     2 >     3 ==    4 >=    5 <=    6 !=
            bool ans,Ans = true;
            int n = ctx -> arith_expr().size();
            antlrcpp::Any tmp1 = visit(ctx -> arith_expr(0)),tmp2,op;
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                tmp1 = quality[tmp1.as<std::string>()];
            }
            for(int i = 1;i < n;i++){
                tmp2 = visit(ctx -> arith_expr(i));
                if(tmp2.is<std::string>() && tmp2.as<std::string>()[0] != '"'){
                   tmp2 = quality[tmp2.as<std::string>()];
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
            return visitChildren(ctx);
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
        return visitChildren(ctx);
        else{
            bool flag;//true 为加法，false 为减法；
            antlrcpp::Any tmp1 = visit(ctx -> term(0));
            antlrcpp::Any tmp2,Tmp2;
            int addnow = 0,minusnow = 0;
            size_t atmp = 0,mtmp = 0;
            int addN = ctx -> ADD().size(),minusN = ctx -> MINUS().size();
            //std::cout<<"["<<addN<<","<<minusN<<"]";
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                tmp1 = quality[tmp1.as<std::string>()];
            }
            //std::cout<<"{"<<n<<"}";
            for(int i = 1;i<n;i++){
                //获取tmp2
                Tmp2 = visit(ctx -> term(i));
                if(Tmp2.is<std::string>() && Tmp2.as<std::string>()[0] != '"'){
                   tmp2 = quality[Tmp2.as<std::string>()];
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

        if(n == 1) return visitChildren(ctx);
        else{
            antlrcpp::Any tmp1 = visit(ctx -> factor(0));
            if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                tmp1 = quality[tmp1.as<std::string>()];
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
                   tmp2 = quality[Tmp2.as<std::string>()];
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
                tmp = quality[tmp.as<std::string>()];
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
                tmp = quality[tmp.as<std::string>()];
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
            return visitChildren(ctx);
        }
        
    }

    antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
        //std::cout<<"    visit atom_expr    ";
        //std::cout<<"hit atom expr"<<std::endl;
        if(ctx -> trailer())
        {
            antlrcpp::Any tmp0,tmp1,tmp2;
            
            tmp0 = visit(ctx -> trailer());
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
                        if(quality.at(tmp1.as<std::string>()).is<std::string>()){
                            std::string out = quality.at(tmp1.as<std::string>()).as<std::string>();
                            out.erase(0,1);
                            std::cout<<out;
                        }
                        else if(quality.at(tmp1.as<std::string>()).is<bigInteger>()){
                            std::cout<<quality.at(tmp1.as<std::string>()).as<bigInteger>();
                        }
                        else if(quality.at(tmp1.as<std::string>()).is<double>()){
                            //printf("%.6lf",quality.at(tmp1.as<std::string>()).as<double>());
                            std::cout<<std::fixed<<std::setprecision(6)<<quality.at(tmp1.as<std::string>()).as<double>();
                        } 
                        else if(quality.at(tmp1.as<std::string>()).is<bool>()){
                            if(quality.at(tmp1.as<std::string>()).as<bool>())
                            std::cout<<"True";
                            else std::cout<<"False";
                        } 
                        else if(quality.at(tmp1.as<std::string>()).is<int>()){
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

                std::cout<<" ";
              }  //
              std::cout<<std::endl; 
            } else if(tmp2.as<std::string>() == "int"){
                if(num != 1) std::cerr<<"Grammer";
                tmp1 = tmp0.as<std::vector<antlrcpp::Any>>()[0];
                if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                    tmp1 = quality[tmp1.as<std::string>()];
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
                }else if(tmp1.is<bigInteger>())return tmp1;
            }else if(tmp2.as<std::string>() == "float"){
                if(num != 1) std::cerr<<"Grammer";
                tmp1 = tmp0.as<std::vector<antlrcpp::Any>>()[0];
                if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                    tmp1 = quality[tmp1.as<std::string>()];
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
                }else if(tmp1.is<double>())return tmp1;
            }else if(tmp2.as<std::string>() == "str"){
                if(num != 1) std::cerr<<"Grammer";
                tmp1 = tmp0.as<std::vector<antlrcpp::Any>>()[0];
                if(tmp1.is<std::string>() && tmp1.as<std::string>()[0] != '"'){
                    tmp1 = quality[tmp1.as<std::string>()];
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
                    tmp1 = quality[tmp1.as<std::string>()];
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
            }   
        }
        return visit(ctx -> atom());
    }

    antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
        // std::cout<<"    visit trailer    ";
        return visit(ctx -> arglist());
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
            fina.push_back(tmp);
        } 
        return fina;
    }

    antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
        return visitChildren(ctx);
    }

//todo:override all methods of Python3BaseVisitor
};


#endif //PYTHON_INTERPRETER_EVALVISITOR_H

