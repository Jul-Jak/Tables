#include "gtest.h"
#include "tables.h"

TEST(Record, can_create_table)
{
	TabRecord* tab;
	ASSERT_NO_THROW(tab = new TabRecord("1", "a"));
}

TEST(Record, can_get_key)
{
	TabRecord* tab = new TabRecord("1", "ab");
	string k;
	ASSERT_NO_THROW(k = tab->GetKey());
}

TEST(Record, can_get_data)
{
	TabRecord* tab = new TabRecord("2", "abc");
	char* d;
	ASSERT_STREQ("abc", tab->GetData());
}

TEST(Record, get_key_correctly)
{
	TabRecord* tab = new TabRecord("1", "abc");
	string k = tab->GetKey();
	EXPECT_EQ("1", k);
}

TEST(Record, get_data_correctly)
{
	TabRecord* tab = new TabRecord("10", "abc");
	char* d = tab->GetData();
	ASSERT_STREQ("abc", d);
}

TEST(Record, can_assign)
{
	TabRecord* tab1 = new TabRecord("1", "a");
	TabRecord* tab2 = new TabRecord("2", "ab");
	ASSERT_NO_THROW(tab1 = tab2);
}

TEST(Record, assign_correctly)
{
	TabRecord* tab1 = new TabRecord("1", "abc");
	TabRecord* tab2 = new TabRecord("7", "def");
	tab1 = tab2;
	ASSERT_STREQ("def", tab1->GetData());
}

TEST(Scantable, can_create_table_by_size)
{
	ASSERT_NO_THROW(ScanTable * tab = new ScanTable(4));
}

TEST(Scantable, can_create_table)
{
	ASSERT_NO_THROW(ScanTable * tab = new ScanTable());
}

TEST(Scantable, can_copy_table)
{
	ScanTable s(2);
	ASSERT_NO_THROW(ScanTable s1(s));
}

TEST(Scantable, can_find)
{
	ScanTable* s = new ScanTable();
	s->Find("2");
	TabRecord* t;
	ASSERT_NO_THROW(t = s->Find("2"));
}

TEST(Scantable, can_insert)
{
	ScanTable s(5);
	ASSERT_NO_THROW(s.Ins("47", "a"));
}

TEST(Scantable, insert_correctly)
{
	ScanTable s(5);
	ASSERT_NO_THROW(s.Ins("2", "abcde"));
	ASSERT_STREQ("abcde", s.Find("2")->GetData());
}

TEST(Scantable, insert_correctly_in_too_key)
{
	ScanTable* s = new ScanTable(5);
	s->Ins("1", "a");
	s->Ins("1", "b");
	ASSERT_STREQ("b" ,s->Find("1")->GetData());
}

TEST(Scantable, can_delete)
{
	ScanTable* s = new ScanTable(5);
	s->Ins("1", "a");
	s->Ins("2", "b");
	ASSERT_NO_THROW(s->Del("1"));
}

TEST(Scantable, delete_correctly)
{
	ScanTable* s = new ScanTable(5);
	s->Ins("1", "a");
	s->Ins("2", "b");
	s->Del("1");
	EXPECT_TRUE(s->Find("1") == NULL);
}

TEST(Sorttable, can_create_table_by_size)
{
	ASSERT_NO_THROW(SortTable * tab = new SortTable(5));
}

TEST(Sorttable, can_create_table)
{
	ASSERT_NO_THROW(SortTable * tab = new SortTable());
}

TEST(Sorttable, can_copy_table)
{
	SortTable s(2);
	ASSERT_NO_THROW(SortTable s1(s));
}

TEST(Sorttable, can_find)
{
	SortTable* s = new SortTable();
	s->Find("1");
	s->Find("5");
	TabRecord* t;
	ASSERT_NO_THROW(t = s->Find("5"));
}

TEST(Sorttable, can_insert)
{
	SortTable s(5);
	ASSERT_NO_THROW(s.Ins("3", "abc"));
}

TEST(Sorttable, insert_correctly)
{
	SortTable s(5);
	ASSERT_NO_THROW(s.Ins("3", "abc"));
	ASSERT_STREQ("abc",s.Find("3")->GetData());
}

TEST(Sorttable, insert_correctly_in_too_key)
{
	SortTable* s = new SortTable(5);
	s->Ins("1", "a");
	s->Ins("1", "b");
	ASSERT_STREQ("b",s->Find("1")->GetData());
}

TEST(Sorttable, can_delete)
{
	SortTable* s = new SortTable(5);
	s->Ins("1", "a");
	s->Ins("2", "b");
	ASSERT_NO_THROW(s->Del("1"));
}

TEST(Sorttable, delete_correctly)
{
	SortTable* s = new SortTable(5);
	s->Ins("1", "a");
	s->Ins("2", "b");
	s->Del("1");
	EXPECT_TRUE(s->Find("1") == NULL);
}