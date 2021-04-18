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