#include <iostream>
#include <initializer_list>
#include "Container.h"
#include "Iterator.h"
#include "Algorithms.h"
#include "Exception.h"

//!!! Должен быть файл с исключениями (Исправлено)

struct Goods { // The first task
	uint32_t m_id;
	std::string m_name;
	std::string m_manufacturer;
	std::string m_warehouse_address;
	double m_weight;

	Goods() noexcept
		: m_id(0)
		, m_name("\0")
		, m_manufacturer("\0")
		, m_warehouse_address("\0")
		, m_weight(0.0) 
	{}

    //!!! Отформатировать в столбик по примеру (Исправлено)
	Goods(uint32_t           id,
	      const std::string& name,
	      const std::string& manufacturer, 
		  const std::string& warehouse_address, 
		  double weight) noexcept
		: m_id(id)
		, m_name(name)
		, m_manufacturer(manufacturer)
		, m_warehouse_address(warehouse_address)
		, m_weight(weight) 
	{}

	friend std::ostream& operator << (std::ostream& out, const Goods& m) noexcept {
		if (&m != nullptr && !equalnull(m)) {
			out << "ID: " << m.m_id << std::endl;
			out << "Name: " << m.m_name << std::endl;
			out << "Manufacturer: " << m.m_manufacturer << std::endl;
			out << "Warehouse address: " << m.m_warehouse_address << std::endl;
			out << "Weight: " << m.m_weight << std::endl;
		}
		else out << "Object is NULL" << std::endl;
		return out;
	}

	friend bool equalnull(const Goods& m) {
		return (m.m_id == 0
			&& m.m_name.compare("\0") == 0
			&& m.m_manufacturer.compare("\0") == 0
			&& m.m_warehouse_address.compare("\0") == 0
			&& std::fabs(m.m_weight) < std::numeric_limits<double>::epsilon());
	}


	bool operator == (const Goods& m) const noexcept {
		return (m_id == m.m_id
			&& m_name.compare(m.m_name) == 0
			&& m_manufacturer.compare(m.m_manufacturer) == 0
			&& m_warehouse_address.compare(m.m_warehouse_address) == 0
			&& std::fabs(m_weight - m.m_weight) < std::numeric_limits<double>::epsilon());
	}

	bool operator != (const Goods& m) const noexcept {
		return !(*this == m);
	}

	bool operator < (const Goods& m) const noexcept {
		return (m_name.compare(m.m_name) > 0);
	}

	bool operator > (const Goods& m) const noexcept {
		return (m_name.compare(m.m_name) < 0);
	}

};


bool manufacture(const Goods& m) {
	static bool b = true;
	static std::string MF;

	if (b) {
		MF = m.m_manufacturer;
		b = false;
	}

	return (MF.compare(m.m_manufacturer) == 0);
}

int main() {
	try {
	    //!!! Списки инициализации заранее никто не создает, а используют в конструкции Foo bar{ ...  }; (Исправлено)
		List<Goods> cont_1{ {1, "N", "M", "W_A", 1.1}, {2, "N2", "M2", "W_A2", 2.2}, {} };
		List<Goods> cont_2{};

		auto print = [](const auto& x) {
			std::cout << x << " ";
			return x;
		};

		try {
			std::cout << "---------------------------- The first test (The second task) ----------------------------" << std::endl;
			//!!! Никаких +1 быть не должно (Исправлено)
			forEach(cont_1.begin(), cont_1.end(), print);
		}
		catch (std::exception& error) {
			std::cerr << error.what() << std::endl;
		}

		try {
			std::cout << std::endl << "---------------------------- The second test (The third task) ----------------------------" << std::endl;
			Goods m(1, "n", "m", "w_a", 1.1);
			Goods m2(2, "n2", "m2", "w_a2", 2.2);
			Goods m3;
			cont_2.pushBack(m);
			cont_2.insert(cont_2.middle(), m2);
			cont_2.pushBack(m3);
			forEach(cont_2.begin(), cont_2.end(), print);
		}
		catch (std::exception& error) {
			std::cerr << error.what() << std::endl;
		}

		try {
			std::cout << std::endl << "---------------------------- The third test (The fourth task) ----------------------------" << std::endl;
			cont_1.insert(cont_1.middle(), cont_2.begin(), cont_2.end());
			forEach(cont_1.begin(), cont_1.end(), print);
		}
		catch (std::exception& error) {
			std::cerr << error.what() << std::endl;
		}

		try {
			std::cout << std::endl << "---------------------------- The fourth test (The fifth task) ----------------------------" << std::endl;
			sort(cont_1.begin(), cont_1.end());
			forEach(cont_1.begin(), cont_1.end(), print);
		}
		catch (std::exception& error) {
			std::cerr << error.what() << std::endl;
		}

		try {
			std::cout << std::endl << "---------------------------- The fifth test (The sixth task) ----------------------------" << std::endl;

			double lower, upper;
			std::cout << "Enter the lower and upper range of weight" << std::endl;
			std::cin >> lower >> upper;

			auto predicate = [lower, upper](const Goods& x) {
				return (x.m_weight - lower > std::numeric_limits<double>::epsilon()
					&& upper - x.m_weight > std::numeric_limits<double>::epsilon());
			};

			ListIterator<Goods> res = cont_1.end();
			++res;

			ListIterator<Goods> iter = findIf(cont_1.begin(), cont_1.end(), predicate);
			
			if (iter == res) {
				std::cout << "No found elements in the specified range" << std::endl;
				std::cout << std::endl << "---------------------------- The sixth test (The seventh task) ----------------------------" << std::endl;
				std::cout << "Specified element doesn't exist" << std::endl << std::endl;
			}
			else {
				std::cout << *iter << std::endl;
				std::cout << std::endl << "---------------------------- The sixth test (The seventh task) ----------------------------" << std::endl;
				iter->m_warehouse_address += " xD";
			}

			forEach(cont_1.begin(), cont_1.end(), print);

		}
		catch (std::exception& error) {
			std::cerr << error.what() << std::endl;
		}

		try {
			std::cout << std::endl << "---------------------------- The seventh test (The eighth task) ----------------------------" << std::endl;

			List<Goods> cont_3{};

			if (cont_1.get_size() > cont_3.get_size())
				cont_3.resize(cont_1.get_size());

			copyIf(cont_1.begin(), cont_1.end(), cont_3.begin(), manufacture);

			forEach(cont_3.begin(), cont_3.end(), print);

		}
		catch (std::exception& error) {
			std::cerr << error.what() << std::endl;
		}
	}
	catch (...) {
		std::cerr << "An error has been just detected" << std::endl;
	}

	std::cout << std::endl;
	system("pause");
	return 0;

}