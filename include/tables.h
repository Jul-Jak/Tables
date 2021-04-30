#pragma once
#include <iostream>
#include <locale>
#include <string>

using namespace std;

typedef std::string TKey;
typedef char TData;

const int TabMaxSize = 100;

struct TabRecord
{
	TKey key;      // ключ
	TData* data;   // указатель на значение
	TabRecord(TKey k = "", TData* d = NULL) : key(k), data(d) {}
	TKey GetKey() { return key; }
	TData* GetData() { return data; }
	TabRecord& operator=(const TabRecord& t) { key = t.key; data = t.data; return *this; }
	bool operator==(const TabRecord& t) const { return key == t.key && data == t.data; }
};

class Table
{
protected:
	int size;        // размер
	int count;       // к-во записей
	int pos;         // позиция
	int efficiency;  // эффективность
public:
	Table(int s = TabMaxSize) : count(0), pos(0), efficiency(0), size(s) {}
	virtual ~Table() {}
	bool IsEmpty() const { return count == 0; }
	bool IsFull() const { return count == size; }
	virtual TabRecord* Find(TKey) = 0;
	virtual void Ins(TKey, TData*) = 0;
	virtual void Del(TKey) = 0;
	friend class SortTable;
	virtual int Reset()
	{
		pos = 0;
		return IsTabEnded();
	}
	virtual int IsTabEnded() const { return pos >= count; }
	virtual int GoNext()
	{
		if (!IsTabEnded()) pos++;
		return IsTabEnded();
	}
};

class ScanTable : public Table
{
protected:
	TabRecord** rec;
public:
	ScanTable(int size = TabMaxSize);
	ScanTable(const ScanTable&);
	virtual ~ScanTable();
	virtual TabRecord* Find(TKey);
	virtual void Ins(TKey, TData*);
	virtual void Del(TKey);
	friend class SortTable;
};

ScanTable::ScanTable(int size) : Table(size)
{
	rec = new TabRecord * [size];
	for (int i = 0; i < size; i++)
	{
		rec[i] = NULL;
	}
}

ScanTable::ScanTable(const ScanTable& tab)
{
	size = tab.size;
	count = tab.count;
	pos = tab.pos;
	efficiency = tab.efficiency;
	rec = new TabRecord * [size];
	for (int i = 0; i < count; i++)
	{
		rec[i] = tab.rec[i];
	}
}

ScanTable::~ScanTable()
{
	for (int i = 0;i< count; i++)
	{
		delete (rec[i]);
	}
	delete[] rec;
}

TabRecord* ScanTable::Find(TKey k)
{
	int i;
	for (i = 0; i < count; i++)
		if (rec[i]->GetKey() == k)
		{
			pos = i;
			return rec[i];
		}
	efficiency = i;
	return NULL;
}

void ScanTable::Ins(TKey k, TData* d)
{
	if (Find(k) != NULL)
	{
		Find(k)->data = d;
		return;
	}
	if (!IsFull())
	{
		rec[count++] = new TabRecord(k, d);
		efficiency++;
	}
}

void ScanTable::Del(TKey k)
{
	if (!IsEmpty())
	{
		if (Find(k) != NULL)
		{
			delete rec[pos];
			rec[pos] = rec[--count];
			efficiency++;
		}
	}
}

class SortTable : public Table
{
protected:
	TabRecord** rec;
public:
	void SortData();
public:
	SortTable(int tabsize = TabMaxSize);
	SortTable(const SortTable&);
	virtual TabRecord* Find(TKey);
	virtual void Ins(TKey, TData*);
	virtual void Del(TKey);
};

SortTable::SortTable(int tabsize) : Table(tabsize) {
	rec = new TabRecord * [tabsize];
}

SortTable::SortTable(const SortTable& tab)
{
	size = tab.size;
	count = tab.count;
	pos = 0;
	rec = new TabRecord * [size];
	for (int i = 0; i < count; i++)
		rec[i] = new TabRecord(tab.rec[i]->GetKey(), tab.rec[i]->GetData());
	SortData();
}

TabRecord* SortTable::Find(TKey key)
{
	int i = 0, i1 = 0, i2 = count - 1;
	efficiency = 0;
	TabRecord* rez = NULL;
	while (i1 <= i2)
	{
		i = (i1 + i2) / 2;
		if (rec[i]->key == key)
		{
			i1 = i + 1;
			i2 = i;
			rez = rec[i];
		}
		else if (key > rec[i]->key)
		{
			i1 = i + 1;
		}
		else
		{
			i2 = i - 1;
		}
		efficiency++;
	}
	pos = i;
	return rez;
}


void SortTable::Ins(TKey k, TData* d)
{
	if (IsFull() || rec == nullptr)
		return;
	Find(k);
	for (int i = count; i > pos; i--)
		rec[i] = rec[i - 1];
	count++;
	efficiency++;
	if (rec[pos] != nullptr) 
	rec[pos] = new TabRecord(k, d);
	SortData();
}

