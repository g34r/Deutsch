#include <iostream>
#include <functional>

#include "qubit.hpp"
#include "qubit_expr.hpp"

qubitExpr<1> Hadamard(qubit_set<1> bit)
{
	qubitExpr<1> result;

	bool data = bit.get_data()[0];
	if (!data)
	{
		(result += qubit_set<1>({ 0 })) += qubit_set<1>({ 1 });
	}
	else
	{
		(result += qubit_set<1>({ 0 })) -= qubit_set<1>({ 1 });
	}
	return result;
}

qubitExpr<2> Hadamard_pair(qubit_set<2> bit_pair)
{
	return Hadamard(qubit_set<1>({ bit_pair.get_data()[0] })) 
		 * Hadamard(qubit_set<1>({ bit_pair.get_data()[1] }));
}

qubitExpr<2> Hadamard_expr(qubitExpr<2> expr)
{
	qubitExpr<2> result;
	for (qubit_set<2> qs : expr.get_data())
	{
		if (qs.is_signed())
		{
			result -= Hadamard_pair(qs);
		}
		else
		{
			result += Hadamard_pair(qs);
		}
	}

	return result;
}

qubit_set<2> U_f(qubit_set<2> operand, std::function<bool(bool)> f)
{
	bool i = operand.get_data()[0], j = operand.get_data()[1];
	qubit_set<2> result({ i, static_cast<bool>(j ^ f(i)) });
	result.set_signed(operand.is_signed());

	return result;
}

//template <std::size_t bit_size>
//void print_qubit_expr(qubitExpr<bit_size> expr)
//{
//	for (auto qubit : expr.get_data())
//	{
//		if (qubit.is_signed()) std::cout << "-";
//		for (auto bit : qubit.get_data())
//		{
//			std::cout << bit;
//		}
//		std::cout << std::endl;
//	}
}

void deutsch(std::function<bool(bool)> f)
{
	qubit_set<2> pair({ 0, 1 });

	qubitExpr<2> expr = Hadamard_pair(pair);

	qubitExpr<2> expr2;
	for (auto qubit_set : expr.get_data())
	{
		expr2 += U_f(qubit_set, f);
	}

	qubitExpr<2> expr3 = Hadamard_expr(expr2);

	expr3.clean_expr();

	if (expr3.get_data()[0].get_data()[0])
	{
		std::cout << "function f is balanced\n";
	}
	else
	{
		std::cout << "function f is constant\n";
	}
}

int main()
{
	auto f = [](bool i) { return false; };

	deutsch(f);
	return 0;
}