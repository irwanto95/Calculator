#ifndef CL_INPUTS_H
#define CL_INPUTS_H

class Inputs
{
public:
	enum ENumber
	{
		Number_0,
		Number_1,
		Number_2,
		Number_3,
		Number_4,
		Number_5,
		Number_6,
		Number_7,
		Number_8,
		Number_9,
		Number_Count,
	};

	enum EOperator
	{
		Op_None = -1,

		Op_Multiplication,
		Op_Division,
		Op_Addition,
		Op_Subtraction,

		Op_Inverse,
		Op_Point,
		Op_Result,
		Op_Count
	};

public:
	Inputs() {};
	~Inputs() {};

	static si16 ConvertToOperator(char c);
};

#endif // !CL_INPUTS_H