void SortTable::Del(TKey k)
{
	if (!IsEmpty())
	{
		Find(k);
		if (rec != NULL)
		{
			delete rec[pos];
			for (int i = pos; i < count - 1; i++)
			{
				rec[i] = rec[i + 1];
				efficiency++;
			}
			count--;
		}
	}
}

void SortTable::SortData()
{
	for (int i = 0; i < count; i++)
		for (int j = i + 1; j < count; j++)
			if (rec[i]->GetKey() > rec[j]->GetKey())
				swap(rec[i], rec[j]);
}

#define TabHashStep 5

class HashTable : public Table
{
protected:
	virtual unsigned long HashFunc(const TKey key);
public:
	HashTable() :Table() {}
};

class ArrayHash : public HashTable
{
protected:
	TabRecord** rec;    // память для записей таблицы
	int tabsize;        // макс. возможное кол-во записей
	int freepos;        // первая своб. строка, обнаруженная при поиске
	int hashstep;       // шаг вторичного перемешивания
	TabRecord* mark;    // маркер для индикации строк с удалёнными записями
	// функция открытого перемешивания
	int GetNextPos(int pos) { return (pos + hashstep) % tabsize; }
public:
	ArrayHash(int s = TabMaxSize, int step = TabHashStep);
	~ArrayHash();
	// информационное поле 
	virtual int isFull() const { return count >= tabsize; }
	// основные методы
	virtual TabRecord* Find(TKey);
	virtual void Ins(TKey, TData*);
	virtual void Del(TKey);
	// навигация
	virtual int Reset();
	virtual int IsTabEnded();
	virtual int GoNext();
	friend ostream& operator<< (ostream& os, ArrayHash& tab)
	{
		setlocale(LC_ALL, "Rus");
		os << "Печать таблицы: " << endl;
		os << "+----------------------------------------------------" << endl;
		for (tab.Reset(); !tab.IsTabEnded(); tab.GoNext())
		{
			os << "|\tКлюч: " << tab.rec[tab.pos]->GetKey()
				<< " Данные: " << tab.rec[tab.pos]->GetData(); os << endl;
		}
		os << "+----------------------------------------------------" << endl << endl;
		return os;
	}
};

unsigned long HashTable::HashFunc(const TKey key)
{
	unsigned long hashval = 0;
	int len = key.length();
	for (int i = 0; i < len; i++)
		hashval = (hashval << 3) + key[i];
	return hashval;
}

ArrayHash::ArrayHash(int s, int step) : HashTable()
{
	rec = new TabRecord * [s];
	for (int i = 0; i < s; i++)
	{
		rec[i] = NULL;
	}
	tabsize = s;
	hashstep = step;
	mark = new TabRecord(string(""), NULL);
}

ArrayHash::~ArrayHash()
{
	for (int i = 0; i < tabsize; i++)
	{
		if (rec[i] != NULL && rec[i] != mark)
			delete rec[i];
	}
	delete[]rec;
	delete mark;
}

TabRecord* ArrayHash::Find(TKey k)
{
	TabRecord* tmp = NULL;
	freepos = -1;
	efficiency = 0;
	pos = HashFunc(k) % tabsize;
	for (int i = 0; i < tabsize; i++)
	{
		efficiency++;
		if (rec[pos] == NULL) break; // свободная строка - завершаем поиск
		else if ((rec[pos] == mark) && (freepos == -1)) // пустая строка - запоминаем первую
			freepos = pos;
		if (rec[pos]->key == k) // нашли ключ
		{
			tmp = rec[pos];
			break;
		}
		pos = GetNextPos(pos); // открытое перемешивание
	}
	if (freepos != -1)
		pos = freepos;
	return tmp;
}

void ArrayHash::Ins(TKey k, TData* d)
{
	if (IsFull()) return;
	if (Find(k) == NULL)
	{
		rec[pos] = new TabRecord(k, d);
		count++;
		efficiency++;
	}
}

void ArrayHash::Del(TKey k)
{
	if (Find(k) == NULL)
		return;
	delete rec[pos];
	rec[pos] = mark;
	efficiency++;
	count--;
}

int ArrayHash::Reset()
{
	pos = 0;
	while (!IsTabEnded())
	{
		if (rec[pos] != NULL && rec[pos] != mark)
			break;
		else pos++;
	}
	return IsTabEnded();

}

int ArrayHash::IsTabEnded()
{
	return count == tabsize;
}

int ArrayHash::GoNext()
{
	if (!IsTabEnded())
	{
		while (++pos < tabsize)
		{
			if (rec[pos] != NULL && rec[pos] != mark)
				break;
		}
	}
	return IsTabEnded();
}
