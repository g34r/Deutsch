#pragma once
#include <vector>

#include "qubit.hpp"

template <std::size_t bit_size>
class qubitExpr
{
private:
	using m_qubitVec = std::vector<qubit_set<bit_size>>;
	m_qubitVec m_sum_list;
public:
	m_qubitVec get_data()
	{
		return m_sum_list;
	}

	void clean_expr()
	{
		m_qubitVec::iterator i = m_sum_list.begin();
		while (i != m_sum_list.end())
		{
			auto match_it = std::find_if(m_sum_list.begin(), m_sum_list.end(), [i](qubit_set<bit_size> e)
			{
				return ((*i).get_data() == e.get_data() 
					 && (*i).is_signed() != e.is_signed());
			});

			if (match_it != m_sum_list.end())
			{
				match_it = m_sum_list.erase(match_it);
				i = m_sum_list.erase(i);
			}
			else
			{
				i++;
			}
		}
	}
	qubitExpr& operator+=(qubit_set<bit_size> to_add)
	{
		m_sum_list.push_back(to_add);
		return *this;
	}

	qubitExpr& operator-=(qubit_set<bit_size> to_sub)
	{
		to_sub.set_signed(true);
		m_sum_list.push_back(to_sub);
		return *this;
	}

	qubitExpr& operator+=(qubitExpr<bit_size> rhs)
	{
		for (auto qs : rhs.get_data())
		{
			m_sum_list.push_back(qs);
		}
		return *this;
	}

	qubitExpr& operator-=(qubitExpr<bit_size> rhs)
	{
		for (auto qs : rhs.get_data())
		{
			qs.set_signed(!qs.is_signed());
			m_sum_list.push_back(qs);
		}
		return *this;
	}

	template <std::size_t rhs_size>
	qubitExpr<bit_size+rhs_size> operator*(qubitExpr<rhs_size> rhs)
	{
		qubitExpr<bit_size + rhs_size> result;

		for (auto element : m_sum_list)
		{
			for (auto rhs_element : rhs.get_data())
			{
				result += element * rhs_element;
			}
		}
		return result;
	}
};