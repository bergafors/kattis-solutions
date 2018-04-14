#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

std::vector<std::vector<char>> extract_orderings(
	const std::vector<std::string>::iterator start, 
	const std::vector<std::string>::iterator end,
	const std::string::size_type depth) 
{
	std::vector<char> order;
	std::vector<std::vector<char>> orderings;
	for (auto it = start; it != end; ++it) {
		if (it->size() == depth) {
			continue;
		}

		order.push_back((*it)[depth]);

		if (it != end - 1) {
			auto new_start = it;
			auto new_end = it + 1;
			while (new_end != end && new_end->size() > depth && (*new_start)[depth] == (*new_end)[depth]) {
				++new_end;
			}

			if (new_end != it + 1) {
				auto new_orderings = extract_orderings(new_start, new_end, depth + 1);
				for (auto& o : new_orderings) {
					if (o.size() > 1) {
						orderings.push_back(o);
					}
				}
				it = new_end - 1;
			}
		}
	}

	orderings.push_back(order);
	return orderings;
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

	auto orderings = extract_orderings(words.begin(), words.end(), 0);

	std::unordered_map<char, std::pair<std::unordered_set<char>, std::unordered_set<char>>> alphabet;
	for (auto it = orderings.begin(); it != orderings.end(); ++it) {
		for (auto itt = it->begin(); itt != it->end(); ++itt) {
			alphabet[*itt].first.insert(it->begin(), itt);
			alphabet[*itt].second.insert(itt + 1, it->end());
		}
	}

	for (auto& p : alphabet) {
		auto& pre_chars = p.second.first;
		auto& post_chars = p.second.second;

		std::unordered_set<char> pre_additional;
		for (const auto& c : pre_chars) {
			pre_additional.insert(alphabet[c].first.begin(), alphabet[c].first.end());
		}

		std::unordered_set<char> post_additional;
		for (const auto& c : post_chars) {
			post_additional.insert(alphabet[c].second.begin(), alphabet[c].second.end());
		}

		pre_chars.insert(pre_additional.begin(), pre_additional.end());
		post_chars.insert(post_additional.begin(), post_additional.end());
	}

	std::vector<char> sorted_alphabet(ALPHABETSIZE);
	for (const auto& p : alphabet) {
		if (p.second.first.count(p.first) > 0 || p.second.second.count(p.first) > 0) {
			std::cout << "IMPOSSIBLE" << std::endl;
			return 0;
		}
		else if (p.second.first.size() + p.second.second.size() != ALPHABETSIZE - 1) {
			std::cout << "AMBIGUOUS" << std::endl;
			return 0;
		}

		sorted_alphabet[p.second.first.size()] = p.first;
	}

	for (const auto& c : sorted_alphabet) {
		std::cout << c;
	}
	std::cout << std::endl;

	return 0;
}