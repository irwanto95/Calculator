#ifndef CL_OUTPUT_SCREEN_H
#define CL_OUTPUT_SCREEN_H

/*	Maximum number of value stored before processing by operator assignment
*/
#define OS_FIRST_ARG 0
#define OS_SECOND_ARG 1
#define OS_THIRD_ARG 2
#define OS_STORED_ARG_COUNT 3

class OutputScreen
{
public:
	OutputScreen();
	~OutputScreen();

	void Clear();
	void AssignNumber(uint8_t _num);
	void AssignOperator(uint8_t _op);
	void ProcessResult(uint8_t leftArg);

	const CString& GetText() { return m_text; };

private:
	inline bool IsNeedPriority(uint8_t op);

private:
	union
	{
		int		m_iValue[OS_STORED_ARG_COUNT];
		float	m_fValue[OS_STORED_ARG_COUNT];
	};

	CString		m_text;
	uint8_t		m_operator[OS_STORED_ARG_COUNT];
	bool		m_bIsFloatingNumber;
	uint8_t		m_orde;
	uint8_t		m_argIndex;
};

#endif // !CL_OUTPUT_SCREEN_H
