//daemon_procedure_policy.hpp    william k. johnon    2017


#include <memory>
#include <string>

//contrib
#include "ace/Log_Msg.h"

namespace cci_policy
{
	
	//
	//daemon procedure policies
	//
	template<typename T>
	class default_daemon_procedure
	{

		public :

		
			int proc_init()
			{
				return 0;
			}
			int perform()
			{
				return 0;
			}
			int fini()
			{
				return 0;
			}


		protected :

			//dtor
			~default_daemon_procedure()
			{}


	};

		
}
