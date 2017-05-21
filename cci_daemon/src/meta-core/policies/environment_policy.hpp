//environment_policy.hpp    willian k. johnson 2017

//contrib
#include "ace/Log_Msg.h"

namespace cci_policy
{
	
	//
	//environment policies
	//
	template<typename T>
	class default_environment_context
	{
		public :
			

			void configure_environment()
			{
			   ACE_TRACE("defailt_environment_context::configure_environment_context");
			}

		protected :
			
			//dtor
			~default_environment_context()
			{}


	};
	//
	template<typename T>
	class chroot_environment_context
	{
		public :
			

			void configure_environment()
			{
				//
			}

		protected :
			
			//dtor
			~chroot_environment_context()
			{}


	};
		
}
