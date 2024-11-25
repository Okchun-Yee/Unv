#include "playfair.h"
#include <iostream>
#include <iomanip> // std::setw 포함
#include <vector> //가변길이 배열 사용
#include <map> //딕셔너리 구조 사용
#include <string>
#include <algorithm> // std::find

using namespace std;

void Playfair::makeTable() {
	vector<char> temp;	//key값을 char단위로 자르는 버퍼

	//Key를 중복 제거하고 나눠서 Table 제작하기
	for (int i = 0; i < this->mKey.length(); i++) {
		if (find(temp.begin(), temp.end(), this->mKey[i]) == temp.end()) {
			temp.push_back(this->mKey[i]);
		}
	}
	//alphabet 배열에서 key와의 중복제거
	for (int i = 0; i < temp.size(); i++) {
		auto index = find(alphabet.begin(), alphabet.end(), string(1, temp[i]));
		if (index != alphabet.end()) {
			alphabet.erase(index);
		}
	}
	vector <string> str;
	int cnt = 0;
	for (int i = 0; i < temp.size() + alphabet.size(); i++) {
		if (i < temp.size()) str.push_back(string(1, temp.at(i)));
		else str.push_back(alphabet.at(cnt++));
	}

	//알파벳에서 '/'로 구분된 mPair 만들기
	auto pos = this->mPair.find('/');  // '/'의 위치를 찾음
	string pair_1, pair_2;
	if (pos != string::npos) {
		pair_1 = this->mPair.substr(0, pos);      // 첫 번째 부분 (a)
		pair_2 = this->mPair.substr(pos + 1);
	}

	auto index = find(str.begin(), str.end(), pair_1);
	if (index != str.end()) {
		index = str.erase(index);
		str.insert(index, pair_1 + "/" + pair_2);

		index = find(str.begin(), str.end(), pair_2);
		str.erase(index);
		//for (string c : str) cout << c << endl;
	}

	//mtable 생성
	cnt = 0;
	for (int i = 0; i < SIZE; i++) {
		this->mTable[i] = str[i];
	}
	//for (int i = 0; i < SIZE; i++) cout << this->mTable[i]<< " ";
}

void Playfair::showTable() {
	for (int i = 0; i < SIZE; i++) {
		if (i % 5 == 0 && i != 0) cout << endl;
		cout.width(3);
		cout << this->mTable[i] << " ";
	}
	cout << endl;
}

string Playfair::makeEncryption(string mEncryption) {
	//평문을 암호문으로 바꾸기 위한 작업
	vector<vector<string>> str;
	vector<string> checkStr;

	for (int i = 0; i < mEncryption.size();){
		string firstStr = (i< mEncryption.size()) ? string(1, mEncryption[i++]) : "x";
		if ((firstStr == string(1, this->mPair.at(0))
			|| (firstStr == string(1, this->mPair.at(2))))) {
			firstStr = this->mPair;
			cout << firstStr<<endl;
		}
		// 첫 번째 문자 가져오기
		checkStr.push_back(firstStr);
		// 중복된 문자일 경우 "x" 추가
		if (i < mEncryption.size() 
			&& firstStr == string(1, mEncryption[i])
			&& checkStr.size()<=1) {
			checkStr.push_back("x");
		}
		else if ((i < mEncryption.size()
			&& firstStr == this->mPair)
			&& (mEncryption[i] == this->mPair.at(0) || mEncryption[i] == this->mPair.at(2))
			&& checkStr.size() <= 1) {
			checkStr.push_back("x");
		}
		// 2개의 열이 채워졌으면 행으로 추가
		if (checkStr.size() == 2) {
			str.push_back(checkStr);
			checkStr.clear();
		}
	}

	// 남은 데이터를 처리
	if (!checkStr.empty()) {
		// 마지막 열 채우기
		while (checkStr.size() < 2) {
			checkStr.push_back("x");
		}
		str.push_back(checkStr);
	}

	/*for (const auto& row : str) {
		for (const auto& elem : row) {
			cout << "\"" << elem << "\"" << " ";
		}
		cout<< endl;
	}*/


	for (int i = 0; i < str.size(); i++) {
		// 두 문자의 좌표를 계산
		int index1 = -1, index2 = -1;

		// `mTable`에서 두 문자의 인덱스를 찾음
		for (int k = 0; k < SIZE; k++) {
			if (mTable[k] == str[i][0]) index1 = k;
			if (mTable[k] == str[i][1]) index2 = k;
		}

		int row1 = index1 / ROW, col1 = index1 % COL;
		int row2 = index2 / ROW, col2 = index2 % COL;
		//cout << row1 << col1 << " " << row2 << col2 << endl;
		// 같은 열
		if (col1 == col2) {
			row1 = (row1 + 1) % ROW;
			row2 = (row2 + 1) % ROW;
		}
		// 같은 행
		else if (row1 == row2) {
			if (col1 + 1 >= 5)col1 = (col1 - 1) % COL;
			else col1 = (col1 + 1) % COL;
			if (col2 + 1 >= 5)col2 = (col2 - 1) % COL;
			else col2 = (col2 + 1) % COL;
		}
		// 다른 행, 다른 열
		else {
			swap(col1, col2);
		}

		// 새 문자를 `mTable`에서 가져옴
		str[i][0] = mTable[row1 * COL + col1][0];
		str[i][1] = mTable[row2 * COL + col2][0];
	}
	mEncryption = "";	//평문 제거
	for (const auto& pair : str) {
		mEncryption += pair[0];
		mEncryption += pair[1];
	}
	return mEncryption;
}