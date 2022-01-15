#ifndef MATRIX_H
#define MATRIX_H

#include<vector>
#include<stdexcept>
#include<type_traits>

using namespace std;

class dimension_mismatch : logic_error{
public:
	dimension_mismatch(const string& str):logic_error(str){}
	dimension_mismatch(const char* str):logic_error(str){}

	virtual ~dimension_mismatch(){}
};

class dimension_error : logic_error{
public:
	dimension_error(const string& str):logic_error(str){}
	dimension_error(const char* str):logic_error(str){}

	virtual ~dimension_error(){}
};

class non_arithmetic_type: logic_error{
public:
	non_arithmetic_type(const string& str):logic_error(str){}
	non_arithmetic_type(const char* str):logic_error(str){}

	virtual ~non_arithmetic_type(){}
};

class non_square_matrix : logic_error{
public:
	non_square_matrix(const string& str):logic_error(str){}
	non_square_matrix(const char* str):logic_error(str){}

	virtual ~non_square_matrix(){}
};

class non_singular_matrix : logic_error{
public:
	non_singular_matrix(const string& str):logic_error(str){}
	non_singular_matrix(const char* str):logic_error(str){}

	virtual ~non_singular_matrix(){}
};

/**
 * Works for arithmetic types only.
 */
template<typename T>
class Matrix{

private:
	/**
	 * Holds the matrix elements.
	 */
	vector<T> vec;
	/**
	 * The dimensions of the matrix;
	 */
	unsigned short n_rows, n_columns;

	Matrix(vector<T> vec,  int rows, int columns){
		if(is_arithmetic<T>::value == false){
			non_arithmetic_type exc{"Matrix is not implemented for non-arithmetic types"};
			throw exc;
		}
		if(rows<1 || columns<1){
			invalid_argument exc{"Wrong Matrix Initialization"};
			throw exc;
		}
		this->n_rows = rows;
		this->n_columns = columns;
		this->vec = move(vec);
	}

public:
	/*
	 * TODO:
	 * 1. The class must be internally thread-safe.
	 * 2. We can use initializer list to initialize a matrix : operator=(std::initializer_list<std::initializer_list>)
	 * 3. Also the matrix needs to be copy constructible and copy assignable.
	 * 4. The matrix needs to be move constructible and move assignable.
	 * 5. Index out of range exception for illegal matrix access.
	 * 6. Illegal argument exception for wrong matrix initialization.
	 * 7. Constructor wouldn't need mutex locking.
	 * 8. No default constructor.  Just the Matrix(rows,columns) constructor.
	 */

	Matrix():n_rows{1}, n_columns{1}{
		if(is_arithmetic<T>::value == false){
			non_arithmetic_type exc{"Matrix is not implemented for non-arithmetic types"};
			throw exc;
		}
	}

	explicit Matrix(int rows, int columns){
		if(is_arithmetic<T>::value == false){
			non_arithmetic_type exc{"Matrix is not implemented for non-arithmetic types"};
			throw exc;
		}
		if(rows<1 || columns<1){
			invalid_argument exc{"Wrong Matrix Initialization"};
			throw exc;
		}
		this->n_rows = rows;
		this->n_columns = columns;
		for(int i=0;i<rows*columns;i++){
			vec.push_back(0);
		}
	}

	explicit Matrix(initializer_list<initializer_list<T>> init){
		if(is_arithmetic<T>::value == false){
			non_arithmetic_type exc{"Matrix is not implemented for non-arithmetic types"};
			throw exc;
		}
		this->n_rows = init.size();
		this->n_columns = init.begin()->size();

		for(initializer_list<T> init_row : init){
			if(init_row.size()!=(this->n_columns)){
				dimension_mismatch exc{"Variable number of n_columns"};
				throw exc;
			}
			vec.insert(vec.end(),init_row.begin(),init_row.end());
		}
	}

	Matrix(const Matrix&) = default;
	Matrix& operator=(const Matrix&) = default;

	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;

	void addRow(initializer_list<T> list){
		if(list.size()!=n_columns){
			dimension_mismatch exc{"Wrong number of n_columns"};
			throw exc;
		}
		n_rows++;
		vec.insert(vec.end(),list.begin(),list.end());
	}

