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
public:
	Processor();
	~Processor();

	void AssignNumber(unum num);
	void AssignOperator(snum op);
	void Clear();

	const std::string	GetText() { return m_text; }
	const char*			GetTextC() { return m_text.c_str(); }

private:
	bool IsHighPriority(snum op);
	void ProcessResult();
	void AssignStreamToValue();
	void AssignValueToStream();
	void ResetStream(bool addDefaultValue = false);

private:
	union
	{
		int		m_iArg[OS_MAX_ARG_COUNT];
		float	m_fArg[OS_MAX_ARG_COUNT];
	};

	std::stringstream	m_valueStr;
	std::string			m_text;

	bool m_bIsFloatingNumber;
	bool m_bLastInputIsOperator;
	bool m_bNeedToInputFirstNumber;

	short	m_argIdx;
	snum	m_operator[OS_MAX_ARG_COUNT];
};

#endif // !CL_PROCESSOR_H
