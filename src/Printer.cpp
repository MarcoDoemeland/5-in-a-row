#include "Printer.h"

namespace FIAR
{
	//~ Printer::Printer( int m )
	//~ : num( m )
	//~ {}

	//~ void Printer::print_array()
	//~ {
	//~ }

	void Printer::print_array( std::vector<std::vector<char>> const &array )
	{
		unsigned int size_x { (unsigned int) array[0].size() };
		//~ unsigned int size_y ( array.size() );

		//~ for

		std::cout << std::endl << std::endl;
		print_array_number_row( size_x );
		print_array_top_row( size_x );


		for (int i {0}; i<(int)array.size(); ++i)
		{
			print_array_empty_row( size_x );
			print_array_filled_row( i+1, array[i] );
			print_array_bottom_row( size_x );
			//~ for (auto ij_content: i_row)
			//~ {
				//~ std::cout << ij_content << " ";
			//~ }
			//~ std::cout << std::endl;
		}

		std::cout << std::endl << std::endl;
		//~ print_array_bottom_row( size_x );
	}


	void Printer::print_array_top_row( int size )
	{
		std::cout << "    ";
		for (int i {0}; i < (int)size; ++i)
		{
			std::cout << "____";
		}
		std::cout << "_" << std::endl;
	}

	void Printer::print_array_bottom_row( int size )
	{
		std::cout << "    ";
		for (int i {0}; i < (int)size; ++i)
		{
			std::cout << "|___";
		}
		std::cout << "|" << std::endl;
	}

	void Printer::print_array_empty_row( int size )
	{
		std::cout << "    ";
		for (int i {0}; i < (int)size; ++i)
		{
			std::cout << "|   ";
		}
		std::cout << "|" << std::endl;
	}

	void Printer::print_array_filled_row(	int pos,
											std::vector<char> const &row	)
	{
		int padlen_left;
		int padlen_right;
		std::string str;

		str = std::to_string(pos);
		padlen_left = (4 + 1 - str.size()) / 2;
		padlen_right = 4 - str.size() - padlen_left;
		printf("%*s%s%*s", padlen_left, "", str.c_str(), padlen_right, "");

		//~ std::cout << " " << pos << " ";
		for (auto i_content: row)
		{
			std::cout << "| " << i_content << " ";
		}
		std::cout << "|" << std::endl;
	}

	void Printer::print_array_number_row( int size )
	{
		int padlen_left;
		int padlen_right;
		std::string i_str;

		std::cout << R"( x\y)";
		for (int i {0}; i < (int)size; ++i)
		{
			i_str = std::to_string(i+1);
			padlen_left = (3 + 1 - i_str.size()) / 2;
			padlen_right = 3 - i_str.size() - padlen_left;
			printf(" %*s%s%*s", padlen_left, "", i_str.c_str(), padlen_right, "");
		}
		std::cout << " " << std::endl;
	}

















}
