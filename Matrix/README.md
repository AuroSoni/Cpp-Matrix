Commit History
===============
##Matrix Library Functional
As of this commit all the major matrix utilities are functional in the library
1. Updates
	- matrix.h is a stand-alone header file. It can be included in any C++ project to do matrix calculations.
	- Matrix can be default initialized, zero initialized based on the dimensions and using nested initializer_list.
	- Matrix takes care of all logical errors that can be committed by the programmer. They cannot be checked at compile
	time but instead they throw exceptions.
	- Matrices can be added, subtracted, multiplied and compared with other matrices.
	- Matrices can be multiplied or divided by scalars(of the same type as matrix elements) as well.
	- Matrix element Cofactors and Minors can be calculated.
	- Matrix determinant can be calculated as well.
2. TODOS
	- Cofactor Matrix calculation of a particular matrix needs to be implemented.
	- Matrix inverse and adjoint needs to be implemented as well.
	- Matrix singularity check also needs to be implemented.
