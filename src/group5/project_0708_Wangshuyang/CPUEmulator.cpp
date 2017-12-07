#include "CPUEmulator.h"
using namespace std;
int CPUEmulator::Read10(string cnt)
{
	int ret = 0;
	int len = cnt.length();
	for (int i = 0; i<len; i++)
		ret = ret * 10 + cnt[i] - '0';
	return ret;
}

void CPUEmulator::A_instruction(string cnt)
{
	string suf = cnt.substr(1, cnt.length());
	if (cnt[1] >= '0' && cnt[1] <= '9')
		A = Read10(suf);
	LOOP = suf;
	return;
}

int CPUEmulator::judgetype(string cnt)
{
	int len = cnt.length();
	for (int i = 0; i<len; i++)
		if (cnt[i] == '=')
			return 0;
	return 1;
}

int CPUEmulator::getval(string cnt)
{
	if (cnt == "0") return 0;
	if (cnt == "1") return 1;
	if (cnt == "-1") return -1;
	if (cnt == "D") return D;
	if (cnt == "A") return A;
	if (cnt == "!D") return !D;
	if (cnt == "!A") return !A;
	if (cnt == "-D") return -D;
	if (cnt == "-A") return -A;
	if (cnt == "D+1") return D + 1;
	if (cnt == "A+1") return A + 1;
	if (cnt == "D-1") return D - 1;
	if (cnt == "A-1") return A - 1;
	if (cnt == "D+A") return D + A;
	if (cnt == "D-A") return D - A;
	if (cnt == "A-D") return A - D;
	if (cnt == "D&A") return D&A;
	if (cnt == "D|A") return D | A;
	if (cnt == "M") return M();
	if (cnt == "!M") return ~M();
	if (cnt == "-M") return -M();
	if (cnt == "M+1") return M() + 1;
	if (cnt == "M-1") return M() - 1;
	if (cnt == "D+M") return D + M();
	if (cnt == "D-M") return D - M();
	if (cnt == "M-D") return M() - D;
	if (cnt == "D&M") return D&M();
	if (cnt == "D|M") return D | M();
}

void CPUEmulator::C_instruction0(string cnt)
{
	int Place = 0;
	int len = cnt.size();
	for (int i = 0; i<len; i++)
		if (cnt[i] == '=')
		{
			int flagA = 0, flagD = 0, flagM = 0;
			for (int j = 0; j<i; j++)
			{
				if (cnt[j] == 'A') flagA = 1;
				if (cnt[j] == 'D') flagD = 1;
				if (cnt[j] == 'M') flagM = 1;
			}
			int val = getval(cnt.substr(i + 1, len));
			if (flagM == 1) RAM[A] = val;
			if (flagA == 1) A = val;
			if (flagD == 1) D = val;
			break;
		}
	return;
}

void CPUEmulator::C_instruction1(string cnt)
{
	string pre;
	pre.clear();
	int len = cnt.size();
	for (int i = 0; i<len; i++)
	{
		if (cnt[i] == ';')
		{
			pre = cnt.substr(0, i);
			int val = getval(pre);
			int flag = 0;
			if (cnt.substr(i + 1, len) == "JGT" && val>0) flag = 1;
			if (cnt.substr(i + 1, len) == "JEQ" && val == 0) flag = 1;
			if (cnt.substr(i + 1, len) == "JGE" && val >= 0) flag = 1;
			if (cnt.substr(i + 1, len) == "JLT" && val<0) flag = 1;
			if (cnt.substr(i + 1, len) == "JNE" && val != 0) flag = 1;
			if (cnt.substr(i + 1, len) == "JLE" && val <= 0) flag = 1;
			if (cnt.substr(i + 1, len) == "JMP") flag = 1;
			if (flag == 1)
				Jump();
			break;
		}
	}
	return;
}

void CPUEmulator::C_instruction(string cnt)
{
	if (judgetype(cnt) == 0)
		C_instruction0(cnt);
	else C_instruction1(cnt);
	return;
}


void CPUEmulator::Work()
{
	for (string cnt;;)
	{
		if (cin >> cnt)
		{
			Centence.push_back(cnt);
			if (cnt[0] == '(')
			{
				int len = cnt.size();
				string subs = cnt.substr(1, len - 2);
				G[subs] = N;
			}
		}
		else break;
		N++;
	}
	Cur = 0;
	for (; Cur<N;)
	{
		string cnt = Centence[Cur];
		if (cnt[0] == '@')
			A_instruction(cnt);
		else
		{
			if (cnt[0] != '(')
				C_instruction(cnt);
		}
		if (A == 265 && M() == 90)
			fprintf(stderr, "%d %d %d\n", A, M(), D);
		Cur++;

	}
	return;
}