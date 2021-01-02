#ifndef HISTORY_H
#define HISTORY_H

#include <tuple>

class CDialogEx;

// is operator, string, is decimal, digits
typedef vector<tuple<bool, string, bool, int>> ArgumentPacks;

class History
{
	/* declaration
	*/
public:
	enum EUsage : si16
	{
		HISTORY_UNKNOWN = -1,
		HISTORY_ADD,
		HISTORY_REPLACE,
	};


public:
	History();
	~History();

	void Clear();
	void Add(const string& text);
	void Add(LPCTSTR wtext);

	void	SetUsage(si64 idc_idx);
	EUsage	GetUsage() { return m_usage; }

	ArgumentPacks	GetSelected();
	ArgumentPacks	Get(ui16 idx);
	CListBox&		GetBoard() { return m_board; }

private:
	CListBox	m_board;
	EUsage		m_usage;
};

#endif // !HISTORY_H