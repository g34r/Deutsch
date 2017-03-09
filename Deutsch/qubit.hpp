#pragma once
#include <array>

template <std::size_t bit_size>
class qubit_set
{
private:
	using m_dataType = std::array<bool, bit_size>;
	std::array<bool, bit_size> m_bit_data;

	bool m_signed = false;
public:
	qubit_set() = default;
	qubit_set(m_dataType init) : m_bit_data(init) {}
	qubit_set(m_dataType init, bool init_signed) : m_bit_data(init), m_signed(init_signed) {}
	
	m_dataType get_data()
	{
		return m_bit_data;
	}

	qubit_set& set_data(m_dataType data)
	{
		this->m_bit_data = data;
		return *this;
	}

	qubit_set& operator=(m_dataType data)
	{
		this->m_bit_data = data;
		return *this;
	}

	void set_signed(bool mark)
	{
		m_signed = mark;
	}

	bool is_signed()
	{
		return m_signed;
	}

	template <std::size_t rhs_size>
	qubit_set<bit_size + rhs_size> operator*(qubit_set<rhs_size> rhs)
	{
		qubit_set<bit_size + rhs_size> result;
		std::array<bool, bit_size + rhs_size> result_data;

		auto rhs_data = rhs.get_data();

		std::copy(m_bit_data.cbegin(), m_bit_data.cend(), result_data.begin());
		std::copy(rhs_data.cbegin(), rhs_data.cend(), result_data.begin()+bit_size);

		result.set_data(result_data);
		result.set_signed(m_signed^rhs.is_signed());
		
		return result;
	}
};