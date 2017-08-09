//stram_handle_policies.hpp   william k. johnson 2017


namespace cci_policy
{
	
		
		//meta
		//
		//stream handle policies
		//
		template<typename T>
		class close_all_descriptors
		{
			public :
			

				void configure_descriptors()
				{
					ACE_TRACE ("close_all_descriptors::configure_descriptors");
				}


			protected :

				//dtor
				~close_all_descriptors()
				{}


		};

	
}