	void addRow(vector<T> list){
		if(list.size()!=n_columns){
			dimension_mismatch exc{"Wrong number of n_columns"};
			throw exc;
		}
		n_rows++;
		vec.insert(vec.end(),list.begin(),list.end());
	}

	void addColumn(initializer_list<T> list){
		if(list.size()!=n_rows){
			dimension_mismatch exc{"Wrong number of n_rows"};
			throw exc;
		}
		int i=0;
		auto it = vec.begin();
		for(T t: list){
			it = vec.insert(it+n_columns,t);
			it++;
		}
		n_columns++;
	}

	void addColumn(vector<T> list){
			if(list.size()!=n_rows){
				dimension_mismatch exc{"Wrong number of n_rows"};
				throw exc;
			}
			auto it = vec.begin();
			for(T t: list){
				it = vec.insert(it+n_columns,t);
				it++;
			}
			n_columns++;
		}

	vector<T> getRow(int r) const{
		if(r<0 || r>=n_rows){
			out_of_range exc{"Row index out of bounds of matrix"};
			throw exc;
		}

		vector<T> v;
		for(auto it = (vec.begin()+r*n_columns); it!=(vec.begin()+(r+1)*n_columns);it++){
			v.push_back(*it);
		}
		return v;
	}

	vector<T> getColumn(int c) const{
		if(c<0 || c>=n_columns){
			out_of_range exc{"Column index out of bounds of matrix"};
			throw exc;
		}

		vector<T> v;
		for(auto it = (vec.begin()+c); it!=(vec.begin()+c+n_columns*n_rows); it+=n_columns){
			v.push_back(*it);
		}
		return v;
	}

	T get(int r, int c) const{
		return *(vec.begin()+r*n_columns+c);
	}

	Matrix<T> getMinor(int r, int c){
		if(r<0 || r>=n_rows || c<0 || c>=n_columns){
			out_of_range exc{"Index out of bounds of matrix"};
			throw exc;
		}
		if(n_rows!=n_columns){
			non_square_matrix exc{"Matrix is not a square matrix."};
			throw exc;
		}
		if(n_rows<=1){
			dimension_error exc{"Matrix dimension too low. The matrix should be at least 2x2."};
			throw exc;
		}
		vector<T> v;
		auto it = vec.begin();

		for(int  i=0; i<n_rows; i++){
			for(int j=0; j<n_columns; j++){
				if((i==r)||(j==c)){
					it++;
					continue;
				}
				v.push_back(*it);
				it++;
			}
		}

		return Matrix(v, n_rows-1, n_columns-1);
	}

	Matrix<T> getCofactor(int r, int c){
		if((r+c)%2==0){
			return getMinor(r, c);
		}
		else{
			return getMinor(r, c)*((T)-1);
		}
	}

	T det(){
		if(n_rows!=n_columns){
			non_square_matrix exc{"Matrix is not a square matrix."};
			throw exc;
		}
		if(n_rows<1){
			dimension_error exc{"Matrix dimension too low. The matrix should be at least 2x2."};
			throw exc;
		}
		if(n_rows == 1){
			return vec[0];
		}
		else{
			T sum = (T)NULL;
			for(int i=0; i<n_columns;i++){
				if(i%2==0){
					sum += vec[i]*(getMinor(0, i).det());
				}
				else{
					sum -= vec[i]*(getMinor(0, i).det());
				}
			}
			return sum;
		}
	}

	void removeRow(int r){
		if(r<0 || r>=n_rows){
			out_of_range exc{"Row index out of bounds of matrix"};
			throw exc;
		}
		vec.erase(vec.begin()+r*n_columns,vec.begin()+(r+1)*n_columns);
		n_rows--;
	}

	void removeColumn(int c){
		if(c<0 || c>=n_columns){
			out_of_range exc{"Column index out of bounds of matrix"};
			throw exc;
		}

		n_columns--;
		for(auto it = vec.begin()+c; it != (vec.begin()+c+n_columns*n_rows); it += n_columns){
			it = vec.erase(it);
		}
	}

