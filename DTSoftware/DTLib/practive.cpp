#include "practive.h"

void linklist_test()
{
    //LinkList<int> *list = new LinkList<int>;

    LinkList<QString> liststr;

    liststr.insert("asd");
    qDebug()<<liststr.get(0)<<endl;

    LinkList<int> list;
    for(int i =0;i<5;i++)
    {
        list.insert(i*10);
    }
    list.insert(3,22);

    list.remove(2);
    list.set(0,100);
    list.set(4,44);

    list.move(1,2);


    for(int i =0; i<list.length();i++)
    {
        cout<<i<<"  "<<list.get(i)<<endl;
        //cout<<list.next()<<endl;
    }

    for(list.move(0,1); !list.end(); list.next())
    {
        cout<<list.current()<<endl;
    }

    cout<<list.find(20)<<endl;

    //delete list;
}

void stl_vector_test()
{
    // 创建一个向量存储 int
    vector<int> vec;
    int i;

    // 显示 vec 的原始大小
    cout << "vector size = " << vec.size() << endl;

    // 推入 5 个值到向量中
    for(i = 0; i < 5; i++){
        vec.push_back(i);
    }

    // 显示 vec 扩展后的大小
    cout << "extended vector size = " << vec.size() << endl;

    // 访问向量中的 5 个值
    for(i = 0; i < 5; i++){
        cout << "value of vec [" << i << "] = " << vec[i] << endl;
    }

    //使用迭代器 iterator 访问值
    vector<int>::iterator v = vec.begin();
    while( v != vec.end()) {
        cout << "value of v = " << *v << endl;
        v++;
    }

}

void stl_deque_test()
{
    deque<int> que(10,1);
    cout<<"init"<<endl;
    for(int i =0;i<10;i++)
    {
        cout<<que[i]<<endl;
    }

    cout<<"pop"<<endl;
    for(int i =5;i<10;i++)
    {
        que.pop_back();
    }

    cout<<"push"<<endl;
    for(int i =5;i<10;i++)
    {
        que.push_back(i);
    }

    cout<<"que"<<endl;
    for(int i =0;i<10;i++)
    {
        cout<<que[i]<<endl;
    }

}

void stl_stack_test()
{
    stack<int> sta;
    int v;
    // 显示 sta 的原始大小
    cout << "vector size = " << sta.size() << endl;

    // 推入 5 个值到向量中
    for(int i = 0; i < 5; i++){
        sta.push(i);
    }
    for(int i = 0; i < 5; i++){
        sta.pop();
    }


}

void stl_list_test()
{
    list<int> olist;
    for(int i=0;i<5;i++)
    {
        olist.push_back(i);
    }
    for(list<int>::const_iterator iter = olist.begin();iter != olist.end();iter++)
    {
        cout<<*iter;

    }
    cout<<endl;
    olist.insert(olist.begin(),3,10);
}

bool parenthesis_matching(string& ss)
{
    stack<char> stk;
    bool flag = true;
    for(char c:ss)
    {
        cout<<c<<endl;
        if( c=='{' || c=='[' || c=='(')
        {
            stk.push(c);
        }
        else if ( c=='}')
        {
            if (!stk.empty() &&stk.top()=='{')
            {
                stk.pop();
            }
            else
            {
                flag = false;

            }
        }
        else if ( c==']')
        {
            if ( !stk.empty() &&stk.top()=='[')
            {
                stk.pop();
            }
            else
            {
                flag = false;

            }
        }

        else if ( c==')')
        {
            if (!stk.empty() && stk.top()=='(')
            {
                stk.pop();
            }
            else
            {
                flag = false;
            }
        }


    }
    flag = flag && stk.empty();
    return flag;
}
class MyLinkedList {
public:
    struct listNode{
        int val;
        listNode* next;
        listNode(int x):val(x),next(nullptr){}
    };

    listNode* _vhead;
    int _size;

    MyLinkedList() {
        _vhead = new listNode(0);
        _size =0;
    }

    int get(int index) {
        if(index > _size-1 || index<0)
        {
            return -1;
        }
        listNode* p = _vhead->next;
        while(index--)
        {
            p = p->next;
        }
        return p->val;
    }

    void addAtHead(int val) {
        listNode* node = new listNode(val);
        node->next=_vhead->next;
        _vhead->next = node;
        _size++;
        //addAtIndex(0,val);
    }

    void addAtTail(int val) {
        listNode* node = new listNode(val);
        listNode* t = _vhead;
        while(t->next!=nullptr)
        {
            t=t->next;
        }
        t->next = node;
        _size++;

        //addAtIndex(_size,val);
    }

    void addAtIndex(int index, int val) {
        if(index>_size)
        {
            return;
        }
        listNode* t= _vhead;
        while(index--)
        {
            t=t->next;
        }

        listNode* node = new listNode(val);
        node->next = t->next;
        t->next = node;
        _size++;
    }

