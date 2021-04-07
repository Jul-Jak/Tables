#pragma once
#include <iostream>

using namespace std;

template <class T>
class Node {
public:
	T data;
	Node<T>* next;
	Node(T value, Node* next_) : data(value), next(next_) {}
};

template<class T>
class List {
protected:
	Node<T>* head;
	int size;
public:
	class iterator {
	private:
		Node<T>* ptr;
	public:
		iterator(Node<T>* ptr_) : ptr(ptr_) {}
		iterator(const iterator& iter) : ptr(iter.ptr) {}
		Node<T>& operator*()	{	return *ptr;	}
		Node<T>*& get_iter()	{	return ptr;	}
		Node<T>* operator->()	{	return ptr;	}
		iterator operator++()
		{
			ptr = ptr->next;
			return (*this);
		}
		bool operator==(iterator iter)
		{
			return (ptr == iter.ptr);
		}
		bool operator !=(iterator iter)
		{
			return !(*this == iter);
		}
	};

	iterator begin() const
	{
		iterator iter(head);
		return iter;
	}

	iterator end() const
	{
		iterator iter(head);
		return iter;
	}
	List(Node<T>* p)
	{
		head = p;
		head->next = head;
		size = 0;
	}

	List(const List& list)
	{
		Node<T>* p = new Node<T>(list.get_head()->data, NULL);
		head = p;
		head->next = head;
		size = 0;
		iterator it1 = begin();
		iterator it2 = list.begin()->next;
		while (it2 != list.end())
		{
			insert(it2->data, it1.get_iter());
			++it1;
			++it2;
		}
	}

	void Clear()
	{
		while (head->next != head)	pop_front();
		size = 0;
	}

	~List()
	{
		Clear();
	}

	Node<T>* get_head() const
	{
		return head;
	}

	bool operator==(const List& lst) const
	{
		iterator it1 = begin();
		iterator it2 = lst.begin();
		bool point = 1;
		if (size != lst.get_size()) return 0;
		else {
			do
			{
				if (it1->data != it2->data)
				{
					point = 0;
					break;
				}
				else
				{
					++it1;
					++it2;
				}
			} while (it1 != end());
		}
		return point;
	}

	List& operator=(const List& lst)
	{
		if (*this == lst) return (*this);
		else {
			this->Clear();
			iterator it1 = begin();
			iterator it2 = lst.begin()->next;
			while (it2 != lst.end())
			{
				insert(it2->data, it1.get_iter());
				++it1;
				++it2;
			}
		}
		return (*this);
	}

	int get_size() const	{	return size;	}

	void push_front(T elem)
	{
		Node<T>* p = new Node<T>(elem, head->next);
		head->next = p;
		size++;
	}

	void insert(T elem, Node<T>* nod)
	{
		Node<T>* p = new Node<T>(elem, nod->next);
		nod->next = p;
		size++;
	}

	void pop_front()
	{
		if (!size) throw "Empty list";
		Node<T>* p = head->next;
		head->next = p->next;
		delete p;
		size--;
	}
};

class Polynom;
class Monom {
public:
	double k;
	int power;
	Monom(double coeff = 0, int pow = -1);
	Monom(const Monom& a);
	int Power_x () const {	return (power / 100);	}
	int Power_y() const	{	return ((power / 10) % 10);	}
	int Power_z() const	{	return (power % 10);	}
	bool Correctly_power();
	bool operator ==(const Monom& monom_);
	bool operator !=(const Monom& monom_);
	Monom operator+(const Monom& monom_);
	Monom operator* (const Monom& monom_);
	Polynom Monom::operator* (const Polynom& polynom_);
	friend std::ostream& operator<<(std::ostream& os, const Monom& monom_)
	{
		os << monom_.k << " " << monom_.power;
		return os;
	}
};

