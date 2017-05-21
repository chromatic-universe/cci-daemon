//creator_policy.hpp

#include <memory>

//contrib
#include "ace/Log_Msg.h"

namespace cci_policy
{
	//
	//meta
	//
	//creation policies
	//
	template<typename T>
	class op_new_creator
	{
		public :
			
			static T* create()
			{
				return new T;
			}

		protected :

			//dtor
			~op_new_creator()
			{}

	};	
	//
	template<typename T>
	class runtime_creator
	{
		public :
			
			static T* create()
			{
				void* mem_buffer = std::malloc( sizeof(T) );
				return mem_buffer ? new(mem_buffer) T : nullptr;
			}

		protected :
			
			//dtor
			~runtime_creator()
			{}

	};
	//
	template<typename T>
	class prototype_creator
	{
		public :

			//ctor
			prototype_creator( T* obj_ptr = nullptr) 
				: m_ptr_prototype( obj_ptr )
			{}
			
			T* create()
			{
				return m_ptr_prototype ? m_ptr_prototype->clone()
	            	                               : nullptr;
			}	
			
		private :

			//attributes
			T* 	m_ptr_prototype;

		protected :

			//dtor
			~prototype_creator()
			{}


		public :

			//accessors-inspectors
			T* prototype() const { return m_ptr_prototype; }
			//mutators
			void prototype( T* proto ) { m_ptr_prototype = proto; };
	};

		
}

