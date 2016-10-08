#include <iostream>
using namespace std;

// void f(void) {
// 
//     cout << "in global" << endl;

class A {
private:
	int m_a;
public:
	A(int n) :m_a(n){}
	friend ostream& operator<<(ostream& os, const A& a) {
		return os << a.m_a;
	}
	friend void f(const A& a) {
		cout << "in class" << endl;
	}
	friend void f(void) {
		cout << "in class void" << endl;
	}
};


class B
{
private:
	int m_b;

public:
	B(int b): m_b(b) {}
	friend ostream& operator<<(ostream& os, const B &b);
	friend void g(const B &b);
	friend void g();
};


int main(void) {

	A a(3);
	cout << a << endl;
	//f();//�������
	f(a);//��������,Ϊʲô��?,˳��˵һ�£�VC�����()Ҳ��ͨ������.
	return 0;
}