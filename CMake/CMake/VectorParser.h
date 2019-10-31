#pragma once

#ifndef  __VECTOR_PARSER__
#define __VECTOR_PARSER__
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace parser {
	template<typename T>
	void make_vector_from_text(std::string path, std::vector<std::vector<T>>& output, int no_read_col, int no_read_row, short int anshii_separator);

	template<typename T>
	void delete_nul_row(std::vector<std::vector<T>>& check);


	template<typename T>
	void make_vector_from_text(std::string path, std::vector<std::vector<T>>& output, int no_read_col, int no_read_row, short int anshii_separator) {

		std::string str;
		int  eqv;
		float point = 1.;
		const T EPS = 0.0001;
		std::ifstream fin(path);
		int separator = anshii_separator - 48;
		int cur_col, cur_row;
		int vec_col, vec_row;

		if (!fin.is_open()) {
			std::cout << "File does not open!" << std::endl;
		}
		else
		{
			std::cout << "Reading file!" << std::endl;
			cur_col = 0;
			cur_row = 0;
			vec_col = 0;
			vec_row = 0;
			std::vector<T> v = { 0 };
			output.push_back(v);
			bool bWasMunis = false;
			while (!fin.eof()) {

				std::getline(fin, str, '\n');

				for (unsigned x = 0; x < str.length(); x++) {
					eqv = static_cast<short int> (str.at(x));
					eqv -= 48;

					switch (eqv) {

					case -3: // minus
						try
						{
							bWasMunis = true;
							//output.at(vec_row).at(vec_col) += -1.;
						}
						catch (std::exception const &e)
						{
							std::cerr << e.what() << std::endl;
							std::abort();
						}
						break;
					case -2: //point
						point *= 10.;
						break;
					default:
						if ((cur_col != no_read_col) && (cur_row != no_read_row)) {
							if (eqv == separator) {
								try
								{
									output.at(vec_row).at(vec_col) /= point;
									output.at(vec_row).push_back(0.);
									if (bWasMunis)
									{
										bWasMunis = false;
										output.at(vec_row).at(vec_col) *= -1.f;
									}
									point = 1.;
									vec_col++;
									cur_col++;
								}
								catch (std::exception const &e)
								{
									std::cerr << e.what() << std::endl;
									std::abort();
								}

							}
							if ((eqv >= 0) && (eqv <= 9)) {
								try
								{
									if (output.at(vec_row).at(vec_col) <= EPS) { //zero
										T  a = static_cast<T> (eqv);
										output.at(vec_row).at(vec_col) += a;
									}
									else {
										T  a = static_cast<T> (eqv);
										output.at(vec_row).at(vec_col) = output.at(vec_row).at(vec_col) * 10. + a;
									}
								}
								catch (std::exception const &e)
								{
									std::cerr << e.what() << std::endl;
									std::abort();
								}
							}
							if (x == str.length() - 1) {
								try
								{
									output.at(vec_row).at(vec_col) /= point;
									point = 1.;
									if (bWasMunis)
									{
										bWasMunis = false;
										output.at(vec_row).at(vec_col) *= -1.f;
									}
								}
								catch (std::exception const &e)
								{
									std::cerr << e.what() << std::endl;
									std::abort();
								}
							}
						}
						else {
							if (eqv == separator) {
								cur_col++;

							}
						}
						break;

					}
				}

				if ((cur_col != no_read_col) && (cur_row != no_read_row)) {
					std::vector<T> v = { 0. };
					output.push_back(v);
					vec_row++;
					vec_col = 0;
					cur_col = 0;
					cur_row++;
				}
				else {
					cur_col = 0;
					cur_row++;
				}
			}

			delete_nul_row<T>(output);

		}

	}
	template<typename T>
	void delete_nul_row(std::vector<std::vector<T>>& check) {
		const T EPS = 0.0001;
		int last_row = check.size() - 1;
		if (!check.at(last_row).size()) {
			check.at(last_row).clear();
			check.erase(check.begin() + last_row);
		}
		else if ((check.at(last_row).at(0) <= EPS) && (check.at(last_row).size() <= 1)) {
			check.at(last_row).clear();
			check.erase(check.begin() + last_row);
		}
	}
}

#endif // ! __CSV_PARSER__
