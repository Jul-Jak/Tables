#include "polinom.h"
#include <gtest.h>

TEST(TMonom, can_create_empty_monom)
{
	ASSERT_NO_THROW(Monom mon);
}

TEST(TMonom, can_create_monom)
{
	ASSERT_NO_THROW(Monom mon(1, 345));
}

TEST(TMonom, throw_when_incorrect_power)
{
	ASSERT_ANY_THROW(Monom mon(1, 1000));
}

TEST(TMonom, can_create_copy_monom)
{
	Monom mon1(1, 345);
	ASSERT_NO_THROW(Monom mon2(mon1));
}

TEST(TMonom, correct_add_two_monoms)
{
	Monom mon1(1, 123);
	Monom mon2(3, 123);
	Monom mon3(5, 124);
	ASSERT_NO_THROW(mon1 + mon2);
	ASSERT_ANY_THROW(mon1 + mon3);
	EXPECT_EQ((mon1 + mon2).power, 123);
	EXPECT_EQ((mon1 + mon2).k, 4);
}

TEST(TMonom, can_multiply_two_monoms)
{
	Monom mon1(2, 123);
	Monom mon2(1, 345);
	ASSERT_NO_THROW(mon1 * mon2);
}

TEST(TMonom, correctly_of_multiply_monoms)
{
	Monom mon1(7, 123);
	Monom mon2(2, 354);
	EXPECT_EQ((mon1 * mon2).power, 477);
	EXPECT_EQ((mon1 * mon2).k, 14);
}

TEST(TMonom, throws_then_res_of_multiply_has_incorrect_power)
{
	Monom mon1(-2, 911);
	Monom mon2(3, 245);
	ASSERT_ANY_THROW(mon1 * mon2);
}

TEST(TPolynom, can_create_polynom)
{
	Monom mon;
	Node<Monom>* pol = new Node<Monom>(mon, NULL);
	ASSERT_NO_THROW(Polynom pol1(pol));
}

TEST(TPolynom, can_create_copy_polynom)
{
	Monom mon;
	Node<Monom>* pol = new Node<Monom>(mon, NULL);
	Polynom pol1(pol);
	ASSERT_NO_THROW(Polynom pol2(pol1));
}

TEST(TPolynom, can_multiply_monom_with_polynom)
{
	Monom mon;
	Node<Monom>* p = new Node<Monom>(mon, NULL);
	Polynom pol(p);
	Polynom res(pol);
	Monom mon1(1, 123), mon2(7, 354), mon3(4, 511), mon4(7, 477), mon5(4, 634);
	pol.push_front(mon2);
	pol.push_front(mon3);
	res.push_front(mon4);
	res.push_front(mon5);
	EXPECT_EQ(mon1 * pol, res);
}

TEST(TPolynom, can_add_two_polynoms)
{
	Monom mon;
	Node<Monom>* p = new Node<Monom>(mon, NULL);
	Polynom pol(p);
	Polynom pol2(pol);
	Polynom res(pol);
	Monom mon1(7, 154), mon2(8, 111), mon3(9, 101);
	pol.push_front(mon1);
	pol2.push_front(mon2);
	pol2.push_front(mon3);
	res.push_front(mon1);
	res.push_front(mon2);
	res.push_front(mon3);
	EXPECT_EQ(pol + pol2, res);
}

TEST(TPolynom, can_add_polynoms_with_equal_powers_of_monoms)
{
	Monom mon;
	Node<Monom>* p = new Node<Monom>(mon, NULL);
	Polynom pol(p);
	Polynom pol2(pol);
	Polynom res(pol);
	Monom mon1(1, 123), mon2(2, 123), mon3(3, 123), mon4(7,789 ), mon5(-3, 789), mon6(4, 789 );
	pol.push_front(mon1);
	pol.push_front(mon4);
	pol2.push_front(mon2);
	pol2.push_front(mon5);
	res.push_front(mon3);
	res.push_front(mon6);
	EXPECT_EQ(pol + pol2, res);
}

TEST(TPolynom, can_multiply_two_polynoms)
{
	Monom mon;
	Node<Monom>* p = new Node<Monom>(mon, NULL);
	Polynom pol(p);
	Polynom pol2(pol);
	Polynom res(pol);
	Monom mon1(2, 211), mon2(3, 213), mon3(7, 134), mon4(1, 132), mon6(17, 345), mon5(2, 343), mon7(21, 347);
	pol.push_front(mon2);
	pol.push_front(mon1);
	pol2.push_front(mon3);
	pol2.push_front(mon4);
	res.push_front(mon7);
	res.push_front(mon6);
	res.push_front(mon5);
	EXPECT_EQ(pol * pol2, res);
}