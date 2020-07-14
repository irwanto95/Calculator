#ifndef CL_PROCESSOR_H
#define CL_PROCESSOR_H

#include <sstream>
#include "Inputs.h"

/*	Maximum number of argument stored before processed automatically by operator assignment
*/
#define OS_MAX_ARG_COUNT 5
#define OS_MAX_ARG_INDEX (OS_MAX_ARG_COUNT - 1)
#define OS_FIRST_ARG 0
#define OS_SECOND_ARG 1
#define OS_THIRD_ARG 2

typedef uint16_t unum;
typedef int16_t	snum;

class Processor
{
	enum Error
	{
		OPERATION_SUCESSFUL = 0,
		OPERATION_EXCEPTION_DIV_ZERO = -1,
		OPERATION_EXCEPTION_DIV_ZERO_INF = -2
	};

	const string k_exceptionMsgDivZero		= "Divided by Zero, result : undefinition";
	const string k_exceptionMsgDivZeroInf	= "Zero divided by Zero, result : infinity";

public:
	Processor();
	~Processor();

	void AssignNumber(unum num);
	void AssignOperator(snum op);
	void Clear();

	const string	GetText() { return m_text; }
	const char*		GetTextC() { return m_text.c_str(); }

private:
	int	ProcessResult();

	bool IsHighPriority(snum op);
	void AssignStreamToValue();
	void AssignValueToStream();
	void ResetStream(bool addDefaultValue = false);

private:
	union
	{
		int		m_iArg[OS_MAX_ARG_COUNT];
		float	m_fArg[OS_MAX_ARG_COUNT];
	};

	stringstream	m_valueStr;
	string			m_text;

	bool m_bIsFloatingNumber;
	bool m_bLastInputIsOperator;
	bool m_bNeedToInputFirstNumber;

	short	m_argIdx;
	snum	m_operator[OS_MAX_ARG_COUNT];
	int		m_lastError;
};

#endif // !CL_PROCESSOR_H
