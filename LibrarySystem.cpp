#include <iostream>
#include <algorithm>
using namespace std;
const int MAX_BOOKS = 1000;
const int MAX_USERS = 1000;

struct Book{
	string name;
	int id;
	int quantity;
	int borrowed;
	Book(){
		id=quantity=borrowed=0;
		name="";
	}
	void read(){
		cout<<"Enter book id, name and total quantity: ";
		cin>>id>>name>>quantity;
		borrowed=0;
	}
	bool borrow(int _id){
		if(quantity==0)
			return false;
		borrowed++;
		quantity--;
		return true;
	}
	void return_copy(){
		if(borrowed>0)
			borrowed--;
		quantity++;
	}
	bool has_prefix(string prefix){
		if(prefix.size() >name.size())
			return false;
		for(int i = 0; i < prefix.size();i++)
			if(prefix[i]!=name[i])
				return false;
		return true;
	}
	void print(){
		cout<<"id = "<<id<<" name = "<<name<<" total quantity = "<<quantity
				<<" total borrowed = "<<borrowed<<"\n";

	}
};

struct User{
	string name;
	int id;
	int books_borrowed[MAX_BOOKS];
	int len; //for borrowed books
	User(){
		id=len=0;
		name="";
	}
	void read(){
		cout<<"Enter user name and national id: ";
		cin>>name>>id;
	}
	void borrow(int _id){
		books_borrowed[len++]=_id;
	}
	void retun_copy(int _id){
		bool founed=false;
		for(int i = 0; i < len;i++)
			if(_id==books_borrowed[i]){
				for(int j = i;j<len-1;j++)
					books_borrowed[j]=books_borrowed[j+1];
				len--;
				founed=true;
				break;
			}
		if(!founed)
			cout<<"No books with this name\n";
	}
	void print(){
		cout<<"user "<<name<<" id = "<<id<<" borrowed books ids: ";
		for(int i = 0; i < len;i++)
			cout<<books_borrowed[i]<<" ";
		cout<<"\n";
	}

	bool has_borrowed(int _id){
		for(int i = 0; i < len;i++)
			if(books_borrowed[i]==_id)
				return true;
		return false;
	}

};
bool cmp_by_name(Book &b1,Book &b2){
	return b1.name < b2.name;
}
bool cmp_by_id(Book &b1,Book &b2){
	return b1.id < b2.id;
}
struct System{
	User user[MAX_USERS];
	Book book[MAX_BOOKS];
	int total_books,total_users;
	System(){
		total_books=total_users=0;
	}
	void add_book(){
		book[total_books++].read();
	}
	void search_books_by_prefix(){
		string prefix;
		cout<<"Enter book name prefix: ";
		cin>>prefix;
		bool founed=false;
		for(int i = 0; i < total_books;i++)
			if(book[i].has_prefix(prefix)){
				cout<<book[i].name<<"\n";
				founed=true;
			}
		if(!founed)
			cout<<"No result please, try again: \n";
	}

	int find_book_by_name(string name){
		for(int i = 0; i < total_books;i++)
			if(book[i].name==name)
				return i;
		return -1;
	}
	int find_user_by_name(string name){
		for(int i = 0; i < total_users;i++)
			if(user[i].name==name)
				return i;
		return -1;
	}
	void print_who_borrowed_book_by_name(){
		string name;
		cout<<"Enter book name: ";
		cin>>name;
		int idx= find_book_by_name(name);
		if(idx==-1){
			cout<<"No books with this name\n";
			return;
		}
		int id = book[idx].id;
		if(book[idx].borrowed==0){
			cout<<"No borrowed copies\n";
			return;
		}
		for(int i = 0; i < total_users;i++)
			if(user[i].has_borrowed(id))
				cout<<user[i].name<<"\n";
	}

	void add_user(){
		user[total_users++].read();
	}
	bool read_user_and_book(int &user_idx,int &book_idx){
		string user,book;
		cout<<"Enter user name and book name\n";
		cin>>user>>book;
		user_idx = find_user_by_name(user);
		book_idx = find_book_by_name(book);
		if(user_idx==-1){
			cout<<"Invalid user name\n";
			return false;;
		}
		if(book_idx==-1){
			cout<<"Invalid book name\n";
			return false;
		}
		return true;
	}

	void user_borrow_book(){
		int book_idx,user_idx;
		if(!read_user_and_book(user_idx,book_idx))
			return;
		int book_id = book[book_idx].id;
		int user_id = user[user_idx].id;
		if(book[book_idx].borrow(user_id))
			user[user_idx].borrow(book_id);
		else
			cout<<"No more copies available\n";
	}

	void user_return_book(){
		int book_idx,user_idx;
		if(!read_user_and_book(user_idx,book_idx))
			return;
		int book_id = book[book_idx].id;
		book[book_idx].return_copy();
		user[user_idx].retun_copy(book_id);
	}

	void print_lib_by_id(){
		sort(book,book+total_books,cmp_by_id);
		cout<<"---------------------------\n";
		for(int i = 0; i <total_books;i++)
			book[i].print();
		cout<<"\n";
	}

	void print_lib_by_name(){
		sort(book,book+total_books,cmp_by_name);
		cout<<"---------------------------\n";
		for(int i = 0; i <total_books;i++)
			book[i].print();
		cout<<"\n";
	}

	void print_users(){
		cout<<"--------------------------\n";
		for(int i = 0;i <total_users;i++)
			user[i].print();
		cout<<"\n";
	}

	int menu(){
		int choice =-1;
		while(choice==-1){
			cout<<"Library system\n";
			cout<<"1) add book\n";
			cout<<"2) search books by prefix\n";
			cout<<"3) print who borrowed books by name\n";
			cout<<"4) print library by id\n";
			cout<<"5) print library by name\n";
			cout<<"6) add user\n";
			cout<<"7) user borrow book\n";
			cout<<"8) user return book\n";
			cout<<"9) print users\n";
			cout<<"10) Exit\n";

			cout<<"Enter choice [1:10]: ";
			cin>>choice;
			if(!(choice>=1&&choice<=10)){
				cout<<"Invalid choice, try again\n";
				choice =-1;
			}
		}
		return choice;
	}

	void run(){
		while(true){
			int choice = menu();
			if(choice==1)
				add_book();
			else if(choice == 2)
				search_books_by_prefix();
			else if(choice == 3)
				print_who_borrowed_book_by_name();
			else if(choice == 4)
				print_lib_by_id();
			else if(choice == 5)
				print_lib_by_name();
			else if(choice==6)
				add_user();
			else if(choice == 7)
				user_borrow_book();
			else if(choice == 8)
				user_return_book();
			else if(choice == 9)
				print_users();
			else
				break;
		}
	}

};

int main() {
	System system;
	system.run();
}
