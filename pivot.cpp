#include <cstdio>
#include <cstdint>
#include <vector>

int main()
{
	std::int32_t n = 0;
	scanf("%i", &n);

	std::vector<std::int32_t> a;
	a.reserve(n);

	while (scanf("%i", &n) != EOF) {
		a.push_back(n);
	}

	std::int32_t maxd = 0;
	for (auto& d : a) {
		if (d > maxd) {
			maxd = d;
		}
		else {
			d = -d;
		}
	}

	std::int32_t ans = 0;
	std::int32_t mind = maxd + 1;
	auto it = a.rbegin();
	const auto end = a.rend();
	for (; it != end; ++it) {
		const auto& d = *it;
		if (d < 0) {
			if (-d < mind) {
				mind = -d;
			}
		}
		else if (d < mind) {
			mind = d;
			++ans;
		}
	}

	printf("%i", ans);

	return 0;
}