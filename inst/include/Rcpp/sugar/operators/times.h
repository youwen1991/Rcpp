// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// times.h: Rcpp R/C++ interface class library -- operator*
//                                                                      
// Copyright (C) 2010	Dirk Eddelbuettel and Romain Francois
//
// This file is part of Rcpp.
//
// Rcpp is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Rcpp__sugar__times_h
#define Rcpp__sugar__times_h

namespace Rcpp{
namespace sugar{

	template <int RTYPE,bool LHS_NA, bool RHS_NA>
	class times{
	public:
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		inline STORAGE apply( STORAGE lhs, STORAGE rhs) const {
			return traits::is_na<RTYPE>(lhs) ? lhs : ( traits::is_na<RTYPE>(rhs) ? rhs : (lhs * rhs) ) ;
		}
	} ;
	template <int RTYPE,bool RHS_NA>
	class times<RTYPE,false,RHS_NA>{
	public:
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		inline STORAGE apply( STORAGE lhs, STORAGE rhs) const {
			return traits::is_na<RTYPE>(rhs) ? rhs : (lhs * rhs);
		}
	} ;
	template <int RTYPE,bool LHS_NA>
	class times<RTYPE,LHS_NA,false>{
	public:
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		inline STORAGE apply( STORAGE lhs, STORAGE rhs) const {
			return traits::is_na<RTYPE>(lhs) ? lhs : (lhs * rhs);
		}
	} ;
	template <int RTYPE>
	class times<RTYPE,false,false>{
	public:
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		inline STORAGE apply( STORAGE lhs, STORAGE rhs) const {
			return lhs * rhs;
		}
	} ;
	

	template <int RTYPE, bool _NA_, typename VEC_TYPE>
	class Times_Vector_Primitive : public Rcpp::VectorBase<RTYPE,true, Times_Vector_Primitive<RTYPE,_NA_,VEC_TYPE> > {
	public:
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		typedef times<RTYPE,_NA_,true> OPERATOR ;
		
		Times_Vector_Primitive( const VEC_TYPE& lhs_, STORAGE rhs_ ) : 
			lhs(lhs_), rhs(rhs_), op() {}
		
		inline STORAGE operator[]( int i ) const {
			return op.apply( lhs[i], rhs ) ;
		}
		
		inline int size() const { return lhs.size() ; }
	
		
	private:
		const VEC_TYPE& lhs ;
		STORAGE rhs ;
		OPERATOR op ; 
	} ;
	
	template <int RTYPE, bool LHS_NA, typename LHS_VEC_TYPE, bool RHS_NA, typename RHS_VEC_TYPE >
	class Times_Vector_Vector : public Rcpp::VectorBase<RTYPE,true, Times_Vector_Vector<RTYPE,LHS_NA,LHS_VEC_TYPE,RHS_NA,RHS_VEC_TYPE> > {
	public:
		typedef typename traits::storage_type<RTYPE>::type STORAGE ;
		typedef times<RTYPE,LHS_NA,RHS_NA> OPERATOR ;
		
		Times_Vector_Vector( const LHS_VEC_TYPE& lhs_, const RHS_VEC_TYPE& rhs_ ) : 
			lhs(lhs_), rhs(rhs_), op() {}
		
		inline STORAGE operator[]( int i ) const {
			return op.apply( lhs[i], rhs[i] ) ;
		}
		
		inline int size() const { return lhs.size() ; }
		
	private:
		const LHS_VEC_TYPE& lhs ;
		const RHS_VEC_TYPE& rhs ;
		OPERATOR op ; 
	} ;
}
}

template <int RTYPE,bool _NA_, typename T>
inline Rcpp::sugar::Times_Vector_Primitive< RTYPE , _NA_ , Rcpp::VectorBase<RTYPE,_NA_,T> >
operator*( 
	const Rcpp::VectorBase<RTYPE,_NA_,T>& lhs, 
	typename Rcpp::traits::storage_type<RTYPE>::type rhs 
) {
	return Rcpp::sugar::Times_Vector_Primitive<RTYPE,_NA_, Rcpp::VectorBase<RTYPE,_NA_,T> >( lhs, rhs ) ;
}


template <int RTYPE,bool _NA_, typename T>
inline Rcpp::sugar::Times_Vector_Primitive< RTYPE , _NA_ , Rcpp::VectorBase<RTYPE,_NA_,T> >
operator*( 
	typename Rcpp::traits::storage_type<RTYPE>::type rhs, 
	const Rcpp::VectorBase<RTYPE,_NA_,T>& lhs
) {
	return Rcpp::sugar::Times_Vector_Primitive<RTYPE,_NA_, Rcpp::VectorBase<RTYPE,_NA_,T> >( lhs, rhs ) ;
}

template <int RTYPE,bool LHS_NA, typename LHS_T, bool RHS_NA, typename RHS_T>
inline Rcpp::sugar::Times_Vector_Vector< 
	RTYPE , 
	LHS_NA, Rcpp::VectorBase<RTYPE,LHS_NA,LHS_T>, 
	RHS_NA, Rcpp::VectorBase<RTYPE,RHS_NA,RHS_T>
	>
operator*( 
	const Rcpp::VectorBase<RTYPE,LHS_NA,LHS_T>& lhs,
	const Rcpp::VectorBase<RTYPE,RHS_NA,RHS_T>& rhs
) {
	return Rcpp::sugar::Times_Vector_Vector<
		RTYPE, 
		LHS_NA, Rcpp::VectorBase<RTYPE,LHS_NA,LHS_T>,
		RHS_NA, Rcpp::VectorBase<RTYPE,RHS_NA,RHS_T>
		>( lhs, rhs ) ;
}

#endif