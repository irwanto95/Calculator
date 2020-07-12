#ifndef CL_INPUT_FIELDS_H
#define CL_INPUT_FIELDS_H

class InputFields
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
		Op_Substraction,
		Op_Inverse,
		Op_Point,
		Op_Result,
		Op_Count
	};

public:
	InputFields();
	~InputFields();
};

#endif // !CL_INPUT_FIELDS_H
