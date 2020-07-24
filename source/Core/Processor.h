#ifndef CL_PROCESSOR_H
#define CL_PROCESSOR_H

#include <sstream>
#include "Inputs.h"

/*	Maximum number of argument stored before processed automatically by operator assignment
*/
#define OS_MAX_ARG_NUMBER_COUNT (3)
#define OS_MAX_ARG_COUNT		(OS_MAX_ARG_NUMBER_COUNT * 2)
#define OS_MAX_ARG_INDEX		(OS_MAX_ARG_COUNT - 1)
#define OS_FIRST_ARG			(0)
#define OS_SECOND_ARG			(OS_FIRST_ARG + 1)
#define OS_THIRD_ARG			(OS_SECOND_ARG + 1)
#define OS_FIRST_NUM			(OS_FIRST_ARG)
#define OS_FIRST_OP				(OS_FIRST_NUM + 1)
#define OS_SECOND_NUM			(OS_FIRST_OP + 1)
#define OS_SECOND_OP			(OS_SECOND_NUM + 1)
#define OS_THIRD_NUM			(OS_SECOND_OP + 1)
#define OS_THIRD_OP				(OS_THIRD_NUM + 1)

class Processor
{
	const string k_exceptionMsgDivZero		= "Divided by Zero, result : undefinition";
	const string k_exceptionMsgDivZeroInf	= "Zero divided by Zero, result : infinity";

	enum Error
	{
		OPERATION_SUCESSFUL = 0,
		OPERATION_EXCEPTION_DIV_ZERO = -1,
		OPERATION_EXCEPTION_DIV_ZERO_INF = -2
	};

public:
	enum EArgumentType
	{
		ARG_UNDEFINED = 0,

		ARG_NUMBER = 1 << 0,
		ARG_NUMBER_DECIMAL = 1 << 1,
		ARG_NUMBER_INVERSE = 1 << 2,
		ARG_NUMBER_RESULT = 1 << 3,
		ARG_NUMBER_EXTENDED = ARG_NUMBER_INVERSE | ARG_NUMBER_DECIMAL,

		ARG_OPERATOR_ADDITION = 1 << 4,
		ARG_OPERATOR_SUBTRACTION = 1 << 5,
		ARG_OPERATOR_MULTIPLICATION = 1 << 6,
		ARG_OPERATOR_DIVISION = 1 << 7,
		ARG_OPERATOR = ARG_OPERATOR_ADDITION | ARG_OPERATOR_SUBTRACTION | ARG_OPERATOR_MULTIPLICATION | ARG_OPERATOR_DIVISION,
	};

	struct Argument
	{
		Argument();
		Argument(const Argument& pArg);
		~Argument();

		union
		{
			int		niArg;
			float	nfArg;
		};

		stringstream	nStream;
		sbit16			nType;

		void reset(Argument* pArg = nullptr);
		void applyStream(bool isDecimal);
		void applyValue();

		string str() { return nStream.str(); }

		template<typename T>
		void operator<<(T val)
		{
			nStream << val;
		};

		Argument* operator=(const Argument& oth);
	};

	LISTV(Argument)	Arguments;

public:
	Processor();
	~Processor();

	void AssignNumber(si16 num);
	void AssignOperator(si16 op);
	void Clear();
	void EraseBack();

	const string	GetText() { return m_text; }
	const char*		GetTextC() { return m_text.c_str(); }

private:
	int	ProcessResult();

	void PassInputAsOperator(Argument* pArg, si16 op);
	void ChangeArgumentsToDecimal(bool skipCurrent);
	void NextArgument();
	void PrevArgument();
	void ValidateStreamAndText(Argument* pArg, string* text);
	bool IsHighPriority(sbit16 op);

private:
	string	m_text;
	bool	m_bIsDecimal;

	short		m_argIdx;
	Argument*	m_prevArg;
	Argument*	m_arg;
	Arguments	m_arguments;
	
	int			m_lastError;
};

#endif // !CL_PROCESSOR_H