#include <iostream>
#include <algorithm>
class Stack{
    private:
        size_t m_cap;
        int *m_stack;
        size_t m_len;
        int flag_enlarge=0,flag_reduce=0;
        void resize(int &flag_enl,int &flag_redu){
            if((flag_enl==0)&&(flag_redu==0)){
                return;
            }
            else if((flag_enl==1)&&(flag_redu==1)){
                std::cout<<"Error, flag enlrage and flage reduce can not be ON the same time\n";
                std::cout<<"Both flags are now off\n";
                flag_enl=0;
                flag_redu=0;
            }
            else if((flag_enl==1)&&(flag_redu==0)){            
                this->m_cap*=2;               
                int *tmp=new int[m_cap];
                if(tmp==nullptr){
                    std::cout<<"Error, falied to allocated memory\n";                    
                    exit(EXIT_FAILURE);
                }
                for(size_t i=0;i<m_len;i++){
                    tmp[i]=m_stack[i];
                }
                delete [] m_stack;
                m_stack=tmp;
                flag_enl=0;
            }
            else if((flag_enl==0)&&(flag_redu==1)){
                int *tmp=new int[m_len];
                if(tmp==nullptr){
                    std::cout<<"Error, falied to allocated memory\n";                    
                    exit(EXIT_FAILURE);
                }
                for(size_t i=0;i<m_len;i++){
                    tmp[i]=m_stack[i];
                }
                delete [] m_stack;
                m_stack=tmp;
                m_cap--;
                flag_reduce=0;
            }
        }
    public:
        Stack(size_t cap=1):m_cap{cap},m_stack{new int[m_cap]},m_len{0}{
            std::cout<<"C-tor was created for "<<this<<" object\n";
        }
        ~Stack(){
            delete [] m_stack;
            std::cout<<"D-Tor is called for "<<this<<'\n';
        }
        Stack(Stack &other):m_cap{other.m_cap},m_stack{new int[m_cap]},m_len{other.m_len}{            
            for(size_t i=0;i<m_len;i++){
                m_stack[i]=other.m_stack[i];
            }
            std::cout<<"Copy C-tor from "<<&other<<" to "<<this<<" was created\n";            
        }
        Stack & operator =(Stack &o){
            if(this==&o){
                return *this;
            }
            this->m_cap=o.m_cap;
            this->m_len=o.m_len;
            delete [] this->m_stack;
            this->m_stack=new int[m_cap];
            for(size_t i=0;i<m_len;i++){
                m_stack[i]=o.m_stack[i];
            }
        }

        Stack(Stack && o):m_cap{o.m_cap},m_stack{o.m_stack},m_len{o.m_len}{
            std::cout<<"Move copy C-tor was called from "<<&o<<" to "<<this<<'\n';
            o.m_stack=nullptr;

        }
        Stack & operator=(Stack &&o){
            if (this==&o){
                return *this;
            }
            delete [] m_stack;
            m_cap=o.m_cap;
            m_stack=o.m_stack;
            m_len=o.m_len;
            o.m_stack=nullptr;
        }
        
        friend Stack operator + (Stack &o1,Stack &o2);
        friend Stack operator + (Stack &&o1,Stack &o2);
        friend Stack operator + (Stack &o1,Stack &&o2);
        friend Stack operator + (Stack &&o1,Stack &&o2);

        Stack & operator += (Stack const &o){
            m_cap+=o.m_cap;
            size_t idx_to_start=m_len;
            flag_enlarge=1;
            resize(flag_enlarge,flag_reduce);
            m_len+=o.m_len;
            for(size_t i=0;i<o.m_len;i++){
                m_stack[idx_to_start]=o.m_stack[i];
                idx_to_start++;
            }
            return *this;
        }
        Stack &push(int v){            
            if(m_cap==1){                
                m_cap*=2;
            }
            else if(m_len>=m_cap){
                flag_enlarge=1;
                resize(flag_enlarge,flag_reduce);                
            }
            m_stack[m_len]=v;
            m_len++;
            return *this;
        }
        Stack& pop(int &re){
            if(m_len==0){
                std::cout<<"Array is in size of zero, pop can't take place\n";
                m_cap=0;
                return *this;
            }
            re=m_stack[--m_len];
            flag_reduce=1;
            resize(flag_enlarge,flag_reduce);    
            return *this;
        }
        void print() const {
            //prevents case of dangling pointer/reference
            if(this->m_stack!=nullptr){
                std::cout<<"print id: "<<this<<'\n';
                if(m_len==0){
                    std::cout<<"Array is empty";
                }
                for(size_t i=0;i<m_len;i++){
                    std::cout<<m_stack[i]<<' ';
                }
                std::cout<<'\n';
            }
            else{
                std::cout<<"Objet is null\n";
            }
        }


};
Stack operator + (Stack &o1,Stack &o2){
    Stack result{o1.m_cap+o2.m_cap};
    size_t i=0;
    for(;i<o1.m_len;i++){
        result.m_stack[i]=o1.m_stack[i];

    }
    for(size_t j=0;j<o2.m_len;j++){
        result.m_stack[i]=o2.m_stack[j];
        i++;
    }
    result.m_len=i;
    return result;
}
Stack operator + (Stack &&o1,Stack &o2){
    //using in move C-tor
   Stack result=std::move(o1);
   result+=o2;
   return result;
}
Stack operator + (Stack &o1,Stack &&o2){
    Stack result=std::move(o2);
    result+=o1;
    return result;
}
Stack operator + (Stack &&o1,Stack &&o2){
    if(o1.m_len>=o2.m_len){
        Stack result=std::move(o1);
        result+=o2;
        return result;
    }
    Stack result=std::move(o2);
    result+=o1;
    return result;
}

int main() {
    //code
    //for cout only: 
    std::cout << std::unitbuf;
    Stack s1{};
    s1.push(12).push(56).push(15);
    Stack s2{(s1)};
    Stack s3=s1+s2;
    Stack s4=std::move(s1)+std::move(s3);
    std::cout<<"s4: ";
    s4.print();
    std::cout<<"s3: \n";
    s3.print();    
    
    return 0;
}