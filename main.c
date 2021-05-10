#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
* Item: An item that is being pushed to or popped from the stack
* It may be float (to be used while evaluating the postfix)
* It may be char (to be used while converting infix to postfix)
*/
#define MAXSIZE 100
typedef union
{
    float fData;
    char  cData;
} Item;
/*
*
*/
typedef struct
{
    Item items[MAXSIZE];
    int top;
} Stack;
/*
*
*/
Stack * initialize()
{
    Stack *s=malloc(sizeof(Stack));
    s->top=0;
    return s;
}
/*
*
*/
int isEmpty(Stack *s)
{
    if( s->top == 0 ){
        return 1;
    }
    else{
        return 0;
    }
}
/*
*
*/
int isFull(Stack *s){
    if(s->top==MAXSIZE){
        return 1;
    }
    else{
        return 0;
    }
}
Item top(Stack *s)
{
    if(isEmpty(s)){
        exit(1);
    }
    else{
        return s->items[s->top-1];
    }
}
/*
*
*/
Item pop(Stack *s)
{
    if(isEmpty(s)){
        exit(1);
    }
    else{
        s->top--;
        return s->items[s->top];
    }
}
/*
*
*/
void push(Stack *s, Item val)
{
    if(isFull(s)==0){
        s->items[s->top]=val;
        s->top++;
    }
    else{
        printf("Stack is full");
    }
}
float calculate(char x,float op1,float op2){
    if(x=='+')
        return op1+op2;
    if(x=='/')
        return op1/op2;
    if(x=='-')
        return op1-op2;
    if(x=='*')
        return op1*op2;
    return 0;
}
/*
* infixToPostfix: converts an expression in infix notation to
* to a postfix notation (Reverse-Polish Notation)
* ASSUME single-digit operands
* ASSUME ^*+-/ operators
* e.g., 22 + 31 --> 22 31 +
* e.g., ( 2 + 3 ) * 4 --> 2 3 + 4 *
*/
int precedence(char c)
{
    switch(c){
    case '*':
        return 2;
    case '/':
        return 2;
    case '%':
        return 2;
    case '+':
        return 1;
    case '-':
        return 1;
    case '(' :
        return 0;
    }
    return 0;
}
void infixToPostfix(char* infix, char* postfix)
{
    strcpy(postfix,"");
    Stack *s= initialize();
    Item op;
    char str[2] = "\0";
    char* token = strtok(infix," ");
    while(token){
        if(*token=='('){
            op.cData=*token;
            push(s,op);
        }
        else if(atof(token)){
            strcat(postfix,token);
            strcat(postfix," ");
        }
        else if(*token==')'){
            op=pop(s);
            while(op.cData!='('){
                str[0]=op.cData;
                strcat(postfix,str);
                strcat(postfix," ");
                op=pop(s);
            }
        }
        else{
            op.cData=*token;
            if(isEmpty(s)){
                push(s,op);
            }
            else{
                if(precedence(op.cData)>precedence(top(s).cData)){
                    push(s,op);
                }
                else{
                    while((!isEmpty(s))&&precedence(op.cData)<=precedence(top(s).cData)){
                        str[0] = pop(s).cData;
                        strcat(postfix,str);
                        strcat(postfix," ");
                    }
                    push(s,op);
                }
            }
        }
        token=strtok(NULL," ");
    }
    while(!isEmpty(s)){
        str[0] = pop(s).cData;
        strcat(postfix,str);
        strcat(postfix," ");
    }
    strcat(postfix,"\0");
}
/*
* evaluatePostfix: Evaluates an expression in postfix notation
* (Reverse-Polish Notation)
*/
float evaluatePostfix(char* postfix)
{
    Stack *s=initialize();
    float op1,op2,result;
    Item temp;
    char* token=strtok(postfix," ");
    while(token){
        if(atof(token)){
            temp.fData=atof(token);
            push(s,temp);
        }
        else{
            op2=pop(s).fData;
            op1=pop(s).fData;
            result=calculate(*token,op1,op2);
            temp.fData=result;
            push(s,temp);
        }
        token=strtok(NULL," ");
    }
    result = pop(s).fData;
    return result;
}
/*
*
*/
void replaceNewLineBySpace(char *s)
{
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1 = ' ';
}
/*
*
*/
int main()
{
    char infixExpr[256] = "";
    char postfixExpr[256] = "";
    printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    while(fgets(infixExpr, 255, stdin) != NULL)
    {
        replaceNewLineBySpace(infixExpr);
        infixToPostfix(infixExpr, postfixExpr);
        printf("Postfix : %s\n", postfixExpr);
        float result = evaluatePostfix(postfixExpr);
        printf("Result: %f\n\n", result);
        printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    }
    return 0;
}
