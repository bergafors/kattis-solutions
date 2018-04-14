#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

std::vector<std::vector<char>> extract_orderings(
	const std::vector<std::string>::iterator start, 
	const std::vector<std::string>::iterator end,
	const std::string::size_type depth,
	const std::size_t& ALPHABETSIZE) 
{
	std::vector<char> order;
	order.reserve(std::min(static_cast<std::size_t>(std::distance(start, end)), ALPHABETSIZE));
	std::vector<std::vector<char>> orderings;
	for (auto it = start; it != end; ++it) {
		if (it->size() == depth) {
			continue;
		}

		order.push_back((*it)[depth]);

		if (order.size() == ALPHABETSIZE + 1) {
			std::cout << "IMPOSSIBLE" << std::flush;
			exit(0);
		}

		if (it + 1 != end && (*it)[depth] == (*(it + 1))[depth]) {
			auto new_start = it;
			auto new_end = it + 1;
			while (new_end != end && (*new_start)[depth] == (*new_end)[depth]) {
				++new_end;
			}

			auto new_orderings = extract_orderings(new_start, new_end, depth + 1, ALPHABETSIZE);

			for (auto& o : new_orderings) {
				if (o.size() > 1) {
					orderings.push_back(std::move(o));
				}
			}

			it = new_end - 1;
		}
	}

	if (order.size() > 1) {
		orderings.push_back(std::move(order));
	}

	return orderings;
}

std::unordered_map<char, std::unordered_set<char>>
combine_orderings(std::vector<std::vector<char>>& orderings)
{
	std::unordered_map<char,  std::unordered_set<char>> combinations;
	for (auto it = orderings.begin(); it != orderings.end(); ++it) {
		for (auto itt = it->begin(); itt != it->end(); ++itt) {
			combinations[*itt].insert(itt + 1, it->end());
		}
	}

	for (auto& p : combinations) {
		std::unordered_set<char> additional;
		for (const auto& c : p.second) {
			additional.insert(combinations[c].begin(), combinations[c].end());
		}

		p.second.insert(additional.begin(), additional.end());
	}

	return combinations;
}

int main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(NULL);

	char L = 0;
	int N = 0;
	std::cin >> L >> N;
	std::cin.ignore();

	const std::size_t ALPHABETSIZE = L - 96;

	std::vector<std::string> words(N);
	for (auto& w : words) {
		std::getline(std::cin, w);
	}

	auto orderings = extract_orderings(words.begin(), words.end(), 0, ALPHABETSIZE);

	/*for (const auto& o : orderings) {
		for (const auto& c : o) {
			std::cout << c;
		}
		std::cout << std::endl;
	}*/

	for (const auto& order : orderings) {
		if (order.size() == ALPHABETSIZE) {
			auto sorted_ordering = order;
			std::sort(sorted_ordering.begin(), sorted_ordering.end());
			if (sorted_ordering.end() != std::unique(sorted_ordering.begin(), sorted_ordering.end())) {
				std::cout << "IMPOSSIBLE" << std::flush;
				return 0;
			}
			else {
				for (const auto& c : order) {
					std::cout << c;
				}
				std::cout << std::flush;
				return 0;
			}
		}
	}

	auto combinations = combine_orderings(orderings);

	std::vector<char> alphabet(ALPHABETSIZE);
	for (const auto& p : combinations) {
		if (p.second.end() != std::find(p.second.begin(), p.second.end(), p.first)) {
			std::cout << "IMPOSSIBLE" << std::flush;
			return 0;
		}

		alphabet[ALPHABETSIZE - 1 - p.second.size()] = p.first;
	}

	if (alphabet.end() != std::find(alphabet.begin(), alphabet.end(), 0)) {
		std::cout << "AMBIGUOUS" << std::flush;
		return 0;
	}

	for (const auto& c : alphabet) {
		std::cout << c;
	}
	std::cout << std::flush;

	return 0;
}