class Polynom : public List<Monom> {
public:
	Polynom(Node<Monom>* p);
	Polynom(const Polynom& polynom_);
	~Polynom();
	Polynom operator+ (const Polynom& polynom_);
	Polynom operator* (const Polynom& polynom_);
	friend std::ostream& operator<<(std::ostream& os, const Polynom& polynom_)
	{
		Polynom::iterator it = polynom_.begin()->next;
		while (it != polynom_.end())
		{
			os << "+" << it->data;
			++it;
		}
		return os;
	}
};

Monom::Monom(double coeff, int power_)
{
	k = coeff;
	power = power_;
	if (!Correctly_power()) throw "Incorrect power!";
}

Monom::Monom(const Monom& monom_)
{
	k = monom_.k;
	power = monom_.power;
}

bool Monom::Correctly_power()
{
	return ((Power_x() < 10) && (Power_y() < 10) && (Power_z() < 10));
}

bool Monom:: operator ==(const Monom& monom_)
{
	return ((monom_.power == power) && (monom_.k == k));
}

bool Monom:: operator !=(const Monom& monom_)
{
	return !(*this == monom_);
}

Monom Monom::operator+(const Monom& monom_)
{
	Monom res;
	if (monom_.power != power) throw "Different powers in monoms!";
	else {
		res.k = monom_.k + k;
		res.power = power;
		return res;
	}	
}

Monom Monom::operator* (const Monom& monom_)
{
	Monom res;
	if (((Power_x() + monom_.Power_x()) < 10) && ((Power_y() + monom_.Power_y()) < 10) && ((Power_z() + monom_.Power_z() < 10)))
	{
		res.k = monom_.k * k;
		res.power = monom_.power + power;
		return res;
	}
	else throw "Incorrect power in result!";
}

Polynom Monom::operator* (const Polynom& pol)
{
	Monom m;
	Node<Monom>* p = new Node<Monom>(m, nullptr);
	p->next = p;
	Polynom res(p);
	Polynom::iterator it1 = pol.begin()->next;
	Polynom::iterator it2 = res.begin();
	while (it1 != pol.end())
	{
		res.insert((*this) * (it1->data), it2.get_iter());
		++it1;
		++it2;
		if (!Correctly_power()) throw "Incorrect power!";
	}
	return res;
}

Polynom::Polynom(Node<Monom>* p) :List<Monom>(p) {}

Polynom::Polynom(const Polynom& polynom_) : List<Monom>(polynom_) {}

Polynom::~Polynom()
{
	Clear();
}

Polynom Polynom::operator+ (const Polynom& polynom_)
{
	Monom monom_;
	Node<Monom>* p = new Node<Monom>(monom_, NULL);
	p->next = p;
	Polynom res(p);
	Polynom::iterator it1 = begin()->next;
	Polynom::iterator it2 = polynom_.begin()->next;
	Polynom::iterator it3 = res.begin();
	while ((it1 != end()) && (it2 != polynom_.end()))
	{
		if ((it1->data.power) < (it2->data.power))
		{
			res.insert(it1->data, it3.get_iter());
			++it1;
			++it3;
		}
		else if ((it2->data.power) < (it1->data.power))
		{
			res.insert(it2->data, it3.get_iter());
			++it2;
			++it3;
		}
		else if ((it2->data.power) == (it1->data.power))
		{
			res.insert((it1->data + it2->data), it3.get_iter());
			++it1;
			++it2;
			++it3;
		}
	}
	if (it1 == end())
	{
		while (it2 != polynom_.end())
		{
			res.insert(it2->data, it3.get_iter());
			++it2;
			++it3;
		}
	}
	else
	{
		while (it1 != end())
		{
			res.insert(it1->data, it3.get_iter());
			++it1;
			++it3;
		}
	}
	return res;
}

Polynom Polynom::operator* (const Polynom& polynom_)
{
	Monom monom_;
	Node<Monom>* p = new Node<Monom>(monom_, NULL);
	p->next = p;
	Polynom res(p);
	Polynom pol2(res);
	Polynom::iterator it1 = begin()->next;
	while (it1 != end())
	{
		pol2 = (it1->data) * polynom_;
		res = res + pol2;
		++it1;
	}
	return res;
}