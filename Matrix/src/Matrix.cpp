//============================================================================
// Name        : Matrix.cpp
// Author      : Auro
// Version     :
// Copyright   : 
// Description : Testing Matrix Library
//============================================================================

#include <iostream>
#include"matrix.h"

using namespace std;

void test1(){
	//Matrix initialization
	Matrix<int> m1{{1,2,5},{3,4,6}};
	cout<<m1.toString()<<endl;

	Matrix<double> m2;
	cout<<m2.toString()<<endl;

	Matrix<long> m3{2,4};
	cout<<m3.toString()<<endl;

	//Matrix element addition/deletion
	m1.addRow({2,5,7});
	cout<<m1.toString()<<endl;

	auto second = m1[1];
	for(int i:second){
		cout<<i<<"\t";
	}
	cout<<endl;
	cout<<m1[1][2]<<endl;
	cout<<m1.toString()<<endl;

	Matrix<int> n = m1.transpose();
	cout<<(n).toString()<<endl;

	vector<int> row_vec = {3,13,21};
	n.addColumn(row_vec);
	cout<<n.toString()<<endl;

	cout<<n.transpose().toString()<<endl;
	n = n.transpose();

	auto row_vec2 = n.getRow(1);
	for(int i:row_vec2){
		cout<<i<<"\t";
	}
	cout<<endl;

	auto column_vec = n.getColumn(1);
	for(int i:column_vec){
		cout<<i<<"\t";
	}
	cout<<endl;

	cout<<n.get(2, 2)<<endl;

	n.removeRow(1);
	cout<<n.toString()<<endl;

	n.removeColumn(2);
	cout<<n.toString()<<endl;

	n.removeRow(2);
	cout<<n.toString()<<endl;

	cout<<(n==(Matrix<int>({{-1,1,4},{2,5,4}})))<<endl;

	n+=Matrix<int>({{2,2},{3,3}});
	cout<<n.toString()<<endl;

	auto n2 = n - Matrix<int>({{4,4},{3,3}});
	cout<<n2.toString()<<endl;

	cout<<((n*n2).toString())<<endl;

	cout<<(Matrix<int>({{2,0},{0,2}})*=Matrix<int>({{-1,-1},{-1,-1}})).toString()<<endl;
}

void test2(){
	Matrix<double> m{{1,2,3,4},{5,6,0,8},{9,10,11,11},{13,14,15,17}};
	cout<<m.toString()<<endl;
	cout<<m.getMinor(0, 1).toString()<<endl;
	cout<<m.getCofactor(0, 1).toString()<<endl;
	cout<<m.det()<<endl;
	cout<<(m/2.0).toString()<<endl;
}

int main() {
	cout << "Hello World" << endl; // prints Hello World

	test2();

	return 0;
}
