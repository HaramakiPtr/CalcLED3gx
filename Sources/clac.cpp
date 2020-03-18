#include "common.hpp"

#include <iostream>
#include <sstream>

#define OPE_SET     (' ')
#define OPE_ADD     ('+')
#define OPE_SUB     ('-')
#define OPE_MUL     ('*')
#define OPE_DIV     ('/')
#define OPE_MOD     ('%')
#define OPE_EQU     ('=')

namespace CTRPluginFramework
{
	u32 data;
	int choice;
	int g_ans, g_num, g_ope;
	std::stringstream hexans, Progress;
	std::string decans;

	static const std::vector<Item> g_calc =
	{
		{"+"},
		{"-"},
		{"×"},
		{"÷"},
		{"="},
		{"%"}
	};
	
	static void pushOpe(int ope)
	{
		switch (g_ope) {
            case OPE_SET:
				g_ans = g_num;
				break;
            case OPE_EQU:
				g_ans = g_num;
				break;
            case OPE_ADD:
				g_ans += g_num;
				break;
            case OPE_SUB:
				g_ans -= g_num;
				break;
            case OPE_MUL:
				g_ans *= g_num;
				break;
            case OPE_DIV:
				g_ans /= g_num;
				break;
            case OPE_MOD:
				g_ans %= g_num;
				break;
        }
		g_ope = ope;
	}
	
	extern int Calculate(void)
	{
		switch (choice) {
			case 0:
				pushOpe(OPE_ADD);
				break;
			case 1:
				pushOpe(OPE_SUB);
				break;
			case 2:
				pushOpe(OPE_MUL);
				break;
			case 3:
				pushOpe(OPE_DIV);
				break;
			case 4:
				pushOpe(OPE_EQU);
				return 0;
				break;
			case 5:
				pushOpe(OPE_MOD);
				break;
		}
		return 1;
	}
	
	void Calculator(MenuEntry *entry)
	{
		g_ans = 0; g_num = 0; data = 0;
		g_ope = OPE_SET;
		Progress.str("");
		
		while(true)
		{
			Keyboard keyboard("Enter the number (in hex):\n" + Progress.str());
			if (keyboard.Open(data) != -1)
			{
				g_num = data;
				Progress << std::uppercase << std::hex << g_num;
				static StringVector calc;
				if (calc.empty()) {
					for (const Item &i : g_calc)
						calc.push_back(i.name);
				}
				Keyboard keyboard("Enter the calculating option:\n" + Progress.str(), calc);
				choice = keyboard.Open();
				
				if (choice != -1)
				{
					Progress << calc[choice];
					if(!Calculate())
					{
						hexans << std::uppercase << std::hex << g_ans;
						decans = std::to_string(g_ans);
						MessageBox("Hex: " + hexans.str() + "\nDec: " + decans)();
						hexans.str("");
						hexans.clear(std::stringstream::goodbit);
						break;
					}
				}else break;
			}else break;
		}
	}
}