    void deleteAtIndex(int index) {
        if(index>_size-1 || index<0)
        {
            return;
        }
        listNode* t= _vhead;
        while(index--)
        {
            t=t->next;
        }
        listNode* p = t->next;
        t->next = p->next;
        delete p;
        _size--;
    }
};

Node<int>* create_list(int len, int stv)
{
    Node<int>* ret = NULL;
    Node<int>* slider = NULL;
    for(int i=0;i<len;i++)
    {
        Node<int>* n = new Node<int>;
        n->value = stv++;
        n->next = NULL;
        if( slider == NULL)
        {
            slider = n;
            ret = n;
        }
        else
        {
            slider->next = n;
            slider = n;
        }
    }
    return ret;
}

void delete_list(Node<int>* list)
{
    while(list)
    {
        Node<int>* nd = list;
        list = list->next;
        delete nd;
    }
}

void print_list(Node<int>* list)
{
    while(list != NULL)
    {
        cout<< list->value<<"->";
        list = list->next;
    }
    cout <<"NULL"<<endl;
}

Node<int>* flip_list(Node<int>* list)
{
    if(list == NULL or list->next==NULL)
        return  list;
    else
    {
        Node<int>* guard = list->next;
        Node<int>* ret = flip_list(list->next);
        guard->next = list;
        list->next = NULL;
        return  ret;
    }
}


void reverse_list_test()
{
    Node<int>* list = create_list(5);
    print_list(list);
    Node<int>* rlist = flip_list(list);
    print_list(rlist);
}


Node<int>* merge_list(Node<int>* list1, Node<int>* list2)
{
    if(list1 == NULL )
        return  list2;
    else if(list2 == NULL)
        return list1;
    else if (list1->value < list2->value)
    {
        Node<int>* list1_ = list1->next;
        list1->next = merge_list(list1_, list2);
        return list1;
    }
    else
    {
        Node<int>* list2_ = list2->next;
        Node<int>* list = merge_list(list1, list2_);

        list2->next = list;
        return list2;
    }
    return NULL;
}


void merge_list_test()
{
    Node <int>* list1 = create_list(5,1);
    Node <int> * list2 = create_list(5,2);
    Node<int>* list = merge_list(list1,list2);
    print_list(list);

}






void choiceSort(vector<int>& arr, int len)
{
    int min ,min_Index,temp ;
    for (int i = 0; i < len; i++)
    {
        min = arr[i];           // 假设第一个元素为最小值
        min_Index = i;
        for (int j = i + 1; j < len; j++)
        {
            if (arr[j] < min)        // 从小到大排序
            {
                min = arr[j];          // 找最小值
                min_Index = j;      //最小值位置
            }
        }
        temp = arr[min_Index];     // 把取得的最小值和第一个数相交换
        arr[min_Index] = arr[i];
        arr[i] = temp;
    }

}


void quickSort(int left, int right, vector<int>& arr)
{
    if(left >= right)
        return;
    int i, j, base, temp;
    i = left, j = right;
    base = arr[left];  //取最左边的数为基准数
    while (i < j)
    {
        while (arr[j] >= base && i < j)
            j--;
        while (arr[i] <= base && i < j)
            i++;
        if(i < j)
        {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    //基准数归位
    arr[left] = arr[i];
    arr[i] = base;
    quickSort(left, i - 1, arr);//递归左边
    quickSort(i + 1, right, arr);//递归右边
}

int sum(int n)
{
    return (n>1? (n+ sum(n-1)) : 1);
}

int str_len(const char* s)
{
    return s!=NULL ? (  (*s!='\0')?(1+str_len(s+1)) : 0 ) : 0;
}

void hanoiTower(int n,char a,char b,char c)
{
    if(n==1)
        cout<<a<<"-->"<<c<<endl;
    else {
        hanoiTower(n-1,a,c,b);
        hanoiTower(1,a,b,c);
        hanoiTower(n-1,b,a,c);
    }

}

void tree_test()
{
    GTree<char> tree;
    GTreeNode<char>* node = NULL;
    GTreeNode<char> root;
    root.value = 'A';
    root.parent = NULL;

    tree.insert('A',NULL);
    node = tree.find('A');
    tree.insert('B',node);
    tree.insert('C',node);
    tree.insert('D',node);

    node = tree.find('B');
    tree.insert('E',node);
    tree.insert('F',node);

    node = tree.find('C');
    tree.insert('G',node);

    node = tree.find('D');
    tree.insert('H',node);
    tree.insert('I',node);
    tree.insert('J',node);

    node = tree.find('E');
    tree.insert('K',node);
    tree.insert('L',node);

    node = tree.find('H');
    tree.insert('M',node);


    cout<<tree.find('A')->value<<endl;


    for(tree.begin();!tree.end();tree.next())
    {
        cout<<tree.current()<<endl;
    }



}

