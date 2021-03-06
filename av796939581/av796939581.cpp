// av796939581.cpp: 定义控制台应用程序的入口点。
//
// Author GYF
// bilibili @gyf10162
// 
#include "stdafx.h"

using namespace std;

typedef array<int, 2> array2;
typedef vector<wchar_t> mapY_Type;
typedef vector<vector<wchar_t>> mapV_Type;
void printMap(mapV_Type &mapVisual)
{
	size_t mapVisualSizeX = mapVisual.size();
	size_t mapVisualSizeY = mapVisual.front().size();
	for (size_t ry = 0; ry < mapVisualSizeY; ++ry)
	{
		for (size_t x = 0; x < mapVisualSizeX; ++x)
		{
			wcout << mapVisual.at(x).at(mapVisualSizeY - ry - 1);
		}
		wcout << endl;
	}
}
class Point : public array2
{
	public:
		Point() = default;
		Point(int x, int y) 
		{
			(*this)[0] = x;
			(*this)[1] = y;
		};
		Point(array2 arr) :array2(arr) {}
		//friend int MDist(const Point a, const Point b) { return abs(a[0] - b[0]) + abs(a[1] - b[1]); }	// Bugs found!
		friend int MDist(const Point a, const Point b) { return 0; }	// 暂时将评估函数替换成0，算法退化成Dijkstra算法
		void operator()(int x, int y)
		{
			(*this)[0] = x;
			(*this)[1] = y;
		}
		inline bool operator==(const Point &cmp) const
		{
			return (*this)[0] == cmp[0] && (*this)[1] == cmp[1];
		}
};
class solveType
{
	int _G;
	int _H;
	int _score;
	Point Player;
	Point 蝎子;
	Point 木乃伊;
	Point 骷髅;
	Point Exit;
	Point Key;
	array2 栅栏;
	bool is栅栏Closed;
	char _action = ' ';
	shared_ptr<solveType> _parent = nullptr;
	bool movePoint( Point &p, char a, const mapV_Type &mapVisual)
	{
		if (a == 'U')
		{
			// 向上移动
			if (p[1] < mapVisual.size() / 2 && mapVisual.at((p[0]) * 2 - 1).at((p[1]) * 2) != L'#' &&
				((!is栅栏Closed) || ((p[0]) * 2 - 1 != 栅栏[0] || (p[1]) * 2 != 栅栏[1])))
			{
				p[1] += 1;
				return true;
			}
		}
		else if (a == 'D')
		{
			// 向下移动
			if (p[1] > 1 && mapVisual.at((p[0]) * 2 - 1).at((p[1] - 1) * 2) != L'#' &&
				((!is栅栏Closed) || ((p[0]) * 2 - 1 != 栅栏[0] || (p[1] - 1) * 2 != 栅栏[1])))
			{
				p[1] -= 1;
				return true;
			}
		}
		else if (a == 'L')
		{
			// 向左移动
			if (p[0] > 1 && mapVisual.at((p[0] - 1) * 2).at((p[1]) * 2 - 1) != L'#' &&
				((!is栅栏Closed) || ((p[0] - 1) * 2 != 栅栏[0] || (p[1]) * 2 - 1 != 栅栏[1])))
			{
				p[0] -= 1;
				return true;
			}
		}
		else if (a == 'R')
		{
			// 向右移动
			if (p[0] < mapVisual.size() / 2 && mapVisual.at((p[0]) * 2).at((p[1]) * 2 - 1) != L'#' &&
				((!is栅栏Closed) || ((p[0]) * 2 != 栅栏[0] || (p[1]) * 2 - 1 != 栅栏[1])))
			{
				p[0] += 1;
				return true;
			}
		}
		else if (a == 'S')
		{
			return true;
		}
		return false;
	}
public:
	solveType() : _G(0), _H(0),_score(0) {}
	solveType(Point Player, Point 蝎子, Point 木乃伊, Point 骷髅,Point Exit,Point Key, array2 栅栏, bool is栅栏Closed = true) : _G(0), _H(0), _score(0), Player(Player), 蝎子(蝎子), 木乃伊(木乃伊),骷髅(骷髅), Exit(Exit), Key(Key), 栅栏(栅栏), is栅栏Closed(is栅栏Closed) {}
	inline bool operator<(const solveType &cmp) const { return _score < cmp._score; }
	inline bool operator>(const solveType &cmp) const { return _score > cmp._score; }
	void calF(int G, int H) { _G = G; _H = H; _score = G + H; }
	inline int getG(void) const {return _G;}
	inline Point getPlayer(void) const {return Player;}
	inline Point get蝎子(void) const {return 蝎子;}
	inline Point get木乃伊(void) const {return 木乃伊;}
	inline Point get骷髅(void) const {return 骷髅;}
	inline Point getExit(void) const {return Exit;}
	inline Point getKey(void) const {return Key;}
	inline Point get栅栏(void) const {return  栅栏;}
	inline bool get栅栏State(void) const {return  is栅栏Closed;}
	inline void parent(shared_ptr<solveType> p) { _parent = p; }
	inline shared_ptr<solveType> parent(void) { return _parent; }
	inline void action(char a) { _action = a; }
	inline char getAction() { return _action; }
	bool action(char a, const mapV_Type &mapVisual)
	{
		action(a);
		// 玩家移动
		if (true != movePoint(Player, a, mapVisual))
		{
			return false;
		}
		if (Player == 骷髅 || Player == 木乃伊 || Player == 蝎子)
		{
			return false;
		}
		// 木乃伊移动
		auto move敌人 = [&](Point &敌人) {
			bool moved = false;
			if (敌人 != Point(0, 0))
			{
				if (敌人[0] - Player[0] > 0)
				{
					// 向左移动
					moved =  movePoint(敌人, 'L', mapVisual);
				}
				else if (敌人[0] - Player[0] < 0)
				{
					// 向右移动
					moved =  movePoint(敌人, 'R', mapVisual);
				}
				if (!moved)
				{
					if (敌人[1] - Player[1] > 0)
					{
						// 向下移动
						moved =  movePoint(敌人, 'D', mapVisual);
					}
					else if (敌人[1] - Player[1] < 0)
					{
						// 向上移动
						moved =  movePoint(敌人, 'U', mapVisual);
					}
				}
			}
			return moved;
		};
		// bool 木乃伊moved = false;
		if (move敌人(木乃伊))
		{
			if (Player == 木乃伊)
			{
				return false;
			}
		}
		// 蝎子移动
		if (move敌人(蝎子))
		{
			if (Player == 蝎子)
			{
				return false;
			}
		}
		if (蝎子 == 木乃伊)
		{
			蝎子(0, 0);
		}
		// 栅栏
		if(Player==Key||木乃伊== Key||蝎子==Key)
		{
			is栅栏Closed = !is栅栏Closed;
		}
		if (move敌人(木乃伊))
		{

			if (Player == 木乃伊)
			{
				return false;
			}
		}
		if (蝎子 == 木乃伊)
		{
			蝎子(0, 0);
		}
		// 栅栏	// 木乃伊再次移动后仍需检测Key
		if (Player == Key || 木乃伊 == Key || 蝎子 == Key)
		{
			is栅栏Closed = !is栅栏Closed;
		}
		return true;
	}
};
class solveTypePtr : public shared_ptr<solveType>
{
public:
	solveTypePtr() = default;
	solveTypePtr(shared_ptr<solveType> ptr) : shared_ptr<solveType>(ptr) {}
	solveTypePtr(solveType *ptr) : shared_ptr<solveType>(ptr) {}
	inline bool operator<(const solveTypePtr cmp) const { return **this < *cmp; }
	inline bool operator>(const solveTypePtr cmp) const { return **this > *cmp; }
};
string encode(Point Player, Point 蝎子, Point 木乃伊, bool is栅栏Closed)
{
	string ret;
	ret.push_back('0' + Player[0]);
	ret.push_back('0' + Player[1]);
	ret.push_back('0' + 蝎子[0]);
	ret.push_back('0' + 蝎子[1]);
	ret.push_back('0' + 木乃伊[0]);
	ret.push_back('0' + 木乃伊[1]);
	ret.push_back('0' + is栅栏Closed);
	return ret;
}
string solve(mapV_Type &mapVisual, Point Player, Point 蝎子, Point 木乃伊, Point 骷髅, Point Exit, Point Key, array2 栅栏)
{
	string ret = "X";
	if (Player == Exit)
	{
		return string("");
	}
	if (Player == 蝎子 || Player == 木乃伊 || Player == 骷髅)
	{
		return ret;
	}
	unordered_set<string> history;	// hash表
	priority_queue < solveTypePtr ,vector<solveTypePtr>, greater<solveTypePtr>> openList;
	vector<solveTypePtr> closedList;
	solveTypePtr pStart = solveTypePtr(new solveType(Player, 蝎子, 木乃伊,骷髅, Exit,Key,栅栏, !(Player==Key)));
	int G = 0;	// 代价（步数）
	int H = MDist(Exit, Player);
	pStart->calF(G, H);
	openList.push(pStart);
	history.insert(encode(Player, 蝎子, 木乃伊, 1));
	while (!openList.empty())
	{
		solveTypePtr pCurr = openList.top();
		openList.pop();
		closedList.push_back(pCurr);
		if(pCurr->getPlayer()==Exit)
		{
			string tmp;
			solveTypePtr pTmp = pCurr;
			while(1)
			{
				if (pTmp->parent())
				{
					tmp.push_back(pTmp->getAction());
					pTmp = pTmp->parent();
				}
				else
				{
					break;
				}
			}
			ret.clear();
			for(auto it=tmp.rbegin();it!=tmp.rend();++it)
			{
				ret.push_back(*it);
			}
			break;
		}
		solveTypePtr pTmp;
		pTmp = solveTypePtr(new solveType(*pCurr));
		if(pTmp->action('S',mapVisual))
		{
			pTmp->calF(pTmp->getG() + 1, MDist(Exit, pTmp->getPlayer()));
			pTmp->parent(pCurr);
			string code = encode(pTmp->getPlayer(), pTmp->get蝎子(), pTmp->get木乃伊(), pTmp->get栅栏State());
			if(history.find(code)==history.end())
			{
				history.insert(code);
				openList.push(pTmp);
			}
		}
		pTmp = solveTypePtr(new solveType(*pCurr));
		if(pTmp->action('U',mapVisual))
		{
			pTmp->calF(pTmp->getG() + 1, MDist(Exit, pTmp->getPlayer()));
			pTmp->parent(pCurr);
			string code = encode(pTmp->getPlayer(), pTmp->get蝎子(), pTmp->get木乃伊(), pTmp->get栅栏State());
			if(history.find(code)==history.end())
			{
				history.insert(code);
				openList.push(pTmp);
			}
		}
		pTmp = solveTypePtr(new solveType(*pCurr));
		if(pTmp->action('D',mapVisual))
		{
			pTmp->calF(pTmp->getG() + 1, MDist(Exit, pTmp->getPlayer()));
			pTmp->parent(pCurr);
			string code = encode(pTmp->getPlayer(), pTmp->get蝎子(), pTmp->get木乃伊(), pTmp->get栅栏State());
			if(history.find(code)==history.end())
			{
				history.insert(code);
				openList.push(pTmp);
			}
		}
		pTmp = solveTypePtr(new solveType(*pCurr));
		if(pTmp->action('L',mapVisual))
		{
			pTmp->calF(pTmp->getG() + 1, MDist(Exit, pTmp->getPlayer()));
			pTmp->parent(pCurr);
			string code = encode(pTmp->getPlayer(), pTmp->get蝎子(), pTmp->get木乃伊(), pTmp->get栅栏State());
			if(history.find(code)==history.end())
			{
				history.insert(code);
				openList.push(pTmp);
			}
		}
		pTmp = solveTypePtr(new solveType(*pCurr));
		if(pTmp->action('R',mapVisual))
		{
			pTmp->calF(pTmp->getG() + 1, MDist(Exit, pTmp->getPlayer()));
			pTmp->parent(pCurr);
			string code = encode(pTmp->getPlayer(), pTmp->get蝎子(), pTmp->get木乃伊(), pTmp->get栅栏State());
			if(history.find(code)==history.end())
			{
				history.insert(code);
				openList.push(pTmp);
			}
		}
		//mapV_Type mapPlay = mapVisual;
		//mapPlay.at(pCurr->getPlayer()[0] * 2 - 1).at(pCurr->getPlayer()[1] * 2 - 1) = L'P';
		//mapPlay.at(pCurr->get骷髅()[0] * 2 - 1).at(pCurr->get骷髅()[1] * 2 - 1) = L'C';
		//if(pCurr->get蝎子()!=Point(0,0)) mapPlay.at(pCurr->get蝎子()[0] * 2 - 1).at(pCurr->get蝎子()[1] * 2 - 1) = L'A';
		//mapPlay.at(pCurr->get木乃伊()[0] * 2 - 1).at(pCurr->get木乃伊()[1] * 2 - 1) = L'B';
		//mapPlay.at(pCurr->getExit()[0] * 2 - 1).at(pCurr->getExit()[1] * 2 - 1) = L'E';
		// if(pCurr->get栅栏State()) mapPlay.at(pCurr->get栅栏()[0]).at(pCurr->get栅栏()[1]) = L'Z';
		//mapPlay.at(pCurr->getKey()[0] * 2 - 1).at(pCurr->getKey()[1] * 2 - 1) = L'K';
		//system("cls");
		//printMap(mapPlay);
		//system("pause");
	}
	//system("pause");
	return ret;
}
int main()
{
	wcout.imbue(locale(locale(), "", LC_CTYPE)); // ①
	vector<array2> wallRight = {{1, 1}, {1, 3}, {4, 2}, {4, 5}, {5, 3}, {5, 10}, {6, 2}, {6, 3}, {7, 6}, {7, 9}, {8, 3}, {9, 5}, {9, 7}, {9, 9}};
	vector<array2> wallTop = {{1, 9}, {2, 1}, /* {3, 2},  */{3, 3}, {3, 4}, {4, 2}, {4, 5}, {4, 8}, {5, 1}, {5, 6}, {5, 7}, {8, 6}, {8, 7}, {8, 9}, {9, 2}, {9, 5}, {9, 9}, {10, 2}};
	size_t mapSizeX = 10, mapSizeY = 10;
	array2 Player = { 2,7 };
	array2 蝎子 = { 2,1 };
	array2 木乃伊 = { 9,5 };
	array2 骷髅 = {5,7};
	array2 Exit = { 6,1 };
	array2 Key = { 10,3 };
	array2 栅栏 = { 5,4 }; // 扩展坐标
	size_t mapVisualSizeX = mapSizeX * 2 + 1;
	size_t mapVisualSizeY = mapSizeY * 2 + 1;
	mapV_Type mapVisual(mapVisualSizeX, vector<wchar_t>(mapVisualSizeY, L' '));
	// for (size_t x = 0; x < mapVisualSizeX; x+=2)
	// {
	// 	for (size_t y = 0; y < mapVisualSizeY; ++y)
	// 	{
	// 		mapVisual.at(x).at(y) = L'.';
	// 	}
	// }
	// for (size_t y = 0; y < mapVisualSizeY; y += 2)
	// {
	// 	for (size_t x = 0; x < mapVisualSizeX; ++x)
	// 	{
	// 		mapVisual.at(x).at(y) = L'.';
	// 	}
	// }
	for (size_t x = 0; x < mapSizeX; ++x)
	{
		for (size_t y = 0; y < mapSizeY; ++y)
		{
			mapVisual.at(x * 2 + 1).at(y * 2 + 1) = L':';
		}
	}
	for (auto item : wallRight)
	{
		mapVisual.at(item[0] * 2).at(item[1] * 2 - 1) = L'#';
	}
	for (auto item : wallTop)
	{
		mapVisual.at(item[0] * 2 - 1).at(item[1] * 2) = L'#';
	}
	string 说明 = R"(A = 蝎子
B = 木乃伊
C = 骷髅
E = 出口
P = 玩家
Z = 栅栏
# = 墙
: = 格子
)";
	mapV_Type mapPlay = mapVisual;
	mapPlay.at(蝎子[0] * 2 - 1).at(蝎子[1] * 2 - 1) = L'A';
	mapPlay.at(木乃伊[0] * 2 - 1).at(木乃伊[1] * 2 - 1) = L'B';
	mapPlay.at(骷髅[0] * 2 - 1).at(骷髅[1] * 2 - 1) = L'C';
	mapPlay.at(Exit[0] * 2 - 1).at(Exit[1] * 2 - 1) = L'E';
	mapPlay.at(Player[0] * 2 - 1).at(Player[1] * 2 - 1) = L'P';
	if (!(Player == Key)) mapPlay.at(栅栏[0]).at(栅栏[1]) = L'Z';
	printMap(mapPlay);
	cout << "步数：0，步骤：\n" << 说明;
	system("pause");
	string sol = solve(mapVisual, Player, 蝎子, 木乃伊, 骷髅, Exit, Key, 栅栏);
	solveType tmp(Player, 蝎子, 木乃伊, 骷髅, Exit, Key, 栅栏, !(Player == Key));
	for (size_t i = 0; i < sol.size(); ++i)
	{
		tmp.action(sol[i], mapVisual);
		auto pCurr = &tmp;
		mapV_Type mapPlay = mapVisual;
		mapPlay.at(pCurr->getKey()[0] * 2 - 1).at(pCurr->getKey()[1] * 2 - 1) = L'K';
		if (pCurr->get栅栏State()) mapPlay.at(pCurr->get栅栏()[0]).at(pCurr->get栅栏()[1]) = L'Z';
		mapPlay.at(pCurr->get骷髅()[0] * 2 - 1).at(pCurr->get骷髅()[1] * 2 - 1) = L'C';
		if (pCurr->get蝎子() != Point(0, 0)) mapPlay.at(pCurr->get蝎子()[0] * 2 - 1).at(pCurr->get蝎子()[1] * 2 - 1) = L'A';
		mapPlay.at(pCurr->get木乃伊()[0] * 2 - 1).at(pCurr->get木乃伊()[1] * 2 - 1) = L'B';
		mapPlay.at(pCurr->getExit()[0] * 2 - 1).at(pCurr->getExit()[1] * 2 - 1) = L'E';
		mapPlay.at(pCurr->getPlayer()[0] * 2 - 1).at(pCurr->getPlayer()[1] * 2 - 1) = L'P';
		system("cls");
		printMap(mapPlay);
		cout << "步数：" << i + 1 << "，步骤：" << sol.substr(0, i + 1) << "\n" << 说明;
		//system("pause");
	}
	system("pause");
	// 思考题
	// Author GYF
	// bilibili @gyf10162
	auto time_start = chrono::system_clock::now();	// 计时开始
	typedef struct
	{
		Point player;
		string solve;
		int steps;
		bool pass;
	} ansType;
	vector<ansType> ansSet;
	ofstream rst;
	rst.open("result.csv");
	int passCount = 0;
	int totalCount = 0;
	long stepsSum = 0;
	for(int x=1;x<=mapSizeX;++x)
	{
		for(int y=1;y<=mapSizeY;++y)
		{
			Point player(x,y);
			ansType ans;
			ans.player=player;
			cout << "(" << player[0] << "," << player[1] << ") ";
			if (rst.is_open())
			{
				rst << player[0] << "," << player[1] << ",";
			}
			string s = solve(mapVisual, player, 蝎子, 木乃伊,骷髅, Exit,Key,栅栏);
			cout << s << " ";
			ans.solve = s;
			if(s!="X")
			{
				ans.pass = true;
				ans.steps = s.size();
				cout << "步数：" << s.size() << endl;
				if (rst.is_open())
				{
					rst << 1 << "," << s << "\n";
				}
				++passCount;
				stepsSum += s.size();
			}
			else
			{
				ans.pass = false;
				ans.steps = -1;
				cout << "无法通关！"<< endl;
				if (rst.is_open())
				{
					rst << 0 << ",\n";
				}
			}
			++totalCount;
		}
	}
	if (rst.is_open())
	{
		rst.close();
	}
	auto time_end = chrono::system_clock::now();	// 计时结束
	cout << "通关概率：" << passCount <<" / "<<totalCount<<" = "<< double(passCount) / double(totalCount) << endl;
	cout << "通关情况下最短步数期望：" << double(stepsSum) / double(passCount) << endl;
	auto duration = chrono::duration_cast<chrono::microseconds>(time_end - time_start);
	cout << "耗时" << double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "秒" << endl;
	system("pause");
    return 0;
}

