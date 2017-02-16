#include<iostream>
#include<algorithm>
#include<math.h>
#include<cstring>
#include<iomanip>
#include<stdio.h>
#include<limits>
#include<unordered_map>
#include<set>
#include<list>
#include<vector>
#include<stack>
#define gcd __gcd
#define pb(x) push_back(x)
#define ll long long
#define in(x) scanf("%d",&x)
#define mod 1000000007
#define sz(x) x.size()
#define mst(x,a) memset(x,a,sizeof(x))
#define pii pair<int,int>
#define F first
#define S second
#define m_p make_pair
#define all(v) (v.begin(),v.end())
#define red 0
#define black 1
using namespace std;
bool flag=0;
struct node
{
    pair<int,int> p;//contains the range
    int maxi;//maximum high of all subtree rooted at node
    bool clr;//color of the node
    node *left,*right,*parent;//the left child, right child and the parent nodes
};
void upgrade(node *root)//upgrading the maximum value with that as root
{
    if(root==NULL)
        return;
    upgrade(root->left);
    upgrade(root->right);
    if(root->left!=NULL&&root->right!=NULL)
        root->maxi=max(root->maxi,max(root->left->maxi,root->right->maxi));
    else if(root->left!=NULL)
        root->maxi=max(root->maxi,root->left->maxi);
    else if(root->right!=NULL)
        root->maxi=max(root->maxi,root->right->maxi);
    return;
}
node *insert_node(node *root,node *curr)//inserting the node
{
    if(root==NULL)
        return curr;

    root->maxi=max(root->maxi,curr->maxi);
    if(curr->p.F<root->p.F)
    {
        root->left=insert_node(root->left,curr);
        root->left->parent=root;
    }
    else
    {
        root->right=insert_node(root->right,curr);
        root->right->parent=root;
    }
    return root;

}
void rotateleft(node *&curr,node *&root)//left rotation
{
    node *right=curr->right;
    curr->right=right->left;

    if(curr->right!=NULL)
        curr->right->parent=curr;
    right->parent=curr->parent;
    if(curr->parent==NULL)
        root=right;
    else if(curr==curr->parent->left)
        curr->parent->left=right;
    else
        curr->parent->right=right;
    right->left=curr;
    curr->parent=right;
}
void rotateright(node *&curr,node *&root)//right rotation
{
    node *left=curr->left;
    curr->left=left->right;
    if(curr->left!=NULL)
        curr->left->parent=curr;
    left->parent=curr->parent;
    if(curr->parent==NULL)
        root=left;
    else if(curr==curr->parent->left)
        curr->parent->left=left;
    else
        curr->parent->right=left;
    left->right=curr;
    curr->parent=left;
}
void refresh(node *&root,node *&curr)//removing the discrepancy
{
    node *parent=NULL;
    node *grandparent=NULL;
    while(curr!=root&&curr->clr==red&&curr->parent->clr==red)
    {
        parent=curr->parent;
        grandparent=curr->parent->parent;
        if(grandparent->left==parent)
        {
            node *uncle=grandparent->right;
            if(uncle!=NULL&&uncle->clr==red)
            {
                grandparent->clr=red;
                parent->clr=black;
                uncle->clr=black;
                curr=grandparent;
            }
            else
            {
                if(curr==parent->right)
                {
                    rotateleft(curr,root);
                    curr=parent;
                    parent=curr->parent;
                }
                rotateright(grandparent,root);
                swap(parent->clr,grandparent->clr);
                curr=parent;
            }
        }
        else
        {
            node *uncle=grandparent->left;
            if(uncle!=NULL&&uncle->clr!=black)
            {
                grandparent->clr=red;
                parent->clr=black;
                uncle->clr=black;
                curr=grandparent;
            }
            else
            {
                if(curr==parent->left)
                {
                    rotateright(parent,root);
                    curr=parent;
                    parent=curr->parent;
                }
                rotateleft(grandparent,root);
                swap(grandparent->clr,parent->clr);
                curr=parent;
            }
        }
    }
    root->clr=black;

}
void search_interval_tree(node *root,pair<int,int> p)//searching a range
{
    if(root==NULL)
        return;
    int l=max(root->p.F,p.F),r=min(root->p.S,p.S);
    if(l<=r)
    {
        cout<<"the overlapping range is ("<<root->p.F<<" "<<root->p.S<<") :)"<<endl;
        flag=1;
        return;
    }
    if(root->left!=NULL&&root->left->maxi>=p.F)
        search_interval_tree(root->left,p);
    else
        search_interval_tree(root->right,p);
}
void print(node *root)//printing the in-order traversal
{
    if(root==NULL)
        return;
    print(root->left);
    cout<<root->p.F<<" "<<root->p.S<<" "<<root->maxi<<" "<<root->clr<<endl;
    print(root->right);
}
node *next_node(node* cur)
{
    node* temp;
    if(cur->right!=NULL)
    {
        while(cur->left)
            cur=cur->left;
        return cur;
    }
    temp=cur->parent;
    while(temp!=NULL && cur==temp->right)
    {
        cur=temp;
        temp=temp->parent;
    }
    return temp;
}
node *checkrangegiven(node *root,pair<int,int> p)//check if the range given to be deletehd is there or not
{
    if(p.F==root->p.F&&p.S==root->p.S)
        return root;
    if(root->left!=NULL&&root->left->maxi>=p.F)
        return checkrangegiven(root->left,p);
    else
        return checkrangegiven(root->right,p);
}

void deleteh(node *root,pair<int,int> p)//delete a node
{
    node *curr=checkrangegiven(root,p);
    node *temp1=new node,*temp2=new node;
    int gh=0;
    //cout<<gh++<<endl;
    if(curr->left==NULL || curr->right==NULL)
        temp1=curr;
    else
        temp1=next_node(curr);
    //cout<<gh++<<endl;

    if(temp1->left!=NULL)
        temp2=temp1->left;
    else
        temp2=temp1->right;


    temp2->parent=temp1->parent;
    if(temp1->parent==NULL)
        root=temp2;
    else if(temp1==temp1->parent->left)
    {
        temp1->parent->left=temp2;
        //cout<<gh++<<endl;
    }
    else
        temp1->parent->right=temp2;
    //cout<<gh++<<endl;
    if(temp1!=curr)
    {
        curr->p.F=temp1->p.F;
    }
    //cout<<gh++<<endl;
    if(temp1->clr==black)
        refresh(root,temp2);
    //cout<<gh++<<endl;
}

int main()
{
    ios::sync_with_stdio(0);
    int r;
    cout<<"enter the number of ranges given\n";
    cin>>r;
    cout<<"enter the ranges\n";
    pair<int,int>a;
    node *root=NULL;
    for(int i=0;i<p;i++)
    {
        cin>>a.F>>a.S;
        node *curr=new node;
        curr->clr=red;
        curr->p=a;
        curr->maxi=a.S;
        curr->left=curr->right=curr->parent=NULL;
        root=insert_node(root,curr);
        refresh(root,curr);
        upgrade(root);
    }
    cout<<"the in-order traversal \n";
    print(root);

    cout<<"enter your search\n";
    pair<int,int>p;

        cin>>p.F>>p.S;
        flag=0;
        search_interval_tree(root,p);
        if(!flag)
            cout<<"interval not found :("<<endl;

//    cout<<"enter the node to deleteh\n";
//    cin>>a.F>>a.S;
//    deleteh(root,a);
//    upgrade(root);
//    cout<<"node deletehd\n";
    return 0;
}
