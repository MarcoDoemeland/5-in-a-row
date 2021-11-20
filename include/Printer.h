// -----------------------------------------------------------------------------
// 5iar -- Printer.h
// -----------------------------------------------------------------------------
//
// ...
//

#ifndef __FIAR_PRINTER_H__
#define __FIAR_PRINTER_H__

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

namespace FIAR
{
	class Printer
	{
		public:
			// constructor
			Printer() {};

			// destructor
			//~ ~Printer( void );

			// ...
			void print_array( std::vector<std::vector<char>> const &array );

			//~ // bla
			//~ double num;


		private:

			void print_array_top_row( int size );
			void print_array_bottom_row( int size );
			void print_array_empty_row( int size );
			void print_array_filled_row(	int pos,
											std::vector<char> const &row
										);
			void print_array_number_row( int size );
			//~ void print_array_top_row   ( unsigned int const size );
			//~ void print_array_bottom_row( unsigned int const size );
			//~ void print_array_empty_row ( unsigned int const size );
			//~ void print_array_filled_row( std::vector<char> const row );
	};
}


#endif