	vector<T> operator[](int i) const{
		if(i<0||i>=n_rows){
			out_of_range exc{" Row index out of bounds of matrix"};
			throw exc;
		}
		vector<T> row{};
		row.insert(row.begin(),vec.begin()+i*n_columns,vec.begin()+(i+1)*n_columns);
		return row;
	}

	unsigned short rows() const{
		return n_rows;
	}

	unsigned short columns() const{
		return n_columns;
	}

	Matrix<T> operator+(const Matrix<T>& a){
		if((a.rows()!=this->rows())||(a.columns()!=this->columns())){
			dimension_mismatch exc{"Matrix Dimensions do not match"};
			throw exc;
		}
		vector<T> v;
		for(int i=0; i<n_rows; i++){
			for(int j=0; j<n_columns; j++){
				v.push_back(((a.vec)[i*n_columns+j])+((this->vec)[i*n_columns+j]));
			}
		}
		return Matrix(v,n_rows,n_columns);
	}

	Matrix<T> operator-(const Matrix<T>& a){
		if((a.rows()!=this->rows())||(a.columns()!=this->columns())){
			dimension_mismatch exc{"Matrix Dimensions do not match"};
			throw exc;
		}
		vector<T> v;
		for(int i=0; i<n_rows; i++){
			for(int j=0; j<n_columns; j++){
				v.push_back(((this->vec)[i*n_columns+j])-((a.vec)[i*n_columns+j]));
			}
		}
		return Matrix(v,n_rows,n_columns);
	}

	Matrix<T> operator*(const Matrix<T>& a){
		if((this->n_columns)!=(a.n_rows)){
			dimension_mismatch exc{"Matrix Dimensions not compatible for multiplication"};
			throw exc;
		}

		Matrix<T> temp = a.transpose();
		vector<T> result;

		auto it = (this->vec).begin();

		for(int r=0; r<(this->n_rows); r++){
			auto it2 = (temp.vec).begin();
			auto it_temp = it;
			for(int c=0; c<(temp.n_rows); c++){
				T sum = (T)NULL;
				for(int i=0; i<(this->n_columns);i++){
					sum += (*it)*(*it2);
					it++;
					it2++;
				}
				result.push_back(sum);
				it = it_temp;
			}
			it += (this->n_columns);
		}

		return Matrix(result, (this->n_rows), (a.n_columns));
	}

	Matrix<T> operator*(const T& no){
		vector<T> v;
		for(T t:vec){
			v.push_back(t*no);
		}
		return Matrix(v, n_rows, n_columns);
	}

	Matrix<T> operator/(const T& no){
		return this->operator *(1/no);
	}

	Matrix<T>& operator+=(const Matrix<T>& a){
		this->operator =(this->operator +(a));
		return *this;
	}

	Matrix<T>& operator-=(const Matrix<T>& a){
		this->operator =(this->operator -(a));
		return *this;
	}

	Matrix<T>& operator*=(const Matrix<T>& a){
		this->operator =(this->operator *(a));
		return *this;
	}

	Matrix<T>& operator/=(const Matrix<T>& a){
		this->operator =(this->operator /(a));
		return *this;
	}

	bool operator==(const Matrix<T>& a){
		if((a.rows()!=this->rows())||(a.columns()!=this->columns())){
			return false;
		}
		for(int i=0; i<n_columns*n_rows; i++){
			if((this->vec[i])!=(a.vec[i])){
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Matrix<T>& a){
		return !(this->operator ==(a));
	}

	Matrix<T> transpose() const{
		vector<T> v;

		for(int i=0; i<n_columns; i++){
			for(int j=0; j<n_rows; j++){
				v.push_back((this->vec)[j*n_columns+i]);
			}
		}

		return Matrix(v,n_columns,n_rows);
	}

	string toString() const{
		string str = "{";
		if(vec.size()!=0){
			if(is_arithmetic<T>::value == true){
				for(int i=0; i<n_rows;i++){
					for(int j=0; j<n_columns; j++){
						if((i==n_rows-1)&&(j==n_columns-1)){
							str+=to_string(vec[i*n_columns+j]);
							str+="}";
							return str;
						}
						str+=to_string(vec[i*n_columns+j])+",";
					}
					str+="\n";
				}
			}
		}
		str+="}";
		return str;
	}
};

#endif
