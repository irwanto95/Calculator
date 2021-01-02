#include "pch.h"
#include "Inputs.h"

si16 Inputs::ConvertToOperator(char c)
{
	if (c == k_op_mulplication) return Inputs::Op_Multiplication;
	else if (c == k_op_division) return Inputs::Op_Division;
	else if (c == k_op_addition) return Inputs::Op_Addition;
	else if (c == k_op_subtraction) return Inputs::Op_Subtraction;
	else return Inputs::Op_None;
}
